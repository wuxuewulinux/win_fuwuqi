
#include "Handler.hpp"

IHandler::~IHandler()
{

}
/*
int IHandler::OnClientMsg(const SSMsg& rSSMsg, int iFd)
{
	return 0;
}
*/


int IHandler::OnServerMsg(const SSMsg& rSSMsg, int iFd)
{

	return 0;
}