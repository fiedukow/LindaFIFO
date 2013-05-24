#include "Database.h"

#include <cassert>

#include "Common/QueryStructures.h"
#include "Exceptions.h"

namespace Server
{

Common::Tuple Database::read(const Common::SelectDescription& description) const
{
  TuplesCollection::const_iterator iter = getPositionOfTuple(description);
  if (iter == tuplesCollection.cend())
    throw Exceptions::TupleDoesNotExistException(description);

  return *iter;
}

Common::Tuple Database::input(const Common::SelectDescription& description)
{
  TuplesCollection::iterator iter = getPositionOfTuple(description);
  if (iter == tuplesCollection.cend())
    throw Exceptions::TupleDoesNotExistException(description);

  Common::Tuple tuple = *iter;
  removeTupleByPosition(iter);

  return tuple;
}

void Database::output(const Common::InsertDescription& description)
{
  tuplesCollection.insert(description.tuple);
}

std::size_t Database::getTuplesCount() const
{
  return tuplesCollection.size();
}

Database::TuplesCollection::const_iterator Database::getPositionOfTuple(
  const Common::SelectDescription& description) const
{
  auto iter = tuplesCollection.cbegin();
  auto endIter = tuplesCollection.cend();

  for (; iter != endIter; ++iter)
  {
    if (description.doesTupleMatch(*iter))
      return iter;
  }

  return tuplesCollection.cend();
}

void Database::removeTupleByPosition(TuplesCollection::const_iterator position)
{
  assert(position != tuplesCollection.cend()); // iterator has to be valid and in range of collection

  tuplesCollection.erase(position);
}

} // namespace Server

