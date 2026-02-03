enum TELES_ENUM
{
	aID,
	Float:telesPosX,
	Float:telesPosY,
	Float:telesPosZ,
	telesInt,
	telesMapName[32],
	telesOn,
	telesType
};
new AdminTeles[MAX_INTERIORS][TELES_ENUM];

new ClickedPlayerID[MAX_PLAYERS];

new gSanctionDialog[MAX_PLAYERS][MAX_DIALOG_TEXT];
new gPersonnageDialog[MAX_PLAYERS][MAX_DIALOG_TEXT];