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
  ~SelectDescription();

  const FieldConditions fieldConditions;
};

struct InsertDescription
{
  InsertDescription(const Tuple&);

  Tuple* tuple;
};

} // namespace Common

} // namespace Server

#endif // SERVER_COMMON_QUERY_STRUCTURES_H

