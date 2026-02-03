Dialog:LSPDElevator(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
    if (!IsPlayerInRangeOfPoint(playerid, 2.0, 2268.0640,2448.2449,3.5313) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2781.0559,-1056.2823,1028.4969) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2874.6665,-940.6707,1035.9259) && !IsPlayerInRangeOfPoint(playerid, 2.0, 2872.8813,-1072.9934,1037.1782)) return 1;
		
	new world_list[] = {2, 1, 2, 3};

	switch(listitem)
	{
	    case 0: SetPlayerPosEx(playerid, 2268.0640,2448.2449,3.5313), SetPlayerFacingAngle(playerid, 181.2476);
	    case 1: SetPlayerPosEx(playerid, 2780.9036,-1056.2358,1028.4969), SetPlayerFacingAngle(playerid, 87.9492);
	    case 2: SetPlayerPosEx(playerid, 2873.9063,-940.8126,1035.9259), SetPlayerFacingAngle(playerid, 84.3340);
	    case 3: SetPlayerPosEx(playerid, 2872.7168,-1072.2004,1037.1770), SetPlayerFacingAngle(playerid, 0.2384);
	}

	SetWorld(playerid, world_list[listitem]);
	SetInterior(playerid, (listitem == 0)?(0):(1));
	return 1;
}

Dialog:MPCarVoice(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	    
    MegaphoneSoundVoice(playerid, listitem + 1);
	return 1;
}

/*
Dialog:MainMDC(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem)
	{
		case 0:	Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Recherche par nom", "{B4B5B7}RECHERCHE PAR NOM\n\nEntrez un nom :\n\nFormat : Prénom_Nom (( ou ID joueur ))", "Entrer", "Retour");
		case 1:	Dialog_Show(playerid, MDCRecord, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Présentation d'accusation", "{B4B5B7}PRÉSENTATION D'ACCUSATION\n\nEntrez un nom :\n\nFormat : Prénom_Nom (( ou ID joueur ))", "Entrer", "Retour");
		case 2:	Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Avertissement sur licence", "{B4B5B7}AJOUTER UN AVERTISSEMENT SUR LICENCE\n\nEntrez un nom :\n\n(( Prénom_Nom / ID joueur ))", "Entrer", "Retour");
		case 3:	Dialog_Show(playerid, MDCVehicle, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Recherche de véhicule", "{B4B5B7}RECHERCHE D'INFORMATIONS SUR UN VÉHICULE\n\nEntrez une plaque d'immatriculation :\n\n(( Plaque ))", "Entrer", "Retour");
		case 4: Get_Calls(playerid);
		case 5:
		{
			new string[MAX_CALLS_AND_ARREST * 32], count;

			for(new i = 0; i != MAX_CALLS_AND_ARREST; i++)
			{
				if (!ArrestInfo[i][arrestExists]) continue;

				format(string, sizeof(string), "%s\n%i. %s", string, count+1, GetNameEx(ArrestInfo[i][arrestSuspect]));
			}

			Dialog_Show(playerid, MDCLastArrest, DIALOG_STYLE_LIST, "{8D8DFF}MDC - Dernières arrestations", string, "Choisir", "Retour");
		}
		case 6:
		{
			new string[1024], count;

			for(new i = 0; i != MAX_PATROL; i++)
			{
				if (!PatrolInfo[GetPatrolID(playerid)][i][patrulExists]) continue;

				format(string, sizeof(string), "%s\n{8c8cfe}%s:{FFFFFF}  ", string, PatrolInfo[GetPatrolID(playerid)][i][patrulName]);

				if (IsPlayerLogged(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0]))	format(string, sizeof(string), "%s%s  ", string, GetNameEx(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0]));
				if (IsPlayerLogged(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1]))	format(string, sizeof(string), "%s%s  ", string, GetNameEx(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1]));

				count++;
			}

			if (!count) return SendErrorMessage(playerid, "Aucune patrouille active trouvée.");

			Dialog_Show(playerid, None, DIALOG_STYLE_LIST, "{8D8DFF}MDC - Patrouilles actives", string, "Fermer", "");
		}
		case 7: ShowWeaponLicense(playerid, 0);
	}
	return 1;
}

Dialog:MDCLastArrest(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new count;

	for(new i = 0; i != MAX_CALLS_AND_ARREST; i++)
	{
		if (!ArrestInfo[i][arrestExists]) continue;

		if (count == listitem)
		{
			new string[500];

			format(string, sizeof(string), "{B4B5B7}INFORMATIONS SUR L'ARRESTATION\n\nHeure de l'arrestation : %s\nArrêté : %s\nEffectué par : %s\nDate : %s", ConvertSecond(ArrestInfo[i][arrestTime]), ArrestInfo[i][arrestSuspect], ArrestInfo[i][arrestOfficer], ArrestInfo[i][arrestDate]);
			Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{8D8DFF}MDC - Informations sur l'arrestation", string, "-", "");
			return 1;
		}
		count++;
	}

	SendErrorMessage(playerid, "Erreur d'arrestation.");
	return 1;
}

Dialog:MDCWeapon(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::mdc(playerid, "");
		
	if (listitem == 0)
	{
		if (GetPVarInt(playerid, #weapon_list) < 1) 	ShowWeaponLicense(playerid, 0);
		else										ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list)-1);
		
		return 1;
	}

	else if (listitem == 1)
	{
		return ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list)+1);
	}
	
	listitem -= 2;

	if (pTemp[playerid][pWeaponListed][listitem] == 0)	return SendErrorMessage(playerid, "Erreur de requête.");
		
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `id`=%i", pTemp[playerid][pWeaponListed][listitem]);
	mysql_tquery(dbHandle, query, "ShowWeaponLicensePlayer", "i", playerid);

	for(new i; i != 30; i++)
	{
		pTemp[playerid][pWeaponListed][i] = 0;
	}
	return 1;
}
*/

