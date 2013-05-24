#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <Common/NamedPipe.h>

class Client
{
public:
  Client();
  ~Client();
  void operator()();

private:
  bool registerInServer();

private:
  WeakPipeChannel* channel_;
};

#endif
