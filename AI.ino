void BasicMovement(){ //original function to make car stop before a wall
  if (Distances[0] >= 200 || Distances[0] <= 0){ //filter flawed data
    off();
  }
  else if (Distances[0] > 50){ //if the distance is greater than 50 cm move forward
    move("F");
  }
  else if (Distances[0] <= 50 && Distances[0] >= 15 ){ //if the distance is between 50 and 15 cm move slowly
    Slow();
    move("F");
  }
  else if (Distances[0] < 15 && Distances[0] >= 10){ //if the distance is between 10 and 5 cm move stand still
    off();
  }
  else if (Distances[0] < 5){ //if the distance is less than 5 cm move forward slowly
    Slow();
    move("B");
  }
}

void HillDetection(){ //function for hill detection (never teted bacause of lack off time thanks to hardware problems)
  if(event.orientation.y > 10 && event.orientation.y <= 180){
    onHill = true;
    bno_read();
  }
  else if(event.orientation.y > 180 && event.orientation.y < 355){
    onHill = true;
    bno_read();
  }
  else if(onHill){
    //Slow();
    onHill = false;
    afterHill = true;
    bno_read();
  }
}

void CorrectRot(){ //function that keeps the rotation the same as the original (if we could adjust for starting postion the 0 would be replaced by a variable)
  if(event.orientation.x > 10 && event.orientation.x <= 180){ 
    move("Rl");
    rotState = true; //the car stands still while rotating
    bno_read(); ///Read gyro data
  }
  else if(event.orientation.x > 180 && event.orientation.x < 355){
    move("Rr");
    rotState = true; //the car stands still while rotating
    bno_read(); //Read gyro data
  }
  else{
    rotState = false; //the car stands still while rotating
  }
}

String CollisionEvasion(int sensor){
  if (Distances[sensor] < 10 && Distances[sensor] >= 5){ //if distance is between 10 en 5 cm the car is in a good position
    return "";
  }
  else if (Distances[sensor] < 5){ //if distance is less than 5 cm the car is too close to the wall
    Slow(); //slow down
    return "B"; //move back
  }
  else{
    return "Nothing";
  }
}

String CheckSensor(int sensor){
  if (Distances[sensor] < 1000){ //if distance is less than 5 cm --> move
    return "F";
  }
  else if (Distances[sensor] <= 50 && Distances[sensor] >= 10 ){ //if distance is between 50 en 10 cm --> move slowly
    Slow();
    return "F";
  }
  else{
    return "Nothing";
  }
}


void PathFinding(){
  if(OldDistances[0] - 20 > Distances[0]){ //if forward distance decreases forget you moved back
    bool movedB = false;
  }
  else if (CheckSensor(0) == "F" && (OldDistances[0] + 20 < Distances[0])){ //if forward distance increases wait 2s before moving forward
    clockMillis[5] = millis() + period[5];
  }
  else if(CollisionEvasion(0) == "B"){ //move away from wall in front
    move("B");
  }
  else if(CollisionEvasion(1) == "B"){ //move away from wall on the right
    move("L");
  }
  else if(CollisionEvasion(2) == "B"){ //move away from wall on the left
    move("R");
  }
  else if (CollisionEvasion(0) == "" && CheckSensor(1) == "F" && last_movement != "L"){ //move right if blocked in front and you didn't just move left
    Serial.print("R");
    Serial.println("");
    last_movement = "R";
    move("R");
  }
  else if (CollisionEvasion(1) == "" && CheckSensor(2) == "F"){ //move left if blocked from the right and you have space on the left
    Serial.print("L");
    Serial.println("");
    last_movement = "L";
    move("L");
  }
  else if (CollisionEvasion(2) == "" && !(OldDistances[1] + 20 < Distances[1])){ //move back if blocked from the left and the distance on the right doesn't increase
    SerialBT.println("B");
    //last_movement = "B"; 
    bool movedB = true;
    move("B");
  }
  else if (CheckSensor(0) == "F" && !movedB && (millis() >= clockMillis[5])){ //move forward if you can, you didn't move back and 2s have passed
    Serial.print("F");
    Serial.println("");
    last_movement = "F";
    move("F");
  }
  else if (CheckSensor(1) == "F" && last_movement != "L"){ //move right if you can and you didn't just move left
    Serial.print("R");
    Serial.println("");
    last_movement = "R";
    move("R");
  }
  else if (CheckSensor(2) == "F"){ //move left if you can
    Serial.print("L");
    Serial.println("");
    last_movement = "L";
    move("L");
  }
  else if (CheckSensor(0) == "F"){ //dead end probably move back
    Serial.print("B");
    Serial.println("");
    last_movement = "B";
    move("B");
  }
}
