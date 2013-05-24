#include "PipeNameBuilder.h"
#include <sstream>

const std::string lindaPrefix = "LINDA_";
const std::string clientToServerInfix = "CS_";
const std::string serverToClientInfix = "SC_";

std::string getClientToServerPipeName(int lindaId)
{
  std::stringstream ss;
  ss << lindaPrefix << clientToServerInfix << lindaId;
  return ss.str();
}

std::string getServerToClientPipeName(int lindaId)
{
  std::stringstream ss;
  ss << lindaPrefix << serverToClientInfix << lindaId;
  return ss.str();
}


