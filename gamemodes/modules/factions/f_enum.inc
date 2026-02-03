enum CARFIX_ENUM
{
	Float: fixX,
	Float: fixY,
	Float: fixZ,
	fixVW,
	fixInt,
	fixPickUp
}

enum FACTION_ENUM
{
	fID,
	fPickUp,
	fName[32],
	fShortName[8],
	Float:fPosX,
	Float:fPosY,
	Float:fPosZ,
	fSpawnInt,
	fSpawnVW,
	fJoinRank,
	fType,
	fRanks,
	fChatON,
	fColor,
	Float:fUniformX,
	Float:fUniformY,
	Float:fUniformZ,
	fUniformInt,
	fUniformVW,
	Carfix[CARFIX_ENUM],
	
	fPoint,
	fPointTime,
	fON
};
new Factions[MAX_FACTIONS][FACTION_ENUM];
new FactionRanks[MAX_FACTIONS][20][32];

enum RANK_ENUM
{
	rInvite,
	rUninvite,
	rRank,
	r_eRank,
	rSpawn,
	rChat,
	rTow,
	r_eRights,
};
new FRank[MAX_FACTIONS][20][RANK_ENUM];

enum RANK_SETTINGS_DATA
{
	right_id,
	right_name[64],
	right_cmd[32]
};

new RS[8][RANK_SETTINGS_DATA] = {
    {0, "{FFFFFF}Recruter dans la faction", "/invite"},
    {1, "{FFFFFF}Exclure de la faction", "/uninvite"},
    {2, "{FFFFFF}Modifier le rang", "/giverank"},
    {3, "{FFFFFF}Modifier le nom du rang", "/editrankname"},
    {4, "{FFFFFF}Modifier le point d'apparition", "/editspawn"},
    {5, "{FFFFFF}Gestion du chat faction", "/fchat"},
    {6, "{FFFFFF}Envoyer les véhicules inutilisés", "/towcars"},
    {7, "{FFFFFF}Modifier les permissions de rang", "/editrights"}
};

enum BORT_ENUM
{
    bortOn,
	bortName[32],
	bortObject,
	bortUsing
}
new BortInfo[MAX_PLAYERS][MAX_BORTES][BORT_ENUM];