#ifndef SERVER_EXCEPTIONS_H
#define SERVER_EXCEPTIONS_H

#include <exception>

#include "Common/QueryStructures.h"

namespace Server
{

namespace Exceptions
{

class DatabaseServerException : public std::exception
{
public:
  virtual ~DatabaseServerException() throw() = default;

  virtual const char* what() const throw();
};

class TupleDoesNotExistException : public DatabaseServerException
{
public:
  TupleDoesNotExistException(const Common::SelectDescription&);
  virtual ~TupleDoesNotExistException() throw();//= default;

  virtual const char* what() const throw();

private:
  const Common::SelectDescription description;
};

} // namespace Exceptions

} // namespace Server

#endif // SERVER_EXCEPTIONS_H

