# L298x library for arduino

> provide some useful function make it easy to control car with DC motors use L298x

````
//LispMotor(left1=2 left2=4 right1=7 right2=8 leftEn=3 rightEn=5)
//it can change speed(0~255) by leftEn and rightEn which must be pwm pin.
LispMotor car (2,4,7,8,3,5);
car.exchangeDir(left, right);
car.control(left_speed, right_speed);
car.brake();
````
