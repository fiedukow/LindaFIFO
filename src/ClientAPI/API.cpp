#include "API.h"
#include "APIHelpers.h"
#include <Parser/Parser.hpp>
#include <thread>

namespace Linda
{

const char* InvalidQueryException::what() const throw()
{
  return "Given query is invalid.";
}

const char* InvalidQueryTypeException::what() const throw()
{
  return "Given query is not of declared type.";
}

const char* QueryTimeoutException::what() const throw()
{
  return "Timeout - there is now such tuple in database.";
}

const char* ServerInactiveException::what() const throw()
{
  return "The server is not up.";
}

Linda::Tuple read(const std::string& query)
{
  std::string answer;
  handleQueryTypeCheck(query, Operation::READ);
  answer = generalQueryHandler(query);
  handleMagicAnswers(answer);
  return answer;
}

Linda::Tuple input(const std::string& query)
{
  std::string answer;
  handleQueryTypeCheck(query, Operation::INPUT);
  answer = generalQueryHandler(query);
  handleMagicAnswers(answer);
  return answer;
}

void output(const std::string& query)
{
  std::string answer;
  handleQueryTypeCheck(query, Operation::OUTPUT);
  answer = generalQueryHandler(query);
  handleMagicAnswers(answer);
}

}//namespace Linda
