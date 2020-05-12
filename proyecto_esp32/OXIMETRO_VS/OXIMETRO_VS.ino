// Pantalla OLED
#include <U8g2lib.h>
#include <U8x8lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);



#include <arduinoFFT.h>
#include <defs.h>
#include <types.h>

#include "Arduino.h"
#include "Esp.h"

#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;
//#define debug Serial


arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

//SEÑALES DE PRUEBA CON SATUARACIÓN 92.89% Fc= 98 bpm
float senalIR[512]={122741,122762,122778,122776,122790,122810,122796,122807,122860,122886,122856,122747,122673,122586,122458,122318,122263,122243,122215,122150,122076,122064,122017,121991,121972,121974,122014,122055,122039,122037,122064,122056,122078,122069,122131,122209,122214,122218,122211,122216,122212,122203,122219,122278,122326,122318,122294,122321,122322,122322,122309,122322,122419,122460,122466,122456,122486,122501,122518,122493,122600,122658,122708,122705,122724,122731,122742,122734,122786,122849,122893,122906,122851,122801,122732,122615,122485,122414,122359,122316,122225,122151,122110,122059,122014,121957,121984,122040,122054,122013,122009,122021,122054,122059,122056,122099,122203,122221,122202,122233,122244,122247,122241,122261,122316,122345,122320,122307,122321,122331,122346,122330,122362,122453,122468,122498,122466,122505,122515,122530,122537,122568,122650,122680,122657,122684,122714,122723,122707,122753,122813,122862,122860,122798,122761,122684,122582,122420,122337,122283,122215,122110,122037,121977,121927,121884,121825,121834,121869,121879,121856,121840,121861,121877,121876,121883,121961,122035,122066,122066,122109,122116,122126,122093,122143,122190,122216,122256,122207,122231,122254,122255,122241,122273,122344,122422,122430,122407,122427,122464,122463,122444,122496,122577,122614,122656,122649,122675,122702,122708,122687,122752,122797,122841,122806,122721,122668,122568,122458,122302,122256,122239,122156,122088,121987,121952,121919,121863,121812,121883,121910,121891,121875,121896,121911,121929,121890,121966,122064,122119,122124,122127,122158,122171,122162,122128,122188,122245,122278,122271,122271,122297,122300,122314,122329,122373,122439,122499,122482,122503,122509,122544,122554,122558,122609,122689,122734,122757,122752,122773,122806,122796,122786,122853,122919,122916,122879,122825,122757,122632,122513,122424,122387,122340,122263,122187,122148,122111,122074,122042,122052,122094,122112,122072,122052,122071,122101,122101,122108,122196,122274,122288,122287,122323,122326,122320,122319,122330,122405,122450,122433,122441,122440,122451,122457,122438,122490,122572,122603,122628,122612,122634,122653,122634,122648,122713,122792,122829,122819,122823,122876,122876,122869,122890,122949,123009,122989,122935,122880,122789,122684,122570,122505,122473,122440,122372,122315,122287,122278,122228,122211,122212,122270,122281,122270,122258,122285,122309,122319,122306,122377,122455,122463,122450,122457,122482,122482,122465,122502,122528,122576,122600,122567,122573,122592,122601,122597,122638,122700,122759,122745,122743,122758,122781,122784,122772,122825,122878,122921,122935,122951,122979,122981,122991,122966,122996,123025,123009,122908,122781,122714,122613,122514,122425,122420,122449,122401,122346,122325,122303,122302,122258,122270,122308,122345,122352,122343,122361,122385,122387,122373,122424,122506,122535,122531,122501,122502,122511,122495,122485,122525,122591,122587,122573,122583,122619,122611,122605,122664,122723,122777,122784,122787,122801,122822,122835,122824,122860,122931,122987,122995,122979,123030,123036,123034,122992,122993,123010,122945,122852,122730,122636,122554,122473,122407,122398,122407,122382,122337,122320,122296,122272,122242,122263,122297,122336,122353,122351,122361,122402,122396,122408,122434,122510,122540,122532,122506,122518,122526,122517,122483,122531,122613,122647,122649,122637,122650,122670,122685,122650,122724,122795,122846,122856,122844,122875,122889,122871,122888,122935,123009,123038,123044,123050,123064,123055,123014,122999,122985,122919,122813,122703,122622,122541,122470,122401,122377,122415,122392,122364,122320,122308};
float senalRojo[512]={153818,153849,153922,153923,153975,154092,154155,154184,154131,154105,153992,153819,153578,153359,153192,153016,152787,152600,152473,152355,152267,152185,152161,152190,152128,152093,152105,152124,152131,152136,152200,152283,152388,152434,152455,152496,152511,152512,152502,152533,152600,152634,152647,152633,152662,152677,152693,152652,152732,152839,152896,152882,152930,152979,153026,153041,153077,153186,153289,153342,153368,153426,153473,153508,153521,153590,153711,153785,153824,153846,153862,153866,153767,153610,153466,153300,153090,152834,152609,152413,152265,152116,151983,151943,151956,151891,151834,151812,151799,151795,151790,151851,151955,152049,152099,152126,152171,152226,152254,152241,152278,152364,152411,152400,152387,152396,152408,152407,152410,152466,152543,152621,152620,152642,152688,152710,152738,152768,152863,152958,153007,153030,153064,153126,153143,153175,153253,153354,153434,153479,153462,153507,153481,153404,153248,153073,152911,152667,152424,152158,151960,151772,151608,151437,151400,151389,151312,151259,151242,151234,151223,151189,151256,151346,151447,151483,151555,151612,151665,151698,151703,151779,151869,151925,151914,151911,151929,151956,151952,151960,152025,152101,152155,152196,152242,152290,152313,152362,152371,152473,152579,152651,152685,152731,152786,152854,152883,152900,153021,153111,153157,153187,153214,153206,153174,153024,152836,152687,152476,152221,151969,151736,151580,151410,151267,151177,151170,151152,151087,151030,151015,151013,151013,151013,151075,151191,151297,151349,151401,151482,151522,151587,151590,151656,151751,151765,151781,151796,151810,151852,151877,151870,151950,152078,152108,152159,152210,152269,152317,152315,152379,152487,152604,152629,152677,152750,152813,152868,152882,152946,153044,153145,153192,153214,153234,153232,153130,152957,152812,152647,152417,152179,151969,151821,151683,151527,151414,151394,151389,151352,151288,151295,151282,151289,151310,151303,151456,151556,151612,151659,151738,151795,151827,151829,151902,151982,152033,152061,152049,152071,152097,152102,152101,152179,152302,152342,152380,152415,152452,152509,152531,152570,152679,152779,152815,152847,152894,152964,153018,153026,153121,153221,153295,153339,153336,153356,153315,153214,153037,152855,152729,152533,152328,152141,152016,151906,151814,151717,151705,151731,151743,151711,151719,151718,151740,151742,151849,151946,152045,152080,152132,152166,152225,152225,152241,152298,152366,152405,152408,152419,152452,152458,152461,152482,152548,152655,152681,152718,152754,152789,152841,152858,152891,152968,153075,153149,153174,153210,153252,153294,153335,153360,153470,153561,153548,153509,153452,153324,153146,152891,152708,152563,152415,152283,152138,152043,151977,151912,151829,151837,151887,151918,151879,151872,151899,151934,151971,151975,152079,152217,152256,152265,152306,152340,152332,152335,152385,152414,152493,152479,152477,152507,152532,152558,152564,152612,152739,152793,152822,152857,152907,152955,152965,152985,153101,153195,153282,153306,153337,153400,153443,153473,153478,153559,153647,153657,153570,153421,153249,153032,152795,152602,152470,152361,152228,152111,152040,151966,151906,151822,151838,151885,151896,151897,151900,151939,151978,151995,152048,152150,152265,152279,152301,152344,152383,152381,152386,152387,152437,152514,152520,152541,152569,152602,152638,152632,152691,152782,152859,152893,152934,152980,153043,153074,153095,153155,153281,153346,153393,153417,153487,153527,153567,153573,153638,153665,153661,153530,153336,153129,152913,152716,152474,152356,152287,152206,152113,152003,151958,151912,151880,151805};

