const int xpin = A10; // x-axis of the accelerometer
const int ypin = A11; // y-axis
const int zpin = A12; // z-axis
 
void setup()
{
Serial.begin(9600);

}
 
void loop()
{
int x = analogRead(xpin); //read from xpin
int y = analogRead(ypin); //read from ypin
int z = analogRead(zpin); //read from zpin

Serial.print(x);
Serial.print(" ");

Serial.print(y);
Serial.print(" ");
//
//Serial.print(z);
//Serial.println();

//digitalWrite(7, HIGH);
//Serial.println(digitalRead(6));




float zero_G = 512.0; //ADC is 0~1023 the zero g output equal to Vs/2
float scale = 102.3; //ADXL335330 Sensitivity is 330mv/g
//330 * 1024/3.3/1000

/*
Serial.print("\t");
Serial.print("X= ");
Serial.print(((float)x - 331.5)/65*9.8); //print x value on serial monitor

Serial.print("\t");
Serial.print("Y= ");
Serial.print(((float)y - 329.5)/68.5*9.8); //print y value on serial monitor

Serial.print("\t");
Serial.print("Z= ");
Serial.print(((float)z - 340)/68*9.8); //print z value on serial monitor
Serial.print("\n");*/
//delay(500); //wait for 1 second 
}
