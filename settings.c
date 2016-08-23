void settings()
{
  int ESet = 1;
  //Position in LCD from set,MSB to LSB (Select Single digits - 4 digits)
  int Cursor = 1;
  
  //Holds location of sets [1,5,9,...77]
  int locate = 1;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Count Setting  ");
  delay(1500);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Set ->  Count  ");

  //To disp set & its current count hpnd
  lcd.setCursor(0,1);
  lcd.print("  ");
  lcd.setCursor(2,1);
  lcd.print( locate/4 + 1 );
  if( (locate/4 + 1)  < 10)
  {
    lcd.setCursor(3,1);
    lcd.print(" ");
  }
  lcd.setCursor(4,1);
  lcd.print(" =>   ");
  countDisp(Eread(locate));
  int temp = 0;
  
  while(digitalRead(set) == HIGH)
  {
    if(digitalRead(select) == HIGH){
      delay(500);
      if(Cursor > 5)
        Cursor=1;
      else
        ++Cursor;
//      if(Cursor%5 == 1) {temp = (locate/4)+1;}
    }
    
    if(digitalRead(incr) == HIGH){
      delay(500);
      ++temp;
    }
    if(digitalRead(decr) == HIGH){
      delay(500);
      --temp;
    }
    
    if(Cursor%5 == 1)
    {
      if(temp <= 0 || temp >= 20){
        ESet=1;
      }
      else{
        ESet = temp;
      }
      
      lcd.setCursor(0,1);
      lcd.print("  ");
      lcd.setCursor(2,1);
      lcd.print(ESet);
      blinkCursor();
      if( ESet < 10)
      {
        lcd.setCursor(3,1);
        lcd.print(" ");
      }
      int x = ESet-1;
      //x => [0,1,2,...19] => [1, 1*4+1=5,  2*4+1=9, ... , 20*4+1]
      locate = x*4+1;
      //Display counts according to locate/set
      countDisp(Eread(locate));
    }
    else if(Cursor%5 == 2)
    {
      lcd.setCursor(0,1);
      lcd.print(" 0");
      lcd.print(ESet);
    }
    else if(Cursor%5 == 3)
    {
      lcd.setCursor(0,1);
      lcd.print(" 0");
      lcd.print(ESet);
    }
    else if(Cursor%5 == 4)
    {
      lcd.setCursor(0,1);
      lcd.print(" 0");
      lcd.print(ESet);
    }
    else if(Cursor%5 == 0)
    {
      lcd.setCursor(0,1);
      lcd.print(" 0");
      lcd.print(ESet);
    }
  }
}

//Display Count value
void countDisp(int currentCount)
{
  lcd.setCursor(10,1);
  if(currentCount < 10)                                 { lcd.print(" ");lcd.print(currentCount);lcd.print("  "); }
  else if(currentCount > 9 && currentCount < 100)       { lcd.print(currentCount);lcd.print("  ");  }
  else if(currentCount > 99 && currentCount < 1000)     { lcd.print(currentCount);lcd.print(" ");   }
  else if(currentCount > 999 && currentCount < 10000)   { lcd.print(currentCount); }
  lcd.setCursor(14,1);
  lcd.print("  ");
}

// Blinking cursor
void blinkCursor()
{
  lcd.blink();
  delay(50);
  lcd.noBlink();
}
