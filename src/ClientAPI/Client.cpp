#include "Client.h"
#include <Common/NamedPipe.h>
#include <Common/MagicMessages.h>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <ClientAPI/API.h>

const std::string Client::BREAK_COMMAND = "STOP";

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
    throw Linda::ServerInactiveException();
    return;
  }

  std::string query;
  NamedPipeWriter writer = channel_->getClientWriter();
  NamedPipeReader reader = channel_->getClientReader();

  while(true)
  {
    queryBQ_.pop(query);
    if(query == BREAK_COMMAND)
      query = Linda::Messages::CLOSE_CONNECTION_REQUEST;  
    writer.open();
    writer.write(query);
    writer.close();
    if(query == Linda::Messages::CLOSE_CONNECTION_REQUEST)
      break;
    reader.open();
    answerBQ_.push(reader.read());
    reader.close();
  }
}

bool Client::registerInServer()
{
  std::string pipeId = "";

  while(pipeId.empty())
  {
    NamedPipePtr regPipe(new NamedPipe("/tmp/LINDA_REGISTER_PIPE"));
    NamedPipeReader reader(regPipe);
    if(!reader.open())
      return false;

    pipeId = reader.read();
    reader.close();
  }

  channel_ = new WeakPipeChannel(boost::lexical_cast<int>(pipeId));
  return true;
}

BlockingQueue<std::string>& Client::getQueryBQ()
{
  return queryBQ_;
}

BlockingQueue<std::string>& Client::getAnswerBQ()
{
  return answerBQ_;
}
