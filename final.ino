//scientist BENIELS LAB
/* CREATED BY: HOW TO ELECTRONICS | MODIFIED BY: SCIENTIST BENIELS  LAB */

#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <SoftwareSerial.h>


#define REPORTING_PERIOD_MS     1000

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

int choice;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



byte smile[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
byte mod[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000
};
byte sad[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B01110,
  B10001,
  B00000,
  B00000
};

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{

  //  Serial.println("Beat!!!");

}

void setup()
{
  Serial.begin(9600);

  pinMode(9, INPUT); // Setup for leads off detection LO +
  pinMode(10, INPUT); // Setup for leads off detection LO -

  Serial.println("press 1 for pluse oximeter\npress 2 for temperature\npress 3 for ECG monitoring\nEnter your choice : ");
  while (Serial.available() == 0) {}
  choice = Serial.parseInt();

  if (choice == 1) {

    lcd.begin(16, 4);

    lcd.createChar(1 , smile);
    lcd.createChar(2 , mod);
    lcd.createChar(3 , sad);
    lcd.setCursor(0, 0);
    lcd.print("      Pluse");
    lcd.setCursor(0, 1);
    lcd.print("    Oximeter");
    delay(2000);

    if (!pox.begin()) {
      Serial.println("FAILED");
      for (;;);
    }
    else {
      Serial.println("SUCCESS");
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    pox.setOnBeatDetectedCallback(onBeatDetected);

  }

  if (choice == 2) {

    // initialize serial communication at 9600 bits per second:
    lcd.print("LM35 Test");
    delay(1000);
  }

  if (choice == 3) {
    lcd.print("ECG Test");
    delay(1000);
  }
}

void loop()
{
  if (choice == 1) {
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {

      lcd.clear();
      lcd.setCursor(0 , 0);
      lcd.print("BPM : ");
      lcd.print(pox.getHeartRate());
      lcd.setCursor(0 , 1);
      lcd.print("Sp02: ");
      lcd.print(pox.getSpO2());
      lcd.print("%");
      tsLastReport = millis();

      int sensorValue = analogRead(A0);
      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltage = sensorValue * (5.0 / 1023.0);
      float temperature = voltage * 100;

      // print out the value you read:
      //  Serial.println("Voltage : ");
      //  Serial.println(voltage);
      //  Serial.println("Temperature : ");
     
    //Wait for a bit to keep serial data from saturating
      Serial.print(temperature);
      Serial.print(",");
      Serial.print(pox.getSpO2());
      Serial.print("#");
      Serial.println(analogRead(A1));
      delay(20);
      //      Serial.println(pox.getHeartRate()); //  serial monitor printing

      if (pox.getSpO2() >= 96) {
        lcd.setCursor(15 , 1);
        lcd.write(1);
      }
      else if (pox.getSpO2() <= 95 && pox.getSpO2() >= 91) {
        lcd.setCursor(15 , 1);
        lcd.write(2);
      }
      else if (pox.getSpO2() <= 90) {
        lcd.setCursor(15 , 1);
        lcd.write(3);
      }

    }
  }
  else if (choice == 2) {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    float temperature = voltage * 100;

    // print out the value you read:
    //  Serial.println("Voltage : ");
    //  Serial.println(voltage);
    //  Serial.println("Temperature : ");
    Serial.println(temperature);
    delay(500);

    lcd.setCursor(0, 1);
    lcd.print("T :");
    lcd.print(temperature);
    lcd.print((char)223);
    lcd.print("C");
    delay(500);
  }

  else if (choice == 3) {
    if ((digitalRead(9) == 1) || (digitalRead(10) == 1)) {
      Serial.println('!');
    }
    else {
      // send the value of analog input 0:
      Serial.println(analogRead(A1));
    }
    //Wait for a bit to keep serial data from saturating
    delay(20);
  }
}
