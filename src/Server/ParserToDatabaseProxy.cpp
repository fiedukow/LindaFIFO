#include "ParserToDatabaseProxy.h"
#include "Database.h"
#include "Exceptions.h"
#include "Common/FieldCondition.h"
#include "Common/Field.h"
#include <Common/MagicMessages.h>
#include <Parser/Parser.hpp>

#include <cassert>

namespace Server
{

ParserToDatabaseProxy::ParserToDatabaseProxy(Database& db)
  : db_(db),
    lastOperationWaiting_(false),
    lastOperationAdded_(false),
    lastOperationAnswer_("")
{}

void ParserToDatabaseProxy::handleOperation(OperationPtr operation)
{
  assert(operation.get() != NULL);
  switch(operation->type)
  {
    case Operation::INPUT:
    case Operation::READ:
      handleSelectOperation(operation);
      break;
    case Operation::OUTPUT:
      handleInsertOperation(operation);
      break;
  }
}

bool ParserToDatabaseProxy::shouldLastOperationWait() const
{
  return lastOperationWaiting_;
}

bool ParserToDatabaseProxy::hasLastOperationAddedElement() const
{
  return lastOperationAdded_;
}

std::string ParserToDatabaseProxy::getLastOperationAnswer() const
{
  return lastOperationAnswer_;
}

void ParserToDatabaseProxy::handleSelectOperation(OperationPtr operation)
{ 
  Common::SelectDescription selectDesc = buildSelectDescription(operation);
  Common::Tuple answer;

  try
  {
    switch(operation->type)
    {
      case Operation::INPUT:
        answer = db_.input(selectDesc);
      break;
      case Operation::READ:
        answer = db_.read(selectDesc);
      break;
      default:
        assert(false);
    }
  }
  catch(Exceptions::TupleDoesNotExistException& e)
  {
    lastOperationWaiting_ = true;
    lastOperationAdded_ = false;
    lastOperationAnswer_ = Linda::Messages::TIMEOUT_MESSAGE;
    return;
  }
  
  lastOperationAnswer_ = serializeAnswer(answer);
  lastOperationWaiting_ = false;
  lastOperationAdded_ = false;
  return;
}

void ParserToDatabaseProxy::handleInsertOperation(OperationPtr operation)
{
  Common::InsertDescription insertDesc = buildInsertDescription(operation);
  db_.output(insertDesc);
  lastOperationWaiting_ = false;
  lastOperationAdded_ = true;
  lastOperationAnswer_ = Linda::Messages::ADDED_MESSAGE;
}

Common::SelectDescription ParserToDatabaseProxy::buildSelectDescription(OperationPtr operation)
{
  Common::SelectDescription::FieldConditions conditions;
  for(Element* element : *(operation->elements))
  {
    Common::FieldCondition* condition;
    Common::FieldCondition::ConditionType type = getConditionType(element);
    switch(element->type)
    {
      case Element::INT:
        condition = new Common::IntFieldCondition(element->int_value, type);
        break;
      case Element::NUM:
        condition = new Common::FloatFieldCondition(element->num_value, type);
        break;
      case Element::STR:
        condition = new Common::StringFieldCondition(element->str_value, type);
        break;
    }
    conditions.push_back(condition);
  }

  return Common::SelectDescription(conditions);
}

Common::FieldCondition::ConditionType ParserToDatabaseProxy::getConditionType(Element* element)
{
  switch(element->constraint)
  {
    case Element::ANY:
      return Common::FieldCondition::Any;
    case Element::LT:
      return Common::FieldCondition::Less;
    case Element::GT:
      return Common::FieldCondition::Greater;      
    case Element::EQ:
      return Common::FieldCondition::Equal;
    case Element::GEQ:
      return Common::FieldCondition::GreaterEqual;
    case Element::LEQ:
      return Common::FieldCondition::LessEqual;
  }
}

Common::InsertDescription ParserToDatabaseProxy::buildInsertDescription(OperationPtr operation)
{
  Common::Tuple tuple;
  for(Element* element : *(operation->elements))
  {
    switch(element->type)
    {
      case Element::INT:
        tuple.push_back(new Common::IntField(element->int_value));
      break;
      case Element::NUM:
        tuple.push_back(new Common::FloatField(element->num_value));
      break;
      case Element::STR:
        tuple.push_back(new Common::StringField(element->str_value));
      break;
    }
  }
  return Common::InsertDescription(tuple);
}

std::string ParserToDatabaseProxy::serializeAnswer(Common::Tuple tuple)
{
  std::string answer = "(";
  bool first = true;
  for(Common::Field* field : tuple)
  {
    if(!first)
      answer += ", ";
    else
      first = false;
    answer += field->asString();    
  }
  answer += ")";
  return answer;
}

}//namespace Server
