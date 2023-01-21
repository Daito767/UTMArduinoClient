#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <string.h>
#include <SentMessage.h>
#include <ReceivedMessage.h>
#include <Servo.h>

// const char *ssid = "White Network";
// const char *password = "Jw2XHp1Bh5";

const char *ssid = "Daito";
const char *password = "qvon3478";

// const char *host = "192.168.0.19";
const char *host = "192.168.25.215";
const uint16_t port = 8080;
const uint16_t localPort = 4210;

ESP8266WiFiMulti WiFiMulti;
WiFiUDP Udp;
SentMsg SentMessage("Arduino Wi-Fi 1", "esp12e", "espressif8266");
ReceivedMsg ReceivedMessage;
Servo ServoDor;

unsigned long myTime  = 0;
unsigned long myTimeToAnalogRead = 0;
int AutomaticNightLight= 1;
const int BrightnessForNight = 250;
bool isMotion = false;
unsigned long lastMotionTime = 0;
int ServoDorState = 0;
String msgFromArduinoUno = "";

int PinFotoResistor = 17;
int PinPir = 16;
int PinLed1 = 5;
int PinLed2 = 14;
int PinPirLed = 4;
int PinRelay1 = 0;
int PinRelay2 = 2;
int PinServoDor = 12;


bool RequestConnection();
int UdpSendMsg(String msg);
String UdpReciveMsg();

void setup()
{
	pinMode(PinFotoResistor, INPUT);
	pinMode(PinPir, INPUT);
	pinMode(PinLed1, OUTPUT);
	pinMode(PinPirLed, OUTPUT);
	pinMode(PinRelay1, OUTPUT);
	pinMode(PinRelay2, OUTPUT);
	pinMode(PinLed2, OUTPUT);

	ServoDor.attach(PinServoDor);
	ServoDor.write(0);

	Serial.begin(115200);

	// We start by connecting to a WiFi network
	WiFi.mode(WIFI_STA);
	WiFiMulti.addAP(ssid, password);

	Serial.println();
	Serial.println();
	Serial.print("Wait for WiFi... ");

	while (WiFiMulti.run() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(500);
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address = ");
	Serial.println(WiFi.localIP());

	Udp.begin(localPort);
	Udp.setTimeout(250);

	myTime  = millis();
	myTimeToAnalogRead = millis();

	//RequestConnection();
}

void loop()
{
	if (millis() - myTime  > 1000)
	{
		SentMessage.ClearMsg();
		SentMessage.AddCommand("Luminozitatea", analogRead(PinFotoResistor));
		SentMessage.AddCommand("Miscare", digitalRead(PinPir));
		SentMessage.AddCommand("ButtonTF-Becul 1", digitalRead(PinLed1));
		SentMessage.AddCommand("ButtonTF-Becul 2", digitalRead(PinLed2));
		SentMessage.AddCommand("ButtonTF-Becul de la PIR", digitalRead(PinPirLed));
		SentMessage.AddCommand("ButtonTF-Lumina automata de noapte", AutomaticNightLight);
		SentMessage.AddCommand("ButtonTF-Usa", ServoDorState);
		SentMessage.AddCommand("ButtonTF-Releu 1", digitalRead(PinRelay1));
		SentMessage.AddCommand("ButtonTF-Releu 2", digitalRead(PinRelay2));
		UdpSendMsg(SentMessage.GetStrMsg() + msgFromArduinoUno);

		myTime  = millis();
		myTimeToAnalogRead = millis();
	}

	String msg = UdpReciveMsg();
	// if (msg == "Succes")
	// {
	// 	Serial.println("Iregistrare cu succes la server !");
	// }

	if (msg.length() > 0)
	{
		Serial.println(msg + msg + msg + '%');

		ReceivedMessage.SetMsg(msg);
		if (ReceivedMessage.IsKeyExist("ButtonTF-Becul 1"))
		{
			digitalWrite(PinLed1, ReceivedMessage.GetValue("ButtonTF-Becul 1").toInt());
		}

		if (ReceivedMessage.IsKeyExist("ButtonTF-Becul 2"))
		{
			digitalWrite(PinLed2, ReceivedMessage.GetValue("ButtonTF-Becul 2").toInt());
		}

		if (ReceivedMessage.IsKeyExist("ButtonTF-Lumina automata de noapte"))
		{
			AutomaticNightLight = ReceivedMessage.GetValue("ButtonTF-Lumina automata de noapte").toInt();
		}

		if (ReceivedMessage.IsKeyExist("ButtonTF-Becul de la PIR"))
		{
			digitalWrite(PinPirLed, ReceivedMessage.GetValue("ButtonTF-Becul de la PIR").toInt());
		}

		if (ReceivedMessage.IsKeyExist("ButtonTF-Releu 1"))
		{
			digitalWrite(PinRelay1, ReceivedMessage.GetValue("ButtonTF-Releu 1").toInt());
		}

		if (ReceivedMessage.IsKeyExist("ButtonTF-Releu 2"))
		{
			digitalWrite(PinRelay2, ReceivedMessage.GetValue("ButtonTF-Releu 2").toInt());
		}

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

		myTimeToAnalogRead = millis();
	}

	if (AutomaticNightLight && millis() - myTimeToAnalogRead  > 50)
	{
		if (isMotion && BrightnessForNight > analogRead(PinFotoResistor))
		{
			digitalWrite(PinLed1, HIGH);
		}
		else
		{
			digitalWrite(PinLed1, LOW);
		}

		myTimeToAnalogRead = millis();
	}

	digitalWrite(PinPirLed, digitalRead(PinPir));

	if (digitalRead(PinPir))
	{
		isMotion = true;
		lastMotionTime = millis();
	}
	else if (isMotion == true && millis() - lastMotionTime > 3000)
	{
		isMotion = false;
	}

	if (Serial.available() > 0) {
		msgFromArduinoUno = Serial.readString();
	}
}

int UdpSendMsg(String msg)
{
	Udp.beginPacket(host, port);
	int packetSize = Udp.print(msg);
	Udp.endPacket();

	return packetSize;
}

String UdpReciveMsg()
{
	int packetSize = Udp.parsePacket();
	String msg = "";

	if (packetSize)
	{
		char incomingPacket[256];
		int len = Udp.read(incomingPacket, 255);
		if (len > 0)
		{
			incomingPacket[len] = 0;
		}

		for (size_t i = 0; i < len; i++)
		{
			msg += incomingPacket[i];
		}	
	}

	return msg;
}