#ifndef _PARSER_TO_DATABASE_PROXY_
#define _PARSER_TO_DATABASE_PROXY_
#include <memory>
#include "Common/QueryStructures.h"

class Operation;
class Element;
typedef std::shared_ptr<Operation> OperationPtr;

namespace Server
{

class Database;

/**
 * This class is only reentrant, use multiinstances in multithreading
 */
class ParserToDatabaseProxy
{
public:
  ParserToDatabaseProxy(Database& db);

  void handleOperation(OperationPtr operation);
  bool shouldLastOperationWait() const;
  std::string getLastOperationAnswer() const;

private:
  void handleSelectOperation(OperationPtr operation);
  void handleInsertOperation(OperationPtr operation);

  static Common::SelectDescription buildSelectDescription(OperationPtr operation);
  static Common::InsertDescription buildInsertDescription(OperationPtr operation);
  
  static Common::FieldCondition::ConditionType getConditionType(Element* element);
  static std::string serializeAnswer(Common::Tuple tuple);

private:
  Database& db_;
  bool lastOperationWaiting_;
  std::string lastOperationAnswer_;
};

}//namespace Server

#endif 
