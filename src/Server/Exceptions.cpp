#include "Exceptions.h"

namespace Server
{

namespace Exceptions
{

const char* DatabaseServerException::what() const throw()
{
  return "Unknown database server exception.";
}

TupleDoesNotExistException::TupleDoesNotExistException(const Common::SelectDescription& descr)
  : description(descr)
{}

TupleDoesNotExistException::~TupleDoesNotExistException() throw()
{}

const char* TupleDoesNotExistException::what() const throw()
{
  return "Requested tuple does not exist in database.";
}

} // namespace Exceptions

} // namespace Server

