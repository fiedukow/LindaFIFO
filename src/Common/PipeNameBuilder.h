#ifndef _PIPE_NAME_BUILDER_H_
#define _PIPE_NAME_BUILDER_H_

#include <string> 

std::string getClientToServerPipeName(int lindaId);
std::string getServerToClientPipeName(int lindaId);

#endif