//SEÑALES DE PRUEBA CON SATUARACIÓN 88.5% Fc= 105 bpm 
//float senalIR[512]={121072,121148,121168,121168,121211,121238,121235,121246,121256,121338,121413,121398,121404,121414,121433,121444,121414,121440,121509,121547,121568,121562,121607,121608,121616,121611,121633,121746,121766,121770,121771,121774,121801,121787,121783,121857,121948,121949,121919,121898,121850,121776,121668,121598,121570,121511,121424,121330,121303,121238,121192,121159,121185,121228,121275,121267,121262,121262,121286,121301,121309,121393,121483,121478,121486,121502,121527,121514,121503,121520,121566,121607,121591,121567,121577,121595,121588,121581,121610,121679,121730,121730,121713,121737,121760,121749,121756,121804,121891,121935,121949,121931,121941,121942,121890,121862,121827,121804,121713,121598,121531,121459,121395,121326,121299,121323,121330,121285,121242,121247,121235,121251,121243,121281,121349,121429,121421,121443,121484,121495,121530,121514,121541,121618,121610,121583,121609,121628,121625,121637,121652,121699,121722,121714,121703,121728,121743,121743,121723,121752,121852,121876,121887,121894,121926,121975,121981,121981,122000,122045,122021,121942,121814,121733,121640,121544,121464,121453,121480,121438,121390,121365,121343,121302,121280,121283,121347,121404,121412,121418,121442,121469,121482,121473,121511,121593,121650,121656,121628,121632,121643,121640,121626,121649,121717,121752,121764,121753,121775,121799,121794,121802,121834,121907,121937,121945,121952,121969,121991,122012,122007,122063,122143,122150,122121,122049,122008,121923,121840,121742,121707,121701,121662,121600,121516,121496,121462,121420,121378,121405,121472,121504,121473,121470,121487,121502,121503,121527,121606,121656,121650,121638,121627,121638,121628,121590,121623,121676,121727,121706,121683,121690,121710,121714,121708,121749,121812,121857,121845,121833,121866,121879,121897,121903,121938,122014,122061,122062,122065,122073,122071,122016,121985,121948,121919,121830,121747,121702,121652,121621,121549,121531,121558,121563,121552,121504,121482,121500,121498,121477,121528,121588,121673,121659,121672,121690,121705,121693,121700,121754,121823,121851,121840,121832,121854,121883,121891,121919,121954,122046,122115,122095,122094,122126,122128,122133,122153,122214,122269,122274,122248,122268,122291,122286,122282,122346,122418,122452,122423,122356,122288,122224,122138,122053,122071,122080,122062,121964,121950,121944,121911,121894,121875,121918,121980,121968,121951,121981,121988,121993,121973,122006,122073,122096,122114,122067,122081,122079,122048,122019,122046,122094,122154,122137,122121,122133,122152,122134,122121,122179,122233,122253,122283,122269,122276,122295,122293,122298,122361,122427,122477,122473,122455,122467,122460,122418,122380,122365,122378,122312,122209,122172,122132,122115,122046,122043,122057,122097,122077,122057,122060,122050,122039,122041,122055,122099,122145,122143,122134,122149,122162,122146,122135,122174,122236,122251,122242,122245,122257,122253,122240,122245,122279,122368,122378,122355,122371,122404,122396,122382,122437,122502,122548,122562,122552,122572,122581,122584,122571,122614,122679,122708,122682,122628,122567,122519,122425,122379,122351,122377,122360,122319,122303,122309,122282,122238,122199,122236,122298,122284,122250,122280,122288,122291,122292,122322,122378,122421,122438,122414,122429,122441,122422,122389,122407,122477,122518,122508,122517,122536,122545,122547,122530,122560,122651,122691,122695,122693,122714,122736,122744,122742,122768,122842,122872,122876,122852,122879,122893,122882,122874,122930,122980,122933,122854,122805,122736,122684,122616,122613,122645,122654,122620,122575,122575,122562,122540,122513,122541,122608,122640,122628};
//float senalRojo[512]={150720,150803,150858,150907,150953,151040,151132,151215,151253,151277,151315,151337,151366,151353,151392,151496,151544,151541,151560,151586,151625,151663,151663,151752,151839,151904,151921,151970,152012,152060,152070,152137,152241,152307,152372,152414,152453,152460,152411,152305,152205,152074,151893,151645,151402,151198,151062,150875,150729,150679,150703,150648,150617,150650,150665,150709,150714,150799,150868,151003,151070,151109,151168,151217,151231,151248,151288,151356,151400,151415,151398,151417,151409,151417,151393,151469,151553,151625,151638,151667,151699,151749,151773,151792,151865,151974,152023,152062,152112,152184,152223,152226,152274,152321,152298,152160,151966,151757,151520,151267,151042,150906,150796,150712,150602,150499,150450,150395,150350,150315,150362,150460,150517,150536,150581,150651,150703,150762,150835,150941,150999,151015,151028,151044,151040,151046,151011,151063,151153,151193,151213,151204,151214,151248,151270,151276,151366,151445,151540,151561,151570,151624,151688,151725,151768,151869,151977,151992,151953,151868,151766,151566,151305,151081,150929,150756,150591,150413,150333,150260,150159,150054,150031,150057,150083,150083,150068,150135,150194,150236,150241,150343,150473,150547,150571,150615,150650,150679,150674,150665,150713,150798,150838,150849,150867,150887,150901,150925,150936,151039,151162,151192,151219,151278,151324,151363,151383,151425,151526,151648,151702,151740,151750,151746,151696,151561,151415,151278,151088,150858,150690,150526,150407,150268,150135,150071,150082,150050,150022,150007,150000,150037,150051,150059,150129,150269,150365,150391,150434,150471,150516,150520,150491,150563,150630,150688,150668,150689,150697,150739,150741,150751,150830,150930,151008,151020,151044,151100,151128,151176,151210,151328,151466,151495,151528,151576,151627,151675,151676,151715,151746,151688,151541,151351,151154,150969,150790,150615,150573,150554,150513,150429,150364,150341,150322,150293,150278,150333,150433,150460,150488,150562,150648,150701,150719,150760,150836,150944,150973,150968,151004,151027,151041,151065,151116,151225,151276,151315,151338,151395,151439,151487,151482,151565,151660,151738,151787,151794,151847,151889,151907,151933,152008,152124,152208,152233,152215,152204,152120,151971,151815,151687,151577,151417,151283,151182,151104,151039,150957,150969,151004,151044,151025,151005,151038,151078,151079,151102,151165,151289,151362,151390,151435,151442,151475,151476,151442,151474,151568,151587,151599,151591,151626,151673,151685,151689,151776,151868,151921,151946,151975,152017,152056,152084,152101,152213,152329,152363,152404,152449,152506,152545,152528,152595,152610,152592,152478,152310,152155,151993,151846,151703,151651,151641,151617,151590,151542,151518,151520,151491,151482,151517,151615,151654,151693,151699,151764,151793,151808,151827,151905,151987,152031,152036,152031,152063,152075,152090,152131,152194,152261,152290,152306,152363,152388,152414,152416,152515,152611,152683,152705,152747,152816,152864,152883,152918,152986,153107,153162,153191,153185,153184,153141,153011,152850,152756,152661,152525,152424,152342,152278,152241,152165,152163,152200,152194,152194,152162,152198,152210,152208,152225,152300,152407,152463,152500,152544,152565,152598,152601,152606,152650,152734,152754,152767,152768,152799,152836,152845,152872,152955,153052,153082,153096,153144,153192,153251,153274,153278,153405,153501,153522,153551,153586,153641,153664,153665,153733,153838,153908,153925,153891,153824,153724,153582,153387,153256,153211,153152,153035,152960,152918,152869,152840,152814,152821,152879,152881,152888,152903,152921,152942};

