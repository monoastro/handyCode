//this can be used to test the weights after training
//basically it acts as a function; insert input and get output
//the architecture consists of one hidden layer only along with input and output layers
#include <iostream>
#include <math.h>
#include <random>

/******************************************************************
   Network Configuration - customized per network
*******************************************************************/

const int PatternCount = 2;
const int InputNodes = 1;
const int HiddenNodes = InputNodes + 1;
const int OutputNodes = 1;


//the training data because I can't be bothered to write a file handling system for this

//0 is min value of the sensor and 1 is the max value of the sensor
double Input[PatternCount][InputNodes] = 
{
  {52},
  {100}
};

const double Target[PatternCount][OutputNodes] = 
{
  {0},
  {1}
};

/******************************************************************
   End Network Configuration
*******************************************************************/

unsigned i, j; //arduino code relic; just ignore these two 
double Accum;

double Hidden[HiddenNodes];
double Output[OutputNodes];

double HiddenWeights[InputNodes + 1][HiddenNodes] =
// {{-0.9172558188438416, -2.1189775466918945, -0.26907649636268616, -2.1384246349334717, -2.182420253753662}, {3.801981210708618, -4.245895862579346, -0.6301335692405701, 2.8833022117614746, -0.5141245126724243}, {-1.8388429880142212, 4.3400702476501465, 0.6287580132484436, -0.4154495596885681, 4.206478118896484}, {2.092860221862793, 0.12635615468025208, -1.8306483030319214, 3.33329701423645, -0.6020764708518982}, {-3.1290602684020996, -0.02937624230980873, 0.7729891538619995, 1.1912368535995483, 1.6863088607788086}}; 
{{1.2654412984848022, 0.10761762410402298}, {1.00202476978302, -3.2042646408081055}};

double OutputWeights[HiddenNodes + 1][OutputNodes] = 
// {{2.442823648452759, 4.311673641204834}, {2.4920878410339355, 4.596552848815918}, {-0.7090554237365723, -1.911628007888794}, {3.0956435203552246, -2.9264495372772217}, {-4.031330108642578, 1.2131105661392212}, {0.05616047605872154, -0.040414489805698395}};
{{-0.924720287322998}, {5.928210258483887}, {-2.1390302181243896}};

//the function for which this whole circus is established
void InputToOutput(double TestInput[])
{
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

  std::cout<<"Predicted Output: ";
  for ( i = 0 ; i < OutputNodes ; i++ ) 
  {
    std::cout<<Output[i]<<" ";
  }
  std::cout<<"\n";

}

int main() 
{
  /* double input[InputNodes];
  unsigned whichDataSet;
  std::cout<<"Choose the dataset: ";
  std::cin>>whichDataSet;
  for(i = 0; i<InputNodes; i++)
  {
    input[i] = Input[whichDataSet][i];
  } */

  double input[] = {1};
  std::cout<<"Enter input\n";
  std::cin>>input[0];
  
  InputToOutput(input);
/*   std::cout<<"Actual Output: ";
  for(i = 0; i<OutputNodes; i++)
  {
    std::cout<<Target[whichDataSet][i]<<" ";
  }
  std::cout<<"\n"; */
  return 0;
}

//other weights

//Wild oscillation data
// {{-2.4062089920043945, 0.9986947178840637, -1.0671565532684326, -1.3078217506408691, 2.282473087310791}, {0.023553701117634773, 4.477676868438721, 0.913195788860321, 5.250211715698242, -1.712435245513916}, {3.6402392387390137, -4.424204349517822, 0.7046148777008057, -3.124873638153076, 0.3226388394832611}, {-0.9930652379989624, 0.6067217588424683, -2.885045051574707, 1.894884467124939, -3.402651071548462}, {1.8534878492355347, -0.76656174659729, 0.03851019963622093, -3.167191505432129, -1.8511136770248413}};
//{{-2.664714813232422, 1.3368545770645142}, {-2.16292142868042, -6.365833759307861}, {-0.7271573543548584, -2.766731023788452}, {3.6028904914855957, 5.587453842163086}, {-3.2445333003997803, 1.2343003749847412}, {3.666773557662964, 1.8885525465011597}};