Dialog:MDCCall(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	if (!IsPlayerFactionPolice(playerid) &&
		!IsPlayerFactionMedic(playerid) &&
		!IsPlayerFactionFire(playerid)) return 1;

	new message[512], count, type = (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid)) ? (2) : (1);

	for (new i = 0; i != MAX_CALLS_AND_ARREST; i++)
	{
		if (!CallInfo[i][callExists]) continue;
		if (CallInfo[i][callType] != 3 && CallInfo[i][callType] != type) continue;

		if (listitem == count)
		{
			new statusMessage[65];
			if (CallInfo[i][callAccept]) 
				format(statusMessage, sizeof(statusMessage), "%s", CallInfo[i][callAcceptBy]);
			else 
				format(statusMessage, sizeof(statusMessage), "{A9C4E4}Appel non pris");

			format(message, sizeof(message), "{B4B5B7}911-INFORMATIONS SUR L'APPEL - #%i\n\n\
									Appel de :\t\t%s\n\
									Numéro de téléphone :\t%i\n\
									Situation :\t\t%s\n\
									Localisation :\t%s\n\
									Date :\t\t\t%s\n\n\
									Statut :\t\t\t%s\n\
									L'appel a été répondu %i fois", 
			i + 1, CallInfo[i][callName], CallInfo[i][callPhone], CallInfo[i][callText], CallInfo[i][callLocation], CallInfo[i][callDate], statusMessage, CallInfo[i][callAccept]);
			
			Dialog_Show(playerid, MDCCall2, DIALOG_STYLE_MSGBOX, "{8D8DFF}MDC - Informations sur l'appel", message, "Options", "Retour");
			SetPVarInt(playerid, #last_call, i);
			return 1;
		}
		count++;
	}
	return 1;
}

Dialog:MDCCall2(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	Dialog_Show(playerid, MDCCallRespond, DIALOG_STYLE_LIST, "{8D8DFF}MDC - Informations sur l'appel", "- Accepter l'appel\n- Supprimer l'appel", "Choisir", "Retour");
	return 1;
}

