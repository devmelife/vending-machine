#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define INTERVAL_MESSAGE1 5000
#define INTERVAL_MESSAGE2 7000
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile int  coinscredits = 0;
int coinspulse = 0;


int j = A3;
int directionOne = 8;
int directionTwo = 9;
int speedcontrol = 10;
int Pushbutton = 5;
int directionvalue = 2;//stop default


//price
int ballpen = 10;
int paper = 1;

boolean notpress = HIGH;
boolean value = LOW;
int val2 ;





unsigned long time_1 = 0;
unsigned long time_2 = 0;



int counter = 0 ;
void setup()
{
  Serial.begin(9600); // set up Serial library at 9600 bps

  pinMode(j, INPUT_PULLUP);
  pinMode(speedcontrol, OUTPUT);
  pinMode(directionOne, OUTPUT);
  pinMode(directionTwo, OUTPUT);
  pinMode (Pushbutton, INPUT_PULLUP);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Initialization..");

  pinMode(3, INPUT_PULLUP); //coinslot
  attachInterrupt(1, coinInserted, FALLING); //coinslot
  delay(3000);
  coinscredits = 0;


}

void loop()
{


  value = digitalRead(Pushbutton); //Reads the status of the pushbutton


  if (coinspulse == 1) //coinslot pulse
  {
    coinspulse = 0;
    lcdprint(coinscredits);
  }

  if (coinscredits == 0) //coinslot insertcoin
  {
    lcddefault(ballpen, paper);
  }
  else if (coinscredits > 100)
  {
    lcderror();
  }



  if ( value != notpress )
  {
    if (coinscredits >= paper && value == LOW  )
    {
      coinscredits -= paper;
      Serial.println("total credits");
      Serial.println(coinscredits );
      lcdprint(coinscredits);
      trial();
      delay(100);

    }
    notpress = value;
  }



}
void trial()
{
  counter = 2;

  for (int x = 1; x <= counter; x++)
  {
    motorcontroller(1);
    delay(100);

    bool out = false;
    while (!out)
    {
      val2 = analogRead(j); // read the value from the sensor
      Serial.println(val2);

      if ( analogRead(j) < 200)
      {
        delay(100);
        motorcontroller(0);
        directionvalue = 0;
        delay(100);

      }
      else if (directionvalue == 0 && (val2 > 200) )
      {
        delay(500);
        motorcontroller(2);
        directionvalue = 2;
        out = true;
      }//endmotor

    }

  }
  counter = 0;
}

void motorcontroller (int x)
{
  switch (x)
  {
    case 0:
      analogWrite(speedcontrol, 250);
      digitalWrite(directionOne, LOW);
      digitalWrite(directionTwo, HIGH);
      break;
    case 1:
      analogWrite(speedcontrol, 250);
      digitalWrite(directionOne, HIGH);
      digitalWrite(directionTwo, LOW);
      break;
    case 2:
      analogWrite(speedcontrol, 0);
      break;
  }

}
void coinInserted()
{
  coinscredits = coinscredits + 1;
  coinspulse = 1;

}

void lcdprint(int y)
{
  lcd.clear();
  lcd.print("Credits:");
  lcd.print(y);
}
void lcddefault(int a, int b)
{

  if (millis() > time_1 + INTERVAL_MESSAGE1) {
    time_1 = millis();
    lcd.clear();
    lcd.print("insert coin!");
  }

  if (millis() > time_2 + INTERVAL_MESSAGE2) {
    time_2 = millis();
    lcd.clear();
    lcd.print("ballpen=P");
    lcd.print(a);
    lcd.setCursor(0, 1);
    lcd.print("paper=P");
    lcd.print(b);

  }
}
void lcderror()
{
  lcd.clear();
  lcd.print("ERROR");
  lcd.setCursor(0, 1);
  lcd.print("PLEASE RESET");
  delay(5000);

}
