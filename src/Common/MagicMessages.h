#ifndef _MAGIC_MESSAGES_H_
#define _MAGIC_MESSAGES_H_
#include <string>

namespace Linda
{
namespace Messages
{

const std::string TIMEOUT_MESSAGE = "Timeout.";
const std::string ADDED_MESSAGE = "Added tuple to database.";
const std::string INVALID_MESSAGE = "This message is invalid.";
const std::string CLOSE_CONNECTION_REQUEST = "SRV_CLOSE_CONNECTION";

}//namespace Messages
}//namespace Linda

#endif
