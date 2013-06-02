#ifndef SERVER_COMMON_FIELDCONDITION_H
#define SERVER_COMMON_FIELDCONDITION_H

#include <string>

namespace Server
{

namespace Common
{

class IntField;
class FloatField;
class StringField;

class FieldCondition
{
public:
  enum ConditionType { Less, LessEqual, Equal, Greater, GreaterEqual, Any };

  virtual ~FieldCondition() = default;

  virtual bool visit(const IntField&) const = 0;
  virtual bool visit(const FloatField&) const = 0;
  virtual bool visit(const StringField&) const = 0;

  virtual FieldCondition* clone() const = 0;

protected:
  FieldCondition(ConditionType conditionType);

  template <typename T>
  bool compareUsingBuiltInComparators(T fieldValue, T conditionValue) const
  {
    switch (conditionType)
    {
      case Less:
        return (fieldValue < conditionValue);
      case LessEqual:
        return (fieldValue <= conditionValue);
      case Greater:
        return (fieldValue > conditionValue);
      case GreaterEqual:
        return (fieldValue >= conditionValue);
      case Equal:
        return (fieldValue == conditionValue);
      case Any: // type matches
        return true;
    }
  }

  ConditionType conditionType;
};

class IntFieldCondition : public FieldCondition
{
public:
  IntFieldCondition(const int value, ConditionType conditionType);

  virtual bool visit(const IntField&) const;
  virtual bool visit(const FloatField&) const;
  virtual bool visit(const StringField&) const;

  virtual FieldCondition* clone() const;

private:
  const int value;
};

class FloatFieldCondition : public FieldCondition
{
public:
  FloatFieldCondition(const float value, ConditionType conditionType);

  virtual bool visit(const IntField&) const;
  virtual bool visit(const FloatField&) const;
  virtual bool visit(const StringField&) const;

  virtual FieldCondition* clone() const;

private:
  const float value;
};

class StringFieldCondition : public FieldCondition
{
public:
  StringFieldCondition(const std::string& value, ConditionType conditionType);

  virtual bool visit(const IntField&) const;
  virtual bool visit(const FloatField&) const;
  virtual bool visit(const StringField&) const;

  virtual FieldCondition* clone() const;

private:
  const std::string value;
};

} // namespace Common

} // namespace Server

#endif // SERVER_COMMON_FIELDCONDITION_H

