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

SelectDescription::SelectDescription(const SelectDescription& descr)
{ // copy each FieldCondition, to avoid problems with double delete
  for (FieldCondition* fc : descr.fieldConditions)
  {
    fieldConditions.push_back(fc->clone());
  }
}

SelectDescription::~SelectDescription()
{
  for (FieldCondition* fc : fieldConditions)
  {
    delete fc;
  }
}

bool SelectDescription::doesTupleMatch(const Tuple& tuple) const
{
  if (tuple.size() != fieldConditions.size())
    return false;
  auto iter = tuple.begin();
  auto condIter = fieldConditions.begin();
  for (; iter != tuple.end() && condIter != fieldConditions.end(); ++iter, ++condIter)
  {
    Field* field = *iter;
    FieldCondition* condition = *condIter;
    if (!field->accept(*condition))
      return false;
  }

  return true;
}

InsertDescription::InsertDescription(const Tuple& tuple)
  : tuple(tuple)
{}

} // namespace Common

} // namespace Server

