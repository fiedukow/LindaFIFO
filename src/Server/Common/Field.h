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

  virtual void accept(FieldCondition&) = 0;
};

class IntField : public Field
{
public:
  IntField(const int value);

  virtual void accept(FieldCondition&);

  int getValue() const;

private:
  const int value;
};

class FloatField : public Field
{
public:
  virtual void accept(FieldCondition&);

  float getValue() const;

private:
  const float value;
};

class StringField : public Field
{
public:
  virtual void accept(FieldCondition&);

  std::string getValue() const;

private:
  const std::string value;
};

} // namespace Common

} // namespace Server
