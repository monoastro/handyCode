//this can train and run a custom neural network having an architecture consists of one hidden layer only along with input and output layers on a arduino 

#include <math.h>

#define DEBUG

/******************************************************************
   Network Configuration - customized per network
*******************************************************************/

const int numberOfDataSet = 16; 
const int inputNodes = 10;
const int hiddenNodes = inputNodes+1;
const int outputNodes = 2;
const float learningRate = 0.3;
const float Momentum = 0.9;
const float initialWeightMax = 0.5;
const float Success = 0.0015;


//the training data because I can't be bothered to write a file handling system for this

//0 is min value of the sensor and 1 is the max value of the sensor
float Input[numberOfDataSet][inputNodes] = 
{
  { 0, 1, 1, 0 },  // LIGHT ON LEFT AND RIGHT
  { 0, 1, 0, 0 },  // LIGHT ON LEFT
  { 1, 1, 1, 0 },  // LIGHT ON TOP, LEFT, and RIGHT
  { 1, 1, 0, 0 },  // LIGHT ON TOP and LEFT
  { 0, 0, 1, 0 },  // LIGHT ON RIGHT
  { 1, 0, 0, 0 },  // LIGHT ON TOP
  { 0, 0, 0, 0 },  // NO LIGHT
  { 0, 0, 0, 1 },  // LIGHT ON BOTTOM
  { 0, 1, 0, 1 },  // LIGHT ON BOTTOM AND LEFT
  { 0, 0, 1, 1 },  // LIGHT ON BOTTOM AND RIGHT
  { 0, 1, 1, 1 },  // LIGHT ON BOTTOM, LEFT, and RIGHT
  { 1, 0, 0, 1 },  // LIGHT ON TOP AND BOTTOM
  { 1, 1, 0, 1 },  // LIGHT ON TOP, BOTTOM, and LEFT
  { 1, 0, 1, 1 },  // LIGHT ON TOP, BOTTOM, and RIGHT  { 0.65, 0.55 },   
  { 1, 0, 1, 0 },  // LIGHT ON TOP AND RIGHT
  { 1, 1, 1, 1 },  // LIGHT ON ALL
};

const float Target[numberOfDataSet][outputNodes] = 
{
  { 0.65, 0.55 },   
  { 0.75, 0.5 },    
  { 0.2, 0.2 },     
  { 1, 0.2 },       
  { 0.5, 0.75 },    
  { 0.3, 0.3 },    
  { 0.5, 0.5 },   
  { 0.75, 0.75 },
  { 1, 0.75 },
  { 0.75, 1 },
  { 1, 1 },
  { 1, 0 },
  { 1, 0.75 },
  { 0.75, 1 },
  { 0.2, 1 },
  { 0.65, 0.65},
};

/******************************************************************
   End Network Configuration
*******************************************************************/

int i, j, p, q, r;
int reportEvery1000;
int randomizedIndex[numberOfDataSet];
long  trainingCycle;
float Error = 2;
float Accum;

float Hidden[hiddenNodes];
float Output[outputNodes];

float HiddenWeights[inputNodes + 1][hiddenNodes];
float OutputWeights[hiddenNodes + 1][outputNodes];
float HiddenDelta[hiddenNodes];
float OutputDelta[outputNodes];
float ChangeHiddenWeights[inputNodes + 1][hiddenNodes];
float ChangeOutputWeights[hiddenNodes + 1][outputNodes];

void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(A1));       //Collect a random ADC sample for Randomization.
  reportEvery1000 = 1;
  for ( p = 0 ; p < numberOfDataSet ; p++ ) 
  {
    randomizedIndex[p] = p ;
  }
}

void loop() 
{
 toTerminal();
}


