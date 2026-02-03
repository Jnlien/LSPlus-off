
#define Part_Mixing   9990
#define Extra_Mixing  9991
#define Final_Mixing  9992

#define DRYER_OBJECT 		1208
#define CENTRIFUGE_OBJECT 	19830
#define MIXER_OBJECT 		19585
#define REACTOR_OBJECT 		920  
                            //2360 - reactor
#define DEHYDRATER_OBJECT 	2002
#define PUMP_OBJECT 		1244

enum PLANTS_ENUM
{
	pID,
	pType,
	pPlanter[52],
	pGrowTime,
	pStartTime,
	Float:pX,
	Float:pY,
	Float:pZ,
	pInt,
	pVW,
	pObject,
	Text3D:pLabel,
	pPlanted,
	pPlantDate[64]
}
new CropsInfo[MAX_PLANTS][PLANTS_ENUM];

//#define     MAX_CHEMICAL        (10)
//#define     MAX_DRUGS           (30)
#define     MAX_CHEMISTRY       (150)
#define     MAX_CHEM_DROPS      (60)

enum DRUG_DATA
{
	UID,
    MAIN,
	NAME[32],
	TYPE, // 1 health, 2 armour 3 decreas incoming damage 4 invisible for a couple seconds or run fast (play anmation when they run)
	CODE[32],
	CONTROL, // 1 heating 2 cooling
    POINT, // Need x melting / cooling point to finish
    STATUS // 0 labile / 1 stable
};

enum CHECK_DRUG_DATE
{
	onedrug,
    twodrug
};


enum P_CHEMICALDATA
{
    pSlot,
	bool:Selected
};
new P_CHEMICAL[MAX_PLAYERS][MAX_INV_SLOTS][P_CHEMICALDATA];

enum BreakingBadData
{                       // IN DATABASE by Jewking
    temp_ID,            //id
	bool:IsWorking,     //isworking
    WorkType,           //worktype
    BuildingID,         //building
    BuildingType,       //buildingtype
    FurnID,             //FurnID
    NeedTime,           //time
    Material,           //material
    Float: Amount,      //amount
    Float: Quality,     //quality
    Extra[3],           //extra1 extra2 extra2
    Type,               //type
    Float: Temperature, //temperature
    Float: curTemperature,  //curTemperature
    Timer,                   
    OverPoint,
    Float: Degree,      //Degree
    Control,            //Control
    BeingCrystal,       //ready
    Text3D:osLabel,
    reactorSmoke,

    Float:dropPosX,
	Float:dropPosY,
	Float:dropPosZ,
	Float:dropPosRX,
	Float:dropPosRY,
	Float:dropPosRZ,
    dropInt,
    dropVW,
    dropTime,
    dropObject
}
new Chemistry[MAX_CHEMISTRY][BreakingBadData];
new DropChemistry[MAX_CHEM_DROPS][BreakingBadData];


/*
If it's a labile chemical, when they made a wrong step, black smoke will be actived, and if they don't have gas mask they will be faded away.
*/

new E_CHEMICAL[][DRUG_DATA] =
{
    // ID----MAIN--NOM-----------------------TYPE--FORMULE---CTRL--TEMPЙRATURE--ЙTAT //
    {0,  0, "Phosphore rouge",              1, "2P2O5",     1, 200, 0},
    {1,  0, "Mйthylbenzylcйtone",           1, "C9H10O",    1, 216, 0},
    {2,  1, "Mйthylamine",                  1, "CH3NH2",    1, 172, 0}, // MDMA
    {3,  0, "Hйliotropine",                 1, "C8H6O3",    2, -120, 1},
    {4,  1, "Йphйdrine",                    2, "C10H15NO",  1, 137, 1}, // Mйthamphйtamine
    {5,  0, "Camйlйon minйral",             3, "KMnO4",     1, 240, 1},
    {6,  0, "Chloroforme",                  3, "CHCl3",     1, 161, 1},
    {7,  0, "Йther",                        3, "C4H10O",    1, 160, 0}, // >160°C + >3mg = explosion
    {8,  0, "Acide chlorhydrique",         1, "HCL",       1, -144, 1}, // toxique
    {9,  1, "Mйthanol",                     1, "CH3OH",     1, 97, 0}, // Amphйtamine
    {10, 1, "Acide lysergique",            2, "C8H8O2",    1, 265, 0}, // LSD
    {11, 0, "Ecgonine",                     1, "C9H15NO3",  1, -199, 1},
    {12, 1, "Feuilles de coca",            3, "Feuilles de coca", 1, 15, 0}, // Cocaпne (254)
    {13, 1, "Pavot",                        2, "Pavot",     1, 202, 0}, // Hйroпne
    {14, 1, "Feuilles de coca sйchйes",    3, "Feuilles de coca", 1, 194, 0}, // Cocaпne
    {15, 1, "Pavot sйchй",                 2, "Pavot",     1, 152, 0} // Hйroпne
};


new check_drug[][CHECK_DRUG_DATE] =
{
    {-1, -1},
    {-1, -1},
    {1, 10},  //MDMA
    {-1, -1},
    {6, 7},  //Мет
    {-1, -1},
    {5, 9},  //LSD
    {-1, -1},
    {-1, -1},
    {-1, -1},
    {-1, -1},
    {-1, -1},
    {11, 3}, // Амфетамин 
    {9, 10}, // Кокаин
    {6, 8}, // Героин
    {9, 10}, // Кокаин
    {6, 8}  // Героин
};

new gString[256];