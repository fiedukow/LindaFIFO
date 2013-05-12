#include "Client.h"
#include <Common/NamedPipe.h>
#include <iostream>

Client::Client()
{}

void Client::operator()()
{
  if(!registerInServer())
  {
    std::cout << "Serwer nieaktywny!" << std::endl;
    return;
  }
  std::cout << "Uzyskany LindaID: " << pipeId_ << std::endl;
}

bool Client::registerInServer()
{
  std::cout << "Rejestracja..." << std::endl;
  NamedPipePtr regPipe(new NamedPipe("/tmp/LINDA_REGISTER_PIPE"));
  NamedPipeReader reader(regPipe);
  if(!reader.open())
    return false;

  pipeId_ = reader.read();
  reader.close();
  return true;
}

