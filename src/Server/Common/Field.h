#ifndef SERVER_COMMON_FIELD_H
#define SERVER_COMMON_FIELD_H

#include <string>

#include <boost/logic/tribool.hpp>

namespace Server
{

namespace Common
{

class FieldCondition;

class Field
{
public:
  virtual ~Field() = default;
  virtual std::string asString() const = 0;

  virtual bool accept(const FieldCondition&) const = 0;
};

class IntField : public Field
{
public:
  IntField(const int value);

  virtual std::string asString() const;

  virtual bool accept(const FieldCondition&) const;

  int getValue() const;

private:
  const int value;
};

class FloatField : public Field
{
public:
  FloatField(const float value);

  virtual std::string asString() const;

  virtual bool accept(const FieldCondition&) const;

  float getValue() const;

private:
  const float value;
};

class StringField : public Field
{
public:
  StringField(const std::string& value);

  virtual std::string asString() const;

  virtual bool accept(const FieldCondition&) const;

  std::string getValue() const;

private:
  const std::string value;
};

} // namespace Common

} // namespace Server

#endif // SERVER_COMMON_FIELD_H

