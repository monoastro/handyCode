

void setup() 
{
  // put your setup code here, to run once:
  pinMode(20, INPUT);
  pinMode(21, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  Serial.println(Serial.read(20));  
}
