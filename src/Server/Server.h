#ifndef _SERVER_H_
#define _SERVER_H_

#include <list>
#include <Common/NamedPipe.h>
#include <signal.h>
#include <memory>
#include "Database.h"

typedef std::pair<OwnedPipeChannelPtr, std::string> AddressedAnswer;

class Operation;
typedef std::shared_ptr<Operation> OperationPtr;

namespace Server
{

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
  void handleWaitingQueue();
  std::string handleQuery(const std::string& query);
  OperationPtr parseQuery(const std::string& query);

private:
  struct WaitingQuery
  {
    WaitingQuery(time_t timeoutTime,
                 const std::string& query,
                 OwnedPipeChannelPtr client)
      : timeoutTime(timeoutTime),
        query(query),
        client(client)
    {}

    bool operator==(const WaitingQuery& toCmp)
    {
      return timeoutTime == toCmp.timeoutTime &&
             query == toCmp.query &&
             client == toCmp.client;
    }

    time_t timeoutTime;
    std::string query;
    OwnedPipeChannelPtr client;
  };

private:
  NamedPipePtr registerPipe_;
  int id_;
  pid_t lastRegisteredPid_;
  std::list<OwnedPipeChannelPtr> clients_;
  std::list<AddressedAnswer> answerQueue_;
  std::list<WaitingQuery>   waitingQueue_;
  bool stopped_;
  Database db;
};

}//namespace Server

#endif