Dialog:MDCCallRespond(playerid, response, listitem, inputtext[])
{
	if (!response) 
		return 1;

	if (!IsPlayerFactionPolice(playerid) &&
		!IsPlayerFactionMedic(playerid) &&
		!IsPlayerFactionFire(playerid))
		return 1;

	new type = (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid)) ? 2 : 1;
	new id = GetPVarInt(playerid, #last_call);

	if (!CallInfo[id][callExists] || CallInfo[id][callType] != type)
		return 1;

	if (!listitem)
	{
		if (pTemp[playerid][pPatrol])
			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_LSPD, "* [%s] %s %s [%s] a accepté l'appel 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], id + 1);
		else
			SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_LSPD, "* [%s] %s %s a accepté l'appel 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid), id + 1);

		if (pTemp[playerid][pPatrol])
			format(CallInfo[id][callAcceptBy], 64, "Appel accepté par %s (%s)", GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName]);
		else
			format(CallInfo[id][callAcceptBy], 64, "Appel accepté par %s", GetNameEx(playerid));

		Waypoint_Set(playerid, CallInfo[id][callPosX], CallInfo[id][callPosY], CallInfo[id][callPosZ], 3.0);

		CallInfo[id][callAccept]++;
		pTemp[playerid][pPoliceCalls]++;

		MDC_PAGE(playerid, PAGE_CALLS);
	}
	else
	{
		SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_LSPD, "* [%s] %s %s a supprimé l'appel 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid), id + 1);

		SendWarningMessage(playerid, "L'appel [ID: %i] a été supprimé.", id + 1);
		CallInfo[id][callExists] = 0;
		CallInfo[id][callType] = 0;
		CallInfo[id][callAccept] = 0;

		MDC_PAGE(playerid, PAGE_CALLS);
	}

	return 1;
}

/*
Dialog:MDCSearch(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new userid;
	if (sscanf(inputtext, "u", userid))
	{
		return Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, 
			"{8D8DFF}MDC - Recherche par nom", 
			"{B4B5B7}RECHERCHE PAR NOM\n\nEntrez un nom :\n\nFormat : Prénom_Nom (( ou ID joueur ))", 
			"Entrer", "Annuler");
	}

	if (!IsPlayerLogged(userid))
	{
		return Dialog_Show(playerid, MDCSearch, DIALOG_STYLE_INPUT, 
			"{8D8DFF}MDC - Recherche par nom", 
			"{B4B5B7}RECHERCHE PAR NOM\n\nEntrez un nom :\n\nFormat : Prénom_Nom (( ou ID joueur ))\n"EMBED_RED"Cette personne est absente de la base de données.", 
			"Entrer", "Annuler");
	}

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `criminalrecords` WHERE `userid` = %i", PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query, "PersonInfo", "ii", playerid, userid);
	return 1;
}
*/

Dialog:MDCRecord(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (sscanf(inputtext, "u", pTemp[playerid][pMDCPlayer]))
		return SendErrorMessage(playerid, "Ce joueur n'est pas en ligne !");
	
	if (!IsPlayerLogged(pTemp[playerid][pMDCPlayer]))
		return SendErrorMessage(playerid, "Le joueur n'est pas connecté !");

	new message[256];
	format(message, sizeof(message), "{B4B5B7}PRÉSENTATION D'ACCUSATION\n\nAccusé : %s\n\nEntrez le texte de l'accusation : (( Infraction / 128 caractères ))", GetNameEx(pTemp[playerid][pMDCPlayer]));
	Dialog_Show(playerid, MDCRecord2, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Présentation d'accusation", message, "Entrer", "Annuler");
	return 1;
}

Dialog:MDCRecord2(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (!IsPlayerLogged(pTemp[playerid][pMDCPlayer]))
		return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127)
	{
		new message[256];
		format(message, sizeof(message), "{B4B5B7}PRÉSENTATION D'ACCUSATION\n\nAccusé : %s\n\nEntrez le texte de l'accusation : (( Infraction / 128 caractères ))", GetNameEx(pTemp[playerid][pMDCPlayer]));
		Dialog_Show(playerid, MDCRecord2, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Présentation d'accusation", message, "Entrer", "Annuler");
		return 1;
	}

	AddCriminalRecord(pTemp[playerid][pMDCPlayer], inputtext, PlayerInfo[playerid][pUsername], PlayerInfo[playerid][pFaction]);
	SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "[WANTED] Accusé : %s, Accusateur : %s, Infraction : %s.", GetNameEx(pTemp[playerid][pMDCPlayer]), GetNameEx(playerid), inputtext);
	SendWarningMessage(playerid, "L'accusé a été ajouté avec succès à la liste des recherchés.");
	pTemp[playerid][pMDCPlayer] = INVALID_PLAYER_ID;
	return 1;
}