float senalAnalizarIR[512];
float senalAnalizarRojo[512];
  
float hamming[512]={0,0,0,0,0,0,0.08, 0.08003647, 0.08014586, 0.08032815, 0.08058333, 0.08091135, 0.08131215, 0.08178567, 0.08233184, 0.08295058, 0.08364177, 0.08440532, 0.0852411 , 0.08614898, 0.08712882, 0.08818046, 0.08930373, 0.09049846, 0.09176445, 0.09310151, 0.09450942, 0.09598796, 0.0975369 , 0.09915599, 0.10084497, 0.10260358, 0.10443154, 0.10632855, 0.10829432, 0.11032854, 0.11243087, 0.114601  , 0.11683857, 0.11914323, 0.12151461, 0.12395235, 0.12645605, 0.12902531, 0.13165973, 0.13435889, 0.13712236, 0.13994971, 0.14284048, 0.14579422, 0.14881046, 0.15188872, 0.15502852, 0.15822935, 0.16149071, 0.16481208, 0.16819293, 0.17163273, 0.17513093, 0.17868699, 0.18230032, 0.18597037, 0.18969655, 0.19347827, 0.19731492, 0.20120591, 0.20515061, 0.2091484 , 0.21319865, 0.2173007 , 0.22145392, 0.22565765, 0.22991121, 0.23421393, 0.23856514, 0.24296414, 0.24741023, 0.2519027 , 0.25644086, 0.26102397, 0.26565131, 0.27032215, 0.27503575, 0.27979135, 0.28458821, 0.28942556, 0.29430265, 0.29921868, 0.30417289, 0.30916449, 0.31419268, 0.31925668, 0.32435568, 0.32948886, 0.33465542, 0.33985454, 0.34508538, 0.35034713, 0.35563895, 0.36096   , 0.36630944, 0.37168641, 0.37709007, 0.38251955, 0.38797401, 0.39345257, 0.39895436, 0.40447851, 0.41002415, 0.4155904 , 0.42117637, 0.42678118, 0.43240394, 0.43804376, 0.44369975, 0.449371  , 0.45505662, 0.46075571, 0.46646736, 0.47219067, 0.47792473, 0.48366863, 0.48942147, 0.49518232, 0.50095028, 0.50672443, 0.51250386, 0.51828764, 0.52407487, 0.52986462, 0.53565598, 0.54144803, 0.54723984, 0.55303051, 0.55881912, 0.56460474, 0.57038646, 0.57616336, 0.58193453, 0.58769905, 0.59345601, 0.59920449, 0.60494359, 0.61067239, 0.61638998, 0.62209546, 0.62778793, 0.63346648, 0.63913021, 0.64477823, 0.65040963, 0.65602352, 0.66161903, 0.66719525, 0.6727513 , 0.67828631, 0.68379939, 0.68928967, 0.69475629, 0.70019836, 0.70561504, 0.71100547, 0.71636878, 0.72170412, 0.72701066, 0.73228755, 0.73753396, 0.74274904, 0.74793198, 0.75308196, 0.75819815, 0.76327974, 0.76832594, 0.77333594, 0.77830894, 0.78324416, 0.78814081, 0.79299813, 0.79781533, 0.80259165, 0.80732635, 0.81201866, 0.81666784, 0.82127316, 0.82583388, 0.83034929, 0.83481866, 0.83924129, 0.84361648, 0.84794353, 0.85222176, 0.85645049, 0.86062904, 0.86475676, 0.86883299, 0.87285709, 0.87682842, 0.88074634, 0.88461024, 0.8884195 , 0.89217352, 0.89587171, 0.89951347, 0.90309824, 0.90662543, 0.9100945 , 0.9135049 , 0.91685608, 0.92014751, 0.92337866, 0.92654904, 0.92965813, 0.93270544, 0.93569049, 0.9386128 , 0.94147192, 0.94426739, 0.94699876, 0.9496656 , 0.95226749, 0.95480402, 0.95727479, 0.9596794 , 0.96201747, 0.96428863, 0.96649252, 0.96862879, 0.97069711, 0.97269714, 0.97462857, 0.97649109, 0.97828441, 0.98000824, 0.98166231, 0.98324635, 0.98476012, 0.98620338, 0.98757589, 0.98887744, 0.99010783, 0.99126685, 0.99235433, 0.99337008, 0.99431396, 0.99518581, 0.99598549, 0.99671288, 0.99736786, 0.99795032, 0.99846018, 0.99889735, 0.99926176, 0.99955337, 0.99977211, 0.99991795, 0.99999088, 0.99999088, 0.99991795, 0.99977211, 0.99955337, 0.99926176, 0.99889735, 0.99846018, 0.99795032, 0.99736786, 0.99671288, 0.99598549, 0.99518581, 0.99431396, 0.99337008, 0.99235433, 0.99126685, 0.99010783, 0.98887744, 0.98757589, 0.98620338, 0.98476012, 0.98324635, 0.98166231, 0.98000824, 0.97828441, 0.97649109, 0.97462857, 0.97269714, 0.97069711, 0.96862879, 0.96649252, 0.96428863, 0.96201747, 0.9596794 , 0.95727479, 0.95480402, 0.95226749, 0.9496656 , 0.94699876, 0.94426739, 0.94147192, 0.9386128 , 0.93569049, 0.93270544, 0.92965813, 0.92654904, 0.92337866, 0.92014751, 0.91685608, 0.9135049 , 0.9100945 , 0.90662543, 0.90309824, 0.89951347, 0.89587171, 0.89217352, 0.8884195 , 0.88461024, 0.88074634, 0.87682842, 0.87285709, 0.86883299, 0.86475676, 0.86062904, 0.85645049, 0.85222176, 0.84794353, 0.84361648, 0.83924129, 0.83481866, 0.83034929, 0.82583388, 0.82127316, 0.81666784, 0.81201866, 0.80732635, 0.80259165, 0.79781533, 0.79299813, 0.78814081, 0.78324416, 0.77830894, 0.77333594, 0.76832594, 0.76327974, 0.75819815, 0.75308196, 0.74793198, 0.74274904, 0.73753396, 0.73228755, 0.72701066, 0.72170412, 0.71636878, 0.71100547, 0.70561504, 0.70019836, 0.69475629, 0.68928967, 0.68379939, 0.67828631, 0.6727513 , 0.66719525, 0.66161903, 0.65602352, 0.65040963, 0.64477823, 0.63913021, 0.63346648, 0.62778793, 0.62209546, 0.61638998, 0.61067239, 0.60494359, 0.59920449, 0.59345601, 0.58769905, 0.58193453, 0.57616336, 0.57038646, 0.56460474, 0.55881912, 0.55303051, 0.54723984, 0.54144803, 0.53565598, 0.52986462, 0.52407487, 0.51828764, 0.51250386, 0.50672443, 0.50095028, 0.49518232, 0.48942147, 0.48366863, 0.47792473, 0.47219067, 0.46646736, 0.46075571, 0.45505662, 0.449371  , 0.44369975, 0.43804376, 0.43240394, 0.42678118, 0.42117637, 0.4155904 , 0.41002415, 0.40447851, 0.39895436, 0.39345257, 0.38797401, 0.38251955, 0.37709007, 0.37168641, 0.36630944, 0.36096   , 0.35563895, 0.35034713, 0.34508538, 0.33985454, 0.33465542, 0.32948886, 0.32435568, 0.31925668, 0.31419268, 0.30916449, 0.30417289, 0.29921868, 0.29430265, 0.28942556, 0.28458821, 0.27979135, 0.27503575, 0.27032215, 0.26565131, 0.26102397, 0.25644086, 0.2519027 , 0.24741023, 0.24296414, 0.23856514, 0.23421393, 0.22991121, 0.22565765, 0.22145392, 0.2173007 , 0.21319865, 0.2091484 , 0.20515061, 0.20120591, 0.19731492, 0.19347827, 0.18969655, 0.18597037, 0.18230032, 0.17868699, 0.17513093, 0.17163273, 0.16819293, 0.16481208, 0.16149071, 0.15822935, 0.15502852, 0.15188872, 0.14881046, 0.14579422, 0.14284048, 0.13994971, 0.13712236, 0.13435889, 0.13165973, 0.12902531, 0.12645605, 0.12395235, 0.12151461, 0.11914323, 0.11683857, 0.114601  , 0.11243087, 0.11032854, 0.10829432, 0.10632855, 0.10443154, 0.10260358, 0.10084497, 0.09915599, 0.0975369 , 0.09598796, 0.09450942, 0.09310151, 0.09176445, 0.09049846, 0.08930373, 0.08818046, 0.08712882, 0.08614898, 0.0852411 , 0.08440532, 0.08364177, 0.08295058, 0.08233184, 0.08178567, 0.08131215, 0.08091135, 0.08058333, 0.08032815, 0.08014586, 0.08003647, 0.08, 0,0,0,0,0,0};

