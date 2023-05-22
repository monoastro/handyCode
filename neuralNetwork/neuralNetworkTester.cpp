//this can be used to test the weights after training
//basically it acts as a function; insert input and get output
//the architecture consists of one hidden layer only along with input and output layers
#include <iostream>
#include <math.h>
#include <random>

//Network Configuration - customized per network

const int PatternCount = 3;
const int InputNodes = 9;
const int HiddenNodes = InputNodes + 10;
const int OutputNodes = 9;


//the training data because I can't be bothered to write a file handling system for this

//0 is min value of the sensor and 1 is the max value of the sensor
double Input[PatternCount][InputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {100,100,100,100,100,100,100,100,100},
  {1,1,1,1,1,1,1,1,1}
};

const double Target[PatternCount][OutputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

//End Network Configuration

unsigned i, j; //arduino code relic; just ignore these two 
double Accum;

double Hidden[HiddenNodes];
double Output[OutputNodes];

double HiddenWeights[InputNodes + 1][HiddenNodes] = //{{0.40788745880126953, -0.4361528158187866, -0.3479953408241272, -0.3236440420150757, 2.125603675842285, 0.30508682131767273, -0.43863165378570557, -1.9835561513900757, 0.26741790771484375, -1.1680402755737305, -0.3649548292160034, -0.22882433235645294, 0.06189138814806938, 0.05408106744289398, -0.20584221184253693, 0.42881396412849426, -0.6486574411392212, 0.25485068559646606, 1.362567663192749}, {0.3247884511947632, -0.08729679137468338, -0.3908969759941101, -0.32884007692337036, 0.8438133597373962, 0.29473909735679626, -0.06706410646438599, -1.3936179876327515, -0.37878671288490295, -0.6101034283638, -0.36609360575675964, -0.11825667321681976, 0.2052663415670395, 0.27089089155197144, -0.4710594713687897, -0.7385116219520569, -1.068861722946167, 0.07564198970794678, 0.1554359346628189}, {0.31980472803115845, 0.010021121241152287, -0.18437203764915466, -0.386432021856308, 1.9449881315231323, 0.1246812641620636, 0.36428117752075195, 1.990365982055664, -0.4748932421207428, -0.24427156150341034, -0.24672454595565796, 0.36001238226890564, -0.37480777502059937, -0.5606327652931213, 0.24112021923065186, -0.8544895052909851, -0.17252054810523987, 0.05596578121185303, -1.5149484872817993}, {-0.22201351821422577, 0.27805620431900024, 0.01893553137779236, -0.06971466541290283, -0.979842483997345, -0.16433921456336975, -0.45366278290748596, 1.3425416946411133, 0.14429719746112823, 0.3991241753101349, 0.04026937484741211, -0.029295269399881363, 0.17571794986724854, 1.5624545812606812, 0.16674116253852844, -1.0375803709030151, 0.06589832156896591, -0.2266821265220642, 0.3891756236553192}, {-0.3843138813972473, -0.13446837663650513, 0.16734082996845245, -0.4129665195941925, 0.08948110044002533, 0.2699219286441803, -0.32249224185943604, -0.15135538578033447, 0.11070646345615387, 1.1356220245361328, -0.29275351762771606, 0.2382863163948059, -0.2934669852256775, -2.0915045738220215, -0.34841832518577576, -0.5085658431053162, 0.3997286856174469, -0.2827707529067993, -0.5135178565979004}, {-0.31865692138671875, -0.3205561339855194, -0.33792179822921753, -0.11176601052284241, -0.4950488805770874, -0.4702107012271881, -0.1149592399597168, 1.4680266380310059, -0.24831759929656982, 0.4582294821739197, -0.42668628692626953, -0.46075406670570374, 0.4018532335758209, 1.14976966381073, -0.3663412034511566, 0.36354926228523254, 0.42889824509620667, 0.2915871739387512, 2.680306911468506}, {-0.29586857557296753, 0.29189714789390564, 0.0766947790980339, 0.06249779462814331, -0.4374426305294037, -0.14102694392204285, 0.1524970531463623, -1.1072214841842651, -0.26568788290023804, 0.3533901870250702, -0.28557658195495605, -0.10240471363067627, -0.3848434090614319, 0.11502630263566971, 0.09993763267993927, 0.8748790621757507, 1.066508173942566, 0.056839585304260254, -0.9261329174041748}, {-0.0972515344619751, -0.2921789586544037, 0.26390892267227173, -0.22606252133846283, -0.49881672859191895, -0.12123030424118042, -0.07018715143203735, 0.5417667627334595, -0.24549679458141327, 0.9375701546669006, 0.05147188901901245, -0.15324607491493225, -0.026452623307704926, 1.1428825855255127, 0.009473763406276703, -0.027894403785467148, 0.9511917233467102, -0.303911417722702, -1.2095203399658203}, {0.13472622632980347, -0.1976025551557541, -0.20013977587223053, 0.22684139013290405, -0.9121308922767639, -0.3478614091873169, -0.16149863600730896, -0.7387512922286987, 0.24737727642059326, -1.2253706455230713, -0.1275368332862854, 0.2203618884086609, -0.4181967079639435, -1.5981261730194092, 0.39273640513420105, 1.5476207733154297, -1.0898520946502686, -0.30790096521377563, -0.4543204605579376}, {0.0, -0.06485496461391449, -0.10948494076728821, 0.0, -0.25583675503730774, -0.11512067914009094, 0.0, -0.13316956162452698, -0.07725024223327637, -0.12305805087089539, 0.0, -0.06628063321113586, -0.019621344283223152, -0.14825066924095154, -0.046479348093271255, -0.06731727719306946, -0.6088702082633972, 0.0, -0.6034234762191772}};
//{{2.0090010166168213, 0.9164603352546692, -0.39176902174949646, -2.0763261318206787, -6.089452743530273, 0.5568827390670776, -2.685368776321411, 1.698843002319336, -0.3029080331325531, -1.3712427616119385, -0.8833332657814026, 3.916396141052246, -2.7014622688293457, 1.4329383373260498, -0.9265817999839783, 2.373422384262085, -2.847163200378418, -9.086898803710938, -1.386370062828064}, {0.4953332841396332, -0.9608941078186035, -0.5340107679367065, 0.310898095369339, 1.3500486612319946, -8.589598655700684, -1.1691923141479492, -1.0093225240707397, 0.7737630009651184, 0.9383821487426758, -0.10471032559871674, 1.7654484510421753, 0.9555957913398743, 2.008375406265259, 0.6281931400299072, 0.660741925239563, -1.2063407897949219, 4.230349540710449, -1.764047622680664}, {5.429308891296387, -2.988046407699585, 8.209311485290527, 0.35200634598731995, -3.3422937393188477, 0.177134707570076, 0.2387770563364029, -1.0656768083572388, 1.8831266164779663, 1.017304539680481, -1.558264970779419, 6.915225505828857, 0.6476768851280212, 0.0020668460056185722, -2.497277021408081, 1.815761923789978, -0.3304728865623474, -0.7459255456924438, -0.39775681495666504}, {0.39732593297958374, -0.17960011959075928, -3.000454902648926, -2.0826144218444824, 0.06144428998231888, -2.2790908813476562, 0.9520445466041565, -1.1093873977661133, 2.6819052696228027, -0.13331060111522675, -0.9682339429855347, -0.5289739966392517, -0.781704306602478, -0.6360613703727722, -0.8630073070526123, -1.1665278673171997, 0.7061499357223511, -1.553481936454773, -0.44279828667640686}, {-8.61706256866455, -0.06795011460781097, -6.239126205444336, -2.215949058532715, 2.116220474243164, 1.268202543258667, -0.4362614154815674, -3.365438938140869, -0.32875141501426697, 5.0761542320251465, -1.3767017126083374, -1.1911348104476929, -2.4203414916992188, -0.26274576783180237, -0.582804799079895, -0.5734472274780273, -0.3099827170372009, 3.2326619625091553, -1.941918134689331}, {0.1513442099094391, -1.7635905742645264, -2.989370346069336, 1.2805249691009521, -0.607323944568634, 5.287376880645752, 0.5918378233909607, 3.1000545024871826, -1.3972339630126953, -2.626469612121582, -1.0928294658660889, -0.8159122467041016, -0.1752488613128662, -1.690161943435669, 2.254523754119873, -2.044950246810913, -0.6950883865356445, 0.9283224940299988, -0.8415866494178772}, {-1.9183173179626465, 1.1422663927078247, 0.2814454436302185, 1.8771101236343384, -0.5270687937736511, 3.314211130142212, 2.9658358097076416, 3.0495636463165283, -0.9927619099617004, -2.4697749614715576, 1.695864200592041, -1.8881574869155884, 0.4090156555175781, -0.5713328719139099, 3.0373342037200928, 0.25592559576034546, 1.6891748905181885, 0.6328374743461609, 0.6954604983329773}, {0.12588411569595337, 1.44865083694458, 2.2504994869232178, -1.8804569244384766, 0.5802839994430542, 0.9270026087760925, -1.7256635427474976, 2.3286657333374023, -1.0647975206375122, 0.8708268404006958, -1.3085353374481201, -0.5969376564025879, -0.20916806161403656, 0.038543783128261566, -3.2963063716888428, 2.4817914962768555, 0.847817599773407, 2.0159876346588135, 1.1827924251556396}, {1.6754847764968872, -1.3402462005615234, 2.835606813430786, 0.5729682445526123, -0.9644140601158142, -0.35159823298454285, 0.23683534562587738, 0.5110614895820618, 3.622615337371826, -3.0980241298675537, 1.0741862058639526, -2.0249998569488525, -1.8034756183624268, -3.0424787998199463, 1.444130301475525, 0.28939592838287354, -0.7714011073112488, 1.100798487663269, 1.2339500188827515}, {0.7828726172447205, -0.407754510641098, -0.4411654770374298, -0.17818091809749603, 0.9317843914031982, -1.385932445526123, -0.4227554202079773, -2.077815055847168, -2.526827096939087, -0.27255192399024963, -0.4498231112957001, -2.4062178134918213, 2.0947508811950684, 0.3301967978477478, -0.47391554713249207, -0.5139546990394592, -0.9728932976722717, -4.389845371246338, 0.5336097478866577}};
{{-784.8325805664062, -51.72740936279297, -0.11103396117687225, -0.2505326569080353, -0.11087983846664429, 0.056867241859436035, -105.89444732666016, -41.242061614990234, -23.829233169555664, 0.2229345440864563, 0.0884142518043518, 0.10548985004425049, -0.1435038149356842, -131.6629180908203, 0.049017928540706635, -0.25967472791671753, -0.225100576877594, -191.12066650390625, -0.3306467533111572}, {-784.8508911132812, -51.51197052001953, 0.0027032941579818726, -0.11114515364170074, -0.38741064071655273, 0.1692611575126648, -105.63493347167969, -39.764739990234375, -22.79246711730957, -0.30159837007522583, -0.08043622970581055, 0.08025158196687698, 0.11832863092422485, -131.50975036621094, 0.3097725212574005, -0.354588121175766, -0.08808040618896484, -191.2528076171875, 0.3450276553630829}, {-785.0841064453125, -52.15766906738281, -0.18543843924999237, -0.029308967292308807, 0.17772996425628662, 0.0786818265914917, -105.8043441772461, -41.71406173706055, -24.472824096679688, -0.1448887288570404, -0.4077598750591278, -0.18810291588306427, -0.2951485812664032, -131.659423828125, 0.11265918612480164, -0.32529234886169434, -0.4154221713542938, -191.3940887451172, -0.06075359135866165}, {-784.512939453125, -51.49931716918945, -0.15195195376873016, -0.04077927768230438, -0.22794418036937714, -0.16569897532463074, -105.63108825683594, -40.21848678588867, -22.838544845581055, -0.18705520033836365, -0.11980500817298889, -0.04323872923851013, -0.09016028046607971, -131.6681671142578, -0.057049643248319626, -0.4419747591018677, 0.11489367485046387, -190.7861785888672, -0.41846469044685364}, {-784.050048828125, -51.63980484008789, -0.2701411247253418, -0.26900896430015564, 0.04132991284132004, -0.36254918575286865, -106.12028503417969, -40.14774703979492, -22.80540657043457, -0.39707618951797485, -0.21382465958595276, -0.3193017244338989, 0.10667424649000168, -131.6343536376953, -0.5254477858543396, 0.060399532318115234, -0.1508711278438568, -191.19979858398438, -0.14836010336875916}, {-784.6273193359375, -51.39286422729492, 0.1718398630619049, 0.09969399869441986, -0.038851868361234665, -0.3556325137615204, -106.0179214477539, -40.930946350097656, -23.279882431030273, -0.03380653262138367, -0.17941564321517944, -0.41602247953414917, -0.45385125279426575, -131.56907653808594, 0.16078434884548187, 0.22768956422805786, -0.43791207671165466, -190.89161682128906, 0.3465680480003357}, {-784.8792114257812, -51.71814727783203, -0.3706449270248413, 0.08681465685367584, 0.2817739248275757, -0.054319024085998535, -105.60872650146484, -41.19460678100586, -23.34560203552246, -0.08919689059257507, 0.283066987991333, -0.2538725733757019, 0.12096864730119705, -132.14846801757812, -0.307233989238739, -0.005232483148574829, -0.07293263077735901, -191.1871795654297, -0.30066725611686707}, {-785.0526733398438, -51.99699020385742, -0.04716429114341736, -0.520087480545044, -0.18462273478507996, 0.44391536712646484, -105.8436508178711, -40.82963180541992, -23.646011352539062, -0.23697379231452942, 0.06624478101730347, 0.2847481369972229, 0.3915901184082031, -131.85687255859375, -0.29203176498413086, 0.2639126777648926, -0.17737650871276855, -191.3802032470703, 0.1517132967710495}, {-784.447998046875, -51.836830139160156, -0.0006970018148422241, -0.10227170586585999, 0.251655638217926, -0.2970597743988037, -106.0647964477539, -41.424461364746094, -22.92280387878418, -0.07463744282722473, -0.05295035243034363, 0.20695774257183075, -0.035431940108537674, -132.06321716308594, -0.17069301009178162, -0.46150267124176025, -0.13569918274879456, -190.69593811035156, -0.11608337610960007}, {-7.247488021850586, -0.5371366143226624, -0.1238616332411766, -0.20517687499523163, -0.17646318674087524, 0.0, -1.2059801816940308, -0.11276310682296753, -0.38993942737579346, 0.0, 0.0, -0.11451271176338196, -0.09045658260583878, -1.339571237564087, -0.1363319605588913, 0.0, 0.0, -1.9596096277236938, -0.08877253532409668}};

double OutputWeights[HiddenNodes + 1][OutputNodes] = //{{0.3987823724746704, 0.25990307331085205, 0.27995365858078003, -0.4034218192100525, -0.3464948534965515, 0.45027244091033936, 0.11017441749572754, -0.3017178773880005, -0.3475404679775238}, {-0.2911466956138611, -0.21166670322418213, 0.36231401562690735, 0.062184035778045654, -0.2917046844959259, 0.06600368767976761, -0.35706570744514465, -0.32548844814300537, -0.3316330015659332}, {-0.044731710106134415, -0.36103829741477966, 0.35492122173309326, 0.23771554231643677, 0.11147463321685791, -0.3547144830226898, 0.2530961036682129, -0.43450838327407837, -0.3178069591522217}, {0.38510483503341675, -0.004979521036148071, -0.2049199640750885, 0.15250998735427856, -0.15205469727516174, 0.018308788537979126, -0.28238236904144287, -0.32411640882492065, -0.06797674298286438}, {-0.032038912177085876, 0.20946653187274933, 0.19711452722549438, 0.17459623515605927, 0.059888314455747604, -0.265841007232666, -1.0756195783615112, -1.0125569105148315, -1.3620233535766602}, {0.03235134482383728, 0.3546636998653412, 0.2715602517127991, 0.05988281965255737, 0.07245749235153198, 0.1390833556652069, 0.1206151694059372, -0.28985893726348877, 0.03131244331598282}, {0.16189861297607422, -0.012262582778930664, -0.227498397231102, 0.25400686264038086, 0.34491950273513794, 0.30097025632858276, -0.16854676604270935, 0.38530826568603516, -0.2406426966190338}, {-0.32186728715896606, 0.10224443674087524, -0.16942480206489563, 0.10571307688951492, 0.05608157068490982, 0.8434013724327087, -0.656745195388794, 0.4535583555698395, -1.501233458518982}, {-0.28397318720817566, 0.3747696280479431, -0.3414836525917053, -0.41579753160476685, 0.19191884994506836, -0.4226748049259186, 0.4414454400539398, 0.19707471132278442, 0.33207884430885315}, {-0.022243570536375046, -0.009119806811213493, -0.03199158236384392, -0.22967037558555603, -0.19519291818141937, 0.39950472116470337, -0.7189139127731323, -1.3411198854446411, -0.00014279774040915072}, {0.24908143281936646, -0.20174351334571838, 0.3649413585662842, 0.09305572509765625, -0.14556628465652466, -0.1227758526802063, 0.40461796522140503, 0.16137051582336426, -0.2901288866996765}, {0.4448866844177246, -0.34117600321769714, -0.4519665837287903, 0.0877382755279541, 0.13024067878723145, 0.06254317611455917, 0.4062882363796234, 0.31072014570236206, -0.41842690110206604}, {-0.06027141213417053, 0.4249926805496216, -0.25804129242897034, -0.17244544625282288, -0.4390818476676941, 0.1846015751361847, -0.34630224108695984, 0.36336612701416016, -0.3448646366596222}, {0.07499497383832932, -0.24474340677261353, -0.40777403116226196, -0.2754214406013489, -0.028247907757759094, -0.13137777149677277, 1.0028637647628784, -0.39219391345977783, 0.5885316729545593}, {-0.31400948762893677, 0.22454489767551422, 0.05370192602276802, -0.060241471976041794, 0.3228612542152405, -0.23467010259628296, -0.4348422884941101, -0.4560965895652771, 0.041073914617300034}, {0.25812679529190063, 0.2167736291885376, 0.02426958829164505, -0.008547528646886349, 0.14948749542236328, 0.200993150472641, 0.34658893942832947, 0.6557018160820007, -0.9849507212638855}, {0.11963637918233871, 0.11994536221027374, 0.18676547706127167, -0.3931336998939514, -0.34149613976478577, 0.12793312966823578, -0.16706031560897827, -0.2801564335823059, 0.9429376125335693}, {-0.32241421937942505, -0.37201035022735596, 0.0032708048820495605, -0.09051212668418884, -0.09737315773963928, -0.20744726061820984, 0.25834721326828003, -0.0781974196434021, -0.25734609365463257}, {-0.284027636051178, 0.22390435636043549, 0.3093751072883606, 0.25960639119148254, 0.15204980969429016, -0.07900215685367584, 1.124640941619873, -0.5463644862174988, -1.1648685932159424}, {-0.377765417098999, -0.49702906608581543, -0.3051963150501251, -0.25901371240615845, -0.24834343791007996, -0.42700427770614624, 0.0005310681881383061, 0.9999790191650391, 0.9997896552085876}};
//{{-4.814497515326366e-05, -4.198963870294392e-05, -2.0170009520370513e-05, 9.161160414805636e-05, -6.549164390889928e-05, 1.4767723083496094, -0.17904455959796906, 0.1323123723268509, 0.3085419833660126}, {0.0002306718088220805, 0.00018176813318859786, 0.00010452545393491164, -0.00036102556623518467, 0.00040888809598982334, -1.5727566480636597, 0.2868851125240326, 1.5543153285980225, -0.35929954051971436}, {-7.861662743380293e-05, -6.341216794680804e-05, -3.5025870602112263e-05, 0.00012890504149254411, -0.00013218022650107741, 2.4079689979553223, 0.33807119727134705, 0.07030351459980011, -0.2732928693294525}, {0.000434454414062202, 0.0003363058785907924, 0.000199280766537413, -0.0006556370062753558, 0.0007996102212928236, -1.7908908128738403, -0.8064109086990356, -0.7001121044158936, -0.6158998012542725}, {3.188733899150975e-05, 2.7352407414582558e-05, 1.3558607861341443e-05, -5.886712096980773e-05, 4.5649863750441e-05, 0.04094704985618591, -1.965779423713684, -1.8072339296340942, -0.5661985278129578}, {-0.00013146104174666107, -0.00010531296720728278, -5.885507562197745e-05, 0.00021263329836074263, -0.00022455181169789284, 1.3721387386322021, -1.2943110466003418, -0.45569363236427307, 0.37303426861763}, {0.00015966041246429086, 0.0001302734308410436, 7.0534399128519e-05, -0.0002677950542420149, 0.00026114724460057914, 1.0835336446762085, -0.5626476407051086, 0.7747160792350769, 0.0459759496152401}, {0.00013227084127720445, 0.00010454701987328008, 5.979200068395585e-05, -0.00020826247055083513, 0.00023286625219043344, -1.2780532836914062, 0.3266037702560425, -0.07540303468704224, -1.1865049600601196}, {-4.0477556467521936e-05, -3.544625360518694e-05, -1.6885944205569103e-05, 7.757449930068105e-05, -5.432195030152798e-05, -0.7410312294960022, -0.0430673211812973, 1.3372637033462524, -1.0121793746948242}, {0.0001467679685447365, 0.00011410594743210822, 6.710781599394977e-05, -0.00022344816534314305, 0.00026767130475491285, 0.7589191198348999, 1.3479821681976318, -0.21967165172100067, -1.778529405593872}, {-0.0019615760538727045, -0.0015395120717585087, -0.0008912053890526295, 0.0030448378529399633, -0.0035070155281573534, -1.0469545125961304, 1.426551342010498, 0.11587999016046524, 1.0225826501846313}, {5.003419209970161e-05, 4.269933560863137e-05, 2.133858833985869e-05, -9.143358329311013e-05, 7.264789019245654e-05, -2.011577606201172, -0.871758759021759, -1.4867933988571167, -0.39546728134155273}, {0.0001578928786329925, 0.00013598402438219637, 6.684571417281404e-05, -0.00029352999990805984, 0.00022321258438751101, -2.104071617126465, -1.181106448173523, -0.42705076932907104, 0.21912430226802826}, {-0.00025522595387883484, -0.00020995487284380943, -0.00011202124733245, 0.00043484766501933336, -0.0004090112051926553, 1.2381184101104736, 0.4920274019241333, 1.0256117582321167, 1.7790125608444214}, {-6.593630678253248e-05, -5.737452374887653e-05, -2.767721343843732e-05, 0.00012494016846176237, -9.03374093468301e-05, 0.8333255052566528, 0.3623029887676239, -0.8944454789161682, -0.17623794078826904}, {-0.00025482941418886185, -0.000203374118427746, -0.00011439745139796287, 0.000409086438594386, -0.0004390432150103152, 1.3604650497436523, -0.5407445430755615, -1.2066388130187988, 0.9340253472328186}, {-0.0008859836379997432, -0.0007386388606391847, -0.0003850174543913454, 0.0015491617377847433, -0.0013721009017899632, 1.213974952697754, 1.3904508352279663, 0.22060295939445496, 0.8706419467926025}, {-0.0001745242770994082, -0.00013995343761052936, -7.808620284777135e-05, 0.0002828802680596709, -0.00029743017512373626, -0.2560468912124634, 1.246046781539917, 1.057081699371338, 2.319755792617798}, {0.0007918431074358523, 0.0006273192120715976, 0.0003573775466065854, -0.0012526160571724176, 0.0013870194088667631, -0.8750327229499817, 0.240896537899971, -0.04583832249045372, 2.1072230339050293}, {0.00016427173977717757, 0.00013755627151113003, 7.111677405191585e-05, -0.0002896074438467622, 0.00025138427736237645, -0.17553949356079102, 0.9542275071144104, 1.112914800643921, 0.8840166926383972}};
{{-72.98120880126953, 18.788177490234375, 36.9820671081543, -89.68566131591797, -122.9686508178711, -142.0961151123047, -126.30738067626953, -172.80142211914062, 79.85865783691406}, {-23.394636154174805, 4.736744403839111, 13.490334510803223, -43.388450622558594, -54.105064392089844, -40.18330383300781, -71.45693969726562, -60.16854476928711, 25.19745445251465}, {-27.04050064086914, 5.79225492477417, 15.149527549743652, -49.9161491394043, -62.376747131347656, -46.587642669677734, -82.42447662353516, -69.47958374023438, 28.40701675415039}, {-0.44244879484176636, 0.26116448640823364, -0.30737361311912537, -0.36670392751693726, -0.35446277260780334, -0.3417663872241974, -0.223184734582901, 0.23665305972099304, -0.18287521600723267}, {0.34076690673828125, -0.25236594676971436, 0.3288339376449585, -0.099076047539711, -0.08251290023326874, 0.06636550277471542, -0.060887470841407776, -0.04397150129079819, 0.09008925408124924}, {0.21132761240005493, -0.33453744649887085, -0.35630321502685547, -0.38990679383277893, 0.07081258296966553, -0.3276129364967346, 0.3887377977371216, -0.34334301948547363, -0.40662485361099243}, {-43.73362350463867, 9.017333030700684, 25.016435623168945, -80.60074615478516, -100.49520111083984, -75.66513061523438, -134.10728454589844, -113.11779022216797, 46.611183166503906}, {-1.60092251955124e-11, -1.1791641447989054e-11, -3.981481117021346e-11, -2.8161186924058335e-11, -7.119942135869195e-11, 0.524381160736084, -0.9638321995735168, -0.5646941661834717, -0.7288563251495361}, {1.5045005056890659e-05, 1.109651566366665e-05, 3.7325775338103995e-05, 2.6371977583039552e-05, 6.6734166466631e-05, -0.15822181105613708, 0.059890054166316986, 0.23358595371246338, 0.03800401836633682}, {-0.2868635654449463, -0.30733078718185425, -0.06632721424102783, 0.41837579011917114, 0.45774954557418823, -0.09447205066680908, 0.4353524446487427, -0.43979933857917786, 0.3748844861984253}, {-0.25682175159454346, 0.01423671841621399, 0.40061914920806885, 0.13996917009353638, 0.1738278865814209, 0.3537200689315796, 0.3656519055366516, -0.07074716687202454, -0.305637001991272}, {-0.19921742379665375, -0.4215702414512634, -0.16476842761039734, -0.4139534831047058, -0.16230334341526031, -0.37158817052841187, 0.005187076982110739, -0.35071033239364624, 0.13985249400138855}, {-0.35950911045074463, -0.28846311569213867, 0.3494657874107361, -0.04135897383093834, 0.057023268193006516, 0.07234852761030197, -0.15618474781513214, -0.17495831847190857, -0.2438427060842514}, {-15.09896469116211, 3.069504737854004, 8.57551097869873, -28.099872589111328, -35.11682891845703, -26.3050537109375, -46.645050048828125, -40.16168975830078, 16.438940048217773}, {0.10600853711366653, 0.18955089151859283, -0.12595294415950775, -0.2781698405742645, 0.14589877426624298, 0.27829235792160034, 0.3970456123352051, -0.09366714209318161, 0.40301835536956787}, {-0.4226982891559601, 0.27262717485427856, 0.19646459817886353, -0.4266708195209503, 0.4348961114883423, -0.22280970215797424, -0.15128996968269348, 0.44514667987823486, 0.30777835845947266}, {0.18155241012573242, -0.2931888699531555, -0.373379647731781, -0.37468263506889343, 0.30244266986846924, 0.017852306365966797, 0.22103655338287354, -0.07708340883255005, 0.24425452947616577}, {-14.787198066711426, 2.826172113418579, 8.610061645507812, -27.436874389648438, -34.349761962890625, -25.963653564453125, -46.314552307128906, -38.27753829956055, 15.982437133789062}, {0.1064932569861412, 0.01774994656443596, -0.0842067077755928, -0.26090091466903687, -0.2865777909755707, -0.3510627746582031, 0.16129769384860992, -0.25444793701171875, 0.19553439319133759}, {-1.3120184881502457e-37, -1.277703042940351e-37, -1.2862558961433268e-37, -1.320228415593032e-37, -1.3157931377976282e-37, 0.2689282298088074, 0.47242993116378784, 0.37089237570762634, 0.458604633808136}};

//the function for which this whole circus is established
void InputToOutput(double TestInput[])
{
//Compute hidden layer activations

  for ( i = 0 ; i < HiddenNodes ; i++ ) 
  {
    Accum = HiddenWeights[InputNodes][i];
    for ( j = 0 ; j < InputNodes ; j++ ) 
    {
      Accum += TestInput[j] * HiddenWeights[j][i];
    }
    Hidden[i] = (Accum>0?Accum:0);
  }

//Compute output layer activations and calculate errors

  for ( i = 0 ; i < OutputNodes ; i++ ) 
  {
    Accum = OutputWeights[HiddenNodes][i];
    for ( j = 0 ; j < HiddenNodes ; j++ ) 
    {
      Accum += Hidden[j] * OutputWeights[j][i];
    }
    //ReLu
    Output[i] = Accum;//(Accum>0?Accum:0);
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

  unsigned inputIndex = 1;
  //std::cout<<"Enter input index\n";
  //std::cin>>inputIndex;
  
  InputToOutput(Input[inputIndex]);
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
