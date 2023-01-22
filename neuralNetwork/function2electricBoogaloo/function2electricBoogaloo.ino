//this can train and run a custom neural network having an architecture consists of one hidden layer only along with input and output layers on a arduino 
//activations in order; sigmoid, linear
#include <math.h>

//this is a project to make a neural network that can take inputs from 0 to 1 corresponding to the movement of finger joints,
//and output a value from 0 to 1 that can be turned into an angle value from 0 to 120 degrees. Also the angle outputs serve as an
//input for another neural network that will turn the input into 8-bit binary number that can be used to play a specific audio file index as the bcd number

//#define DEBUG/

/******************************************************************
   Network Configuration - customized per network
*******************************************************************/

//for first neural network
const int numberOfDataSet = 2; 
const int inputNodes = 9;
const int hiddenNodes = inputNodes+1;
const int outputNodes = 9;

//for second neural network
const int inputNodesTwo = outputNodes;
const int hiddenNodesTwo = inputNodesTwo+1;
const int numberOfDataSetTwo = 2;
const int outputNodesTwo = 8;


//the training data because I can't be bothered to write a file handling system for this\

//for first neural network
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


//for second neural network
const float TargetTwo[numberOfDataSetTwo][outputNodesTwo] = 
{
  {0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1}
};

/******************************************************************
   End Network Configuration
*******************************************************************/

int i, j, p;
float Accum;


float Hidden[hiddenNodes];
float Output[outputNodes];

float HiddenTwo[hiddenNodesTwo];
float OutputTwo[outputNodesTwo];

//for first neural network
float hiddenWeights[inputNodes + 1][hiddenNodes] = {{-0.14004746079444885, -0.0341973640024662, 0.46974337100982666, 0.5358598828315735, 0.3270781636238098, 0.015218441374599934, -0.3996405601501465, 0.45910143852233887, 0.42852136492729187, -0.21932046115398407}, {-0.09146388620138168, -0.05158422514796257, 0.1604357361793518, 0.40996894240379333, -0.46762263774871826, 0.21628166735172272, -0.4508122205734253, 0.10921329259872437, 0.6199372410774231, 0.1858755648136139}, {-0.3137611448764801, -0.5920679569244385, 0.42592519521713257, 0.42010030150413513, -0.13474123179912567, -0.08814888447523117, -0.6996347308158875, -0.16631576418876648, 0.3817422091960907, 0.4883474111557007}, {-0.15622201561927795, 0.3251950442790985, -0.09320999681949615, 0.26863548159599304, -0.03824631869792938, 0.5467292666435242, 0.3470534384250641, 0.47236940264701843, 0.5187270641326904, 0.06639143079519272}, {-0.4730384647846222, -0.01663080044090748, 0.42703360319137573, -0.2870514690876007, -0.19132646918296814, 0.5177885890007019, 0.31754687428474426, 0.0444720983505249, -0.09301332384347916, 0.4972207546234131}, {-0.23115000128746033, -0.35549792647361755, 0.3024226427078247, 0.22559767961502075, 0.10722213983535767, 0.15354838967323303, 0.15564627945423126, 0.24799282848834991, -0.16624347865581512, 0.029666904360055923}, {-0.29768186807632446, -0.08137736469507217, -0.4094178080558777, 0.4156472384929657, -0.5303547382354736, 0.636762797832489, -0.27385982871055603, -0.2521095871925354, 0.6276982426643372, 0.16029831767082214}, {0.07719225436449051, -0.5302117466926575, 0.5274298787117004, 0.16013862192630768, 0.14621439576148987, -0.12378913164138794, -0.6821419596672058, -0.5977811813354492, 0.512808084487915, -0.4014384150505066}, {-0.5509043335914612, -0.32851898670196533, 0.5244812369346619, 0.3760868012905121, -0.3096378445625305, 0.5149139165878296, -0.3331369459629059, 0.19986164569854736, -0.027979306876659393, 0.050546709448099136}, {0.05462450534105301, 0.0024669012054800987, -0.17267876863479614, -0.2183215171098709, 0.03166962414979935, -0.2189970165491104, 0.05929502844810486, -0.07648807764053345, -0.3025897145271301, -0.11896917223930359}};
float OutputWeights[hiddenNodes + 1][outputNodes] = {{0.058223750442266464, -0.5230046510696411, -0.0206752922385931, -0.4214234948158264, 0.13474762439727783, -0.8103290796279907, -0.15461397171020508, -0.6935494542121887, -0.29091423749923706}, {-0.5385140180587769, -0.14978449046611786, 0.10209909826517105, -0.4830637574195862, -0.4570513367652893, 0.27970343828201294, -0.38363125920295715, -0.22908858954906464, -0.3377079665660858}, {0.45500433444976807, 0.051985811442136765, 0.0500774011015892, 0.10561955720186234, 0.2553754448890686, 0.21708182990550995, -0.030131611973047256, 0.26175668835639954, 0.6597111821174622}, {0.38707321882247925, 0.5238921642303467, 0.2942592203617096, 0.5350378155708313, 0.05195122957229614, 0.6467967629432678, -0.07492432743310928, -0.10601941496133804, 0.4949643909931183}, {0.06042734161019325, 0.10697153955698013, -0.7043190598487854, -0.05102056264877319, -0.4342730641365051, 0.1971772164106369, -0.5557236671447754, 0.05109725147485733, -0.47787076234817505}, {0.2359830141067505, 0.5425509810447693, 0.5056074261665344, 0.3619750738143921, 0.5376541018486023, 0.42907747626304626, 0.6847953796386719, 0.6032000780105591, 0.1319372057914734}, {-0.10994210839271545, -0.4762532114982605, -0.5557016134262085, -0.47929099202156067, -0.418182909488678, -0.4988037645816803, -0.31108078360557556, -0.23312896490097046, -0.007427017204463482}, {-0.5391412377357483, -0.1952516734600067, 0.45674261450767517, -0.09627746790647507, 0.08087325096130371, -0.2240244746208191, 0.5669741630554199, 0.043571554124355316, 0.11844240874052048}, {0.8105676770210266, 0.29097723960876465, 0.20605872571468353, 0.1201530173420906, 0.5428004264831543, 0.11983415484428406, 0.24009528756141663, 0.3832971453666687, 0.21253399550914764}, {-0.057365283370018005, -0.2645089328289032, 0.4085399806499481, -0.20413488149642944, -0.15925057232379913, -0.007512636017054319, 0.4370444715023041, 0.12663255631923676, -0.2741537094116211}, {-0.2772826850414276, 0.1300738900899887, -0.27524182200431824, 0.37406957149505615, 0.020711513236165047, -0.084919273853302, -0.12354038655757904, -0.01963127963244915, -0.034190576523542404}};

