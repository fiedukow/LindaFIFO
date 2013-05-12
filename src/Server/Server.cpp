#include "Server.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <unistd.h>
#include <signal.h>
#include <sys/file.h>

static void sigPipe (int sig, siginfo_t *siginfo, void *context)
{
  printf ("Sending PID: %ld, UID: %ld\n",
      (long)siginfo->si_pid, (long)siginfo->si_uid);
}

Server::Server()
  : registerPipe_(new OwnedNamedPipe("/tmp/LINDA_REGISTER_PIPE")),
    id_(0),
    lastRegisteredPid_(0)
{
  struct sigaction act;
  memset (&act, '\0', sizeof(act));

  /* Use the sa_sigaction field because the handles has two additional parameters */
  act.sa_sigaction = &sigPipe;
 
  /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
  act.sa_flags = SA_SIGINFO;
 
  if (sigaction(SIGPIPE, &act, NULL) < 0) {
    perror ("sigaction");
    throw 1;
  }
 
}


void Server::operator()()
{
  while(true)
  {
    std::cout << "Nowy cykl..." << std::endl;
    handleNewClientRegistering();
    handleIncomingQueries();
    handleAnswers();
    handleTimeouts();
  }
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
