#ifndef SERVER_COMMON_QUERY_STRUCTURES_H
#define SERVER_COMMON_QUERY_STRUCTURES_H

#include <memory>
#include <vector>

#include "Field.h"
#include "FieldCondition.h"

namespace Server
{

namespace Common
{

typedef std::vector<Field*> Tuple;

struct SelectDescription
{
  typedef std::vector<FieldCondition*> FieldConditions;

  SelectDescription(const FieldConditions&);
  SelectDescription(const SelectDescription&);
  ~SelectDescription();

  bool doesTupleMatch(const Tuple& tuple) const;

  FieldConditions fieldConditions;
};

struct InsertDescription
{
  InsertDescription(const Tuple&);

  const Tuple tuple;
};

} // namespace Common

} // namespace Server

#endif // SERVER_COMMON_QUERY_STRUCTURES_H