//USES TRAINED NEURAL NETWORK TO DRIVE ROBOT
void drive_nn()
{
  Serial.println("Running NN Drive Test");
  
  while (true) 
  {
    int num;
    int farDist = 35;
    int closeDist = 7;
    float TestInput[] = {0, 0, 0, 0};

    //raw inputs go here
    int LL1 = analogRead(A1);   
    int LL2 = analogRead(A2);   
    int LL3 = analogRead(A3);   
    int LL4 = analogRead(A4);

    //turning the inputs to a float between 0 and 1
    LL1 = map(LL1, 400, 1024, 0, 100);
    LL2 = map(LL2, 400, 1024, 0, 100);
    LL3 = map(LL3, 400, 1024, 0, 100);
    LL4 = map(LL4, 400, 1024, 0, 100);

    LL1 = constrain(LL1, 0, 100);
    LL2 = constrain(LL2, 0, 100);
    LL3 = constrain(LL3, 0, 100);
    LL4 = constrain(LL4, 0, 100);

    TestInput[0] = float(LL1) / 100;
    TestInput[1] = float(LL2) / 100;
    TestInput[2] = float(LL3) / 100;
    TestInput[3] = float(LL4) / 100;

#ifdef DEBUG
    Serial.print("Input: ");
    Serial.print(TestInput[3], 2);
    Serial.print("\t");
    Serial.print(TestInput[2], 2);
    Serial.print("\t");
    Serial.print(TestInput[1], 2);
    Serial.print("\t");
    Serial.println(TestInput[0], 2);
#endif

    InputToOutput(TestInput[0], TestInput[1], TestInput[2], TestInput[3]); //INPUT to the function to obtain OUTPUT

    Serial.print(Output[0]);
    Serial.print(" ");
    Serial.println(Output[1]);

    //do whatever with the outputs here
  }
}

//For displaying information
void toTerminal()
{
  for ( p = 0 ; p < numberOfDataSet ; p++ ) 
  {
    Serial.println();
    Serial.print("  Training Pattern: ");
    Serial.println(p);
    Serial.print ("  Input ");
    for ( i = 0 ; i < inputNodes ; i++ ) 
    {
      Serial.print(Input[p][i], DEC);
      Serial.print(" ");
    }
    Serial.print("  Target ");
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
      Serial.print(Target[p][i], DEC);
      Serial.print(" ");
    }

    //Compute hidden layer activations
    for ( i = 0 ; i < hiddenNodes ; i++ ) 
    {
      Accum = HiddenWeights[inputNodes][i] ;
      for ( j = 0 ; j < inputNodes ; j++ ) 
      {
        Accum += Input[p][j] * HiddenWeights[j][i] ;
      }
      //activation function
      Hidden[i] = 1.0 / (1.0 + exp(-Accum));
    }
    
    //Compute output layer activations and calculate errors
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
      Accum = OutputWeights[hiddenNodes][i] ;
      for ( j = 0 ; j < hiddenNodes ; j++ ) 
      {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }
      Output[i] = 1.0 / (1.0 + exp(-Accum)) ;
    }

    Serial.print("  Output ");
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
     Serial.print(Output[i], 5);
     Serial.print(" ");
    }
  }
}

//the function for which this whole circus is established
void InputToOutput(float In1, float In2, float In3, float In4)
{
  float TestInput[] = {0, 0, 0, 0};
  TestInput[0] = In1;
  TestInput[1] = In2;
  TestInput[2] = In3;
  TestInput[3] = In4;

  //Compute hidden layer activations
  for ( i = 0 ; i < hiddenNodes ; i++ ) 
  {
    Accum = HiddenWeights[inputNodes][i];
    for ( j = 0 ; j < inputNodes ; j++ ) 
    {
      Accum += TestInput[j] * HiddenWeights[j][i];
    }
    Hidden[i] = 1.0 / (1.0 + exp(-Accum));
  }

  //Compute output layer activations
  for ( i = 0 ; i < outputNodes ; i++ ) 
  {
    Accum = OutputWeights[hiddenNodes][i];
    for ( j = 0 ; j < hiddenNodes ; j++ ) 
    {
      Accum += Hidden[j] * OutputWeights[j][i];
    }
    Output[i] = 1.0 / (1.0 + exp(-Accum));
  }

#ifdef DEBUG
  Serial.print ("  Output ");
  for ( i = 0 ; i < outputNodes ; i++ ) 
  {
    Serial.print (Output[i], 5);
    Serial.print (" ");
  }
#endif
}
