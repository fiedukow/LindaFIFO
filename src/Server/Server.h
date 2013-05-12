#ifndef _SERVER_H_
#define _SERVER_H_

#include <list>
#include <Common/NamedPipe.h>
#include <signal.h>

class Server
{
public:
  Server();
  void operator()();

  void handlePosixSignal(int signal);
  void stop();

private:
  void handleNewClientRegistering();
  void handleIncomingQueries();
  void handleAnswers();
  void handleTimeouts();

private:
  NamedPipePtr registerPipe_;
  int id_;
  pid_t lastRegisteredPid_;
  std::list<NamedPipePtr> clients_;
  bool stopped_;
};

#endif
