#include <SentMessage.h>
#include <ReceivedMessage.h>
#include <Servo.h>

SentMsg SentMessage;
ReceivedMsg ReceivedMessage;

unsigned long myTime  = 0;
Servo ServoDor;
int ServoDorState = 0;
int PinServoDor = 12; 

void setup()
{
	ServoDor.attach(PinServoDor);
	ServoDor.write(0);

	Serial.begin(115200);
	myTime = millis();
}

void loop()
{	
	SentMessage.ClearMsg();
	SentMessage.AddCommand("ButtonTF-Usa", ServoDorState);

	if (Serial.available() > 0) {
		ReceivedMessage.SetMsg(Serial.readString());

		if (ReceivedMessage.IsKeyExist("ButtonTF-Usa"))
		{
			ServoDorState = ReceivedMessage.GetValue("ButtonTF-Usa").toInt();

			if (ServoDorState)
			{
				ServoDor.write(180);
			}
			else
			{
				ServoDor.write(0);
			}
		}
	}

	if (millis() - myTime > 3000)
	{
		Serial.println(SentMessage.GetStrMsg());
		myTime = millis();
	}
	
}
