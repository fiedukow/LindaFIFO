#ifndef _SERVER_H_
#define _SERVER_H_

#include <list>
#include <Common/NamedPipe.h>

class Server
{
public:
  Server();
  void operator()();

private:
  void handleNewClientRegistering();
  void handleIncomingQueries();
  void handleAnswers();
  void handleTimeouts();

private:
  NamedPipePtr registerPipe_;
  int id_;
  pid_t lastRegisteredPid_;
};

#endif
