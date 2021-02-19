void buttonCheck(){
  if(digitalRead(button[0]) == HIGH && !buttonState){
    buttonState = 1;
    Serial.println("test");
  }
   else if(digitalRead(button[0]) == LOW && buttonState){
    buttonState = 0;
    Serial.println("test");
  }
}

void JoyStick() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(vrx);
  yValue = analogRead(vry);
 

  if (xValue > (4095-1500) && !buttonState){
    ChangeSpeed(xValue-(4095-1500));
    move("R");
    Serial.println("RIGHT");
  }
  
  else if (xValue < 1500 && !buttonState){
    ChangeSpeed(xValue - 1500);
    move("L");
    Serial.println("LEFT");
  }

  else if (yValue > (4095-1500) && !buttonState){
    ChangeSpeed(yValue-(4095-1500));
    move("B");
    Serial.println("BOTTOM");
  }

  else if (yValue < 1500 && !buttonState){
    ChangeSpeed(yValue - 1500);
    move("F");
    Serial.println("TOP");
  }
  
  else if (xValue > 4095-1500 && buttonState){
    ChangeSpeed(xValue-(4095-1500));
    move("Rr");
    Serial.println("ROTATE-RIGHT");
  }
  
  else if (xValue < 1500 && buttonState){
    ChangeSpeed(xValue - 1500);
    move("Rl");
    Serial.println("ROTATE-LEFT");
  }
  Serial.println("------------------------");
}

void ChangeSpeed(int value) {
  if (abs(value) > 4095-1500 || abs(value) < 1500){
    Fast();
  }
  else{
    Slow();
  }
  //spd = (int)((spd_max / (float)1700) *  abs(value));
}