Dialog:MDCAddWarn(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new userid;

	/*
	if (sscanf(inputtext, "u", userid))
		return Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Avertissement sur permis", "{B4B5B7}AJOUTER UN AVERTISSEMENT SUR PERMIS\n\nEntrez un nom :\n\n(( Prénom_Nom / ID joueur ))\n"EMBED_RED"Cette personne est absente de la base de données.", "Entrer", "Annuler");

	if (!IsPlayerLogged(userid))
		return Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Avertissement sur permis", "{B4B5B7}AJOUTER UN AVERTISSEMENT SUR PERMIS\n\nEntrez un nom :\n\n(( Prénom_Nom / ID joueur ))\n"EMBED_RED"Cette personne est absente de la base de données.", "Entrer", "Annuler");

	if (!PlayerInfo[userid][pCarLic])
		return Dialog_Show(playerid, MDCAddWarn, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Avertissement sur permis", "{B4B5B7}AJOUTER UN AVERTISSEMENT SUR PERMIS\n\nEntrez un nom :\n\n(( Prénom_Nom / ID joueur ))\n"EMBED_RED"Cette personne ne possède pas de permis de conduire.", "Entrer", "Annuler");
	*/
	new message[128];
	format(message, sizeof(message), "a émis un avertissement sur le permis de conduire de %s.", GetNameEx(userid));
	cmd::me(playerid, message);

	SendSyntaxMessage(playerid, "Vous avez ajouté un avertissement sur le permis de conduire de %s.", GetName(userid));

	PlayerInfo[userid][pDriveWarns]++;
	SQL_SetInteger("users", "drivewarns", PlayerInfo[userid][pDriveWarns], PlayerInfo[userid][pID]);

	if (PlayerInfo[userid][pDriveWarns] >= 3)
	{
		SendSyntaxMessage(playerid, "Étant donné que le permis de conduire de %s avait 3 avertissements, il a été automatiquement révoqué.", GetNameEx(userid));
		PlayerInfo[userid][pDriveWarns] = 0;
		PlayerInfo[userid][pCarLic] = 0;

		SQL_SetInteger("users", "drivewarns", PlayerInfo[userid][pDriveWarns], PlayerInfo[userid][pID]);
		SQL_SetInteger("users", "carlic", PlayerInfo[userid][pCarLic], PlayerInfo[userid][pID]);
	}
	return 1;
}

/*Dialog:MDCVehicle(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	if (!strlen(inputtext) || strlen(inputtext) > 23)
		return Dialog_Show(playerid, MDCVehicle, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Recherche de véhicule", "{B4B5B7}RECHERCHE D'INFORMATIONS SUR UN VÉHICULE\n\nEntrez une plaque d'immatriculation :\n\n(( Plaque ))", "Entrer", "Retour");

	new query[156];
	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `plate` = '%e' AND `registered`=1", inputtext);
	mysql_tquery(dbHandle, query, "OnCheckNumber", "d", playerid);
	return 1;
}*/

