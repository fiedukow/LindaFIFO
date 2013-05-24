#ifndef SERVER_COMMON_FIELDCONDITION_H
#define SERVER_COMMON_FIELDCONDITION_H

#include <string>

#include <boost/logic/tribool.hpp>

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
  enum ConditionType { Less, Equal, Greater, Any };

  virtual ~FieldCondition() = default;

  virtual boost::tribool visit(const IntField&) const = 0;
  virtual boost::tribool visit(const FloatField&) const = 0;
  virtual boost::tribool visit(const StringField&) const = 0;

  virtual FieldCondition* clone() const = 0;

protected:
  FieldCondition(ConditionType conditionType);

  ConditionType conditionType;
};

class IntFieldCondition : public FieldCondition
{
public:
  IntFieldCondition(const int value, ConditionType conditionType);

  virtual boost::tribool visit(const IntField&) const;
  virtual boost::tribool visit(const FloatField&) const;
  virtual boost::tribool visit(const StringField&) const;

  virtual FieldCondition* clone() const;

private:
  const int value;
};

class FloatFieldCondition : public FieldCondition
{
public:
  FloatFieldCondition(const float value, ConditionType conditionType);

  virtual boost::tribool visit(const IntField&) const;
  virtual boost::tribool visit(const FloatField&) const;
  virtual boost::tribool visit(const StringField&) const;

  virtual FieldCondition* clone() const;

private:
  const float value;
};

class StringFieldCondition : public FieldCondition
{
public:
  StringFieldCondition(const std::string& value, ConditionType conditionType);

  virtual boost::tribool visit(const IntField&) const;
  virtual boost::tribool visit(const FloatField&) const;
  virtual boost::tribool visit(const StringField&) const;

  virtual FieldCondition* clone() const;

private:
  const std::string value;
};

} // namespace Common

} // namespace Server

#endif // SERVER_COMMON_FIELDCONDITION_H

