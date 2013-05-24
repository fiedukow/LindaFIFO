#ifndef _SERVER_H_
#define _SERVER_H_

#include <list>
#include <Common/NamedPipe.h>
#include <signal.h>
#include <boost/shared_ptr.hpp>

typedef std::pair<OwnedPipeChannel, std::string> AddressedAnswer;

class Operation;
typedef boost::shared_ptr<Operation> OperationPtr;

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
  std::string handleQuery(const std::string& query);
  OperationPtr parseQuery(const std::string& query);

private:
  NamedPipePtr registerPipe_;
  int id_;
  pid_t lastRegisteredPid_;
  std::list<OwnedPipeChannel> clients_;
  std::list<AddressedAnswer> answerQueue_;
  bool stopped_;
};

#endif
