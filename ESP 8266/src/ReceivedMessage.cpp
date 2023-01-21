#include <ReceivedMessage.h>
#include <string.h>


int ClacCountOfelements(char symbol, String text);


ReceivedMsg::ReceivedMsg()
{

}

void ReceivedMsg::SetMsg(String _msg)
{
	msg = _msg;
	countOfKeys = ClacCountOfelements(';', msg);
	ResetDict(countOfKeys);

	int currentCmd = 0;
	bool isKey = true;
	for (size_t i = 0; i < msg.length(); i++)
	{
		if (msg[i] == ';')
		{
			isKey = true;
			currentCmd++;
		}
		else if (msg[i] == ':')
		{
			isKey = false;
		}
		else if (isKey)
		{
			keys[currentCmd] += msg[i];
		}
		else
		{
			values[currentCmd] += msg[i];
		}
	}
}

void ReceivedMsg::ResetDict(int len)
{
	delete keys;
	keys = new String[len];

	delete values;
	values = new String[len];
}

String ReceivedMsg::GetValue(String key)
{
	for (size_t i = 0; i < countOfKeys; i++)
	{
		if (keys[i] == key)
		{
			return values[i];
		}
	}

	return "";
}

bool ReceivedMsg::IsKeyExist(String key)
{
	for (size_t i = 0; i < countOfKeys; i++)
	{
		if (keys[i] == key)
		{
			return true;
		}
	}

	return false;
}

int ClacCountOfelements(char symbol, String text)
{
	int cmdCount = 0;
	for (size_t i = 0; i < text.length(); i++)
	{
		if (text[i] == symbol)
		{
			cmdCount++;
		}	
	}

	return cmdCount;
}