// VARIABLES INTERRUPCIÓN POR TIEMPO (LECTURA A 100 Hz)
//hw_timer_t * timer = NULL;
int led = 2;
//volatile byte state = LOW;
//int tiempo;


// VARIABLES DE FILTROS Y  PSD
float resultadoPasaBajos[512];            // DE FILTROS
float resultadoPasaAltos[512];
float resultadoPasaAltos2[512];
float resultadoPasaBajos2[512];
float  resultadoPasaBajos3[512];
float imprimirPasaBajos[512];
float imprimirPasaAltos[512];
float imprimir[256];
const uint16_t samples = 512;             // DE PSD 
double senal5seg[samples];
const double samplingFrequency = 100;
const uint8_t amplitude = 100;
double senalPSD[samples/2];
double vImag2[samples];
const uint16_t ventana= 128;              // DE MUESTRAS PARA PSD
double vImag[ventana];
double muestra[ventana];
double psd[ventana/2];
double psdPromedio[ventana/2];

float senal[512];
float senalEliminandoPicos[512];          // DE ELIMINACIÓN DE PICOS

int punteroValorSensado;

float calculoFrecuenciaCardiaca(float entrada[512]);
void  obtenerValorAcDc(float miSenal[512], float *direccion_valorDC, float *direccion_frecuenciaCardiaca, float *direccion_valorAC, float *direccion_frecuenciaCardiacaPSD);
void eliminarPicos(float senalPicos[512]);

float resultadoFrecuenciaCardiaca;
float resultadoSpo2;
//int tiempoInterrupcionTranscurrido;

int numeroPicosEncontrados=0;

void IRAM_ATTR onTimer(){
    /*
    tiempoInterrupcionTranscurrido=millis()-tiempo;
    state = !state;
    digitalWrite(led, state);
    tiempo=millis();
    int i;
    if (punteroValorSensado<512){
        for (i=0;i<511;i++){senalAnalizarIR[i]=senalAnalizarIR[i+1];}
        senalAnalizarIR[511]=senalIR[punteroValorSensado];
        //senalAnalizarIR[511]=senalIR[particleSensor.getIR()];
        for (i=0;i<511;i++){senalAnalizarRojo[i]=senalAnalizarRojo[i+1];}
        senalAnalizarRojo[511]=senalRojo[punteroValorSensado];
        //senalAnalizarRojo[511]=senalRojo[particleSensor.getRed()];
        punteroValorSensado=punteroValorSensado+1;
    }
    */
}

byte ledBrightnessIR = 0xA0;


