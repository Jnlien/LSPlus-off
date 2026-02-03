SendTaxiRequest(playerid, location[])
{
    foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || !TaxiDuty[i] || PlayerInfo[i][pJob] != JOB_TAXI) continue;

        SendClientMessage(i, COLOR_GREEN, "|___________Appel de taxi___________|");
		if (pTemp[playerid][pPayphone] == -1) 
			SendClientMessageEx(i, COLOR_WHITE, "Appel de: %s Téléphone: %i", GetNameEx(playerid), PlayerInfo[playerid][pNumber]);
		else
			SendClientMessageEx(i, COLOR_WHITE, "Appel de: Cabine téléphonique, Téléphone: %i", PayphoneInfo[pTemp[playerid][pPayphone]][phNumber]);
		SendClientMessageEx(i, COLOR_WHITE, "Localisation: %s", location);
		SendClientMessageEx(i, COLOR_WHITE, "*/taxi accept %i pour accepter l'appel **", playerid);
	}
	return 1;
}

Taxi_OnGameModeInit()
{
    return SetTimer("Taxi_FiveSecTimer", 5000, false);
}