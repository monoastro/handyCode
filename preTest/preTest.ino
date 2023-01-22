void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  double TestInput[9];
    //raw inputs go here
    //take analog inputs from a0 to a8
    int tem;

    //mapping of each finger to [0, 1]

    //index finger
    //[0]
    tem = analogRead(A0);
    tem = map(tem, 460, 850, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[0] = double(tem)/100;
    //{1}
    tem = analogRead(A5);
    tem = map(tem, 650, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[5] = double(tem)/100;

    //middle finger
    //[0]
    tem = analogRead(A1); 
    tem = map(tem, 350, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[1] = double(tem)/100;
    //[1]
    tem = analogRead(A6);
    tem = map(tem, 650, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[6] = double(tem)/100;

    //ring finger
    //[0]
    tem = analogRead(A2);
    tem = map(tem, 530, 870, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[2] = double(tem)/100;
    //[1]
    tem = analogRead(A7);
    tem = map(tem, 570, 870, 0, 100);
    tem =  constrain(tem, 0, 100);
    TestInput[7] = double(tem)/100;

    //pinky finger
    // [0]
    tem = analogRead(A3);
    tem = map(tem, 300, 770, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[3] = double(tem)/100;
    //[1]
    tem = analogRead(A8);
    tem = map(tem, 570, 870, 0, 100);
    tem =  constrain(tem, 0, 100);
    TestInput[8] = double(tem)/100;

    //thumb - opposite because the magnet is on the other side
    // [0]
    tem = analogRead(A4);
    tem = map(tem, 350, 420, 100, 0);
    tem = constrain(tem, 0, 100);
    TestInput[4] = double(tem)/100;


  Serial.print("[");
  Serial.print(TestInput[0]);
  Serial.print(", ");
  Serial.print(TestInput[1]);
  Serial.print(", ");
  Serial.print(TestInput[2]);
  Serial.print(", ");
  Serial.print(TestInput[3]);
  Serial.print(", ");
  Serial.print(TestInput[4]);
  Serial.print(", ");
  Serial.print(TestInput[5]);
  Serial.print(", ");
  Serial.print(TestInput[6]);
  Serial.print(", ");
  Serial.print(TestInput[7]);
  Serial.print(", ");
  Serial.print(TestInput[8]);
  Serial.println("],");

}
