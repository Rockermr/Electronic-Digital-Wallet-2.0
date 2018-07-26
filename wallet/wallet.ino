#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <SoftwareSerial.h>
float bal;
SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(4, OUTPUT);
  // sets the digital pin 13 as output
  digitalWrite(4, LOW);
  login();

}

void loop() {


  while (!Serial.available()) {
    //Do Absolutely Nothing until something is received over the serial port
  }

  String options = Serial.readStringUntil('\n');
  if (options == "send" || options == "transfer")
  {
    send();
  }
  else if (options == "receive" || options == "load")
  {
    receive();

  }

  else if (options == "logout")
  {
    login();

  }


}

String send()
{

  while (!Serial.available()) {
    //Do Absolutely Nothing until something is received over the serial port
  }

  String otpamt = Serial.readStringUntil('\n');
  String otp = otpamt.substring(0, 4);

  float amt = ((otpamt.substring(4))).toFloat();
  //Serial.print(amt);
  EEPROM_readAnything(0, bal);
  /*if(amt>bal)
    {
    //Serial.println("");
    return "amountexceeded";
    }Serial.println("ok");*/
  if (otpamt == "cancel")
  {
    return "failure";
  }

  mySerial.println(otp + String(amt));

  while (1)
  {
    while (!mySerial.available()) {
    //Do Absolutely Nothing until something is received over the serial port
  }
    String otp1 = mySerial.readStringUntil('\n');
    if (otp==(otp1.substring(0,4)))
    {
      otp1 = otp1.substring(4, 5);

      if (otp1 == "1")
      {

        bal = bal - amt;
        EEPROM_writeAnything(0, bal);
        delay(200);
        EEPROM_readAnything(0, bal);

        Serial.println("success" + String(bal));



        delay(300);
        break;
      }
      else if (otp1 == "0")
      {
        Serial.println("failure");
        break;


      }
     

    }
  }

  return "transactionsuccess";

}
void receive()
{ randomSeed(analogRead(0));


  int randNumber = random(1000, 9999);
  Serial.println(String(randNumber)+"receiveotp");

  while (1) {
    while (!mySerial.available()) {
      //Do Absolutely Nothing until something is received over the serial port
    }
    String otp = mySerial.readStringUntil('\n');
    //Serial.println(otp);

    String otp1 = otp.substring(0, 4);
    if (otp1.toInt() == randNumber)
    {
      otp1 = otp.substring(4);
      bal = bal + otp1.toFloat();
      //Serial.print(bal);


      if (bal > 30000)
      {
        Serial.println("balanceexceeded");
        delay(200);
        otp = String(randNumber) + '0';
        mySerial.println(otp);
        break;

      }



      else {



        EEPROM_writeAnything(0, bal);
        //     Serial.print ("New Balance");
        // Serial.print (g2);
        Serial.println("amounttransferred" + String(bal));


        otp1 = String(randNumber) + '1';
        mySerial.println(otp1);

        delay(200);
        break;
      }
    }
  }
}


void login()
{
  while (1)
  {

    while (!Serial.available()) {
      //Do Absolutely Nothing until something is received over the serial port
    }

    String PASSWORD = Serial.readStringUntil('\n');
    if (PASSWORD == "1998")
    {
      EEPROM_readAnything(0, bal);

      Serial.println("valid" + String(bal));
      break;
    }
    else
    {
      Serial.println("invalid");


    }
  }

}

