//varshita & yuqi
//adm no: p2603872, p2637950
//class: DEEE/FT/1B/22
//description: Monitoring Healthcare System

#include <Wire.h> // make sure communication between Arduino and I2C devices
#include "RichShieldTM1637.h" // allows TM1637 4-digit 7-segment display to be used
#include "RichShieldNTC.h" // allows NTC temperature sensor to be used
#define NTC_PIN A1 // (this is temperature sensor)
NTC temperature(NTC_PIN);   // definition for temperature -> NTC = sensor type, temperature = name, NTC_PIN = input
//by Yuqi

#define LED_RED 4
#define LED_GREEN 5
#define LED_YELLOW 5
#define BUTTON_K1 8
#define BUTTON_K2 9
#define BUZZER 3
#define CLK 10 //CLK of the TM1637 IC connected to D10 of OPEN-SMART UNO R3
#define DIO 11 
//by Varshita

TM1637 display(CLK,DIO); // definition for display -> TM1637 = display module, display = name, CLK,DIO = communication pins

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  disp.init();//The initialization of the display
  delay(1000);
}
//by Yuqi
hi

void loop() {
  // put your main code here, to run repeatedly:

}
