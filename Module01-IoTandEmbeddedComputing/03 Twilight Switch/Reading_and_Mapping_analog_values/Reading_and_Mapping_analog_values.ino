/*
 * Reading Analog values
 */



 void setup(){
  Serial.begin(9600);
 }

 void loop(){
  //Serial.println(analogRead(A0));

  int sensorVal = map(analogRead(A0),0,4095,0,100); //mapping values from 0-4095 to 0-100
  Serial.println(sensorVal);
  delay(500);
 }
