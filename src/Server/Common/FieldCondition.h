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
  virtual ~FieldCondition() = default;

  virtual bool visit(const IntField&) const = 0;
  virtual bool visit(const FloatField&) const = 0;
  virtual bool visit(const StringField&) const = 0;

protected:
  enum ConditionType { Less, Equal, Greater, Any };

  ConditionType conditionType;
};

class IntFieldCondition : public FieldCondition
{
public:
  IntFieldCondition(const int value);

  virtual bool visit(const IntField&) const;
  virtual bool visit(const FloatField&) const;
  virtual bool visit(const StringField&) const;

private:
  const int value;
};

class FloatFieldCondition : public FieldCondition
{
public:
  FloatFieldCondition(const float value);

  virtual bool visit(const IntField&) const;
  virtual bool visit(const FloatField&) const;
  virtual bool visit(const StringField&) const;

private:
  const float value;
};

class StringFieldCondition : public FieldCondition
{
public:
  StringFieldCondition(const std::string& value);

  virtual bool visit(const IntField&) const;
  virtual bool visit(const FloatField&) const;
  virtual bool visit(const StringField&) const;

private:
  const std::string value;
};

} // namespace Common

} // namespace Server

#endif // SERVER_COMMON_FIELDCONDITION_H

