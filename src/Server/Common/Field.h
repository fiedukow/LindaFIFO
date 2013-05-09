#ifndef SERVER_COMMON_FIELD_H
#define SERVER_COMMON_FIELD_H

#include <string>

namespace Server
{

namespace Common
{

class FieldCondition;

class Field
{
public:
  virtual ~Field() = default;

  virtual bool accept(FieldCondition&) = 0;
};

class IntField : public Field
{
public:
  IntField(const int value);

  virtual bool accept(FieldCondition&);

  int getValue() const;

private:
  const int value;
};

class FloatField : public Field
{
public:
  FloatField(const float value);

  virtual bool accept(FieldCondition&);

  float getValue() const;

private:
  const float value;
};

class StringField : public Field
{
public:
  StringField(const std::string& value);

  virtual bool accept(FieldCondition&);

  std::string getValue() const;

private:
  const std::string value;
};

} // namespace Common

} // namespace Server

#endif // SERVER_COMMON_FIELD_H

