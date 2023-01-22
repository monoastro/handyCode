//this can train and run a custom neural network having an architecture consists of one hidden layer only along with input and output layers on a arduino 
//activations in order; sigmoid, linear
#include <math.h>

//#define DEBUG/

/******************************************************************
   Network Configuration - customized per network
*******************************************************************/

const int numberOfDataSet = 2; 
const int inputNodes = 9;
const int hiddenNodes = inputNodes+1;
const int outputNodes = 9;

//the training data because I can't be bothered to write a file handling system for this
//0 is min value of the sensor and 1 is the max value of the sensor
float Input[numberOfDataSet][inputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

const float Target[numberOfDataSet][outputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

/******************************************************************
   End Network Configuration
*******************************************************************/

int i, j, p;
float Accum;

float Hidden[hiddenNodes];
float Output[outputNodes];

float HiddenWeights[inputNodes + 1][hiddenNodes] = {{0.07253871113061905, -0.11016316711902618, -0.8870688676834106, 1.1655160188674927, 1.2815383672714233, 0.48302099108695984, -0.8054625391960144, -0.7840881943702698, 0.4611206352710724, 0.4060262143611908}, {1.135010838508606, -1.5197639465332031, -0.558091402053833, 0.7006745934486389, -0.541337251663208, -0.9325408339500427, -0.7278292775154114, -0.31941038370132446, -0.35210150480270386, 0.22538113594055176}, {-1.1223825216293335, 0.8211764097213745, -0.1406533271074295, -0.4709177017211914, -0.2564251124858856, 1.3825627565383911, -0.04079524055123329, 0.2783917486667633, 0.45201098918914795, 0.3632502555847168}, {1.4381308555603027, 0.9725350737571716, 0.4311944842338562, 0.38189247250556946, -0.6385759115219116, -1.2278978824615479, -1.7357604503631592, 0.7142667174339294, 0.11165741831064224, 0.32153454422950745}, {-0.25183477997779846, 0.46518006920814514, -0.18098275363445282, 2.2334048748016357, 0.3508304953575134, -0.9612783789634705, -0.48619773983955383, 1.201407551765442, -0.9512639045715332, -0.6659014225006104}, {0.5515186786651611, -0.35866206884384155, 0.4356294274330139, -2.1171724796295166, -0.9414775371551514, -0.13600286841392517, 0.10636044293642044, -0.4166450500488281, 0.2823919653892517, 0.8293876647949219}, {-1.4034572839736938, 0.5862275958061218, -0.17981524765491486, -0.31874197721481323, -0.9606115221977234, 0.9938133358955383, 1.2297821044921875, 0.4074850380420685, -1.0598626136779785, -0.4162850081920624}, {0.8196752071380615, -0.9993530511856079, -0.5463271141052246, -0.658140242099762, 0.004351325333118439, 0.31462398171424866, -0.612425684928894, -0.4545843005180359, 0.07289160043001175, -0.3206872344017029}, {-0.8807932138442993, -0.8006339073181152, 0.5387368202209473, 1.1894071102142334, -0.7954421043395996, -0.5113199353218079, 1.7994623184204102, -1.0988492965698242, -0.8943802118301392, 0.05813351646065712}, {-0.5604131817817688, 0.6680333614349365, -0.42738616466522217, -0.13789600133895874, 0.4288743734359741, 0.4559102952480316, -0.45326489210128784, 0.3023372292518616, 0.6371905207633972, -0.2427303045988083}};

float OutputWeights[hiddenNodes + 1][outputNodes] = {{-0.0065091270953416824, -0.04608768969774246, 0.1338391900062561, 0.15067866444587708, 0.1719188541173935, -0.4034062623977661, 2.2582032680511475, -1.2031230926513672, 1.0239683389663696}, {-0.29957687854766846, -0.13250629603862762, 0.20000791549682617, 0.08318974822759628, 0.31504014134407043, 1.394936442375183, -1.5688111782073975, 0.6791439652442932, 1.1120821237564087}, {0.2373790144920349, 0.014686605893075466, 0.08256524056196213, 0.04582053795456886, -0.26925715804100037, 1.316378116607666, 0.7636216282844543, 0.5568804740905762, -0.5763219594955444}, {-0.03399743512272835, -0.18296483159065247, 0.009412639774382114, -0.02018003724515438, -0.03350336104631424, -2.6571695804595947, 0.52684485912323, 1.4308935403823853, 0.1450631320476532}, {0.31159690022468567, 0.6037180423736572, -0.07100041210651398, 0.17306867241859436, -0.031287066638469696, -1.3415465354919434, -0.5832269191741943, -1.3734651803970337, -0.7558227181434631}, {-0.03312996029853821, -0.130863219499588, 0.1658329963684082, -0.07265336811542511, -0.07257075607776642, 0.7387955188751221, -2.284165143966675, -0.2972378730773926, -1.0530198812484741}, {0.12698423862457275, 0.10059423744678497, -0.12267624586820602, 0.13568352162837982, 0.17290394008159637, 0.20713430643081665, -0.9640718102455139, 0.8348422050476074, -2.3608145713806152}, {-0.12172180414199829, -0.041435033082962036, -0.15443682670593262, -0.2836765646934509, -0.14867085218429565, 0.7295133471488953, -0.2957287132740021, 0.8386968374252319, 1.3672541379928589}, {0.11888962984085083, -0.3500905930995941, -0.2289784848690033, -0.051568470895290375, -0.10911062359809875, 0.3179841935634613, -0.5510159730911255, -1.7542402744293213, -0.671096682548523}, {-0.055334243923425674, 0.32286781072616577, -0.1778586208820343, -0.01672244817018509, 0.0026508497539907694, 0.6244262456893921, 0.8300251364707947, -0.7782496809959412, -0.25954562425613403}, {0.09278905391693115, 0.08760768920183182, 0.02544225938618183, 0.021480923518538475, -0.03306255489587784, 0.3300231099128723, 0.06447551399469376, -0.09009462594985962, -0.03185904398560524}};

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(A15));       //Collect a random ADC sample for Randomization.
}

