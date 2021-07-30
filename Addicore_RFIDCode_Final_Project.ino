#include <Servo.h>
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);
 Servo myservo;  // create servo object to control a servo
          // twelve servo objects can be created on most boards
          int pos = 0;    // variable to store the servo position
          int pressed; //Initialized variable to store recieved data
          
// Example sketch to read the ID from an Addicore 13.56MHz RFID tag
// as found in the RFID AddiKit found at: 
// http://www.addicore.com/RFID-AddiKit-with-RC522-MIFARE-Module-RFID-Cards-p/126.htm

#include <AddicoreRFID.h>
#include <SPI.h>

#define	uchar	unsigned char
#define	uint	unsigned int

uchar fifobytes;
uchar fifoValue;

AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module

/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int chipSelectPin = 10;
const int NRSTPD = 5;

//Maximum length of the array
#define MAX_LEN 16

void setup() {                
   Serial.begin(9600);                        // RFID reader SOUT pin connected to Serial RX pin at 9600bps 

  // start the SPI library:
  SPI.begin();
  
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE pin 
    digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);                     // Set digital pin 10 , Not Reset and Power-down
    digitalWrite(NRSTPD, HIGH);

  myRFID.AddicoreRFID_Init();  

   myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
    if (Serial.available() > 0) {
      pressed = Serial.read(); //Read the serial data and store it 
    }
  
  	uchar i, tmp, checksum1;
	uchar status;
        uchar str[MAX_LEN];
        uchar RC_size;
        uchar blockAddr;	//Selection operation block address 0 to 63
        String mynum = "";

        str[1] = 0x4400;
	//Find tags, return tag type
	status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);	
	if (status == MI_OK)
	{
          Serial.println("RFID tag detected");
          Serial.print("Tag Type:\t\t");
          uint tagType = str[0] << 8;
          tagType = tagType + str[1];
          switch (tagType) {
            case 0x4400:
              Serial.println("Mifare UltraLight");
              break;
            case 0x400:
              Serial.println("Mifare One (S50)");
              break;
            case 0x200:
              Serial.println("Mifare One (S70)");
              break;
            case 0x800:
              Serial.println("Mifare Pro (X)");
              break;
            case 0x4403:
              Serial.println("Mifare DESFire");
              break;
            default:
              Serial.println("Unknown");
              break;
          }
	}

	//Anti-collision, return tag serial number 4 bytes
	status = myRFID.AddicoreRFID_Anticoll(str);
	if(pressed == 90)
    {
                Serial.println(pressed);
                myservo.write(0);
                for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(5);             }          // waits 15ms for the servo to reach the position
                for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
                  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(5);                       // waits 15ms for the servo to reach the position  
                }
    }
	if (status == MI_OK)
	{ 
          checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
          Serial.print("The tag's number is:\t");
    	    Serial.print(str[0]);
            Serial.print(" , ");
    	    Serial.print(str[1]);
            Serial.print(" , ");
    	    Serial.print(str[2]);
            Serial.print(" , ");
    	    Serial.println(str[3]);

          Serial.print("Read Checksum:\t\t");
    	      Serial.println(str[4]);
          Serial.print("Calculated Checksum:\t");
            Serial.println(checksum1);
            
            // Should really check all pairs, but for now we'll just use the first
            if(str[0] == 154)                      //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
            {
                myservo.write(0);
                for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(5);             }          // waits 15ms for the servo to reach the position
                for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
                  myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                  delay(5);                       // waits 15ms for the servo to reach the position  
                }
                 
            } else  {             //You can change this to the first byte of your tag by finding the card's ID through the Serial Monitor
                Serial.println("\nAccess Denied!\n");
            }
            Serial.println();
            delay(1000);
	}
 
  //else
  //{
    
  //}
		
        myRFID.AddicoreRFID_Halt();		   //Command tag into hibernation              

}
