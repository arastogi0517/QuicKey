// Blynk-Servo.ino
// For M&TSI 2021

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);

// you should get Auth Token in the Blynk App.
// go to the Project Settings (nut icon).
char auth[] = "P-gjZe_T4yzV14POCJDtNpHn2b6H_4vb";

// your WiFi credentials.
// set password to "" for open networks.
char ssid[] = "B6BEF0";
char pass[] = "83267741";

Servo servo; // servo object
int val; // int to store the slider value

BLYNK_WRITE(V3)
{
  val = param.asInt();
  if (val == 1){
    Serial.write(90); // fetch the slider value from the app
  }
  else {
     Serial.write(0);
  }
}

void setup()
{
  // debug console
  Serial.begin(9600);

  // starts the connection with Blynk using the data provided at the top (Wi-Fi connection name, password, and auth token)
  Blynk.begin(auth, ssid, pass);

  // attaches the servo on pin 16 (D0 on NodeMCU) to the servo object
  // the latter two arguments will give us a full 180-degree servo range
  servo.attach(16, 544, 2400); 
}

void loop()
{
  Blynk.run();
  //Serial.println(val); // for debugging
}
