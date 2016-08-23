#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(2,3,4,5,6,7,8);

//loc --> MSB, loc+1,loc+2,loc+3 LSB => Ex: 1234 stored as 1,2,3,4
#define Eread(loc) EEPROM.read(loc)*1000+EEPROM.read(loc+1)*100+EEPROM.read(loc+2)*10+EEPROM.read(loc+3)*1

void Ewrite(int loc,int val){
  if(val<10000)
    {
      EEPROM.write(loc,val/1000);
      val=val%1000;
      EEPROM.write(loc+1,val/100);
      val=val%100;
      EEPROM.write(loc+2,val/10);
      val=val%10;
      EEPROM.write(loc+3,val);
    }
}

const char set=A0,select=A1,incr=A2,decr=A3;
const char cntIn=9,waptIn=10,weptIn=11;
//To preset the starting point/set while setting the count which should run after settings... If you din specify, it continues with prev val
//Also to reset the current count going on if smthg went wrong in the border
//Reset current Count/Set the starting point --> Handled in Settings part
const char resetNstart=12;
const char out=13;

//Temp to store Dynamic count
const char tempCountLoc=200;
int currentCount=Eread(tempCountLoc);

//Temp to store Dynamic Memory set/block
const char tempSetLoc=210;

void setup()
{
  //1,5,9,13,17,21...,77
  if(EEPROM.read(tempSetLoc) > 80)
    EEPROM.write(tempSetLoc,1);
  pinMode(select,INPUT);
  pinMode(set,INPUT);
  pinMode(incr,INPUT);
  pinMode(decr,INPUT);
  pinMode(cntIn,INPUT);
  pinMode(waptIn,INPUT);
  pinMode(weptIn,INPUT);
  pinMode(resetNstart,INPUT);
  //Active LOW for output
  pinMode(out,OUTPUT);

  lcd.begin(16,2);
  wlcmStart();
}

void loop()
{
  lcd.setCursor(0,0);
  lcd.print(" Set ->  Count  ");
  lcd.setCursor(0,1);
  lcd.print("  ");
  //To disp set & its current count hpnd
  lcd.setCursor(2,1);
  lcd.print( (int)(EEPROM.read(tempSetLoc)/4) + 1);
  if( ((EEPROM.read(tempSetLoc)/4) + 1) < 10)
  {
    lcd.setCursor(3,1);
    lcd.print(" ");
  }
  
  lcd.setCursor(4,1);
  lcd.print(" =>   ");
  lcd.setCursor(10,1);
  currentCount = Eread(tempCountLoc);
  if(currentCount < 10)                                 { lcd.print(" ");lcd.print(currentCount);lcd.print("  "); }
  else if(currentCount > 9 && currentCount < 100)       { lcd.print(currentCount);lcd.print("  ");  }
  else if(currentCount > 99 && currentCount < 1000)     { lcd.print(currentCount);lcd.print(" ");   }
  else if(currentCount > 999 && currentCount < 10000)   { lcd.print(currentCount); }
  lcd.setCursor(14,1);
  lcd.print("  ");

  if(digitalRead(cntIn) == LOW)
  {
    //Adjust the delay timing below in order to eliminate conflict with OFF in the middle
    delay(100);
    if(digitalRead(cntIn) == HIGH)
    {
      if(currentCount < Eread(EEPROM.read(tempSetLoc) ))
      {
        ++currentCount;
        Ewrite(tempCountLoc,currentCount);
      }
      else
      {
        //HIGH to switch OFF motor
        digitalWrite(out,HIGH);
        Ewrite(tempCountLoc,0);
        EEPROM.write(tempSetLoc,EEPROM.read(tempSetLoc)+4);
        //Set is max 77 (20 sets)
        if(EEPROM.read(tempSetLoc) > 80)
          EEPROM.write(tempSetLoc,1);
      }
    }
    //Means sensor is not crossing periodically, smthg got stuck
    else
    {
      //HIGH to switch OFF motor
      digitalWrite(out,HIGH);
    }
  }
  
  //clarified... No need to start immediately bcz starting is manual in Project Loom
  if(digitalRead(weptIn) ==  LOW){
    digitalWrite(out,HIGH);//active LOW
  }
  
  if(digitalRead(waptIn) ==  LOW){
    digitalWrite(out,HIGH);//active LOW
  }
  
  while(digitalRead(set) == HIGH){
    settings();
  }
}

//Welcome msg
void start()
{
  char arr1[]={
  "WELCOME TO T.S.K.INDIA ELECTRONICS"}; 
  lcd.home();
  for (int i=0;i<34;i++)
  { 
    lcd.write(arr1[i]);
    delay(300);
  }
  lcd.home();
  for (int i=0;i<24;i++)
  {
    lcd.scrollDisplayLeft();
    delay(500);
  }
  lcd.clear();
  delay(500);
}

void startNoScroll()
{
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0);
  lcd.print("-->WELCOME TO<--");
  char temp[] = {"T.S.K.INDIA LOOM"};
  for(int x=0;x<16;x++)
  {
    lcd.setCursor(x,1);
    lcd.print(temp[x]);
    delay(200);
  }
  delay(1000);
  lcd.clear();
  delay(500);
}

void wlcmStart()
{
  lcd.clear();
  lcd.home();
  lcd.setCursor(0,0);
  lcd.print("-->WELCOME TO<--");
  char temp[] = {"T.S.K.INDIA LOOM"};
  for(int x=0;x<16;x++)
  {
    lcd.setCursor(x,1);
    lcd.print(temp[x]);
    if(x==5)
    {
      lcd.setCursor(0,0);
      lcd.print("   ");
      lcd.setCursor(13,0);
      lcd.print("   ");
      lcd.setCursor(0,0);
      lcd.print("-");
      lcd.setCursor(15,0);
      lcd.print("-");
    }
    if(x==9)
    {
      lcd.setCursor(1,0);
      lcd.print("-");
      lcd.setCursor(14,0);
      lcd.print("-");
    }
    if(x >= 10)
    {
      lcd.setCursor(2,0);
      (x%2)==0?lcd.print(">"):lcd.print(" ");
      lcd.setCursor(13,0);
      (x%2)==0?lcd.print("<"):lcd.print(" ");
    }
    delay(200);
  }
  delay(1000);
  lcd.clear();
  delay(300);
}
