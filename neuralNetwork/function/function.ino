//this can train and run a custom neural network having an architecture consists of one hidden layer only along with input and output layers on a arduino 
//activations in order; sigmoid, linear
#include <math.h>

const int numberOfDataSet = 2; 
const int inputNodes = 9;
const int hiddenNodes = inputNodes+1;
const int outputNodes = 9;

//the training data because I can't be bothered to write a file handling system for this
//0 is min value of the sensor and 1 is the max value of the sensor
double Input[numberOfDataSet][inputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

const double Target[numberOfDataSet][outputNodes] = 
{
  {0,0,0,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1,1}
};

int i, j, p;
double Accum;

double Hidden[hiddenNodes];
double Output[outputNodes];
double HiddenWeights[inputNodes + 1][hiddenNodes] = 
//+1
{{0.2372424304485321, -0.04173107445240021, 0.10782608389854431, 1.2396361827850342, -0.10749471187591553, -0.5013482570648193, -0.7476458549499512, 1.1017314195632935, 0.39326512813568115, -1.7640278339385986}, {0.7175414562225342, -0.5605186223983765, -0.3781200349330902, -1.2166167497634888, -0.4530803859233856, 0.03383808210492134, -0.11919645965099335, -0.7489916086196899, 0.1216551885008812, -0.35555630922317505}, {-0.47189411520957947, -0.324931800365448, -0.6123314499855042, 0.7144972085952759, -0.40516066551208496, 0.4502118229866028, -0.27037304639816284, -0.6976190209388733, 0.40748584270477295, -0.0308359544724226}, {-0.5268606543540955, -0.26367244124412537, 0.1160619854927063, 0.29049208760261536, 0.11362171173095703, -0.558212161064148, 0.0889626145362854, 0.12270867824554443, -0.36316031217575073, -0.3855084478855133}, {-0.6584124565124512, 0.2820269465446472, -0.23764276504516602, -0.35170871019363403, -0.5416207313537598, 0.13910865783691406, -0.1813768595457077, 0.5115007758140564, 0.16199740767478943, 1.3838088512420654}, {0.26462846994400024, -0.20783071219921112, 0.11520207673311234, -0.32035183906555176, -0.5079848766326904, 0.27908989787101746, -0.0745268389582634, 0.8175747394561768, -0.3756622076034546, -0.02271248959004879}, {0.7112996578216553, -0.025961924344301224, -0.23713085055351257, 0.43394869565963745, -0.4617380201816559, -0.15427084267139435, 0.17863839864730835, 0.05782397836446762, -0.31547343730926514, -0.43340981006622314}, {-0.12372763454914093, 0.5513028502464294, -0.5249045491218567, -0.6629472374916077, 0.5008676648139954, -0.4322834014892578, 0.28594866394996643, -0.07146935164928436, -0.003119384404271841, 0.2143077850341797}, {-0.14961034059524536, -0.48062387108802795, 0.8488260507583618, 0.7083261609077454, -0.2403126358985901, -0.28491896390914917, 1.521106481552124, -0.8637721538543701, -0.37755241990089417, 0.8127353191375732}, {-0.15630193054676056, -0.0535942018032074, 0.45869508385658264, -0.5948426723480225, 0.0, -0.05134006217122078, -0.5917301774024963, -0.5476990342140198, -0.1450568437576294, 0.5562321543693542}};

//+2
//{{0.7617313861846924, 1.2771120071411133, -0.03302490711212158, 0.7697224617004395, 1.04900324344635, 0.2307189702987671, -1.738705039024353, -0.40897417068481445, -0.3709791600704193, -0.016510628163814545, 0.10091515630483627}, {0.05825379490852356, -0.611723005771637, 0.3382148742675781, 0.20729869604110718, 0.7167754173278809, 0.18035295605659485, -1.5399426221847534, -0.05909636244177818, -0.09938077628612518, -0.22557099163532257, 0.2996949553489685}, {-0.7235302925109863, -0.8522010445594788, -0.5336499810218811, -2.0828843116760254, 2.794006586074829, 0.3503645062446594, 2.132178783416748, -0.17861786484718323, 0.11882796138525009, 0.3998217284679413, 0.24298380315303802}, {0.03405896946787834, -0.7534070611000061, 0.0838371068239212, -1.4493108987808228, -1.0885651111602783, -0.10714226216077805, 1.775319218635559, 0.4745936989784241, -0.3015216886997223, -0.5482735633850098, 0.004992026835680008}, {-0.22691503167152405, 0.5621089935302734, 0.018845872953534126, -0.9411344528198242, -0.1587262898683548, -0.5661607384681702, 0.17750249803066254, -0.13150592148303986, -0.1665550321340561, -0.1503223478794098, -0.5965235233306885}, {0.4308552145957947, 0.37955427169799805, 0.011333090253174305, 0.43908941745758057, -0.13728757202625275, -0.3057219088077545, -0.007532347925007343, -0.18190065026283264, 0.189792200922966, 0.28300896286964417, -0.11131829768419266}, {-0.15815824270248413, 0.3771046996116638, -0.11934538930654526, 1.1205053329467773, -0.7805119156837463, -0.07717723399400711, -0.05301770195364952, 0.3446691930294037, 0.44486677646636963, -0.29639559984207153, 0.19119642674922943}, {-0.04521135985851288, 0.3730805218219757, -0.005058547481894493, 3.615126371383667, -1.4044748544692993, 0.27593353390693665, -0.39134788513183594, -0.4816725552082062, -0.1531590074300766, 0.45387619733810425, 0.07617644220590591}, {-0.4409688115119934, -0.8174986243247986, -0.030697232112288475, -0.6663796305656433, -0.07781166583299637, -0.5981059670448303, 0.029261726886034012, -0.3587857484817505, -0.017424628138542175, 0.13242870569229126, -0.15376178920269012}, {-0.0577230341732502, -0.4547232389450073, -0.07666060328483582, -0.7213592529296875, 0.07032431662082672, -0.08694005012512207, -0.7647398114204407, -0.03429170325398445, -0.05816148221492767, -0.16442617774009705, -0.14792180061340332}};
//{{0.6411682963371277, 1.506191372871399, -0.22511692345142365, 0.5755581855773926, 1.3056262731552124, 0.2307189702987671, -1.1319209337234497, -0.40897417068481445, -0.3709791600704193, -0.016510628163814545, 0.10091515630483627}, {-0.06772787868976593, -0.15213987231254578, 0.23396213352680206, 0.007513502612709999, 0.7260396480560303, 0.18035295605659485, -1.1103607416152954, -0.05909636244177818, -0.09938077628612518, -0.22557099163532257, 0.2996949553489685}, {-0.7235302925109863, -1.1514261960983276, -1.0878725051879883, -2.0828843116760254, 3.123628854751587, 0.3503645062446594, 2.928596019744873, -0.17861786484718323, 0.11882796138525009, 0.3998217284679413, 0.24298380315303802}, {-0.013353485614061356, -0.687366247177124, 0.005210844334214926, -1.5159696340560913, -1.5926892757415771, -0.10714226216077805, 2.465506076812744, 0.4745936989784241, -0.3015216886997223, -0.5482735633850098, 0.004992026835680008}, {-0.34883278608322144, 0.7336159348487854, 0.09589877724647522, -1.1300137042999268, 0.30540305376052856, -0.5661607384681702, 0.6770473718643188, -0.13150592148303986, -0.1665550321340561, -0.1503223478794098, -0.5965235233306885}, {0.3021642565727234, 0.5328902006149292, 0.15032970905303955, 0.2369796633720398, -0.02313477173447609, -0.3057219088077545, -0.43205082416534424, -0.18190065026283264, 0.189792200922966, 0.28300896286964417, -0.11131829768419266}, {-0.2841399312019348, 0.293842613697052, -0.05523942410945892, 0.9226512908935547, -0.8005756139755249, -0.07717723399400711, -0.35902976989746094, 0.3446691930294037, 0.44486677646636963, -0.29639559984207153, 0.19119642674922943}, {-0.15493732690811157, 0.21251273155212402, -0.10507343709468842, 3.4076988697052, -2.5306568145751953, 0.27593353390693665, -2.0777769088745117, -0.4816725552082062, -0.1531590074300766, 0.45387619733810425, 0.07617644220590591}, {-0.4910905361175537, -0.8984803557395935, -0.26464492082595825, -0.7563709020614624, 0.6414960622787476, -0.5981059670448303, 0.634452223777771, -0.3587857484817505, -0.017424628138542175, 0.13242870569229126, -0.15376178920269012}, {-0.1931871920824051, -0.6374941468238831, 0.7804905772209167, -0.9341070055961609, 0.11202242225408554, -0.08694005012512207, -1.1546597480773926, -0.03429170325398445, -0.05816148221492767, -0.16442617774009705, -0.14792180061340332}};

//+3
//{{0.07277172803878784, -1.8246489763259888, 0.17646914720535278, 0.6970224380493164, -0.32711538672447205, -1.2151124477386475, -0.2720818519592285, 0.14240334928035736, 0.9208246469497681, -0.44817376136779785, 0.008748531341552734, 0.30461376905441284}, {-0.1737297773361206, 0.713103711605072, -0.3411928713321686, 1.1041864156723022, -0.18575158715248108, -1.4768412113189697, 0.45079419016838074, 0.38917455077171326, -1.1823146343231201, 0.2825430929660797, -0.18751037120819092, 1.1499634981155396}, {-0.2878950834274292, 0.5079748034477234, 0.4835622310638428, -1.035158395767212, -0.3054596185684204, -0.6553043127059937, 0.02960941195487976, 0.1791650652885437, -0.24867211282253265, -0.03219513222575188, 0.4944663643836975, -1.0211436748504639}, {-0.35986098647117615, 0.17452019453048706, -0.5426778197288513, -0.42146745324134827, 0.03697687387466431, -1.3419320583343506, -0.364209920167923, -0.406233549118042, 0.4175410270690918, 0.5005131363868713, 0.18965202569961548, -0.5813739895820618}, {0.1816454529762268, 0.3136470913887024, 0.290154367685318, -0.02464916929602623, -0.2514345943927765, 0.6196752190589905, -0.5120415091514587, -0.40483763813972473, 0.7592641115188599, -0.4125228524208069, -0.4778386354446411, 0.19201895594596863}, {-0.4025419354438782, -0.13407859206199646, 0.15888409316539764, -0.43662089109420776, -0.28588953614234924, 0.18643945455551147, 0.3429528772830963, -0.372012734413147, 0.5162877440452576, -0.2784952223300934, -0.01818746328353882, 1.205819010734558}, {-0.07992556691169739, -0.17907120287418365, 0.37044021487236023, 0.4050655961036682, -0.4629887044429779, 0.04954482987523079, 0.10585512965917587, 0.19888189435005188, -0.4527982771396637, 0.4720711410045624, -0.4980659484863281, -0.1417020857334137}, {-0.052841126918792725, -0.2636776566505432, -0.562370240688324, -0.06870455294847488, 0.2473236322402954, 2.548490047454834, 0.010446329601109028, -0.007555607706308365, -0.14167727530002594, -0.34969282150268555, 0.08919340372085571, 2.3083276748657227}, {-0.2038041055202484, 0.44049200415611267, -0.40014395117759705, -0.021710706874728203, -0.1689547300338745, 0.7554804682731628, -0.49855151772499084, -0.27361124753952026, -0.6649360656738281, -0.23442134261131287, -0.050524383783340454, -1.632813811302185}, {0.0, -0.06018862873315811, -0.2187921404838562, -0.4073014259338379, 0.0, 0.40568262338638306, -0.0896034687757492, -0.08052743226289749, -0.40392810106277466, -0.022754335775971413, 0.0, -1.1740491390228271}};
//{{0.9185472130775452, -0.060944054275751114, -1.1919838190078735, 0.6515083312988281, -0.18392033874988556, 0.13506829738616943, -0.29976770281791687, -0.438590407371521, 0.19797292351722717, -0.32488706707954407, 0.855219304561615, 0.7871593236923218}, {-0.08473378419876099, -0.2635735273361206, 1.4309298992156982, -1.7414299249649048, 0.008551694452762604, 0.21425651013851166, 0.1970440149307251, 0.5169315934181213, -1.2058169841766357, 0.2770933508872986, -1.5398005247116089, 1.3080123662948608}, {-0.48311251401901245, -0.18220925331115723, -0.9431163668632507, 1.0401313304901123, 0.14924776554107666, 0.16179247200489044, 0.3444226086139679, 0.2024592161178589, -0.7232614755630493, -0.026153763756155968, 0.9133160710334778, 1.396500825881958}, {0.12892402708530426, -0.18660375475883484, -0.8587620854377747, 0.7191003561019897, -0.3768928050994873, -0.5441874265670776, -0.5282203555107117, -0.4239017963409424, -0.4751294255256653, 0.1816936880350113, 0.07204437255859375, -0.14308704435825348}, {0.08398787677288055, 0.11693930625915527, -0.43160003423690796, -0.5787924528121948, 0.34847068786621094, -0.2947009205818176, -0.056639302521944046, -0.46068981289863586, -0.7170074582099915, -0.510729193687439, -0.030147487297654152, -0.3147028386592865}, {0.4933931231498718, 0.046177491545677185, -0.1276901662349701, 0.15525929629802704, -0.3692905306816101, 0.3435598313808441, 0.24144905805587769, 0.21633553504943848, 0.694744348526001, -0.16750752925872803, 0.436689555644989, -0.019240887835621834}, {-0.21591731905937195, -0.07678993791341782, 0.8485660552978516, 0.32217490673065186, 0.3134751319885254, -0.38961368799209595, 0.12012393772602081, -0.3844030201435089, 1.2066712379455566, 0.1055702343583107, -0.19954369962215424, -0.9779417514801025}, {-1.1383472681045532, 0.22584575414657593, 0.46920084953308105, -0.009458950720727444, -0.2730051279067993, 0.3387725353240967, -0.4250858426094055, -0.1883973479270935, 3.356964111328125, 0.2886136472225189, -0.36680537462234497, -0.10046274960041046}, {-0.5035662055015564, 0.14637520909309387, 0.8562244176864624, -0.07699306309223175, 0.260429710149765, 0.08996395766735077, 0.09471653401851654, 0.5183997750282288, -1.449819803237915, 0.063158318400383, 0.12514112889766693, -0.6248909831047058}, {-0.02999044395983219, -0.06161189824342728, -0.30396121740341187, -0.4443624019622803, -0.09279560297727585, -0.1471184939146042, -0.050587285310029984, 0.0, -1.0545834302902222, -0.2201004922389984, -0.1389760822057724, -0.1129225641489029}};

//+5
//{{0.8266298770904541, -0.2682269811630249, -0.033664703369140625, 0.0116896852850914, 1.3832412958145142, 0.8979625105857849, 0.33648282289505005, 0.3641309142112732, -0.3387749493122101, 0.7713571786880493, 1.2499440908432007, -0.1630043089389801, -0.3595924377441406, -0.2239314615726471}, {-1.5153093338012695, -0.43241867423057556, -0.3677126467227936, 0.40738144516944885, 1.3740288019180298, 0.1042083278298378, 0.01976621150970459, 0.1883561611175537, 0.31698641180992126, -0.5717867016792297, 1.1628336906433105, -0.1649458408355713, -0.32694122195243835, 0.38347673416137695}, {1.1715326309204102, 0.2300797700881958, -0.023832589387893677, 0.22015923261642456, 0.06356602907180786, 1.4078797101974487, -0.16012156009674072, 0.18721890449523926, -0.2762489914894104, -1.0126960277557373, -1.2059621810913086, 0.1690540909767151, 0.18194667994976044, -0.34815090894699097}, {0.4565785229206085, -0.4163694381713867, -0.5007094740867615, -0.39150193333625793, -0.6015106439590454, -0.11425697058439255, -0.08103156089782715, 0.25987792015075684, -0.2532009780406952, -0.7545546293258667, -0.6307548880577087, -0.41742339730262756, 0.2147643268108368, 0.12111857533454895}, {-0.2711324691772461, -0.2621675133705139, -0.17711052298545837, -0.04025445133447647, -0.6376290321350098, 0.3458913266658783, -0.24493709206581116, -0.1676810085773468, -0.4835125207901001, 0.2636962831020355, -0.393327534198761, -0.254705011844635, -0.45620453357696533, 0.4282890856266022}, {-0.3992668688297272, 0.07185715436935425, -0.20112869143486023, -0.26423269510269165, 0.47266125679016113, 0.5722203254699707, -0.3665459156036377, -0.43859878182411194, -0.025313427671790123, 0.267380952835083, 0.7181980609893799, -0.1129070520401001, 0.43616628646850586, -0.3641168177127838}, {0.16866755485534668, 0.09697872400283813, -0.3429291248321533, -0.3378502428531647, -1.1424870491027832, -0.47756779193878174, 0.119293212890625, -0.19129708409309387, 0.06659165024757385, 0.6941039562225342, 0.25044628977775574, -0.31643661856651306, -0.3295551836490631, -0.3578650653362274}, {0.25725260376930237, 0.2621336579322815, 0.194135844707489, 0.4367581307888031, -1.2384006977081299, -0.6206992864608765, 0.29794830083847046, -0.06607294082641602, 0.3328346014022827, 2.668178081512451, -0.5707070827484131, -0.23802757263183594, 0.11539103090763092, 0.031262680888175964}, {-0.2869603931903839, -0.418422669172287, 0.334714412689209, -0.3448169231414795, 1.2789485454559326, -1.3169296979904175, -0.21812167763710022, -0.3738808035850525, 0.10446323454380035, -1.669568419456482, -0.9361602067947388, -0.09536048769950867, 0.05393289402127266, -0.17550800740718842}, {-0.39525893330574036, 0.0, 0.0, -0.08898958563804626, 0.1997491866350174, -0.28086432814598083, 0.0, 0.0, -0.0590866394340992, 0.03602800518274307, -0.3503105938434601, 0.0, -0.04302126541733742, -0.11079710721969604}};



double OutputWeights[hiddenNodes + 1][outputNodes] = 
{{-2.2613876353716478e-06, 1.139396772487089e-07, 3.793749385749834e-07, 1.8701331327974913e-06, 3.793032306020905e-07, 0.043823376297950745, 0.5783501267433167, 0.08559589833021164, 1.1427924633026123}, {-0.04251106455922127, 0.3308565616607666, 0.534912645816803, -0.10568011552095413, -0.37031450867652893, -0.015674876049160957, -0.16045254468917847, -0.3194564878940582, 0.09162423014640808}, {-2.698858452276909e-06, 1.3598162240668898e-07, 4.527660451003612e-07, 2.231915914308047e-06, 4.52680382068138e-07, 0.8141687512397766, -0.9378588795661926, -0.15242122113704681, 0.1209893524646759}, {-1.4573640783055453e-06, 7.342907792917686e-08, 2.4449036573059857e-07, 1.2052182682964485e-06, 2.4444418045277416e-07, 0.10182185471057892, 0.2968728244304657, -0.9810660481452942, 0.03868797793984413}, {0.03595542907714844, -0.32671865820884705, -0.3218124508857727, 0.4658443331718445, 0.4455079436302185, -0.23995879292488098, 0.15128111839294434, 0.02769923210144043, 0.5533298850059509}, {0.2433527112007141, 0.363773912191391, 0.0013244471047073603, 0.34511199593544006, -0.059730127453804016, 0.12515053153038025, 0.5521854758262634, -0.07571977376937866, -0.28572386503219604}, {1.3108410712447949e-05, -6.604656164199696e-07, -2.1990940695104655e-06, -1.0840460163308308e-05, -2.198677748310729e-06, 0.5341455936431885, 0.9826861023902893, 0.25841012597084045, -0.719546914100647}, {-1.1622993270066218e-06, 5.856227858203056e-08, 1.9498978076626372e-07, 9.61204477789579e-07, 1.9495286096571363e-07, -0.17084909975528717, 0.8542309999465942, 0.14781008660793304, 0.008439501747488976}, {-0.08849490433931351, -0.2745378911495209, 0.04872170463204384, -0.007350231986492872, 0.3024468719959259, 0.4229976236820221, -0.24685277044773102, 0.19500026106834412, -0.2490002065896988}, {-5.465092272061156e-06, 2.7535796220945485e-07, 9.168352335109375e-07, 4.519550202530809e-06, 9.166617473965744e-07, -0.2621993124485016, 0.35728469491004944, -0.6218458414077759, 1.0171408653259277}, {3.414551656533149e-06, -1.7204172308993293e-07, -5.728322776121786e-07, -2.8237836886546575e-06, -5.72723877212411e-07, 0.08278750628232956, -0.1444302648305893, 0.7898278832435608, -0.05334002524614334}};

// {{0.10827790200710297, -0.01830965094268322, -0.059831105172634125, -0.06881660223007202, 0.020183540880680084, -0.004013718105852604, 0.6586358547210693, 0.3737134337425232, -0.6342146992683411}, {-0.04277987405657768, 0.007234021555632353, 0.02363887056708336, 0.0271889790892601, -0.007974383421242237, 0.0015857935650274158, 0.5629478096961975, 0.09355970472097397, -0.5415241122245789}, {0.297111451625824, 0.3348464071750641, 0.4144095778465271, 0.3961879312992096, -0.13546887040138245, -0.16621440649032593, -0.11138881742954254, 0.10738285630941391, 0.33830174803733826}, {-0.0032829842530190945, 0.000555148406419903, 0.001814077841117978, 0.0020865180995315313, -0.0006119645549915731, 0.0001216958844452165, -0.07364904135465622, 1.3636494874954224, 0.0677618607878685}, {0.011833813041448593, -0.0020010832231491804, -0.006539010908454657, -0.0075210463255643845, 0.0022058822214603424, -0.00043866384658031166, 0.020996741950511932, -0.05597527697682381, -1.2640670537948608}, {-0.5332438945770264, 0.5348332524299622, -0.019922055304050446, 0.5362338423728943, 0.4733350872993469, 0.10101386159658432, -0.3080783784389496, 0.21076349914073944, 0.20103907585144043}, {0.009666740894317627, -0.0016346328193321824, -0.0053415498696267605, -0.006143747363239527, 0.0018019286217167974, -0.00035833317087963223, 0.8689666986465454, -0.02031111717224121, 0.04278576374053955}, {0.22994793951511383, 0.034349702298641205, 0.4606072008609772, -0.5004128813743591, 0.3084271252155304, -0.5088657736778259, 0.4462694227695465, -0.04013415053486824, -0.3896638751029968}, {0.5439556241035461, -0.02035309560596943, -0.45759525895118713, 0.2669910490512848, 0.16188324987888336, -0.4508592188358307, 0.17610253393650055, 0.1499790996313095, -0.47362834215164185}, {-0.0680343434214592, -0.2807724177837372, -0.03745124861598015, 0.24820588529109955, -0.44581276178359985, -0.012240649200975895, -0.09141124784946442, 0.2784827947616577, -0.03342320770025253}, {0.16748151183128357, 0.14963532984256744, -0.14004379510879517, 0.19925446808338165, 0.1844034641981125, -0.5122633576393127, -0.36273127794265747, -0.010183777660131454, -0.40311864018440247}, {-0.0024706292897462845, 0.00041778016020543873, 0.0013651951448991895, 0.0015702216187492013, -0.00046053744154050946, 9.158288594335318e-05, 0.008300576359033585, 0.019114287570118904, 0.9666301012039185}};
// {{0.10621846467256546, -0.02055436372756958, -0.06258898228406906, -0.07144690304994583, 0.0210847407579422, -0.0028923426289111376, 0.6714664101600647, 0.21153046190738678, -0.5998671650886536}, {-7.866506335219015e-10, -8.856962940839708e-10, -1.0959206875327254e-09, -1.047660513897597e-09, 3.5824807009809945e-10, 4.397314734561064e-10, 0.6699001789093018, 0.5806511640548706, -0.6571357846260071}, {3.713917173797654e-09, 4.181530233182684e-09, 5.1740358664176256e-09, 4.946191012322743e-09, -1.6913529465867327e-09, -2.0760497765337504e-09, -0.07139270007610321, -1.2597694396972656, 0.09128768742084503}, {1.6461991492189298e-10, 1.8534691836880057e-10, 2.2933997756435787e-10, 2.1924073667634048e-10, -7.496961385022871e-11, -9.202127254637205e-11, -0.06010414659976959, -0.03944157809019089, 0.6902080774307251}, {2.369459295437082e-09, 2.6677937636776505e-09, 3.3010072453265593e-09, 3.1556428581325235e-09, -1.0790741633570633e-09, -1.32450861389799e-09, -0.39988523721694946, -0.5265554785728455, -1.1343902349472046}, {-0.5332438945770264, 0.5348332524299622, -0.019922055304050446, 0.5362338423728943, 0.4733350872993469, 0.10101386159658432, -0.3080783784389496, 0.21076349914073944, 0.20103907585144043}, {1.2147984840282788e-09, 1.36775135661793e-09, 1.6923936696500164e-09, 1.6178672845867936e-09, -5.532306834865608e-10, -6.790624174080051e-10, 0.7109644412994385, -0.9110415577888489, 0.06419339030981064}, {0.22994793951511383, 0.034349702298641205, 0.4606072008609772, -0.5004128813743591, 0.3084271252155304, -0.5088657736778259, 0.4462694227695465, -0.04013415053486824, -0.3896638751029968}, {0.5439556241035461, -0.02035309560596943, -0.45759525895118713, 0.2669910490512848, 0.16188324987888336, -0.4508592188358307, 0.17610253393650055, 0.1499790996313095, -0.47362834215164185}, {-0.0680343434214592, -0.2807724177837372, -0.03745124861598015, 0.24820588529109955, -0.44581276178359985, -0.012240649200975895, -0.09141124784946442, 0.2784827947616577, -0.03342320770025253}, {0.16748151183128357, 0.14963532984256744, -0.14004379510879517, 0.19925446808338165, 0.1844034641981125, -0.5122633576393127, -0.36273127794265747, -0.010183777660131454, -0.40311864018440247}, {-1.926662385187683e-09, -2.169245005845255e-09, -2.6841255884590964e-09, -2.5659268043654038e-09, 8.774203030803562e-10, 1.076988609405305e-09, 0.06546679884195328, 1.1231228113174438, 0.909814178943634}};
// {{0.19254004955291748, 0.4612845778465271, -0.12781819701194763, -0.18620920181274414, 0.28898584842681885, 0.48037189245224, 0.14037680625915527, -0.2829623520374298, 0.18486690521240234}, {-2.1379095325665953e-13, 4.12931218081794e-13, -4.164894882967285e-13, -1.5112344969175906e-12, -1.7168452532030642e-13, 1.2342911252533756e-13, 0.8354084491729736, -0.399591863155365, -0.10804083198308945}, {0.3036440908908844, -0.02324283868074417, -0.4480556547641754, 0.1921377182006836, -0.2881664037704468, 0.07494619488716125, -0.36530277132987976, -0.09303785115480423, -0.33642593026161194}, {1.4336366295497616e-12, -2.769037820218867e-12, 2.792891569056155e-12, 1.0134049849286342e-11, 1.1512785082906651e-12, -8.276910515477021e-13, 0.9001676440238953, -0.4940796196460724, 1.0808714628219604}, {0.4689487814903259, 0.23444032669067383, -0.16680848598480225, -0.44758614897727966, 0.25648874044418335, -0.5217611193656921, -0.08272761106491089, 0.11745607852935791, -0.05703097581863403}, {-8.652976338924256e-13, 1.6713123940359509e-12, -1.6857033347320027e-12, -6.11661415961251e-12, -6.948733580723621e-13, 4.995690818486442e-13, 0.04986414313316345, 0.7103704810142517, 0.9701281189918518}, {0.5010269284248352, -0.4657306373119354, 0.4270797669887543, -0.397258460521698, -0.18093273043632507, -0.4250003695487976, 0.25470516085624695, 0.40525826811790466, -0.023757165297865868}, {0.4230591654777527, 0.3642309308052063, 0.336282879114151, 0.3233473002910614, 0.08312052488327026, 0.31071770191192627, -0.19569285213947296, -0.26755502820014954, -0.18505215644836426}, {-5.340520777344371e-13, 1.0315129826687919e-12, -1.0403968268499209e-12, -3.775098723207693e-12, -4.2886863765162486e-13, 3.0832809721180465e-13, 0.8927423357963562, 0.23431377112865448, -0.4259609282016754}, {0.5319098234176636, -0.036228425800800323, -0.22873368859291077, -0.5088557600975037, -0.18941746652126312, 0.15752698481082916, -0.3936634361743927, 0.45345714688301086, 0.3016504645347595}, {-0.0009475350379943848, -0.3905792832374573, -0.1430271565914154, 0.38055890798568726, 0.1826024055480957, -0.5280593633651733, 0.10327082872390747, 0.47824913263320923, -0.3251013159751892}, {-1.2973292145418425e-12, 2.505767945568116e-12, -2.527349857173178e-12, -9.170534991109758e-12, -1.0418162643341389e-12, 7.489962426318608e-13, 0.0922447219491005, 1.136125922203064, -0.18892787396907806}, {1.4600989688140498e-12, -2.8201579526515586e-12, 2.844445382357841e-12, 1.0321125898382633e-11, 1.1725272445681223e-12, -8.429695743522592e-13, -0.12112703174352646, -0.5502440333366394, 0.17555692791938782}};
// {{-0.019523348659276962, -0.030179738998413086, 0.02069111354649067, -0.03902750834822655, -0.03637244552373886, -0.030183499678969383, 0.6526814699172974, 0.9769922494888306, -0.44496840238571167}, {-0.13203059136867523, 0.3918420374393463, -0.3991348147392273, -0.04509882256388664, -0.4565146267414093, -0.45037028193473816, -0.2112063616514206, -0.2536919414997101, -0.24530641734600067}, {0.004638479556888342, 0.007170293014496565, -0.004915924277156591, 0.009272398427128792, 0.008641591295599937, 0.007171182427555323, 1.3089210987091064, -0.016955912113189697, 0.5821815729141235}, {-0.04597625508904457, -0.07107137888669968, 0.04872626066207886, -0.09190730005502701, -0.08565481007099152, -0.07108023017644882, 0.17970161139965057, -0.7137504816055298, -0.18193857371807098}, {-0.050744958221912384, -0.2299865186214447, -0.49626588821411133, 0.2165532410144806, -0.044246673583984375, 0.29706209897994995, 0.3164646327495575, -0.15824413299560547, 0.01626814529299736}, {0.29339599609375, -0.23835086822509766, -0.39915671944618225, 0.07678110897541046, 0.4559534788131714, 0.33041098713874817, 0.08136973530054092, -0.3310808539390564, 0.45073074102401733}, {-0.34726428985595703, -0.2233622968196869, -0.42969563603401184, -0.46740996837615967, -0.44385212659835815, 0.2179594188928604, -0.23185445368289948, -0.34305205941200256, -0.09080963581800461}, {0.34882962703704834, 0.4947349429130554, 0.031076788902282715, -0.3539707064628601, -0.3217184245586395, 0.1993812918663025, 0.34498924016952515, -0.2864651679992676, -0.07286337018013}, {0.003545790910720825, 0.005481180734932423, -0.0037578768096864223, 0.007088094018399715, 0.006605887785553932, 0.0054818629287183285, 0.027685321867465973, 1.169566035270691, 0.3492535650730133}, {0.034131862223148346, 0.16869096457958221, -0.10310428589582443, -0.0772734060883522, 0.3004585802555084, -0.24820268154144287, -0.32080769538879395, 0.26964399218559265, -0.18454289436340332}, {0.04412752762436867, 0.0682135745882988, -0.04676695540547371, 0.08821167051792145, 0.08221059292554855, 0.0682220607995987, 0.5918485522270203, -0.11503466963768005, -0.5179654955863953}, {7.00420350767672e-05, 0.00010827393998624757, -7.423182250931859e-05, 0.00014001471572555602, 0.0001304897159570828, 0.00010828598897205666, 0.023272765800356865, 0.039297834038734436, -0.967780590057373}, {-0.001271619345061481, -0.0019657055381685495, 0.0013476787135004997, -0.002541985595598817, -0.00236905412748456, -0.0019659476820379496, 0.027152780443429947, -0.039339032024145126, 0.9858692288398743}};

//{{5.023741297092987e-13, -7.49844739252048e-13, 2.8130405990700602e-12, 2.0087577434768633e-12, 1.419957684420381e-12, 0.2795369029045105, 0.5715013146400452, -0.7106619477272034, -0.02712079882621765}, {0.13870978355407715, -0.34689027070999146, -0.4897569417953491, -0.3760117292404175, 0.4467052221298218, 0.19312512874603271, -0.4560548663139343, 0.20890343189239502, 0.2497738003730774}, {-0.504821240901947, 0.44971799850463867, 0.03343963623046875, -0.27239805459976196, -0.2016841173171997, -0.4579429626464844, -0.3508945405483246, -0.2238396406173706, 0.3312188982963562}, {-0.50211501121521, -0.2650609612464905, 0.2916747033596039, 0.39739519357681274, 0.015570903196930885, 0.1753728687763214, -0.43692174553871155, -0.17648233473300934, -0.2638102173805237}, {2.4772709925069414e-12, -3.697582604683669e-12, 1.3871482087979281e-11, 9.905449724345594e-12, 7.001995337707623e-12, 0.6385688781738281, -0.5855563879013062, 0.061884552240371704, -0.675339937210083}, {-2.8584396134789802e-12, 4.2665155262910925e-12, -1.600583207639339e-11, -1.1429563025189271e-11, -8.079368771229944e-12, -0.27499446272850037, 0.38511502742767334, 0.1622494012117386, -0.8567711710929871}, {-0.25237318873405457, -0.04408824443817139, -0.06112736463546753, -0.23601853847503662, 0.10597997903823853, 0.41983509063720703, -0.4405648112297058, -0.04499897360801697, 0.2518662214279175}, {0.18202650547027588, 0.11589610576629639, -0.14468911290168762, -0.18723765015602112, -0.14398112893104553, 0.10968774557113647, 0.24129986763000488, 0.293843150138855, 0.2949313521385193}, {0.16958202421665192, -0.3758523166179657, -0.12538008391857147, -0.4104582667350769, -0.35421913862228394, -0.18537704646587372, -0.3444361686706543, -0.4677853286266327, 0.13202226161956787}, {3.750532870383516e-12, -5.598055005662239e-12, 2.100110363389973e-11, 1.4996621999774362e-11, 1.0600863069309963e-11, 0.02506575919687748, -0.7335176467895508, 0.756679356098175, 0.22101463377475739}, {-2.788403405742934e-12, 4.161978921224385e-12, -1.5613666609626264e-11, -1.1149520542930169e-11, -7.881409067045375e-12, -0.2910365164279938, 1.230764389038086, -0.22592465579509735, 0.3993408679962158}, {0.4876636266708374, -0.3254356384277344, 0.22096043825149536, 0.487606406211853, -0.16069668531417847, 0.12596285343170166, 0.21718668937683105, -0.0631355345249176, 0.3627450466156006}, {0.19837574660778046, 0.10457046329975128, -0.4474376142024994, 0.14354455471038818, -0.5088965892791748, 0.3817233145236969, -0.24329496920108795, 0.3742232918739319, 0.0015050843358039856}, {-0.15503187477588654, 0.36976832151412964, 0.21712474524974823, -0.3024844229221344, -0.20669633150100708, -0.06710444390773773, 0.2010374218225479, 0.28291165828704834, -0.03890309855341911}, {-2.345729079727965e-12, 3.501241831183055e-12, -1.3134907224376935e-11, -9.37947115808857e-12, -6.630192488782383e-12, -0.009179221466183662, 0.719797670841217, -0.11677811294794083, 0.9642325639724731}};


void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(A15));       //Collect a random ADC sample for Randomization.
}

