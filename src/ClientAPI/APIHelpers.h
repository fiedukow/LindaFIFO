#ifndef _API_HELPERS_H_
#define _API_HELPERS_H_
#include <Parser/Parser.hpp>

namespace Linda
{

void handleMagicAnswers(const std::string& answer);
void handleQueryTypeCheck(const std::string& query, Operation::Type requestedType);
std::string generalQueryHandler(const std::string& query);

Operation::Type checkQueryType(const std::string& answer);

}//namespace Linda

#endif
