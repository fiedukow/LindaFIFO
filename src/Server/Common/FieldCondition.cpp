#include "FieldCondition.h"

#include "Field.h"

namespace Server
{

namespace Common
{

IntFieldCondition::IntFieldCondition(const int value)
  : value(value)
{}

bool IntFieldCondition::visit(IntField& field) const
{
  int fieldValue = field.getValue();

  switch (conditionType)
  {
    case Less:
      return (value < fieldValue);
    case Greater:
      return (value > fieldValue);
    case Equal:
      return (value == fieldValue);
    case Any: // type matches
      return true;
  }
}

bool IntFieldCondition::visit(FloatField& /*field*/) const
{
  return false; // float field never matches integral condition
}

bool IntFieldCondition::visit(StringField& /*field*/) const
{
  return false;
}

bool FloatFieldCondition::visit(IntField& /*field*/) const
{
  return false;
}

FloatFieldCondition::FloatFieldCondition(const float value)
  : value(value)
{}

bool FloatFieldCondition::visit(FloatField& field) const
{
  float fieldValue = field.getValue();

  switch (conditionType)
  {
    case Less:
      return (value < fieldValue);
    case Greater:
      return (value > fieldValue);
    case Equal:
      return (value == fieldValue);
    case Any: // type matches
      return true;
  }
}

bool FloatFieldCondition::visit(StringField& /*field*/) const
{
  return false;
}

StringFieldCondition::StringFieldCondition(const std::string& value)
  : value(value)
{}

bool StringFieldCondition::visit(IntField& /*field*/) const
{
  return false;
}

bool StringFieldCondition::visit(FloatField& /*field*/) const
{
 return false;
}

bool StringFieldCondition::visit(StringField& field) const
{
  std::string fieldValue = field.getValue();

  switch (conditionType)
  {
    case Less:
      return (value < fieldValue);
    case Greater:
      return (value > fieldValue);
    case Equal:
      return (value == fieldValue);
    case Any: // type matches
      return true;
  }
}

} // namespace Common

} // namespace Server

