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

bool IntFieldCondition::visit(const IntField& field) const
{
  int fieldValue = field.getValue();
  return compareUsingBuiltInComparators(fieldValue, value);
}

bool IntFieldCondition::visit(const FloatField& /*field*/) const
{
  return false;
}

bool IntFieldCondition::visit(const StringField& /*field*/) const
{
  return false;
}

FieldCondition* IntFieldCondition::clone() const
{
  return new IntFieldCondition(*this);
}

FloatFieldCondition::FloatFieldCondition(const float value, ConditionType conditionType)
  : FieldCondition(conditionType),
    value(value)
{}

bool FloatFieldCondition::visit(const IntField& /*field*/) const
{
  return false;
}

bool FloatFieldCondition::visit(const FloatField& field) const
{
  float fieldValue = field.getValue();

  return compareUsingBuiltInComparators(fieldValue, value);
}

FieldCondition* FloatFieldCondition::clone() const
{
  return new FloatFieldCondition(*this);
}

bool FloatFieldCondition::visit(const StringField& /*field*/) const
{
  return false;
}

StringFieldCondition::StringFieldCondition(const std::string& value, ConditionType conditionType)
  : FieldCondition(conditionType),
    value(value)
{}

bool StringFieldCondition::visit(const IntField& /*field*/) const
{
  return false;
}

bool StringFieldCondition::visit(const FloatField& /*field*/) const
{
  return false;
}

bool StringFieldCondition::visit(const StringField& field) const
{
  std::string fieldValue = field.getValue();
  return compareUsingBuiltInComparators(fieldValue, value);
}

FieldCondition* StringFieldCondition::clone() const
{
  return new StringFieldCondition(*this);
}

} // namespace Common

} // namespace Server

