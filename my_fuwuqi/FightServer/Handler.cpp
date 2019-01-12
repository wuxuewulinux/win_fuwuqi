
#include "Handler.hpp"

IHandler::~IHandler()
{

}

int IHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
{
	return 0;
}



int IHandler::OnServerMsg(const SSMsg& rSSMsg, int iFd)
{

	return 0;
}