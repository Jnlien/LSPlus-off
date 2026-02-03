enum ITEM_ENUM
{
	itemModel,
	itemType,
	itemSize,
	itemName[32],
	itemQ,
	itemSplit,
	itemFloat,
	itemID
};
#define MAX_ITEM_LIST 119
new inventory_items[MAX_ITEM_LIST][ITEM_ENUM] =
{
	{0,    ITEM_TYPE_NONE,   ITEM_SIZE_NONE,  "Aucun",                   0,0,0, 0},
	{331,  ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM,"Poing américain",         0,0,0, 1},
	{333,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Club de golf",            0,0,0, 2},
	{334,  ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM,"Matraque",                0,0,0, 3},
	{335,  ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM,"Couteau",                 0,0,0, 4},
	{336,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Batte de baseball",       0,0,0, 5},
	{337,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Pelle",                   0,0,0, 6},
	{338,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Queue de billard",        0,0,0, 7},
	{339,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Katana",                  0,0,0, 8},
	{341,  ITEM_TYPE_WEAPON, ITEM_SIZE_VBIG,  "Tronçonneuse",            0,0,0, 9},
	{321,  ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM,"Godemichet violet",       0,0,0, 10},
	{322,  ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM,"Godemichet",              0,0,0, 11},
	{323,  ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM,"Vibromasseur",            0,0,0, 12},
	{324,  ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM,"Vibro en argent",         0,0,0, 13},
	{325,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Fleurs",                  0,0,0, 14},
	{326,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Canne",                   0,0,0, 15},
	{342,  ITEM_TYPE_WEAPON, ITEM_SIZE_SMALL, "Grenades",                0,1,0, 16},
	{343,  ITEM_TYPE_WEAPON, ITEM_SIZE_SMALL, "Gaz lacrymogène",         0,1,0, 17},
	{344,  ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,   "Cocktail Molotov",        0,1,0, 18},
	{2656, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM,  "Autocollant de transport", 0, 1, 0, 19},
	{19942, ITEM_TYPE_MISC, ITEM_SIZE_SMALL,  "Radio", 					 0, 1, 0, 20},
	{19801, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM, "Masque",					 0, 1, 0, 21},
	{346, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "M1911",					 0, 0, 0, 22},
	{347, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "M1911 (avec silencieux)", 0, 0, 0, 23},
	{348, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "Desert Eagle",			 0, 0, 0, 24},
	{349, ITEM_TYPE_WEAPON, ITEM_SIZE_BIG, 	  "Fusil à pompe",			 0, 0, 0, 25},
	{350, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "Fusil à canon scié",		 0, 0, 0, 26},
	{351, ITEM_TYPE_WEAPON, ITEM_SIZE_BIG,    "Fusil de combat",		 0, 0, 0, 27},
	{352, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "Uzi",					 0, 0, 0, 28},
	{353, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "MP5",					 0, 0, 0, 29},
	{355, ITEM_TYPE_WEAPON, ITEM_SIZE_BIG, 	  "AK-47",					 0, 0, 0, 30},
	{356, ITEM_TYPE_WEAPON, ITEM_SIZE_BIG, 	  "M4",						 0, 0, 0, 31},
	{372, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "Tec-9",					 0, 0, 0, 32},
	{357, ITEM_TYPE_WEAPON, ITEM_SIZE_VBIG,   "Carabine de chasse",		 0, 0, 0, 33},
	{358, ITEM_TYPE_WEAPON, ITEM_SIZE_VBIG,   "Fusil de sniper",		 0, 0, 0, 34},
	{19094, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Burger",					 0, 1, 0, 35},
	{2702, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM,  "Pizza",					 0, 1, 0, 36},
	{2663, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM,  "Poulet en sachet",		 0, 1, 0, 37},
	{19346, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Hot-Dog",				 0, 1, 0, 38},
	{1650, ITEM_TYPE_MISC, ITEM_SIZE_VBIG, 	  "Jerrican",				 0, 1, 0, 39},
	{2226, ITEM_TYPE_MISC, ITEM_SIZE_VBIG,    "Boombox",				 0, 1, 0, 40},
	{365, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "Bombe de peinture",		 50, 0, 0, 41},
	{366, ITEM_TYPE_WEAPON, ITEM_SIZE_VBIG,   "Extincteur",				 50, 0, 0, 42},
	{367, ITEM_TYPE_WEAPON, ITEM_SIZE_MEDIUM, "Appareil photo",			 50, 0, 0, 43},
	{2040, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM,  "Silencieux",				 0, 0, 0, 44},
	{2040, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM,  "Lunette optique",		 0, 0, 0, 45},
	{371, ITEM_TYPE_WEAPON, ITEM_SIZE_VBIG, "Parachute", 0, 0, 0, 46},
	{19998, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Briquet", 0, 1, 0, 47},
	{19897, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM, "Cigarettes", 0, 1, 0, 48},
	{1906, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Dés", 0, 1, 0, 49},
	{11748, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM, "Cartes à jouer", 0, 1, 0, 50},
	{3027, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Stylo", 0, 1, 0, 51},
	{19921, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Crochet", 0, 1, 0, 52},
	{2041, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Douille", 0, 1, 0, 53},
	{19792, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Poudre", 0, 1, 0, 54},
	{19792, ITEM_TYPE_NONE, ITEM_SIZE_SMALL, "Sac mortuaire", 0, 1, 0, 55},
	{2958, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Bière", 0, 1, 0, 56},
	{1668, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Vodka", 0, 1, 0, 57},
	{1546, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Cola", 0, 1, 0, 58},
	{1455, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Eau", 0, 1, 0, 59},
	{1520, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Whisky", 0, 1, 0, 60},
	{19820, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Brandy", 0, 1, 0, 61},
	{19346, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Soda", 0, 1, 0, 62},
	{1664, ITEM_TYPE_FOOD, ITEM_SIZE_MEDIUM, "Vin", 0, 1, 0, 63},
	{1664, ITEM_TYPE_SEEDS, ITEM_SIZE_SMALL, "Graines de cannabis", 0, 1, 0, 64},
	{1578, ITEM_TYPE_DRUGS, ITEM_SIZE_SMALL, "Cannabis médical", 0, 1, 1, 65},
	{1578, ITEM_TYPE_DRUGS, ITEM_SIZE_SMALL, "Cannabis", 0, 1, 1, 66},
	{1579, ITEM_TYPE_DRUGS, ITEM_SIZE_SMALL, "Drogue", 0, 1, 1, 67},
	{1576, ITEM_TYPE_NONE, ITEM_SIZE_SMALL, "None", 0, 0, 0, 68},
	{1577, ITEM_TYPE_NONE, ITEM_SIZE_SMALL, "None", 0, 0, 0, 69},
	{1575, ITEM_TYPE_NONE, ITEM_SIZE_SMALL, "None", 0, 0, 0, 70},
	{1664, ITEM_TYPE_NONE, ITEM_SIZE_SMALL, "None", 0, 0, 0, 71},
	{1664, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Diéthyle", 0, 1, 0, 72},
	{1650, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Soude", 0, 1, 1, 73},
	{1664, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Acide chlorhydrique", 0, 1, 1, 74},
	{11736, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM, "Trousse de secours", 0, 1, 0, 75},
	{19920, ITEM_TYPE_MISC, ITEM_SIZE_MEDIUM, "Autoradio", 0, 1, 0, 76},
	{19878, ITEM_TYPE_MISC, ITEM_SIZE_BIG, "Skateboard", 0, 0, 0, 77},
	{2814, ITEM_TYPE_INSIDEABLE_OBJECT, ITEM_SIZE_BIG, "Boîte à pizza", 0, 0, 0, 78},
	{2663, ITEM_TYPE_INSIDEABLE_OBJECT, ITEM_SIZE_MEDIUM, "Sac 'Cluckin' Bell'", 0, 0, 0, 79},
	{1664, ITEM_TYPE_SEEDS, ITEM_SIZE_SMALL, "Graines de coca", 0, 1, 0, 80},
	{1664, ITEM_TYPE_SEEDS, ITEM_SIZE_SMALL, "Graines de pavot", 0, 1, 0, 81},
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, ".45 ACP", 7, 0, 0, 82}, // M1911 //82
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, ".45 ACP .ext", 14, 0, 0, 83}, // M1911 //83
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, ".50AE", 7, 0, 0, 84}, // DEAGLE //84
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, ".50AE .ext", 14, 0, 0, 85}, // DEAGLE //85
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "12-G BIRDSHOT", 6, 0, 0, 86}, // SHOTGUN //86
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "12-G BUCKSHOT", 6, 0, 0, 87}, // SHOTGUN //87
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "12-G SLUG", 6, 0, 0, 88}, // SHOTGUN //88
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "12-G BEANBAG", 6, 0, 0, 89}, // SHOTGUN //89
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "9x19mm", 30, 0, 0, 90}, // MAC/TEC //90
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "9x19mm .ext", 60, 0, 0, 91}, // MAC/TEC //91
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "7.62x39mm", 30, 0, 0, 92}, // AK47 //92
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "5.56x45mm", 30, 0, 0, 93}, // M4A1 //93
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "7.62x39mm T", 30, 0, 0, 94}, // AK47 //94
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "5.56x45mm T", 30, 0, 0, 95}, // M4A1 //95
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "7.62x51mm", 4, 0, 0, 96}, // SNIPER / RIFLE //96
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "7.62x39mm (drum)", 80, 0, 0, 97}, // AK47 //97
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "5.56x45mm (drum)", 60, 0, 0, 98}, // M4 //98
	{2041, ITEM_TYPE_AMMO, ITEM_SIZE_SMALL, "5.56x45mm (rubber)", 30, 0, 0, 99}, // M4 //99
	{19921, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Outils", 0, 0, 0, 100},
	{19257, ITEM_TYPE_MISC, ITEM_SIZE_SMALL, "Immatriculation falsifiées", 0, 0, 0, 101},
	{371, ITEM_TYPE_INSIDEABLE_OBJECT, ITEM_SIZE_MEDIUM, "Sac à dos", 0, 0, 0, 102},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Phosphore rouge", 0, 1, 1, 103},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Méthylbenzyl-cétone", 0, 1, 1, 104},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Méthylamine", 0, 1, 1, 105},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Héliotropine", 0, 1, 1, 106},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Éphédrine", 0, 1, 1, 107},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Caméléon minéral", 0, 1, 1, 108},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Chloroforme", 0, 1, 1, 109},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Éther", 0, 1, 1, 110},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Chlorure d'hydrogène", 0, 1, 1, 111},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Méthanol", 0, 1, 1, 112},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Acide lysergique", 0, 1, 1, 113},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Écgonine", 0, 1, 1, 114},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Feuilles de coca", 0, 1, 1, 115},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Pavot", 0, 1, 1, 116},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Feuilles de coca séchées", 0, 1, 1, 117},
	{2709, ITEM_TYPE_REACT, ITEM_SIZE_MEDIUM, "Pavot séché", 0, 1, 1, 118}

};

enum DROP_ENUM
{
	dropBDID,
	dropID,
	dropAmount,
	dropEx,
	dropS,
	Float:dropQFloat,
	dropInvName[16],
	dropObject,
	Float:dropPosX,
	Float:dropPosY,
	Float:dropPosZ,
	Float:dropPosRX,
	Float:dropPosRY,
	Float:dropPosRZ,
	dropInt,
	dropVW,
	dropTime
}
new ItemDrop[MAX_ITEM_DROPS][DROP_ENUM];

enum FOOD_DROPS_ENUM
{
    foodDropped,
	foodObject,
	foodModel,
	Float:foodPosX,
	Float:foodPosY,
	Float:foodPosZ,
	foodVW,
	foodInt,
	foodTime
}
new FoodDrop[MAX_FOOD_DROPS][FOOD_DROPS_ENUM];