#include "Database.h"

#include <cassert>

#include "Common/QueryStructures.h"
#include "Exceptions.h"

namespace Server
{

Common::Tuple Database::read(const Common::SelectDescription& description) const
{
  TuplesCollection::const_iterator iter = getPositionOfTuple(description);
  if (iter == tuplesCollection.end())
    throw Exceptions::TupleDoesNotExistException(description);

  return *iter;
}

Common::Tuple Database::input(const Common::SelectDescription& description)
{
  TuplesCollection::iterator iter = getPositionOfTuple(description);
  if (iter == tuplesCollection.end())
    throw Exceptions::TupleDoesNotExistException(description);

  Common::Tuple tuple = *iter;
  removeTupleByPosition(iter);

  return tuple;
}

void Database::output(const Common::InsertDescription& description)
{
  // FIXME implement this
}

Database::TuplesCollection::iterator Database::getPositionOfTuple(
  const Common::SelectDescription& description) const
{
  // FIXME implement this
}

void Database::removeTupleByPosition(TuplesCollection::iterator position)
{
  assert(position != tuplesCollection.end()); // iterator has to be valid and in range of collection

  tuplesCollection.erase(position);
}

} // namespace Server

