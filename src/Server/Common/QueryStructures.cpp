#include "QueryStructures.h"

#include "Field.h"
#include "FieldCondition.h"

namespace Server
{

namespace Common
{

SelectDescription::SelectDescription(const FieldConditions& conditions)
  : fieldConditions(conditions)
{}

SelectDescription::~SelectDescription()
{
  for (FieldCondition* fc : fieldConditions)
  {
    delete fc;
  }
}

bool SelectDescription::doesTupleMatch(const Tuple& tuple) const
{
  for (Field* field : tuple)
  {
    for (FieldCondition* condition : fieldConditions)
    {
      if (!field->accept(*condition))
        return false;
    }
  }

  return true;
}

InsertDescription::InsertDescription(const Tuple& tuple)
  : tuple(new Tuple(tuple))
{}

InsertDescription::~InsertDescription()
{
  delete tuple;
}

} // namespace Common

} // namespace Server

