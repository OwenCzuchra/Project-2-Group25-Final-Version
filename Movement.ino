void move(String input){
    if(input == "F" && timer(2) && !moveState && !rotState){ //turn the wheels back on when the timer is done and the car isn't moving or rotating
      //start timing when to turn the wheels off
      clockMillis[0] = millis() + period[0];
      //wait for 50 ms or 25 ms before turning on the wheels
      moveState = true;
      Forward();
    }
    else if(input == "B" && timer(2) && !moveState && !rotState){ //turn the wheels back on when the timer is done and the car isn't moving or rotating
      //start timing when to turn the wheels off
      clockMillis[0] = millis() + period[0];
      //wait for 50 ms or 25 ms before turning on the wheels
      moveState = true;
      Backward();
    }
    else if(input == "R" && timer(2) && !moveState && !rotState){ //turn the wheels back on when the timer is done and the car isn't moving or rotating
      //start timing when to turn the wheels off
      clockMillis[0] = millis() + period[0];
      //wait for 50 ms or 25 ms before turning on the wheels
      moveState = true;
      Right();
    }
    else if(input == "L" && timer(2) && !moveState && !rotState){ //turn the wheels back on when the timer is done and the car isn't moving or rotating
      //start timing when to turn the wheels off
      clockMillis[0] = millis() + period[0];
      //wait for 50 ms or 25 ms before turning on the wheels
      moveState = true;
      Left();
    }
    else if(input == "Rr" && timer(3) && !moveState){ //turn the wheels back on when the timer is done and the car isn't moving or rotating
      //start timing when to turn the wheels off
      clockMillis[0] = millis() + period[0];
      //wait for 50 ms or 25 ms before turning on the wheels
      moveState = true;
      RotateRight();
    }
    else if(input == "Rl" && timer(3) && !moveState){ //turn the wheels back on when the timer is done and the car isn't moving or rotating
      //start timing when to turn the wheels off
      clockMillis[0] = millis() + period[0];
      //wait for 50 ms or 25 ms before turning on the wheels
      moveState = true;
      RotateLeft();
    }
}


void Forward() {
    for(int i = 0; i < 4; i= i+2){
      digitalWrite(driver1[i], HIGH);
      digitalWrite(driver2[i], HIGH);
    }
}

void Backward() {
    for(int i = 1; i < 4; i= i+2){
      digitalWrite(driver1[i], HIGH);
      digitalWrite(driver2[i], HIGH);
    }
}

void Right() { 
    int right1[2] = {0,3};
    int right2[2] = {1,2};
    for(int i = 0; i < 2; i++){
      digitalWrite(driver1[right1[i]], HIGH);
      digitalWrite(driver2[right2[i]], HIGH);
    }
}

void Left() {
    int left1[2] = {1,2};
    int left2[2] = {0,3};
    for(int i = 0; i < 2; i++){
      digitalWrite(driver1[left1[i]], HIGH);
      digitalWrite(driver2[left2[i]], HIGH);
    }
}

void RotateRight() {
    int right1[2] = {0,2};
    int right2[2] = {1,3};
    for(int i = 0; i < 2; i++){
      digitalWrite(driver1[right1[i]], HIGH);
      digitalWrite(driver2[right2[i]], HIGH);
    }
}

void RotateLeft() {
    int left1[2] = {1,3};
    int left2[2] = {0,2};
    for(int i = 0; i < 2; i++){
      digitalWrite(driver1[left1[i]], HIGH);
      digitalWrite(driver2[left2[i]], HIGH);
    }
}

void off(){
  if(moveState){
    moveState = false;
    for(int i = 0; i < 4; i++){
      digitalWrite(driver1[i], LOW);
      digitalWrite(driver2[i], LOW);
    }
  }
}

void Fast(){
    //period[0] = 25;
    speedState = true;
}

void Slow(){
    period[0] = 25;
    speedState = true;
}