void setup() {
    
    
    Serial.begin(115200);
    pinMode(led, OUTPUT);
    
    // CONFIGURACIÓN E INICIALIZACIÓN PANTALLA OLED
    u8g2.setBusClock(100000);
    u8g2.begin();
    u8g2.setBusClock(100000);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
    u8g2.drawStr(0,10,"Fc= ");	// write something to the internal memory
    u8g2.setCursor(20,10);
    u8g2.print(u8x8_u8toa(89, 3));
    u8g2.drawStr(40,10,"bpm");
    u8g2.drawStr(70,10,"SpO2= ");  // write something to the internal memory
    u8g2.setCursor(105,10);
    u8g2.print(u8x8_u8toa(94, 2));
    u8g2.drawStr(120,10,"%");
    u8g2.sendBuffer();
    

    // CONFIGURACIÓN DE SENSOR MAX30102
    if (particleSensor.begin(Wire, I2C_SPEED_STANDARD) == false){
    //if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false){
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }


    byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
    byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411; //Options: 69, 118, 215, 411
    int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
    
    particleSensor.setPulseAmplitudeIR(ledBrightnessIR);

    // CONFIGURACIÓN INTERRUPCIÓN 100 hZ
    /* Use 1st timer of 4 */ /* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
    //timer = timerBegin(0, 80, true);
    /* Attach onTimer function to our timer */
    //timerAttachInterrupt(timer, &onTimer, true);
    /* Set alarm to call onTimer function every second 1 tick is 1us
    => 1 second is 1000000us */
    /* Repeat the alarm (third parameter) */
    //timerAlarmWrite(timer, 10000, true);
    /* Start an alarm */
    //timerAlarmEnable(timer);
    //tiempo=millis();
  


}

void loop() {

    // variables Rojo
    float valorDCRojo;
    float frecuenciaCardiacaRojo;
    float valorACRojo;
    float frecuenciaCardiacaPSDRojo;
    
    // variables IR
    float valorDCIR;
    float frecuenciaCardiacaIR;
    float valorACIR;
    float frecuenciaCardiacaPSDIR;

    // resultados
    float R;
    float spo2;
    int i;

    // simulador de artefactos tipo picos
    //senalIR[100]=0;     
    //senalRojo[100]=0;    
    //senalIR[300]=0;
    //senalRojo[300]=0;

    // inicialización lectura de senal rojo e Infrarojo 
    float valorInicialIR=0;
    float valorInicialRojo=0;
    delay(100);
    particleSensor.check();
    while (particleSensor.available()){
        //for (i=0;i<512;i++){senalAnalizarIR[i]=senalIR[0];}
        valorInicialIR=float(particleSensor.getFIFOIR());
        for (i=0;i<512;i++){senalAnalizarIR[i]=valorInicialIR;}
        Serial.println(valorInicialIR);
        //for (i=0;i<512;i++){senalAnalizarRojo[i]=senalRojo[0];}
        valorInicialRojo=float(particleSensor.getFIFORed());
        for (i=0;i<512;i++){senalAnalizarRojo[i]=valorInicialRojo;}
        particleSensor.nextSample();
    }

    int tiempoTranscurrido;
    
    punteroValorSensado=0;
    tiempoTranscurrido=millis();
    int muestrasRepresadas=0;

    while(1){
        
        // LECTURA MAX30102
        particleSensor.check();
        while (particleSensor.available()) //do we have new data?
        {
            punteroValorSensado=punteroValorSensado+1;
            for (i=0;i<511;i++){senalAnalizarIR[i]=senalAnalizarIR[i+1];}
            //senalAnalizarIR[511]=senalIR[punteroValorSensado];
            senalAnalizarIR[511]=float(particleSensor.getFIFOIR());
            for (i=0;i<511;i++){senalAnalizarRojo[i]=senalAnalizarRojo[i+1];}
            //senalAnalizarRojo[511]=senalRojo[punteroValorSensado];
            senalAnalizarRojo[511]=float(particleSensor.getFIFORed());
            particleSensor.nextSample();
            muestrasRepresadas++;    
        }
        //Serial.print("el número de muestras represadas fué: ");
        //Serial.println(muestrasRepresadas);
        muestrasRepresadas=0;
        
        /*
        Serial.println("la señal IR a analizar es: ");
        for (i=0;i<512;i++){
            Serial.print(i);
            Serial.print(" ");
            Serial.println(senalAnalizarIR[i]);
        }
        Serial.println("la señal Roja a analizar fue:");
        for (i=0;i<512;i++){
            Serial.print(i);
            Serial.print(" ");
            Serial.println(senalAnalizarRojo[i]);
        }
        */ 
        //Serial.print("el tiempo transcurrido es: ");
        //Serial.println((millis()- tiempoTranscurrido)/1000);
        //Serial.println("");
        //tiempoTranscurrido=millis();
        
        // ANÁLISIS SEÑAL IR
        obtenerValorAcDc(senalAnalizarIR, &valorDCIR, &frecuenciaCardiacaIR, &valorACIR, &frecuenciaCardiacaPSDIR);
        Serial.print("MI VALOR DC IR ES: ");
        Serial.println(valorDCIR);
        Serial.print("MI FRECUENCIA CARDIACA IR ES: ");
        Serial.println(frecuenciaCardiacaIR); 
        Serial.print("MI VALOR AC IR ES: ");
        Serial.println(valorACIR);
        Serial.print("MI FRECUENCIA CARDIACA PSD IR ES: ");
        Serial.println(frecuenciaCardiacaPSDIR);
        Serial.println("");
        
        //Serial.print("FcIR: ");
        //Serial.print(frecuenciaCardiacaIR); 
        //Serial.print(" FcIRPSD: ");
        //Serial.println(frecuenciaCardiacaPSDIR); 

        // FINALIZACIÓN DE 51 SEGUNDOS DE FUNCIONAMIENTO 
        if (punteroValorSensado>5110){
            Serial.println("IR: señal procesada:");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senalAnalizarIR[i]);
            } 
            Serial.println("IR: señal sin nivel DC es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senal[i]);
            }
            Serial.println("IR: señal con filtro pasa altos es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos[i]);
            }
            Serial.println("IR: señal con filtro pasa altos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos2[i]);
            }
            Serial.println("IR: señal con filtro pasa bajos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos2[i]);
            }
            Serial.println("IR: señal con filtro pasa bajos 3 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos3[i]);
            }
        }

        // ANÁLISIS SEÑAL ROJO
        obtenerValorAcDc(senalAnalizarRojo, &valorDCRojo, &frecuenciaCardiacaRojo, &valorACRojo, &frecuenciaCardiacaPSDRojo);
        Serial.print("MI VALOR DC ROJO ES: ");
        Serial.println(valorDCRojo);
        Serial.print("MI FRECUENCIA CARDIACA ROJO ES: ");
        Serial.println(frecuenciaCardiacaRojo); 
        Serial.print("MI VALOR AC ROJO ES: ");
        Serial.println(valorACRojo);
        Serial.print("MI FRECUENCIA CARDIACA PSD ROJO ES: ");
        Serial.println(frecuenciaCardiacaPSDRojo);
        Serial.println("");


            

        //Serial.print("FcR_: ");
        //Serial.print(frecuenciaCardiacaRojo); 
        //Serial.print(" FcR_PSD: ");
        //Serial.println(frecuenciaCardiacaPSDRojo);
        
        R=(valorACRojo/valorDCRojo)/(valorACIR/valorDCIR);
        R=R*2.5;
        spo2 =110-25*R;
        
        // VALIDACIÓN RESULTADOS DE SpO2 y FC
        //if ((abs(frecuenciaCardiacaIR-frecuenciaCardiacaRojo)>6) || (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaIR)>25) || (abs(frecuenciaCardiacaPSDRojo-frecuenciaCardiacaRojo)>25) ||  (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)>25)  || (frecuenciaCardiacaIR>240) || (frecuenciaCardiacaRojo>240) or (frecuenciaCardiacaIR<30) || (frecuenciaCardiacaRojo<30) || (spo2<50) || (spo2>99) || (valorACRojo<500) || (valorACIR<500) ){
        //if ( (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaIR)>25) || (abs(frecuenciaCardiacaPSDRojo-frecuenciaCardiacaRojo)>25) ||  (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)>25)  || (frecuenciaCardiacaIR>240) || (frecuenciaCardiacaRojo>240) or (frecuenciaCardiacaIR<30) || (frecuenciaCardiacaRojo<30) || (spo2<50) || (spo2>99) ){
        //if ( (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaIR)>25) || (frecuenciaCardiacaIR>240) || (frecuenciaCardiacaIR<30) || (spo2<50) || (spo2>100) ){
        if  (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)>25){ 
            // datos incorrectos
            resultadoSpo2=0;
            resultadoFrecuenciaCardiaca=0;
        }
        else{
            // datos correctos
            resultadoSpo2=spo2;
            resultadoFrecuenciaCardiaca=frecuenciaCardiacaIR;    
        }
        
        Serial.println(resultadoSpo2);

        // ACTUALIZACIÓN DE INTENSIDAD DE LEDS
        int estabilidad;
        //if ((punteroValorSensado % 64==0) && (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)<25)){
        if (abs(frecuenciaCardiacaPSDIR-frecuenciaCardiacaPSDRojo)<20){
            estabilidad++;
            if (estabilidad>10){
                if ((100*(valorDCIR-valorDCRojo)/valorDCIR)>1 && ledBrightnessIR>0x01){
                    ledBrightnessIR=ledBrightnessIR-0x01;
                    particleSensor.setPulseAmplitudeIR(ledBrightnessIR);
                }
                if ((100*(valorDCRojo-valorDCIR)/valorDCIR)>1 && ledBrightnessIR<0xFF){
                    ledBrightnessIR=ledBrightnessIR+0x01;
                    particleSensor.setPulseAmplitudeIR(ledBrightnessIR);
                }
                estabilidad=0;
            }
        }
        else{
            estabilidad=0;
        }

        
        

        // VISUALIZACIÓN EN OLED
        if (punteroValorSensado % 2==0){
            u8g2.clearBuffer();
            int OLEDvalorActual;
            int OLEDvalorAnterior=37;
            for (i=128;i<256;i++){
                OLEDvalorActual=int(map(imprimir[i], -1200, 1000, 0, 40));
                OLEDvalorActual=55-OLEDvalorActual;
                u8g2.drawLine(i-128,OLEDvalorAnterior,i+1-128,OLEDvalorActual);
                OLEDvalorAnterior=OLEDvalorActual;
            }
            u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
            u8g2.drawStr(0,10,"Fc= ");	// write something to the internal memory
            u8g2.setCursor(20,10);
            if (resultadoFrecuenciaCardiaca>30){
                u8g2.print(u8x8_u8toa(resultadoFrecuenciaCardiaca, 3));    
            }
            else{
                u8g2.print(" --");   
            }
            u8g2.drawStr(40,10,"bpm");
            u8g2.drawStr(70,10,"SpO2= ");  // write something to the internal memory
            
            u8g2.setCursor(105,10);
            if (resultadoSpo2>60){
                u8g2.print(u8x8_u8toa(resultadoSpo2, 2));
            }
            else {
                u8g2.print("---");
            }
            u8g2.drawStr(120,10,"%");
            
            
            u8g2.setCursor(0,60);
            u8g2.print(u8x8_u8toa(numeroPicosEncontrados, 2));
            u8g2.sendBuffer();
            
            u8g2.setCursor(80,60);
            u8g2.print(u8x8_u8toa((valorDCIR-valorDCRojo)*1000/valorDCIR, 3));
            u8g2.setCursor(110,60);
            u8g2.print(u8x8_u8toa(int(ledBrightnessIR),3));
            u8g2.sendBuffer();       
        }
        
        // FINALIZACIÓN DE 51 SEGUNDOS DE FUNCIONAMIENTO 
        if (punteroValorSensado>5110){
            Serial.println("ROJO: señal procesada:");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senalAnalizarRojo[i]);
            } 
            Serial.println("ROJO: señal sin nivel DC:");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(senal[i]);
            }
            Serial.println("ROJO: señal con filtro pasa altos es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos[i]);
            }
            Serial.println("ROJO: señal con filtro pasa altos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaAltos2[i]);
            }
            Serial.println("ROJO: señal con filtro pasa bajos 2 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos2[i]);
            }
            Serial.println("ROJO: señal con filtro pasa bajos 3 es :");
            for (i=0;i<512;i++){
                //Serial.print(i);
                //Serial.print(" ");
                Serial.println(resultadoPasaBajos3[i]);
            }
            while(1){};
        }

    }
}