void loop() 
{
  driveNN();
}


void driveNN()
{
  Serial.println("Running the neural network");
  
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
    tem = map(tem, 630, 880, 0, 100);
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
    tem = map(tem, 494, 800, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[3] = double(tem)/100;
    //[1]
    tem = analogRead(A8);
    tem = map(tem, 560, 787, 0, 100);
    tem =  constrain(tem, 0, 100);
    TestInput[8] = double(tem)/100;

    //thumb
    // [0]
    tem = analogRead(A4);
    tem = (tem, 520, 750, 0, 100);
    tem = constrain(tem, 0, 100);
    TestInput[4] = double(tem)/100;
  
    InputToOutput(TestInput); //INPUT to the function to obtain OUTPUT

    //conversion from bcd to decimal    
    int bin = 0;
    int decValue = 0;
    int base = 256; 
    for(i = 0; i<outputNodes; i++)
    {
        bin = Output[i] > 0.6 ? 1: 0;
 
        decValue += bin * base;
        base = base/ 2;
//        Serial.print(Output[i] > 0.5 ? 1: 0);
//        Serial.print(":");
    }
    
    Serial.println(decValue);
    delay(300);
//    Serial.println();/
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
    Hidden[i] = (Accum>0? Accum: 0);
    //Accum
    //(Accum>0? Accum: 0);
    //1.0 / (1.0 + exp(-Accum));
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
//    Accum;
//    (Accum>0? Accum: 0);
  }
}
