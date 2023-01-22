void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float tem = analogRead(A0);
  Serial.println(tem);
  tem = map(tem, 570, 870, 0, 100);
  tem =  constrain(tem, 0, 100);
  //Serial.println(double(tem)/100);
}
