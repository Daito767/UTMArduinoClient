#pragma once
#ifndef ReceivedMsg_h
#define ReceivedMsg_h

#include "Arduino.h" 
#include <string.h>

class ReceivedMsg {
	public:
		ReceivedMsg();
		void SetMsg(String _msg);
		String GetValue(String key);
		bool IsKeyExist(String key);
	private:
		String msg = "";
		String *keys;
		String *values;
		int countOfKeys;
		void ResetDict(int len);
};
#endif