float calculoFrecuenciaCardiaca(float entrada[500]){
    int cruces[512];
    int ubicacionCruces[512];
    int i;
    for (i=0;i<512;i++){
        cruces[i]=0;
    }
    for (i=0;i<512;i++){
        ubicacionCruces[i]=0;
    }
    int j=0;
    for (i=0;i<512;i++){
        if (resultadoPasaBajos2[i]<0 && resultadoPasaBajos2[i+1]>0){
            cruces[i]=100;
            ubicacionCruces[j]=i;
            j=j+1;
        }
    }
    for (i=0;i<512;i++){
        //printf( "%d\n", ubicacionCruces[i]);
    }
    float frecuencias[100];
    for (i=0;i<100;i++){
        frecuencias[i]=0;
    }
    i=0;
    while ((ubicacionCruces[i]>0) && (ubicacionCruces[i+1]>0)){
        frecuencias[i]=(float)100/(ubicacionCruces[i+1]-ubicacionCruces[i]);
        i=i+1;    
    }
    for (i=0;i<100;i++){
        //printf( "%f\n", frecuencias[i]);
    }
    float frecuenciaPromedio;
    i=0;
    frecuenciaPromedio=0;
    while (frecuencias[i]>0){
        frecuenciaPromedio=frecuenciaPromedio+frecuencias[i];
        i=i+1;
    }
    frecuenciaPromedio=frecuenciaPromedio/i;
    frecuenciaPromedio=60*frecuenciaPromedio;
    
    return frecuenciaPromedio;

}


