#define GameModeText "LS+ "GameModeVersion"" 
#define WeburlName "lsplus.online" 
#define ServerName "Los Santos +" 
#define MAP_NAME "Los Santos"

Server_OnGameModeInit()
{
    SendRconCommand("mapname "MAP_NAME"");
	SendRconCommand("hostname (FR) "ServerName"");
	SendRconCommand("weburl "WeburlName"");
    
    DisableInteriorEnterExits();
	ManualVehicleEngineAndLights();
	AllowInteriorWeapons(1);
	EnableStuntBonusForAll(0);
 	DisableNameTagLOS();
	ShowPlayerMarkers(1);
	LimitPlayerMarkerRadius(15.0);
	SetNameTagDrawDistance(15.0);
	SetGameModeText(GameModeText);

	new query[128];
	format(query, sizeof(query), "INSERT INTO `logs_exit` (`date`) VALUES ('%s')", GetFullDate());
    mysql_tquery(dbHandle, query, "OnLogsExitInsert");
	
	//AddPlayerClass(0, 695.9435, -1670.3030, 3.3368, 331.5099, 0, 0, 0, 0, 0, 0);
    return 1;
}