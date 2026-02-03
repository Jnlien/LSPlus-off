Player_OnPlayerUpdate(playerid)
{
	if (IsPlayerNPC(playerid) || !IsPlayerLogged(playerid)) 
		return 1;

	/*if (IsPlayerAFK(playerid))
	{
		if (!(PlayerInfo[playerid][pSettings] & togAFK)) 
			SendSyntaxMessage(playerid, "Vous avez été AFK pendant %i secondes.", pTemp[playerid][pDarkAFK] - 2);
	}*/

	pTemp[playerid][pDarkAFK] = 0;

	return 1;
}