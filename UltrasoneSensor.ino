void echo(){
  for(int i; i < 3; i++){
    int Tijdsduur;
    digitalWrite(trigPins[i], HIGH); //start sending sound wave
    delayMicroseconds(1000);
    digitalWrite(trigPins[i], LOW); //stop sending sound wave
    //Tijdsduur = pulseIn(echoPins[i], HIGH, 100000);
    Tijdsduur = pulseIn(echoPins[i], HIGH); //read how long the sound wave took to come back
    int distance = (Tijdsduur/2) / 29.1; //calculate the distance
    if(distance > 0 && distance < 1000){ //remove flawed readings
      OldDistances[i] = Distances[i]; //save old distance
      Distances[i] = distance; //save new distance
    }
  }
}
