#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>

class Client
{
public:
  Client();
  void operator()();

private:
  bool registerInServer();

private:
  std::string pipeId_;
};

#endif
