//this can train and run a custom neural network having an architecture consists of one hidden layer only along with input and output layers on a arduino 
//activations in order; sigmoid, linear
#include <math.h>

//documentation for Saroj Sir
//this is a project to make a neural network that can take inputs from 0 to 1 corresponding to the movement of finger joints,
//and output a value from 0 to 1 that can be turned into an angle value from 0 to 120 degrees. Also the angle outputs serve as an
//input for another neural network that will turn the input into 8-bit binary number that can be used to play a specific audio file index as the bcd number

//#define DEBUG/


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

int i, j, p;
float Accum;


float Hidden[hiddenNodes];
float Output[outputNodes];

float HiddenTwo[hiddenNodesTwo];
float OutputTwo[outputNodesTwo];

//for first neural network
float hiddenWeights[inputNodes + 1][hiddenNodes] = {{0.19751088321208954, 0.5285995006561279, -0.15907031297683716, -0.1809215098619461, -0.2880074381828308, -0.40882623195648193, 0.4017091691493988, 0.11716918647289276, -0.5138595104217529, -0.23462727665901184}, {-0.379537433385849, 0.1199766993522644, -0.04068242013454437, 0.10686840862035751, 0.37200728058815, 0.10513478517532349, -0.5848343968391418, -0.0453592948615551, -0.08428984880447388, 0.20764091610908508}, {-0.5756648182868958, -0.5195653438568115, 0.05195610970258713, 0.06609980762004852, 0.559144914150238, -0.28215649724006653, -0.5160607695579529, 0.08358167111873627, 0.13057826459407806, -0.25763580203056335}, {0.4052669107913971, -0.4270034730434418, 0.4274890422821045, -0.576393187046051, -0.2500002682209015, -0.4896621108055115, -0.0013030972331762314, 0.17332477867603302, 0.05383603274822235, -0.1375480592250824}, {0.0377299003303051, -0.00523929251357913, -0.49735745787620544, -0.09896567463874817, 0.33498066663742065, -0.4914427697658539, 0.44730302691459656, 0.002800790360197425, -0.025210747495293617, 0.18397900462150574}, {-0.48285603523254395, 0.2748596966266632, -0.16773271560668945, 0.1387069970369339, 0.5465509295463562, 0.38465654850006104, -0.539891242980957, -0.12586364150047302, 0.31456631422042847, -0.1922687590122223}, {0.35911157727241516, -0.0003982794878538698, -0.2705877125263214, -0.3698250353336334, -0.046027328819036484, 0.4323369264602661, 0.3994763195514679, -0.31386104226112366, -0.1781257838010788, 0.07018983364105225}, {-0.47612646222114563, -0.2280072420835495, -0.2359401136636734, -0.3301564157009125, -0.3688298761844635, -0.08860324323177338, 0.22053079307079315, -0.1713200807571411, 0.03853227570652962, 0.19420133531093597}, {-0.17686213552951813, 0.23387984931468964, -0.17112551629543304, 0.3858962655067444, -0.24013173580169678, -0.5367311239242554, 0.1425885260105133, 0.3452584147453308, -0.48756054043769836, 0.542144775390625}, {-0.03893714398145676, -0.02484237775206566, 0.005334691610187292, -0.02505907043814659, -0.000413616857258603, -0.031212879344820976, -0.03294117748737335, -0.007313916925340891, -0.022769784554839134, 0.039898648858070374}};;
float OutputWeights[hiddenNodes + 1][outputNodes] = {{0.4164835214614868, -0.12795817852020264, 0.41896766424179077, 0.0813765600323677, -0.367439866065979, 0.14381630718708038, 0.17043262720108032, 0.49723514914512634, 0.08868126571178436}, {0.2392197549343109, -0.3060477674007416, 0.36904680728912354, -0.3015626072883606, -0.3492518663406372, 0.4728465676307678, -0.15430887043476105, 0.24390584230422974, -0.4014318585395813}, {-0.4164019823074341, -0.3544103801250458, -0.025398973375558853, 0.02833371050655842, 0.34941956400871277, -0.5164251327514648, 0.21616214513778687, -0.3897179663181305, 0.12056820094585419}, {-0.055828601121902466, -0.3209250271320343, -0.184532031416893, -0.5177027583122253, -0.4400853216648102, 0.29178762435913086, 0.08086324483156204, 0.28324371576309204, 0.12824071943759918}, {0.07442665100097656, 0.3767116963863373, -0.4773995578289032, 0.1487320214509964, 0.2990090250968933, -0.3951408565044403, -0.10649587959051132, 0.24133528769016266, 0.0487743578851223}, {0.03338068351149559, 0.2510260343551636, -0.03776489943265915, -0.5400446653366089, 0.049357425421476364, -0.17956148087978363, 0.4377707540988922, 0.3200668692588806, -0.3773273229598999}, {-0.21659021079540253, 0.5637516975402832, 0.36574786901474, 0.4326378405094147, 0.31005850434303284, 0.19140811264514923, 0.42632994055747986, 0.18403083086013794, 0.04394107311964035}, {0.17261257767677307, -0.43980926275253296, -0.42201441526412964, -0.24917079508304596, -0.5605036616325378, -0.33076366782188416, 0.4275318384170532, -0.2804333567619324, 0.19082985818386078}, {-0.2989877760410309, 0.2970556616783142, 0.47726336121559143, -0.1233583614230156, 0.2242526262998581, -0.0034371502697467804, 0.0962972640991211, 0.1891230195760727, -0.39662614464759827}, {-0.054653119295835495, -0.09885349869728088, 0.08427981287240982, 0.43523702025413513, 0.29646068811416626, 0.32167401909828186, -0.6289231777191162, -0.39881375432014465, 0.31183257699012756}, {0.003284964943304658, 0.034212999045848846, -0.07545062154531479, 0.03556890785694122, 0.00046521297190338373, -0.01579630747437477, -0.17198963463306427, -0.20921233296394348, -0.013714136555790901}};
//for second neural network
float hiddenWeightsTwo[inputNodesTwo + 1][hiddenNodesTwo] = ;
float OutputWeightsTwo[hiddenNodesTwo + 1][outputNodesTwo] = ;


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