void loop() 
{
  //trainNN();
  driveNN();
//  double inp[] = {0,0,0,0,0,0,0,0,0};
//  InputToOutput(inp);
}


//USES TRAINED NEURAL NETWORK TO DRIVE ROBOT
void driveNN()
{
  Serial.println("Running NN Drive Test");
  
  while (true) 
  {
    double TestInput[inputNodes];
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


  #ifdef DEBUG
      for(i = 0; i<numberOfDataSet; i++)
      {
        Serial.print("Input: ");
        Serial.print(TestInput[9-i], 2);
        Serial.print("\t");
      }
  #endif
    InputToOutput(TestInput); //INPUT to the function to obtain OUTPUT

    for(i = 0; i<outputNodes; i++)
    {

      Output[i] > 0.5 ? Serial.print(1): Serial.print(0);
//      Serial.print(Output[i]);

      Serial.print(":");
    }
    Serial.println();
  }
}

//the function for which this whole circus is established
void InputToOutput(double TestInput[])
{

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
    
    Output[i] = Accum;
//    (Accum>0? Accum: 0);
  }

  #ifdef DEBUG
    Serial.print ("  Output ");
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
      Serial.print (Output[i], 5);
      Serial.print (" ");
    }
    Serial.println();
  #endif
}

//For displaying information
void toTerminal()
{
  for ( p = 0; p < numberOfDataSet; p++ ) 
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

      //linear activation
      Output[i] = Accum ;
    }

    Serial.print("  Output ");
    for ( i = 0 ; i < outputNodes ; i++ ) 
    {
     Serial.print(Output[i], 5);
     Serial.print(" ");
    }
  }
}
