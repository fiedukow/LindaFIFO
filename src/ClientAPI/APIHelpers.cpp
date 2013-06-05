#include "APIHelpers.h"
#include "API.h"
#include "Client.h"
#include <Common/MagicMessages.h>
#include <boost/thread.hpp>

namespace Linda
{

void handleMagicAnswers(const std::string& answer)
{
  if(answer == Linda::Messages::TIMEOUT_MESSAGE)
    throw QueryTimeoutException();
  else if(answer == Linda::Messages::INVALID_MESSAGE)
    throw InvalidQueryException();
}

void handleQueryTypeCheck(const std::string& query, Operation::Type requestedType)
{
  if(checkQueryType(query) != requestedType)
    throw InvalidQueryTypeException();
}

std::string generalQueryHandler(const std::string& query)
{
  std::string answer;
  Client client;
  boost::thread clientThread(boost::ref(client));
  client.getQueryBQ().push(query);
  client.getQueryBQ().push(Client::BREAK_COMMAND);
  client.getAnswerBQ().pop(answer);
  clientThread.join();
  return answer;
}

Operation::Type checkQueryType(const std::string& query)
{
  Parser parser;
  Operation* op = parser.parse(query.c_str());
  Operation::Type type = op->type;

  delete op;
  return type;
}

}//namespace Linda
