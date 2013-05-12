#include "Server.h"
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

  pid_t registeringPid = registerPipe_->getReadLockingPid();
  if(registeringPid == -1)
    return;

  std::cout << "Zablokowany przez " << registeringPid << std::endl;

  if(lastRegisteredPid_ != registeringPid)
  {
    writer.write(boost::lexical_cast<std::string>(id_++));
    lastRegisteredPid_ = registeringPid;
    std::cout << "WRITING!" << std::endl;
  }
  else
    std::cout << "SKIP" << std::endl;

  writer.close();   
}

void Server::handleIncomingQueries()
{
  std::cout << "Zapytania..." << std::endl;
  //FIXME implement this
}

void Server::handleAnswers()
{
  std::cout << "Odpowiedzi..." << std::endl;
  //FIXME implement this
}

void Server::handleTimeouts()
{
  std::cout << "Timeouty..." << std::endl;
  //FIXME implement this
}
