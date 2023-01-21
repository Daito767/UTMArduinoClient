#pragma once
#ifndef SentMsg_h
#define SentMsg_h

#include "Arduino.h" 
#include <string.h>

class SentMsg {
	public:
		SentMsg(String id, String board, String platform);
		void ClearMsg();
		void AddCommand(String name, int value);
		void AddCommand(String name, float value);
		void AddCommand(String name, char value);
		void AddCommand(String name, String value);
		String GetStrMsg();

	private:
		String msg = "";
		String _id;
		String _board;
		String _platform;
};
#endif