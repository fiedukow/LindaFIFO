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

boost::tribool IntField::accept(const FieldCondition& fc) const
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

boost::tribool FloatField::accept(const FieldCondition& fc) const
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

boost::tribool StringField::accept(const FieldCondition& fc) const
{
  return fc.visit(*this);
}

std::string StringField::getValue() const
{
  return value;
}

} // namespace Common

} // namespace Server
