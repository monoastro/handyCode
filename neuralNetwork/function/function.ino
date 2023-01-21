//this can train and run a custom but very simple neural network on a arduino 
//the architecture consists of one hidden layer only along with input and output layers

#include <math.h>

#define DEBUG

/******************************************************************
   Network Configuration - customized per network
*******************************************************************/

const int PatternCount = 16;
const int InputNodes = 4;
const int HiddenNodes = 5;
const int OutputNodes = 2;
const float LearningRate = 0.3;
const float Momentum = 0.9;
const float InitialWeightMax = 0.5;
const float Success = 0.0015;


//the training data because I can't be bothered to write a file handling system for this

//0 is min value of the sensor and 1 is the max value of the sensor
float Input[PatternCount][InputNodes] = 
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

const float Target[PatternCount][OutputNodes] = 
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
int ReportEvery1000;
int RandomizedIndex[PatternCount];
long  TrainingCycle;
float Rando;
float Error = 2;
float Accum;

float Hidden[HiddenNodes];
float Output[OutputNodes];

float HiddenWeights[InputNodes + 1][HiddenNodes];
float OutputWeights[HiddenNodes + 1][OutputNodes];
float HiddenDelta[HiddenNodes];
float OutputDelta[OutputNodes];
float ChangeHiddenWeights[InputNodes + 1][HiddenNodes];
float ChangeOutputWeights[HiddenNodes + 1][OutputNodes];

void setup() 
{
  Serial.begin(115200);
  randomSeed(analogRead(A1));       //Collect a random ADC sample for Randomization.
  ReportEvery1000 = 1;
  for ( p = 0 ; p < PatternCount ; p++ ) {
    RandomizedIndex[p] = p ;
  }
}


void loop() 
{
//  train_nn();
  drive_nn();
}


//TRAINS THE NEURAL NETWORK
void train_nn() 
{
  /******************************************************************
    Initialize HiddenWeights and ChangeHiddenWeights
  ******************************************************************/
  for ( i = 0 ; i < HiddenNodes ; i++ ) 
  {
    for ( j = 0 ; j <= InputNodes ; j++ ) 
    {
      ChangeHiddenWeights[j][i] = 0.0 ;
      Rando = float(random(100)) / 100;
      HiddenWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }
  /******************************************************************
    Initialize OutputWeights and ChangeOutputWeights
  ******************************************************************/
  for ( i = 0 ; i < OutputNodes ; i ++ ) 
  {
    for ( j = 0 ; j <= HiddenNodes ; j++ ) 
    {
      ChangeOutputWeights[j][i] = 0.0 ;
      Rando = float(random(100)) / 100;
      OutputWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }

  //Serial.println("Initial/Untrained Outputs: ");
  //toTerminal();
  /******************************************************************
    Begin training
  ******************************************************************/

  for ( TrainingCycle = 1 ; TrainingCycle < 2147483647 ; TrainingCycle++) 
  {
    /******************************************************************
      Randomize order of training patterns
    ******************************************************************/

    for ( p = 0 ; p < PatternCount ; p++) 
    {
      q = random(PatternCount);
      r = RandomizedIndex[p] ;
      RandomizedIndex[p] = RandomizedIndex[q] ;
      RandomizedIndex[q] = r ;
    }
    Error = 0.0 ;
    /******************************************************************
      Cycle through each training pattern in the randomized order
    ******************************************************************/
    for ( q = 0 ; q < PatternCount ; q++ ) 
    {
      p = RandomizedIndex[q];
      /******************************************************************
        Compute hidden layer activations
      ******************************************************************/

      for ( i = 0 ; i < HiddenNodes ; i++ ) {
        Accum = HiddenWeights[InputNodes][i] ;
        for ( j = 0 ; j < InputNodes ; j++ ) {
          Accum += Input[p][j] * HiddenWeights[j][i] ;
        }
        Hidden[i] = 1.0 / (1.0 + exp(-Accum)) ; //sigmoid 
      }

      /******************************************************************
        Compute output layer activations and calculate errors
      ******************************************************************/

      for ( i = 0 ; i < OutputNodes ; i++ ) 
      {
        Accum = OutputWeights[HiddenNodes][i] ;
        for ( j = 0 ; j < HiddenNodes ; j++ ) {
          Accum += Hidden[j] * OutputWeights[j][i] ;
        }
        Output[i] = 1.0 / (1.0 + exp(-Accum)) ;
        OutputDelta[i] = (Target[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]) ;
        Error += 0.5 * (Target[p][i] - Output[i]) * (Target[p][i] - Output[i]) ;
      }
      //Serial.println(Output[0]*100);

      /******************************************************************
        Backpropagate errors to hidden layer
      ******************************************************************/
      for ( i = 0 ; i < HiddenNodes ; i++ ) 
      {
        Accum = 0.0 ;
        for ( j = 0 ; j < OutputNodes ; j++ ) {
          Accum += OutputWeights[i][j] * OutputDelta[j] ;
        }
        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]) ;
      }

      /******************************************************************
        Update Inner-->Hidden Weights
      ******************************************************************/

      for ( i = 0 ; i < HiddenNodes ; i++ ) 
      {
        ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i] ;
        HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i] ;
        for ( j = 0 ; j < InputNodes ; j++ ) {
          ChangeHiddenWeights[j][i] = LearningRate * Input[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
          HiddenWeights[j][i] += ChangeHiddenWeights[j][i] ;
        }
      }
      /******************************************************************
        Update Hidden-->Output Weights
      ******************************************************************/
      for ( i = 0 ; i < OutputNodes ; i ++ ) 
      {
        ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i] ;
        OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i] ;
        for ( j = 0 ; j < HiddenNodes ; j++ ) {
          ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i] ;
          OutputWeights[j][i] += ChangeOutputWeights[j][i] ;
        }
      }

    }

    /******************************************************************
      Every 1000 cycles send data to terminal for display 
    ******************************************************************/
    ReportEvery1000 = ReportEvery1000 - 1;
    if (ReportEvery1000 == 0)
    {
      Serial.println();
      Serial.println();
      Serial.print ("TrainingCycle: ");
      Serial.print (TrainingCycle);
      Serial.print ("  Error = ");
      Serial.println (Error, 5);

      toTerminal();

      if (TrainingCycle == 1)
      {
        ReportEvery1000 = 999;
      }
      else
      {
        ReportEvery1000 = 1000;
      }
    }
    
    /******************************************************************
      If error rate is less than pre-determined threshold then end
    ******************************************************************/
    if ( Error < Success ) break ;
  }
}

