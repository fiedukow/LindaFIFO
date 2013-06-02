#ifndef _CLIENT_API_H_
#define _CLIENT_API_H_
#include <string>
#include <exception>
#include <Parser/Parser.hpp>

namespace Linda
{
typedef std::string Tuple;

class InvalidQueryException : public std::exception
{
  const char* what() const throw();
};

class InvalidQueryTypeException : public std::exception
{
  const char* what() const throw();
};

class QueryTimeoutException : public std::exception
{
  const char* what() const throw();
};

class ServerInactiveException : public std::exception
{
  const char* what() const throw();
};

Tuple read(const std::string& query);
Tuple input(const std::string& query);
void output(const std::string& query);

}//namespace Linda

#endif
