#include <boost/thread.hpp>
#include <ClientAPI/Client.h>
#include <ClientAPI/API.h>
#include <iostream>

int main(void)
{
  Client client;
  boost::thread th(boost::ref(client));
  std::string question = "";
  std::string answer;

  while(true)
  {
    std::cout << " > ";
    std::cout.flush();
    std::cin >> question;
    if(question == Client::BREAK_COMMAND)
      break;
    client.getQueryBQ().push(question);
    client.getAnswerBQ().pop(answer);
    std::cout << answer << std::endl;
  }

  th.join();

  return 0;
}