//for second neural network
float hiddenWeightsTwo[inputNodesTwo + 1][hiddenNodesTwo] = {};
float OutputWeightsTwo[hiddenNodesTwo + 1][outputNodesTwo] = {};


void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(A15));       //Collect a random ADC sample for Randomization.
}

void loop() 
{
  driveNN();
//  double inp[] = {0,0,0,0,0,0,0,0,0};
//  InputToOutput(inp);
}


//
void driveNN()
{
  Serial.println("Running first neural network");
  
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
  //at this point, the output layer of the first neural network is in the Output array
  //now we need to feed this into the second neural network
  driveNN2();

  //now we serial print the bcd value
    for(i = 0; i<outputNodesTwo; i++)
    {
      Serial.print(OutputTwo[i]);
      // Serial.print(TestInput[0]);
      //Serial.print(":");
    }
    Serial.println();
  }
}


void driveNN2()
{
  Serial.println("Running second neural network");
  
  inputTwoOutput();


#ifdef DEBUG
    for(i = 0; i<numberOfDataSetTwo; i++)
    {
      Serial.print("Input: ");
      Serial.print(Output[9-i], 2);
      Serial.print("\t");
    }
#endif

}
//the function for which this whole circus is established
void InputToOutput(double TestInput[])
{
  //Compute hidden layer activations
  for ( i = 0 ; i < hiddenNodes ; i++ ) 
  {
    Accum = hiddenWeights[inputNodes][i];
    for ( j = 0 ; j < inputNodes ; j++ ) 
    {
      Accum += TestInput[j] * hiddenWeights[j][i];
    }
    //sigmoid activation
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
    //linear activation
    Output[i] = Accum;
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
void inputTwoOutput()
{
  //test input is first output layer

  //Compute hidden/output layer activations
  for ( i = 0 ; i < hiddenNodesTwo ; i++ ) 
  {
    Accum = hiddenWeightsTwo[inputNodesTwo][i];
    for ( j = 0 ; j < inputNodesTwo ; j++ ) 
    {
      Accum += Output[j] * hiddenWeightsTwo[j][i];
    }
    //sigmoid activation
    HiddenTwo[i] = 1.0 / (1.0 + exp(-Accum));
  }

  //Compute output layer activations
  for ( i = 0 ; i < outputNodesTwo ; i++ ) 
  {
    Accum = OutputWeightsTwo[hiddenNodesTwo][i];
    for ( j = 0 ; j < hiddenNodesTwo ; j++ ) 
    {
      Accum += HiddenTwo[j] * OutputWeightsTwo[j][i];
    }
    //linear activation
    OutputTwo[i] = Accum;
  }

  #ifdef DEBUG
    Serial.print (" Final BCD Output ");
    for(i = 0; i<outputNodesTwo; i++)
    {
      Serial.print(OutputTwo[i]);
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
      Accum = hiddenWeights[inputNodes][i] ;
      for ( j = 0 ; j < inputNodes ; j++ ) 
      {
        Accum += Input[p][j] * hiddenWeights[j][i] ;
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
