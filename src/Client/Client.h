#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <Common/NamedPipe.h>
#include <Common/BlockingQueue.hpp>

class Client
{
public:
  Client();
  ~Client();
  void operator()();
  
  BlockingQueue<std::string>& getQueryBQ();
  BlockingQueue<std::string>& getAnswerBQ();

private:
  bool registerInServer();

public:
  static const std::string BREAK_COMMAND;

private:
  WeakPipeChannel* channel_;
  BlockingQueue<std::string> queryBQ_;
  BlockingQueue<std::string> answerBQ_;
};

#endif
