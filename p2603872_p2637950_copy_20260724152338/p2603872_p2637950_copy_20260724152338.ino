//varshita & yuqi
//adm no: p2603872, p2637950
//class: DEEE/FT/1B/22
//description: Monitoring Healthcare System
//everything in between of the names - declaration of effort

//by Yuqi
#include <Wire.h> // make sure communication between Arduino and I2C devices
#include "RichShieldTM1637.h" // allows TM1637 4-digit 7-segment display to be used
#include "RichShieldNTC.h" // allows NTC temperature sensor to be used
#include "RichShieldPassiveBuzzer.h" // allows RichShield buzzer to be used

#define KNOB_PIN A0
#define NTC_PIN A1 // (this is temperature sensor)
NTC temper(NTC_PIN);   // definition for temperature -> NTC = sensor type, temperature = name, NTC_PIN = input
//by Yuqi

//by Varshita
#define LED_RED 4
#define LED_GREEN 5
#define LED_YELLOW 7 //changed by Yuqi
#define BUTTON_K1 8
#define BUTTON_K2 9
#define BUZZER 3
PassiveBuzzer buz(BUZZER);
#define CLK 10 //CLK of the TM1637 IC connected to D10 of OPEN-SMART UNO R3 - TM1637 Clock Pin
#define DIO 11 //TM1637 Data Pin
//by Varshita

//by Yuqi
TM1637 disp(CLK,DIO); // definition for display -> TM1637 = display module, display = name, CLK,DIO = communication pins - by Yuqi

int temperatureWarningStopped = 0;
int showTime();
int showTemperature(); 
void lightBlink(int pinLight, int onTime, int offTime);
void buzzerBlink(int onTime, int offTime);
void lightUp(int pinLight, int On); 
int knobValue;
int knobReadings[7];
//by Yuqi

//by Varshita
int PIN_LIGHT[3] // array of LED lights 
{
  LED_RED,LED_GREEN,LED_YELLOW
};
//by Varshita

//by Yuqi
int BUTTON[2] //array of Buttons
{
  BUTTON_K1, BUTTON_K2
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //by Yuqi

  //by Varshita
  pinMode(PIN_LIGHT[0], OUTPUT);
  pinMode(PIN_LIGHT[1], OUTPUT);
  pinMode(PIN_LIGHT[2], OUTPUT);
  //by Varshita

  //by Yuqi
  pinMode(BUTTON[0], INPUT_PULLUP);
  pinMode(BUTTON[1], INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  disp.init(); //The initialization of the display
  delay(100);
  knobValue = showTime();
  showTemperature(); 
}
//by Yuqi

//by Varshita
void loop() 
{
  // put your main code here, to run repeatedly:
  lightUp (PIN_LIGHT[1], 1 );
  lightUp (PIN_LIGHT[2], 0 );
  lightUp (PIN_LIGHT[0], 0 );
  //by Varshita
  
  //by Yuqi
  int temperature = showTemperature();

  if ( temperature >= 40 && temperatureWarningStopped == 0)
  {
    while(digitalRead(BUTTON[0]) == HIGH)
    {
      digitalWrite(PIN_LIGHT[1], LOW);
      lightBlink(PIN_LIGHT[0], 200, 70);
      buzzerBlink(300, 30);
    }
    temperatureWarningStopped = 1;
    lightUp (PIN_LIGHT[0], 0 );
    lightUp (PIN_LIGHT[1], 1 );
  }
  //by Yuqi

  //by Varshita
  else
  {
    switch(state) //controls blue led of diff humidity levels gives state based on the humidity
  {
    case 1:
      lightUp (PIN_LIGHT[3], 0 );  //default humidity is normal 
      break;

    case 2:
      lightBlink(PIN_LIGHT[3], 1000, 700);  // too dry = lights up once
      break;

    case 3:
      lightUp (PIN_LIGHT[3], 1 ); //too damp = light up continously
      break;
  }
  }
  {
    digitalWrite(PIN_LIGHT[1], HIGH);
    delay((unsigned long)knobValue * 1000); //changed by Yuqi
    while (digitalRead(BUTTON[0]) == HIGH)
  {
    lightUp (PIN_LIGHT[1], 0 );
    lightUp (PIN_LIGHT[2], 1);
    buzzerBlink(1000, 1000);
  }
  }

//by Varshita

//by Yuqi

int showTemperature() // THis allows temperature to be seen
{
  float celsius = temper.getTemperature(); // Read temperature from sensor
  int temperature = (int)celsius; // Change decimal temperature into a whole number
  int8_t temp[4]; // Array for the four display positions

  if (temperature <= 10 || temperature >= 67)
  {
    temp[0] = INDEX_NEGATIVE_SIGN;
    temp[1] = INDEX_NEGATIVE_SIGN;
    temp[2] = INDEX_NEGATIVE_SIGN;
    temp[3] = INDEX_NEGATIVE_SIGN;
  }
  // Prevent temperatures that doesnt blong to a human
  else
  {
    temp[0] = INDEX_BLANK; // We are not going into negatives, so the first position is blank
    temp[1] = temperature / 10; // Show tens digit
    temp[2] = temperature % 10; // Show ones digit
    temp[3] = 12; // Index of 'C' for celsius degree symbol
  }

  disp.display(temp);
  delay(1000);
  // Display temperature 
  return temperature; // Give temperture to main program
}

int showTime()
{
  while (digitalRead(BUTTON[1]) == HIGH) // keep adjusting until K2 is pressed
  {
    int8_t time[4]; 
    int total = 0;
    for (int i = 0; i < 7; i++)
    {
      knobReadings[i] = analogRead(KNOB_PIN);
      total = total + knobReadings[i];
      delay(100);
    }
    knobValue = total / 7; // Calculate average
    if (knobValue <= 72)
    {
      time[0] = (knobValue / 100) % 10; // looks like S
      time[1] = (knobValue / 10) % 10;
      time[2] = knobValue % 10;
      time[3] = 5;
      disp.display(time);
      delay(30);
    }
    else
    {
      time[0] = INDEX_NEGATIVE_SIGN;
      time[1] = INDEX_NEGATIVE_SIGN;
      time[2] = INDEX_NEGATIVE_SIGN;
      time[3] = INDEX_NEGATIVE_SIGN;
      disp.display(time);
      delay(30);
    }
  }
  delay(100); // prevents K1 from being detected twice
  while (digitalRead(BUTTON[1]) == LOW)
  {
    delay(10);
  }
  delay(30);

  return knobValue;
}

void lightBlink(int pinLight, int onTime, int offTime) // This allows the lights to be on, set up for all the lights
{
  digitalWrite(pinLight, HIGH); // Turn LED on
  delay(onTime); // On duration
  digitalWrite(pinLight, LOW); // Turn LED off
  delay(offTime);//Off duration
}
//by Yuqi

//by Varshita
void buzzerBlink(int onTime, int offTime) // This allows the buzzer to be on, can loop
{
  buz.playTone(1000, onTime); // Play buzzer at 1000 Hz for the given time
  // Buzzer is silent during this delay
  delay(offTime);
}

void lightUp (int pinLight, int On) //turning off or on the light- without blink - without time
{
  if (On == 1)
  {
    digitalWrite(pinLight, HIGH);
  }
  else
  {
    digitalWrite(pinLight, LOW);
  }
}
//by Varshita
