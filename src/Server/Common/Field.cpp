#include "Field.h"

#include <string>

#include "FieldCondition.h"

namespace Server
{

namespace Common
{

int IntField::getValue() const
{
  return value;
}

/******************************************************************************/

float FloatField::getValue() const
{
  return value;
}

/******************************************************************************/

std::string StringField::getValue() const
{
  return value;
}

} // namespace Common

} // namespace Server
