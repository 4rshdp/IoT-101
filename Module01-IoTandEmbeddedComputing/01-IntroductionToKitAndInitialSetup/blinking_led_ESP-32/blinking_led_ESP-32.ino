
const byte led_GPIO = 2;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(led_GPIO, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(led_GPIO, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(led_GPIO, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
