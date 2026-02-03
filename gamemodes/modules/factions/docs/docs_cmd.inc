CMD:isolate(playerid, params[])
{
	new userid, time;

	if (!IsPlayerFactionDocs(playerid))								return SendErrorMessage(playerid, "Vous devez être membre du personnel pénitentiaire.");
	if (sscanf(params,"uI(0)", userid, time))							return SendHelpMessage(playerid, "/isolate [ID/nom du joueur] [durée (minutes)]");
	if (!PlayerInfo[playerid][pOnDuty])									return SendErrorMessage(playerid, "Vous devez être en service.");
	if (!PlayerToPoint(50.0, playerid, -1774.4573, -488.8803, 621.1163))	return SendErrorMessage(playerid, "Vous devez être dans l'isolement.");
	if (!IsPlayerLogged(userid))										return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))						return SendNotNearPlayer(playerid);
	if (userid == playerid)												return 1;

	if (PlayerInfo[userid][pIsolateTime])
	{
	    SendSyntaxMessage(playerid, "Vous avez libéré %s de l'isolement.", GetNameEx(userid));
		SendSyntaxMessage(userid, "Vous avez été libéré de l'isolement.");
		
		PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));
		new mes[5];
		format(mes, sizeof(mes), "A%i", PlayerInfo[userid][pPrisoned]+100);
        UpdateDynamic3DTextLabelText(PrisonLabel[userid], 0x00FF00FF, mes);

		SendSyntaxMessage(userid, "Votre numéro de détenu est - A%i (/myprisonnumber).", PlayerInfo[userid][pPrisoned]+100);
        PlayerInfo[userid][pIsolateTime] = 0;
		
		foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || PlayerInfo[i][pJailed] != PRISON_JAIL) continue;
			SendSyntaxMessage(i, "%s %s a libéré %s de l'isolement.", GetRankName(playerid), GetNameEx(playerid), GetNameEx(userid));
		}
	    return 1;
	}

	if (time < 1 || time > 360) return SendErrorMessage(playerid, "La durée doit être comprise entre 1 et 360 minutes.");

    SendSyntaxMessage(playerid, "Vous avez placé %s en isolement.", GetNameEx(userid));
	SendSyntaxMessage(userid, "Vous avez été placé en isolement.");
	
	PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));
	SendSyntaxMessage(userid, "Votre numéro de détenu est - B%i (/myprisonnumber).", PlayerInfo[userid][pPrisoned]+100);
	PlayerInfo[userid][pIsolateTime] = 60 * time;
	
	PutPlayerInPrison(userid);

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pJailed] != PRISON_JAIL) continue;
		SendSyntaxMessage(i, "%s %s a placé %s en isolement.", GetRankName(playerid), GetNameEx(playerid), GetNameEx(userid));
	}
	return 1;
}

CMD:free(playerid, params[])
{
	new userid;

	if (!IsPlayerFactionDocs(playerid))									return SendErrorMessage(playerid, "Vous devez être membre du personnel pénitentiaire.");
	if (sscanf(params,"u", userid))                                  	return SendHelpMessage(playerid, "/free [ID/nom du joueur]");
	if (!PlayerInfo[playerid][pOnDuty])                              	return SendErrorMessage(playerid, "Vous devez être en service.");
	if (!PlayerToPoint(30.0, playerid, 555.5505, -474.4340, 915.6733))	return SendErrorMessage(playerid, "Vous n'êtes pas dans la prison.");
	if (!IsPlayerLogged(userid))                                     	return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))                    	return SendNotNearPlayer(playerid);
	if (userid == playerid)                                          	return 1;
	if (PlayerInfo[userid][pJailed] != PRISON_JAIL)                  	return SendErrorMessage(playerid, "Le joueur n'est pas emprisonné.");
	if (PlayerInfo[playerid][pRank] < 6)                             	return SendErrorMessage(playerid, "Vous n'avez pas le grade requis pour libérer un prisonnier.");

	SetPlayerPosEx(userid, 135.2682, 1947.4398, 19.3663);
	SetInterior(userid, 0);
	SetWorld(userid, 0);
	PlayerInfo[userid][pIsolateTime] = 0;
	PlayerInfo[userid][pJailTime] = 0;
	PlayerInfo[userid][pJailed] = 0;

	if (IsValidDynamic3DTextLabel(PrisonLabel[userid]))
	{
		DestroyDynamic3DTextLabel(PrisonLabel[userid]);
  		PrisonLabel[userid] = Text3D:INVALID_3DTEXT_ID;
	}

	GameTextForPlayer(userid,"~g~Vous êtes libre~n~~w~Essayez d'adopter un bon comportement", 5000, 1);
	SendSyntaxMessage(userid, "Vous avez été gracié par %s %s.", GetRankName(playerid), GetNameEx(playerid));
	return 1;
}