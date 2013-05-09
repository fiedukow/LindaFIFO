#ifndef SERVER_DATABASE_H
#define SERVER_DATABASE_H

#include <vector>

#include "Common/QueryStructures.h" // for Tuple typedef

namespace Server
{

namespace Common
{
  class InsertDescription;
  class SelectDescription;
}

class Database
{
public:
  /**
   * @brief Retrieves one tuple from DB
   * @throw TupleDoesNotExistException
   */
  Common::Tuple read(const Common::SelectDescription&) const;

  /**
   * @brief Retrieves one tuple from DB and removes it
   * @throw TupleDoesNotExistException
   */
  Common::Tuple input(const Common::SelectDescription&);

  /**
   * @brief Puts tuple to DB.
   */
  void output(const Common::InsertDescription&);

private:
  typedef std::vector<Common::Tuple> TuplesCollection;

  TuplesCollection::iterator
    getPositionOfTuple(const Common::SelectDescription&) const;
  void removeTupleByPosition(TuplesCollection::iterator position);

  TuplesCollection tuplesCollection;
};

} // namespace Server

#endif // SERVER_DATABASE_H

