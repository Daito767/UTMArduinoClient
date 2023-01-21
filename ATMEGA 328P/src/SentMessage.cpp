#include <SentMessage.h>
#include <string.h>

//Seteaza id-ul, platforma si modelul placii pentru a putea fi trimis catre server.
SentMsg::SentMsg()
{
	ClearMsg();
}

void SentMsg::ClearMsg()
{
	msg = "";
}

void SentMsg::AddCommand(String name, int value)
{
	msg += name + ":" + String(value) + ";";
}

void SentMsg::AddCommand(String name, float value)
{
	msg += name + ":" + String(value) + ";";
}

void SentMsg::AddCommand(String name, char value)
{
	msg += name + ":" + String(value) + ";";
}

void SentMsg::AddCommand(String name, String value)
{
	msg += name + ":" + value + ";";
}

String SentMsg::GetStrMsg()
{
	return msg;
}