Dialog:MDCWeaponPlayer(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list));
		
	if (listitem == 5)
	{
		if (IsPlayerFactionPolice(playerid)) return SendErrorMessage(playerid, "L'annulation est gérée uniquement par le LSPD, veuillez envoyer une demande.");
		if (PlayerInfo[playerid][pAuthwep] != 1) return SendErrorMessage(playerid, "Vous n'avez pas les autorisations pour annuler une licence d'arme.");
			
		new userid;

		if ((userid = GetIDByID(pTemp[playerid][pMDCWepPlayer])) != INVALID_PLAYER_ID)
		{
			SendSyntaxMessage(userid, "%s %s a annulé votre licence d'arme.", GetRankName(playerid), GetNameEx(playerid));
			PlayerInfo[userid][pWepLic] = 0;
			PlayerInfo[userid][pWepWarns] = 0;
			PlayerInfo[userid][pWepCount] = 0;
		}
		new query[156];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `weplic`='0',`wepwarns`='0',`wep_officer`='-',`wep_data`='-',`wep_price`='0',`wep_count`='0' WHERE `id` = %i", pTemp[playerid][pMDCWepPlayer]);
		mysql_tquery(dbHandle, query);
		
		SendSyntaxMessage(playerid, "Vous avez annulé la licence d'arme de %s.", pTemp[playerid][pMDCWepName]);
		return 1;
	}

	else if (listitem == 6)
	{
		new userid;
		if ((userid = GetIDByID(pTemp[playerid][pMDCWepPlayer])) != INVALID_PLAYER_ID)
		{
			SendSyntaxMessage(userid, "%s %s a émis un avertissement sur votre licence d'arme.", GetRankName(playerid), GetNameEx(playerid));
			PlayerInfo[userid][pWepWarns]++;
		}
		
		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `wepwarns`= `wepwarns` + 1 WHERE `id` = %i", pTemp[playerid][pMDCWepPlayer]);
		mysql_tquery(dbHandle, query);

		SendSyntaxMessage(playerid, "Vous avez émis un avertissement sur la licence d'arme de %s.", pTemp[playerid][pMDCWepName]);
		return 1;
	}
	
	ShowWeaponLicense(playerid, GetPVarInt(playerid, #weapon_list));
	return 1;
}

Dialog:ShowTrace(playerid, response, listitem, inputtext[])
{
	if (pTemp[playerid][pTraceStage] < 7)
	{
		Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{33AA33}Système de traçage", "{B4B5B7}Vous avez quitté le système de traçage.", "Accepter", "");
		
		if (pTemp[playerid][pTraceGZ] != -1) GangZoneDestroy(pTemp[playerid][pTraceGZ]);
		
		pTemp[playerid][pTraceGZ] = -1;
		pTemp[playerid][pTracePlayer] = INVALID_PLAYER_ID;
		pTemp[playerid][pTraceStage] = 0;
	}
	return 1;
}

Dialog:LSPDGUN(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::duty(playerid, "");
	if (!IsPlayerFactionPolice(playerid)) return 1;
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez d'abord être en service.");
	if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid)) return SendErrorMessage(playerid, "Vous n'êtes pas dans le vestiaire ou dans un véhicule de service.");
	
	GameTextForPlayer(playerid, "~r~-", 1500, 4);

	switch (listitem)
	{
		case 0: if (!GivePlayerWeaponEx(playerid, 3, 1, GetDefaultAmmoType(3), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 1: if (!GivePlayerWeaponEx(playerid, 41, 999, GetDefaultAmmoType(41), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 2: if (!GivePlayerWeaponEx(playerid, 24, 85, GetDefaultAmmoType(24), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 3: if (GiveInvItem(playerid, 85, CheckAmmo(85)) == -1) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 4: if (!GivePlayerWeaponEx(playerid, 29, 91, GetDefaultAmmoType(29), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 5: if (GiveInvItem(playerid, 91, CheckAmmo(91)) == -1) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 6: if (!GivePlayerWeaponEx(playerid, 25, 89, GetDefaultAmmoType(25), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 7: if (GiveInvItem(playerid, 86, CheckAmmo(86)) == -1) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 8: if (GiveInvItem(playerid, 89, CheckAmmo(89)) == -1) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 9: if (!GivePlayerWeaponEx(playerid, 31, 98, GetDefaultAmmoType(31), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 10: if (GiveInvItem(playerid, 98, CheckAmmo(98)) == -1) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 11: if (GiveInvItem(playerid, 99, CheckAmmo(99)) == -1) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 12: if (!GivePlayerWeaponEx(playerid, 34, 96, GetDefaultAmmoType(34), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 13: if (GiveInvItem(playerid, 96, CheckAmmo(96)) == -1) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 14: if (!GivePlayerWeaponEx(playerid, 17, 1, GetDefaultAmmoType(17), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 15: if (!GivePlayerWeaponEx(playerid, 43, 50, GetDefaultAmmoType(43), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
		case 16: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~g~+", 1500, 4);

	Dialog_Show(playerid, LSPDGUN, DIALOG_STYLE_LIST, "Armes", \
		"{FFFFFF}• Matraque\n• Spray au poivre\n• Desert Eagle ( .50AE .ext )\n\t{ffc46a}- Prendre munitions supplémentaires ( .50AE .ext )\n"\
		"{FFFFFF}• MP5 ( 9x19mm .ext )\n\t{ffc46a}- Prendre munitions supplémentaires ( 9x19mm .ext )\n"\
		"{FFFFFF}• Fusil à pompe ( 12-G BIRDSHOT )\n\t{ffc46a}- Prendre munitions supplémentaires ( 12-G BIRDSHOT )\n"\
		"\t{ffc46a}- Prendre munitions non létales ( 12-G BEANBAG )\n"\
		"{FFFFFF}• M4 ( 5.56x45mm (drum) )\n\t{ffc46a}- Prendre munitions supplémentaires ( 5.56x45mm (drum) )\n"\
		"\t{ffc46a}- Prendre munitions non létales ( 5.56x45mm (rubber) )\n"\
		"{FFFFFF}• Fusil de sniper (7.62x51mm)\n\t{ffc46a}- Prendre munitions supplémentaires ( 7.62x51mm )\n"\
		"{FFFFFF}• Grenade lacrymogène\n• Appareil photo\n{FF6347}• Rendre toutes les armes",\
		"Choisir", "Fermer");
	return 1;
}

Dialog:LSPDArmoury(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::duty(playerid, "");
	if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid)) 
		return SendErrorMessage(playerid, "Vous n'êtes pas dans le vestiaire ou dans un véhicule de service.");

	switch (listitem) 
	{
		case 0: 
		{
			GameTextForPlayer(playerid, "~r~-", 1500, 4);
			if (GiveInvItem(playerid, INV_RADIO, 1) == -1) 
				return SendErrorMessage(playerid, "Pas de place dans l'inventaire !");
			GameTextForPlayer(playerid, "~g~+", 1500, 4);
		}
		case 1: 
		{
			return Dialog_Show(playerid, LSPDGUN, DIALOG_STYLE_LIST, "Armes", 
				"{FFFFFF}• Matraque\n• Spray au poivre\n• Desert Eagle ( .50AE .ext )\n\t{ffc46a}- Prendre munitions supplémentaires ( .50AE .ext )\n"\
				"{FFFFFF}• MP5 ( 9x19mm .ext )\n\t{ffc46a}- Prendre munitions supplémentaires ( 9x19mm .ext )\n"\
				"{FFFFFF}• Fusil à pompe ( 12-G BIRDSHOT )\n\t{ffc46a}- Prendre munitions supplémentaires ( 12-G BIRDSHOT )\n"\
				"\t{ffc46a}- Prendre munitions non létales ( 12-G BEANBAG )\n"\
				"{FFFFFF}• M4 ( 5.56x45mm (drum) )\n\t{ffc46a}- Prendre munitions supplémentaires ( 5.56x45mm (drum) )\n"\
				"\t{ffc46a}- Prendre munitions non létales ( 5.56x45mm (rubber) )\n"\
				"{FFFFFF}• Fusil de sniper (7.62x51mm)\n\t{ffc46a}- Prendre munitions supplémentaires ( 7.62x51mm )\n"\
				"{FFFFFF}• Grenade lacrymogène\n• Appareil photo\n{FF6347}• Rendre toutes les armes", 
				"Choisir", "Fermer");
		}
		case 2: 
		{
			if (!PlayerInfo[playerid][pSwat] && !isLeader(playerid)) 
				return SendErrorMessage(playerid, "Vous n'avez pas accès à l'équipement S.W.A.T. !");

			if (!PlayerInfo[playerid][pSwatDuty])
			{
				PlayerInfo[playerid][pSwatDuty] = 1;
				SetPlayerArmour(playerid, 150);
				PlayerInfo[playerid][pChar] = (!IsPlayerFactionPolice(playerid)) ? (287) : (285);
				SetPlayerSkin(playerid, PlayerInfo[playerid][pChar]);
				SetPlayerColor(playerid, RemoveAlpha(Factions[PlayerInfo[playerid][pFaction]][fColor]));
				SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** HQ : %s %s s'est autorisé dans l'unité S.W.A.T. **", GetRankName(playerid), GetNameEx(playerid));
				return 1;
			}

			PlayerInfo[playerid][pSwatDuty] = 0;
			SetPlayerArmour(playerid, 100);
			PlayerInfo[playerid][pChar] = 0;
			SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
			SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** HQ : %s %s s'est désautorisé de l'unité S.W.A.T. **", GetRankName(playerid), GetNameEx(playerid));
		}
	}
	return 1;
}