#include "Server.h"
#include "ParserToDatabaseProxy.h"
#include <Parser/Parser.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <unistd.h>
#include <signal.h>
#include <sys/file.h>

static Server::Server* I_DONT_WANT_TO_LIVE_ON_THIS_PLANET_ANYMORE = NULL;

static void signalHandler(int sig)
{
  if(I_DONT_WANT_TO_LIVE_ON_THIS_PLANET_ANYMORE)
    I_DONT_WANT_TO_LIVE_ON_THIS_PLANET_ANYMORE->handlePosixSignal(sig);
}

namespace Server
{

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
  registeringPid = registerPipe_->getReadLockingPid();

  std::cout << "Zablokowany przez " << registeringPid << std::endl;

  if(lastRegisteredPid_ != registeringPid)
  {
    if(registeringPid > 0)
    {
      clients_.push_back(OwnedPipeChannelPtr(new OwnedPipeChannel(id_)));
      writer.write(boost::lexical_cast<std::string>(id_));
      ++id_;
    }
    lastRegisteredPid_ = registeringPid;
  }
  else
    std::cout << "SKIP" << std::endl;
  
  writer.close();   
}

void Server::handleIncomingQueries()
{
  std::cout << "Zapytania..." << std::endl;
  for(OwnedPipeChannelPtr client : clients_)
  {
    NamedPipeReader& reader = client->getServerReader();

    std::cout << "Awaiting orders..." << std::endl;

    std::string msg = reader.read();    

    if(msg.empty())
      continue;

    std::cout << "Incoming message: " << msg << std::endl;
    std::string answer = handleQuery(msg);
    if(answer == "NO ITEM")
      waitingQueue_.push_back(WaitingQuery(time(NULL) + 15, msg, client));
    else
      answerQueue_.push_back(AddressedAnswer(client, answer));
  } 
}

void Server::handleAnswers()
{
  std::cout << "Odpowiedzi..." << std::endl;
  while(answerQueue_.size() > 0)
  {
    AddressedAnswer addressedAnswer = answerQueue_.front();
    answerQueue_.pop_front();
    NamedPipeWriter writer = addressedAnswer.first->getServerWriter();
    if(!writer.tryOpen())
      continue;
    writer.write(addressedAnswer.second); 
    writer.close();        
  }
}

void Server::handleWaitingQueue()
{
  std::cout << "Waiting..." << std::endl;
  for(auto it = waitingQueue_.begin(); it != waitingQueue_.end();)
  {
    std::string answer = handleQuery(it->query);
    if(answer != "NO ITEM")
    {
      answerQueue_.push_back(AddressedAnswer(it->client, answer));
      it = waitingQueue_.erase(it);
    }
    else
      ++it;
  }
}

void Server::handleTimeouts()
{
  std::cout << "Timeouty..." << std::endl;
  for(auto it = waitingQueue_.begin(); it != waitingQueue_.end();)
  {
    if(it->timeoutTime <= time(NULL))
    {
      answerQueue_.push_back(AddressedAnswer(it->client, "TIMEOUT"));
      it = waitingQueue_.erase(it);
    }
    else
      ++it;
  }
}

std::string Server::handleQuery(const std::string& query)
{
  std::cout << "PARSING QUERY: " << query << std::endl;
  OperationPtr operation = parseQuery(query);
  if(operation.get() == NULL)
    return "INVALID QUERY";
  
  ParserToDatabaseProxy answerHandler(db);
  answerHandler.handleOperation(operation);
  if(answerHandler.shouldLastOperationWait())
    return "NO ITEM";
  else
    if(answerHandler.hasLastOperationAddedElement())
      handleWaitingQueue();
    return answerHandler.getLastOperationAnswer();
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

}//namespace Server
