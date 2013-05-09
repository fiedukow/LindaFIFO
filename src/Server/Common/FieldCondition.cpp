#include "FieldCondition.h"

#include "Field.h"

namespace Server
{

namespace Common
{

FieldCondition::FieldCondition(ConditionType conditionType)
  : conditionType(conditionType)
{}

IntFieldCondition::IntFieldCondition(const int value, ConditionType conditionType)
  : FieldCondition(conditionType),
    value(value)
{}

boost::logic::tribool IntFieldCondition::visit(const IntField& field) const
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

boost::logic::tribool IntFieldCondition::visit(const FloatField& /*field*/) const
{
  return boost::logic::indeterminate;
}

boost::logic::tribool IntFieldCondition::visit(const StringField& /*field*/) const
{
  return boost::logic::indeterminate;
}

FloatFieldCondition::FloatFieldCondition(const float value, ConditionType conditionType)
  : FieldCondition(conditionType),
    value(value)
{}

boost::logic::tribool FloatFieldCondition::visit(const IntField& /*field*/) const
{
  return boost::logic::indeterminate;
}

boost::logic::tribool FloatFieldCondition::visit(const FloatField& field) const
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

boost::logic::tribool FloatFieldCondition::visit(const StringField& /*field*/) const
{
  return boost::logic::indeterminate;
}

StringFieldCondition::StringFieldCondition(const std::string& value, ConditionType conditionType)
  : FieldCondition(conditionType),
    value(value)
{}

boost::logic::tribool StringFieldCondition::visit(const IntField& /*field*/) const
{
  return boost::logic::indeterminate;
}

boost::logic::tribool StringFieldCondition::visit(const FloatField& /*field*/) const
{
  return boost::logic::indeterminate;
}

boost::logic::tribool StringFieldCondition::visit(const StringField& field) const
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

