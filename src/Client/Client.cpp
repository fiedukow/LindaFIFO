#include "Client.h"
#include <Common/NamedPipe.h>
#include <iostream>
#include <boost/lexical_cast.hpp>

Client::Client()
  : channel_(NULL)
{}

Client::~Client()
{
  delete channel_;
}

void Client::operator()()
{
  if(!registerInServer())
  {
    std::cout << "Serwer nieaktywny!" << std::endl;
    return;
  }

  std::string query;
  NamedPipeWriter writer = channel_->getClientWriter();
  NamedPipeReader reader = channel_->getClientReader();

  while(true)
  {
    std::cout << " >  ";
    std::cout.flush();
    std::cin >> query;
    writer.open();
    writer.write(query);
    writer.close();
    reader.open();
    std::cout << "ANSWER IS: " << reader.read() << std::endl;
    reader.close();
  }
}

bool Client::registerInServer()
{
  NamedPipePtr regPipe(new NamedPipe("/tmp/LINDA_REGISTER_PIPE"));
  NamedPipeReader reader(regPipe);
  if(!reader.open())
    return false;

  std::string pipeId = reader.read();
  if(pipeId.empty())
  {
    reader.close();
    return registerInServer();
  }

  reader.close();
  channel_ = new WeakPipeChannel(boost::lexical_cast<int>(pipeId));
  
  return true;
}

