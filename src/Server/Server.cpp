#include "Server.h"
#include <Parser/Parser.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <unistd.h>
#include <signal.h>
#include <sys/file.h>

static Server* I_DONT_WANT_TO_LIVE_ON_THIS_PLANET_ANYMORE = NULL;

static void signalHandler(int sig)
{
  if(I_DONT_WANT_TO_LIVE_ON_THIS_PLANET_ANYMORE)
    I_DONT_WANT_TO_LIVE_ON_THIS_PLANET_ANYMORE->handlePosixSignal(sig);
}

Server::Server()
  : registerPipe_(new OwnedNamedPipe("/tmp/LINDA_REGISTER_PIPE")),
    id_(0),
    lastRegisteredPid_(0),
    stopped_(false)
{
  signal(SIGINT, &signalHandler);
  I_DONT_WANT_TO_LIVE_ON_THIS_PLANET_ANYMORE = this;
}

void Server::operator()()
{
  while(!stopped_)
  {
    std::cout << "Nowy cykl..." << std::endl;
    handleNewClientRegistering();
    handleIncomingQueries();
    handleAnswers();
    handleTimeouts();
  }

  std::cout << "Grzeczny koniec..." << std::endl;
}

void Server::handlePosixSignal(int signal)
{
  switch(signal)
  {
    case SIGINT:
      stop();
      break;
    case SIGPIPE:
      std::cout << "SIGPIPE" << std::endl;
    default:
      assert(false);    
  }
}

void Server::stop()
{
  stopped_ = true;
}

void Server::handleNewClientRegistering()
{
  std::cout << "Rejestracja..." << std::endl;
  NamedPipeWriter writer(registerPipe_);
  if(!writer.tryOpen())
    return;

  pid_t registeringPid;
  do //TODO what if client will die?
  {
    registeringPid = registerPipe_->getReadLockingPid();
  } while(registeringPid == -1);

  std::cout << "Zablokowany przez " << registeringPid << std::endl;

  if(lastRegisteredPid_ != registeringPid)
  {
    OwnedPipeChannel newChannel(id_);
    clients_.push_back(newChannel);
    writer.write(boost::lexical_cast<std::string>(id_));
    lastRegisteredPid_ = registeringPid;
    ++id_;
  }
  else
    std::cout << "SKIP" << std::endl;

  writer.close();   
}

void Server::handleIncomingQueries()
{
  std::cout << "Zapytania..." << std::endl;
  for(OwnedPipeChannel& client : clients_)
  {
    NamedPipeReader reader = client.getServerReader();
    if(!reader.tryOpen())
      continue;

    std::cout << "Awaiting orders..." << std::endl;

    std::string msg = reader.read();    
    reader.close();

    if(msg.empty())
      continue;

    std::cout << "Incoming message: " << msg << std::endl;
    std::string answer = handleQuery(msg);
    answerQueue_.push_back(AddressedAnswer(client, answer));      
  } 
}

void Server::handleAnswers()
{
  std::cout << "Odpowiedzi..." << std::endl;
  for(AddressedAnswer& addressedAnswer : answerQueue_)
  {
    NamedPipeWriter writer = addressedAnswer.first.getServerWriter();
    if(!writer.tryOpen())
      continue;
    writer.write(addressedAnswer.second); 
    writer.close();        
  }
}

void Server::handleTimeouts()
{
  std::cout << "Timeouty..." << std::endl;
  //FIXME implement this
}

std::string Server::handleQuery(const std::string& query)
{
  std::cout << "PARSING QUERY: " << query << std::endl;
  OperationPtr operation = parseQuery(query);
  if(operation.get() == NULL)
    return "INVALID QUERY";
  
  return "OK";
}

OperationPtr Server::parseQuery(const std::string& query)
{
  Parser parser;
  try
  {
    return OperationPtr(parser.parse(query.c_str()));
  }
  catch(Parser::Exception* e)
  {
    delete e; //TODO - pointer? srsly?
    Operation* tmp = NULL;
    return OperationPtr(tmp);
  }
}