void  obtenerValorAcDc(float miSenal[512], float *direccion_valorDC, float *direccion_frecuenciaCardiaca, float *direccion_valorAC, float *direccion_frecuenciaCardiacaPSD){

    
    float valorDC;
    int i;

    for ( i = 0; i < 512; i++ ) {
      senal[i]=miSenal[i];
      //Serial.print(i);
      //Serial.print(" ");
      //Serial.println(senal[i]);
    }
    
    ///---------------CALCULAR Y ELIMINAR EL VALOR DC---------------
    
    // Filtro pasa bajos a 1 Hz
    float b[4]= {2.91464945e-05, 8.74394834e-05, 8.74394834e-05, 2.91464945e-05};
    float a[4]= {1, -2.87435689,  2.7564832, -0.88189313};
    resultadoPasaBajos[0]=senal[0];
    resultadoPasaBajos[1]=senal[0];
    resultadoPasaBajos[2]=senal[0];
    senal[1]=senal[0];
    senal[2]=senal[0];
    senal[3]=senal[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaBajos[i] =senal[i]*b[0]+senal[i-1]*b[1]+senal[i-2]*b[2]+senal[i-3]*b[3]-(resultadoPasaBajos[i-1]*a[1]+resultadoPasaBajos[i-2]*a[2]+resultadoPasaBajos[i-3]*a[3]);
        //Serial.println(resultadoPasaBajos[i]);
    }

    valorDC=0;
    for ( i = 200; i < 512; i++ ) {
        valorDC=valorDC+resultadoPasaBajos[i];
    }
    valorDC=valorDC/312;

    for ( i = 0; i < 512; i++ ) {
        senal[i]=senal[i]-valorDC;
        //Serial.println( senal[i]);
    }


    ///-------------------------------------------------------------

    eliminarPicos(senal);

    ///---------------CALCULAR EL VALOR AC--------------------------    
    
    // Filtro pasa altos a 0.5 Hz
    b[0]=0.96907117; b[1]=-2.90721352;  b[2]=2.90721352; b[3]=-0.96907117;
    a[0]=1; a[1]=-2.93717073; a[2]=2.87629972; a[3]=-0.93909894;
    senal[0]=senal[0]/2;
    resultadoPasaAltos[0]=senal[0];
    resultadoPasaAltos[1]=senal[0];
    resultadoPasaAltos[2]=senal[0];
    senal[1]=senal[0];
    senal[2]=senal[0];
    senal[3]=senal[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaAltos[i] =senal[i]*b[0]+senal[i-1]*b[1]+senal[i-2]*b[2]+senal[i-3]*b[3]-(resultadoPasaAltos[i-1]*a[1]+resultadoPasaAltos[i-2]*a[2]+resultadoPasaAltos[i-3]*a[3]);
        //Serial.print( "%f\n", resultadoPasaAltos[i]);
    }


    

    // INICIO IMPRIMIR
    // Filtro pasa altos a 0.5 Hz
    b[0]=0.96907117; b[1]=-2.90721352;  b[2]=2.90721352; b[3]=-0.96907117;
    a[0]=1; a[1]=-2.93717073; a[2]=2.87629972; a[3]=-0.93909894;
    resultadoPasaAltos[0]=resultadoPasaAltos[0]/2;
    imprimirPasaAltos[0]=resultadoPasaAltos[0];
    imprimirPasaAltos[1]=resultadoPasaAltos[0];
    imprimirPasaAltos[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[1]=resultadoPasaAltos[0];
    resultadoPasaAltos[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[3]=resultadoPasaAltos[0];
    for ( i = 3; i < 512; i++ ) {
        imprimirPasaAltos[i] =resultadoPasaAltos[i]*b[0]+resultadoPasaAltos[i-1]*b[1]+resultadoPasaAltos[i-2]*b[2]+resultadoPasaAltos[i-3]*b[3]-(imprimirPasaAltos[i-1]*a[1]+imprimirPasaAltos[i-2]*a[2]+imprimirPasaAltos[i-3]*a[3]);
        //Serial.print( "%f\n", imprimirPasaAltos[i]);
    }
    // filtro pasa bajos a 5 Hz.
    b[0]=0.00289819; b[1]=0.00869458;  b[2]=0.00869458; b[3]=0.00289819;
    a[0]=1; a[1]=-2.37409474; a[2]=1.92935567; a[3]=-0.53207537;
    imprimirPasaAltos[0]=imprimirPasaAltos[0]/2;
    imprimirPasaBajos[0]=imprimirPasaAltos[0];
    imprimirPasaBajos[1]=imprimirPasaAltos[0];
    imprimirPasaBajos[2]=imprimirPasaAltos[0];
    imprimirPasaAltos[1]=imprimirPasaAltos[0];
    imprimirPasaAltos[2]=imprimirPasaAltos[0];
    imprimirPasaAltos[3]=imprimirPasaAltos[0];
    for ( i = 3; i < 512; i++ ) {
        imprimirPasaBajos[i] =imprimirPasaAltos[i]*b[0]+imprimirPasaAltos[i-1]*b[1]+imprimirPasaAltos[i-2]*b[2]+imprimirPasaAltos[i-3]*b[3]-(imprimirPasaBajos[i-1]*a[1]+imprimirPasaBajos[i-2]*a[2]+imprimirPasaBajos[i-3]*a[3]);
        //Serial.println(imprimirPasaBajos[i]);
    }
    int j=0;
    for ( i = 0; i < 512; i=i+2 ) {
        imprimir[j] = imprimirPasaBajos[i];
        j++;
    }



    // FIN IMPRIMIR
    


    // ventana hamming 
    for ( i = 0; i < 512; i++ ) {
        resultadoPasaAltos[i]=resultadoPasaAltos[i]*hamming[i];
        //Serial.println(resultadoPasaAltos[i]);
    }



    // Filtro pasa altos a 0.5 Hz
    b[0]=0.96907117; b[1]=-2.90721352;  b[2]=2.90721352; b[3]=-0.96907117;
    a[0]=1; a[1]=-2.93717073; a[2]=2.87629972; a[3]=-0.93909894;
    resultadoPasaAltos[0]=resultadoPasaAltos[0]/2;
    resultadoPasaAltos2[0]=resultadoPasaAltos[0];
    resultadoPasaAltos2[1]=resultadoPasaAltos[0];
    resultadoPasaAltos2[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[1]=resultadoPasaAltos[0];
    resultadoPasaAltos[2]=resultadoPasaAltos[0];
    resultadoPasaAltos[3]=resultadoPasaAltos[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaAltos2[i] =resultadoPasaAltos[i]*b[0]+resultadoPasaAltos[i-1]*b[1]+resultadoPasaAltos[i-2]*b[2]+resultadoPasaAltos[i-3]*b[3]-(resultadoPasaAltos2[i-1]*a[1]+resultadoPasaAltos2[i-2]*a[2]+resultadoPasaAltos2[i-3]*a[3]);
        //Serial.print( "%f\n", resultadoPasaAltos2[i]);
    }

    // filtro pasa bajos a 5 Hz.
    b[0]=0.00289819; b[1]=0.00869458;  b[2]=0.00869458; b[3]=0.00289819;
    a[0]=1; a[1]=-2.37409474; a[2]=1.92935567; a[3]=-0.53207537;
    resultadoPasaAltos2[0]=resultadoPasaAltos2[0]/2;
    resultadoPasaBajos2[0]=resultadoPasaAltos2[0];
    resultadoPasaBajos2[1]=resultadoPasaAltos2[0];
    resultadoPasaBajos2[2]=resultadoPasaAltos2[0];
    resultadoPasaAltos2[1]=resultadoPasaAltos2[0];
    resultadoPasaAltos2[2]=resultadoPasaAltos2[0];
    resultadoPasaAltos2[3]=resultadoPasaAltos2[0];
    
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaBajos2[i] =resultadoPasaAltos2[i]*b[0]+resultadoPasaAltos2[i-1]*b[1]+resultadoPasaAltos2[i-2]*b[2]+resultadoPasaAltos2[i-3]*b[3]-(resultadoPasaBajos2[i-1]*a[1]+resultadoPasaBajos2[i-2]*a[2]+resultadoPasaBajos2[i-3]*a[3]);
    }


    // mi ultimo filtro
    // filtro pasa bajos a 3 Hz.

    // # 3HZ
    b[0]=0.00069935; b[1]=0.00209805;  b[2]=0.00209805; b[3]=0.00069935;
    a[0]=1; a[1]=-2.62355181; a[2]=2.31468258; a[3]=-0.68553598;
    //2 Hz
    //b[0]=0.00021961; b[1]=0.00065882;  b[2]=0.00065882; b[3]=0.00021961;
    //a[0]=1; a[1]=-2.74883581; a[2]=2.52823122; a[3]=-0.77763856;
    resultadoPasaBajos2[0]=resultadoPasaBajos2[0]/2;
    resultadoPasaBajos3[0]=resultadoPasaBajos2[0];
    resultadoPasaBajos3[1]=resultadoPasaBajos2[0];
    resultadoPasaBajos3[2]=resultadoPasaBajos2[0];
    resultadoPasaBajos2[1]=resultadoPasaBajos2[0];
    resultadoPasaBajos2[2]=resultadoPasaBajos2[0];
    resultadoPasaBajos2[3]=resultadoPasaBajos2[0];
    for ( i = 3; i < 512; i++ ) {
        resultadoPasaBajos3[i] =resultadoPasaBajos2[i]*b[0]+resultadoPasaBajos2[i-1]*b[1]+resultadoPasaBajos2[i-2]*b[2]+resultadoPasaBajos2[i-3]*b[3]-(resultadoPasaBajos3[i-1]*a[1]+resultadoPasaBajos3[i-2]*a[2]+resultadoPasaBajos3[i-3]*a[3]);
    }





    // fin de mi último filtro 











    //for ( i = -1; i < 512; i++ ) {
        //Serial.println(resultadoPasaBajos2[i]);
    //}

    float frecuenciaCardiaca=calculoFrecuenciaCardiaca(resultadoPasaBajos3);
    //Serial.print("El cálculo de la frecuencia cardíaca es de: ");
    //Serial.println(frecuenciaCardiaca);



     /* Print the results of the simulated sampling according to time */
    for (i=0;i<samples;i++){
      senal5seg[i]=resultadoPasaBajos3[i];
    }
  
    // submuestreo 1/2
    j=0;
    for (i=0;i<samples;i=i+2){
      senalPSD[j]=senal5seg[i];
      j++;
    }
    
    // Hamming
    // FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  
    
    int overlap= int(ventana*0.7);
    int saltoNuevaVentana= ventana-overlap;
    int numeroVentanaActual=0;
    int puntero;
    for (i=0;i<ventana/2;i++) {psdPromedio[i]=0;}
    for (puntero=0;puntero<((samples/2)-ventana);puntero=(puntero+saltoNuevaVentana)){
      numeroVentanaActual=numeroVentanaActual+1;
  
      // Extraer muestra
      for (i=0;i<=ventana;i++){
        muestra[i]=senalPSD[puntero+i];
      }  
  
      // Hamming
      FFT.Windowing(muestra, ventana, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  
      // Fourier
      for (i=0;i<ventana;i++){vImag[i]=0;}
      FFT.Compute(muestra, vImag, ventana, FFT_FORWARD); /* Compute FFT */
      FFT.ComplexToMagnitude(muestra, vImag, ventana); /* Compute magnitudes */
      //Serial.println("psd muestra");
      for (i=0;i<ventana/2;i++){
        psd[i]=2*muestra[i]*muestra[i]/(ventana*50.48);
        psdPromedio[i]=psdPromedio[i]+psd[i];
        //Serial.println(psd[i]);
      }
      
      
  
    }
    //Serial.println("PSD promedio");
    for (i=0;i<ventana/2;i++){
      psdPromedio[i]=psdPromedio[i]/numeroVentanaActual;
      //Serial.println(psdPromedio[i]);
    }
  
    //  Encontrar el PSD[j] máximo
    double maxPSD_v=0;
    double maxPSD_i=0; 
    for ( int i = 0; i < ventana/2; i++ )
    {
      if ( psdPromedio[i] > maxPSD_v )
      {
         maxPSD_v = psdPromedio[i];
         maxPSD_i = i;
      }
    }
  
    double valorAC=maxPSD_v;
    double frecuenciaMaxima= maxPSD_i*(((samplingFrequency/2)/2)/(ventana/2));
    //Serial.print("El valor AC es: ");
    //Serial.println(valorAC);
    
    //Serial.print("la frecuencia cardíaca es: ");
    //Serial.println(frecuenciaMaxima*60);
      


    *direccion_valorDC=valorDC;
    *direccion_frecuenciaCardiaca=frecuenciaCardiaca;
    *direccion_valorAC=valorAC;
    *direccion_frecuenciaCardiacaPSD=frecuenciaMaxima*60;
    
    
}

void eliminarPicos(float senal[512]){
    
    //printf("Senal de entrada \n");
    int i;
    for (i=0;i<512;i++){
        senalEliminandoPicos[i]=senal[i];
        /*printf("%d ",i);
        printf("  ");
        printf("%.1f \n",senalEliminandoPicos[i]);*/
    }



    int numeroDatos=512;

    float p[512];
    for (i=0;i<512;i++){
        p[i]=0;
    }

    float atraz;
    float adelante;
    for (i=1;i<(numeroDatos-1);i++){
        atraz=fabs(senalEliminandoPicos[i]-senalEliminandoPicos[i-1]);
        adelante=fabs(senalEliminandoPicos[i+1]-senalEliminandoPicos[i]);
        if (atraz<adelante){
            p[i]=atraz;
        }
        else
        {
            p[i]=adelante;
        }
    }
    
    float ubicacionPicos[512];
    for (i=0;i<512;i++){
        ubicacionPicos[i]=0;
    }

    float threshold=500;
    for (i=0;i<512;i++){
        if (p[i]>threshold){
            ubicacionPicos[i]=1;
        }
        
    }
    /*
    for (i=0;i<512;i++){
        printf("%d ",i);
        printf("  ");
        printf("%.1f \n",ubicacionPicos[i]);
    }
    */
    numeroPicosEncontrados=0;
    for (i=0;i<512;i++){
        if (ubicacionPicos[i]==1){
            senalEliminandoPicos[i]=senalEliminandoPicos[i-1];
            numeroPicosEncontrados=numeroPicosEncontrados+1;
            //Serial.println("pico eliminado");
        }        
    }

    for (i=0;i<512;i++){
        //senal[i]=senalEliminandoPicos[i];  /// QUITAR COMENTARIO
        //printf("%d ",i);
        //printf("  ");
        //printf("%.1f \n",senalEliminandoPicos[i]);


    }

}