//USES TRAINED NEURAL NETWORK TO DRIVE ROBOT
void drive_nn()
{
  Serial.println("Running NN Drive Test");
  if (Success < Error) 
  {
    Serial.println("NN not Trained");
    return;
  }

  while (Error < Success) 
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

#ifdef DEBUG
    Serial.print("Light Level: ");
    Serial.print(LL1);
    Serial.print("\t");
    Serial.print(LL2);
    Serial.print("\t");
    Serial.print(LL3);
    Serial.print("\t");
    Serial.println(LL4);
#endif

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
    delay(50);
  }
}

//DISPLAYS INFORMATION WHILE TRAINING
void toTerminal()
{
  for ( p = 0 ; p < PatternCount ; p++ ) {
    Serial.println();
    Serial.print ("  Training Pattern: ");
    Serial.println (p);
    Serial.print ("  Input ");
    for ( i = 0 ; i < InputNodes ; i++ ) {
      Serial.print (Input[p][i], DEC);
      Serial.print (" ");
    }
    Serial.print ("  Target ");
    for ( i = 0 ; i < OutputNodes ; i++ ) {
      Serial.print (Target[p][i], DEC);
      Serial.print (" ");
    }
    /******************************************************************
      Compute hidden layer activations
    ******************************************************************/

    for ( i = 0 ; i < HiddenNodes ; i++ ) {
      Accum = HiddenWeights[InputNodes][i] ;
      for ( j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[p][j] * HiddenWeights[j][i] ;


      }
      Hidden[i] = 1.0 / (1.0 + exp(-Accum)) ;
    }

    /******************************************************************
      Compute output layer activations and calculate errors
    ******************************************************************/

    for ( i = 0 ; i < OutputNodes ; i++ ) 
    {
      Accum = OutputWeights[HiddenNodes][i] ;
      for ( j = 0 ; j < HiddenNodes ; j++ ) 
      {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }
      Output[i] = 1.0 / (1.0 + exp(-Accum)) ;
    }

    Serial.print ("  Output ");
    for ( i = 0 ; i < OutputNodes ; i++ ) 
    {
     Serial.print (Output[i], 5);
     Serial.print (" ");
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

  /******************************************************************
    Compute hidden layer activations
  ******************************************************************/

  for ( i = 0 ; i < HiddenNodes ; i++ ) 
  {
    Accum = HiddenWeights[InputNodes][i];
    for ( j = 0 ; j < InputNodes ; j++ ) 
    {
      Accum += TestInput[j] * HiddenWeights[j][i];
    }
    Hidden[i] = 1.0 / (1.0 + exp(-Accum));
  }

  /******************************************************************
    Compute output layer activations and calculate errors
  ******************************************************************/

  for ( i = 0 ; i < OutputNodes ; i++ ) 
  {
    Accum = OutputWeights[HiddenNodes][i];
    for ( j = 0 ; j < HiddenNodes ; j++ ) 
    {
      Accum += Hidden[j] * OutputWeights[j][i];
    }
    Output[i] = 1.0 / (1.0 + exp(-Accum));
  }

#ifdef DEBUG
  Serial.print ("  Output ");
  for ( i = 0 ; i < OutputNodes ; i++ ) 
  {
    Serial.print (Output[i], 5);
    Serial.print (" ");
  }
#endif
}
