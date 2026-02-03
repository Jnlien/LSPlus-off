ALTX:damages("/dmg");
CMD:damages(playerid, params[])
{
	new userid;

	if (!IsPlayerLogged(playerid))
		return SendNotConnected(playerid);

	if (sscanf(params, "u", userid))
		return SendSyntaxMessage(playerid, "/damages [ID du joueur]");

	if (!IsPlayerConnected(userid))
		return SendErrorMessage(playerid, "Le joueur n'est pas en ligne.");

	if (!PlayerNearPlayer(3.0, playerid, userid) && !CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotNearPlayer(playerid);

	pTemp[playerid][pDialogList] = 0;

	DisplayDamages(playerid, userid);
	return 1;
}

CMD:relever(playerid, params[])
{
	new userid;

	if (!IsPlayerLogged(playerid))
		return SendNotConnected(playerid);

	if (sscanf(params, "u", userid))
		return SendSyntaxMessage(playerid, "/relever [ID du joueur]");

	if (userid == playerid)
		return 1;

	if (!IsPlayerConnected(userid))
		return SendErrorMessage(playerid, "Le joueur n'est pas en ligne.");

	if (!PlayerNearPlayer(2.0, playerid, userid))
		return SendNotNearPlayer(playerid);

	if (pTemp[userid][pInjuredTime] - 40 > 0)
		return SendErrorMessage(playerid, "Vous devez encore attendre %i secondes !", pTemp[userid][pInjuredTime] - 40);

	StartProgress(userid, playerid, 1, 839);
	return 1;
}

CMD:acceptermort(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] == 0)
        return SendErrorMessage(playerid, "Vous n'êtes pas blessé.");

	new count = 0;
	foreach(new i : Player)
	{
		if (i == playerid) continue;
		if (GetPlayerDistanceFromPlayer(playerid, i) < 11.0) count++;
	}

	if (pTemp[playerid][pInjuredTime])
	{
		if (count)
		{
			SendWarningMessage(playerid, "Vous ne pouvez pas accepter la mort immédiatement s'il y a des joueurs à proximité. Vous devez encore attendre %i secondes.", pTemp[playerid][pInjuredTime]);
			return 1;
		}
	}

	SendSpecialMessage(playerid, "Votre personnage est mort. Vous devez attendre 60 secondes avant de réapparaître avec \"/respawn\"."); 
	PlayerInfo[playerid][pInjured] = 2;
	pTemp[playerid][pInjuredTime] = 60;
	return 1;
}

CMD:respawn(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 2)
		return SendErrorMessage(playerid, "Vous n'êtes pas mort.");
		
	if (pTemp[playerid][pInjuredTime])
		return SendSpecialMessage(playerid, "Vous avez attendu seulement %i secondes. Vous devez attendre au moins 60 secondes pour utiliser /respawn.", 60 - pTemp[playerid][pInjuredTime]);

	if (PlayerInfo[playerid][pAJailed] != 0)
	{
		SetInterior(playerid, 18);
		SetWorld(playerid, playerid + 100);
		SetPlayerPos(playerid, -219.4580, 1410.6747, 27.7734);
		SendClientMessageEx(playerid, COLOR_WHITE, "Vous avez été renvoyé en prison administrative. Durée : %i minutes.", PlayerInfo[playerid][pAJailTime] / 60);
	}
	else if (PlayerInfo[playerid][pJailed])
	{
		PutPlayerInPrison(playerid);
	}
	else
	{
		CreateCorpse(playerid, PlayerInfo[playerid][pDeathReason]);

		SetPlayerPos(playerid, 1242.1882, 328.0487, 19.7555);
		SetPlayerFacingAngle(playerid, 270.0);
		SetInterior(playerid, 0);
		SetWorld(playerid, 0);

		SM2(playerid, "Vous avez été hospitalisé.");

		new bill = 100;
		new tax = 15;
		new year, month, day;

		getdate(year, month, day);

		SendClientMessageEx(playerid, 0xFF8282AA, "**********************************************");
		SendClientMessageEx(playerid, 0xFF8282AA, "             FACTURE DE L'HÔPITAL :");
		SendClientMessageEx(playerid, 0xFF8282AA, "NOM : %s - DATE : %d-%d-%d.", GetNameEx(playerid), day, month, year);
		SendClientMessageEx(playerid, 0xFF8282AA, "SOINS : $%d - TAXE : $%d.", bill, tax);
		
		bill += tax;

		SendClientMessageEx(playerid, 0xFF8282AA, "TOTAL : $%d.", bill);
		SendClientMessageEx(playerid, 0xFF8282AA, "**********************************************");
	}

	PlayerInfo[playerid][pDeathReason] = 0;
	SQL_SetInteger("users", "deathreason", 0, PlayerInfo[playerid][pID]);

	if (!PlayerInfo[playerid][pPremium]) SetPlayerHealth(playerid, 50.0);
	else SetPlayerHealth(playerid, 100.0);

	ResetDamages(playerid);
	StopPlayerAnims(playerid);
	UnFreezePlayer(playerid);

	SetPlayerSkill(playerid);
	return 1;
}