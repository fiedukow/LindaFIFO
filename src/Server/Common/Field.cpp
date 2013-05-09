#include "Field.h"

#include <string>

#include "FieldCondition.h"

namespace Server
{

namespace Common
{

IntField::IntField(const int value)
  : value(value)
{}

bool IntField::accept(FieldCondition& fc)
{
  return fc.visit(*this);
}

int IntField::getValue() const
{
  return value;
}

/******************************************************************************/

FloatField::FloatField(const float value)
  : value(value)
{}

bool FloatField::accept(FieldCondition& fc)
{
  return fc.visit(*this);
}

float FloatField::getValue() const
{
  return value;
}

/******************************************************************************/

StringField::StringField(const std::string& value)
  : value(value)
{}

bool StringField::accept(FieldCondition& fc)
{
  return fc.visit(*this);
}

std::string StringField::getValue() const
{
  return value;
}

} // namespace Common

} // namespace Server
