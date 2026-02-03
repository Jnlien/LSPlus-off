CMD:news(playerid, params[])
{
	if (!IsPlayerFactionNews(playerid))				return SendErrorMessage(playerid, "Vous devez être employé de SAN News.");
	if (PlayerInfo[playerid][pSettings] & togNews)	return SendErrorMessage(playerid, "Vous avez désactivé l'affichage des nouvelles.");
	if (pTemp[playerid][pNews] != 0)					return SendErrorMessage(playerid, "Vous êtes déjà en direct. Utilisez /exitnews pour quitter l'antenne.");

	pTemp[playerid][pNews] = 1;
	SendSyntaxMessage(playerid, "Vous avez commencé un reportage. Utilisez le chat local pour diffuser en direct.");
	return 1;
}

CMD:exitnews(playerid, params[])
{
	if (!IsPlayerFactionNews(playerid))	return SendErrorMessage(playerid, "Vous devez être employé de SAN News.");
	if (pTemp[playerid][pNews] == 0)		return SendErrorMessage(playerid, "Vous n'êtes pas en direct. Utilisez /news pour commencer un reportage.");

	pTemp[playerid][pNews] = 0;
	SendSyntaxMessage(playerid, "Vous avez terminé le reportage.");
	return 1;
}

CMD:interview(playerid, params[])
{
    if (!IsPlayerFactionNews(playerid)) return SendErrorMessage(playerid, "Vous devez être employé de SAN News.");

	new userid;
	
	if (sscanf(params,"u",userid))								return SendHelpMessage(playerid, "/interview [ID/Nom du joueur]");
	if (PlayerInfo[playerid][pSettings] & togNews)				return SendErrorMessage(playerid, "Vous avez désactivé l'affichage des nouvelles.");
	if (pTemp[playerid][pNews] == 0)								return SendErrorMessage(playerid, "Vous n'êtes pas en direct. Utilisez /news pour entrer en direct.");
	if (userid == playerid)										return SendErrorMessage(playerid, "Vous ne pouvez pas vous inviter vous-même à une interview.");
	if (!PlayerNearPlayer(3.0, playerid, userid))				return SendNotNearPlayer(playerid);
	if (pTemp[playerid][pInterviewPlayer] != INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Vous avez déjà invité quelqu'un pour une interview. Attendez un moment ou annulez l'invitation.");
	if (pTemp[userid][pInterviewPlayer] != INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Ce joueur a déjà reçu une invitation à une interview.");

    pTemp[userid][pInterviewPlayer] = playerid;
    SetPVarInt(userid, #accept_time, gettime() + 30);

    SendSyntaxMessage(playerid, "%s a reçu votre invitation pour une interview, veuillez patienter.", GetNameWithMask(userid));
	SendSyntaxMessage(userid, "%s vous invite à participer à une interview.");
	SendSyntaxMessage(userid, "Appuyez sur Y pour accepter l'invitation.");
	return 1;
}

CMD:exitinterview(playerid, params[])
{
	if (pTemp[playerid][pInterview] != INVALID_PLAYER_ID)
	{
	    SendSyntaxMessage(playerid, "Vous avez terminé l'interview.");
	    pTemp[playerid][pInterview] = INVALID_PLAYER_ID;
	    return 1;
	}

    if (!IsPlayerFactionNews(playerid)) return SendErrorMessage(playerid, "Vous devez être employé de SAN News.");

	new userid;

	if (sscanf(params, "u", userid))					return SendHelpMessage(playerid, "/exitinterview [ID/Nom du joueur]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))		return SendNotNearPlayer(playerid);
	if (pTemp[userid][pInterview] == INVALID_PLAYER_ID) return SendErrorMessage(playerid, "Vous ne menez pas d'interview avec ce joueur.");

    SendSyntaxMessage(playerid, "Vous avez terminé l'interview avec %s.", GetNameWithMask(userid));
	SendSyntaxMessage(userid, "%s a terminé l'interview avec vous.", GetNameWithMask(playerid));
	pTemp[userid][pInterview] = INVALID_PLAYER_ID;
	return 1;
}