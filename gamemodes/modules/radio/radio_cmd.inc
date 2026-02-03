ALTX:channel("/ch");
CMD:channel(playerid, params[])
{
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendSyntaxMessage(playerid, "/(ch)annel [set / leave / rent / sell]");
		return 1;
	}

	if (CompareStrings(option, "set"))
	{
		if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Vous n'avez pas de radio.");
		
		new channelid;

		if (sscanf(secoption, "i", channelid))						return SendHelpMessage(playerid, "(/ch)annel set [canal]");
		if (!ChannelExists(channelid))								return SendErrorMessage(playerid, "Canal introuvable.");
		if (channelid == 911 && !IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Canal réservé.");
		if (channelid == 912 && !IsPlayerFactionMedic(playerid))		return SendErrorMessage(playerid, "Canal réservé.");
		if (channelid == 998 && !IsPlayerFactionFire(playerid))		return SendErrorMessage(playerid, "Canal réservé.");
		if (channelid == GetPlayerChannel(playerid))					return SendErrorMessage(playerid, "Vous êtes déjà connecté à ce canal.");
		
		if (channelid == 911 && IsPlayerFactionPolice(playerid))
		{
			SendRadioMsg(playerid, "Canal changé [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		else if (channelid == 912 && IsPlayerFactionMedic(playerid))
		{
			SendRadioMsg(playerid, "Canal changé [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		else if (channelid == 998 && IsPlayerFactionFire(playerid))
		{
			SendRadioMsg(playerid, "Canal changé [CH: %i].", channelid);
			PlayerInfo[playerid][pAuthed] = 1;
			SetRadioChannel(playerid, channelid);
			return 1;
		}

		PlayerInfo[playerid][pAuthed] = 0;
		SetRadioChannel(playerid, channelid);
		SendRadioMsg(playerid, "Canal changé [CH: %i].", channelid);
		SendHelpMessage(playerid, "Utilisez /auth pour vous authentifier.");
		return 1;
	}

	else if (CompareStrings(option, "leave"))
	{
		if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Vous n'avez pas de radio.");
		if (!GetPlayerChannel(playerid))				return SendErrorMessage(playerid, "Vous n'êtes connecté à aucun canal.");

		SendRadioMsg(playerid, "Vous avez quitté le canal [CH: %i].", GetPlayerChannel(playerid));
		SetRadioChannel(playerid, 0);
		PlaySound(playerid, 6400);
		return 1;
	}

	else if (CompareStrings(option, "rent"))
	{
		if (!IsPlayerInRangeOfPoint(playerid, 2.0, 2269.65, -74.306, 26.772))
		{
			Waypoint_Set(playerid, 2269.65, -74.306, 26.772);
			SendWarningMessage(playerid, "Vous devez être près de la bibliothèque municipale.");
			return 1;
		}

		new channel, pass[33];

		if (sscanf(secoption, "iS()[33]", channel, pass))	return SendHelpMessage(playerid, "(/ch)annel rent [canal (1-9999)] [mot de passe | laissez vide si vous ne voulez pas définir de mot de passe]");
		if (strlen(pass) > 30)								return SendErrorMessage(playerid, "Mot de passe jusqu'à 30 caractères.");
		if (channel >= MAX_RADIOS || channel < 1)			return SendErrorMessage(playerid, "Canal entre 1 et %i.", MAX_RADIOS - 1);
		if (PlayerInfo[playerid][pOwnedChannel])				return SendErrorMessage(playerid, "Vous avez déjà un canal, vendez-le d'abord.");
		if (ChannelExists(channel))							return SendErrorMessage(playerid, "Canal déjà occupé.");
		if (StringHasSymbols(pass))							return SendErrorMessage(playerid, "Le mot de passe ne peut contenir que des lettres et des chiffres.");
		
		new cost = 1500;

		if (GetCash(playerid) < cost)						return SendNotCash(playerid);

		new id = -1;

		for(new i = 0; i < MAX_RADIOS; i++)
		{
			if (RadiosInfo[i][radioOn]) continue;

			id = i;
			break;
		}

		if (id == -1)	return SendErrorMessage(playerid, "Toutes les fréquences sont occupées, revenez plus tard.");

		GiveCash(playerid, -cost);
		SendRadioMsg(playerid, "Vous avez loué le canal [CH: %i] pour $%i.", channel, cost);
		SendHelpMessage(playerid, "Utilisez /radiohelp pour obtenir de l'aide.");
		RadiosInfo[id][radioChannel] = channel;
		RadiosInfo[id][radioOwner] = PlayerInfo[playerid][pID];
		format(RadiosInfo[id][radioPass], 32, "%s", pass);
		AddChannelToFile(playerid, channel, pass, id);
		return 1;
	}

	else if (CompareStrings(option, "sell"))
	{
		if (!PlayerInfo[playerid][pOwnedChannel])	return SendErrorMessage(playerid, "Vous ne louez pas de canal.");
		
		new id = -1;

		if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1)
		{
			PlayerInfo[playerid][pOwnedChannel] = 0;
			SendWarningMessage(playerid, "Canal introuvable.");
			return 1;
		}

		new award = 750;

		GiveCash(playerid, award);
		SendRadioMsg(playerid, "Vous avez vendu le canal [CH: %i] pour $%i.", RadiosInfo[id][radioChannel], award);

		foreach(new i : Player)
		{
			if (!IsPlayerLogged(i) || GetPlayerChannel(i) != RadiosInfo[id][radioChannel] || i == playerid) continue;

			SendRadioMsg(i, "Le canal auquel vous êtes connecté a été vendu.");
			SetRadioChannel(i, 0);
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `channel` = 0, `authed` = 0 WHERE `channel` = %i", RadiosInfo[id][radioChannel]);
		mysql_tquery(dbHandle, query);
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `radios` WHERE `id` = %i", RadiosInfo[id][radioID]);
		mysql_tquery(dbHandle, query);

		RadiosInfo[id][radioOn] = 
		RadiosInfo[id][radioID] = 
		RadiosInfo[id][radioChannel] = 
		RadiosInfo[id][radioOwner] = 0;
		SetRadioChannel(playerid, 0);
		PlayerInfo[playerid][pOwnedChannel] = 0;
		return 1;
	}

	else 
	{
		SendHelpMessage(playerid, "/(ch)annel [set / leave / rent / sell]");
	}
	return 1;
}

CMD:setslot(playerid, params[])
{
	if (!IsPlayerHasItem(playerid, INV_RADIO)) return SendErrorMessage(playerid, "Vous n'avez pas de radio.");

	new slot;

	if (sscanf(params, "d", slot)) return SendHelpMessage(playerid, "/setslot [slot (0-100)]");
	if (slot < 0 || slot > 100) return SendErrorMessage(playerid, "Le slot doit être compris entre 0 et 100.");
	if (slot == GetPlayerSlot(playerid)) return SendErrorMessage(playerid, "Vous utilisez déjà ce slot.");

	SetRadioSlot(playerid, slot);
	SendRadioMsg(playerid, "Fréquence remplacée [SLOT: %i]", slot);
	return 1;
}

CMD:auth(playerid, params[])
{
	if (!IsPlayerHasItem(playerid, INV_RADIO)) return SendErrorMessage(playerid, "Vous n'avez pas de radio.");
	if (!GetPlayerChannel(playerid)) return SendErrorMessage(playerid, "Votre radio est éteinte.");
	if (PlayerInfo[playerid][pAuthed]) return SendErrorRadioMessage(playerid, "Vous êtes déjà authentifié sur ce canal.");

	new id = -1;

	if ((id = ChannelExists(GetPlayerChannel(playerid))) == 0) return SendErrorMessage(playerid, "Canal introuvable.");

	if (CompareStrings(RadiosInfo[id][radioPass], ""))
	{
		SendRadioMsg(playerid, "Vous vous êtes authentifié sur le canal %i.", GetPlayerChannel(playerid));
		PlayerInfo[playerid][pAuthed] = 1;
		PlaySound(playerid, 6400);
		return 1;
	}

	if (isnull(params)) return SendHelpMessage(playerid, "/auth [mot de passe du canal]");
	if (CompareStrings(RadiosInfo[id][radioPass], params)) return SendErrorRadioMessage(playerid, "Mot de passe incorrect.");

	SendRadioMsg(playerid, "Vous vous êtes authentifié sur le canal [CH: %i].", GetPlayerChannel(playerid));
	PlayerInfo[playerid][pAuthed] = 1;
	PlaySound(playerid, 6400);
	return 1;
}

CMD:newauth(playerid, params[])
{
	if (!PlayerInfo[playerid][pOwnedChannel]) return SendErrorMessage(playerid, "Vous ne louez pas de canal.");

	new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1) return SendErrorMessage(playerid, "Canal introuvable."); 
	if (isnull(params)) return SendHelpMessage(playerid, "/newauth [nouveau mot de passe | laissez vide pour supprimer le mot de passe]");
	if (strlen(params) < 1 || strlen(params) > 32) return SendErrorMessage(playerid, "Le mot de passe doit contenir entre 1 et 32 caractères.");
	if (CompareStrings(RadiosInfo[id][radioPass], "") && CompareStrings(params, "")) return SendErrorMessage(playerid, "Votre canal n'a pas de mot de passe.");
	if (!CompareStrings(params, "") && StringHasSymbols(params)) return SendErrorMessage(playerid, "Le mot de passe ne peut contenir que des lettres et des chiffres.");

	if (CompareStrings(params, "")) 
		SendRadioMsg(playerid, "Votre canal n'a désormais plus de mot de passe.");
	else 
		SendRadioMsg(playerid, "Vous avez changé le mot de passe de votre canal en %s.", params);

	format(RadiosInfo[id][radioPass], 33, "%s", params);
	SQL_SetString("radios", "pass", RadiosInfo[id][radioPass], RadiosInfo[id][radioID]);
	return 1;
}

CMD:radiomembers(playerid, params[])
{
	if (!PlayerInfo[playerid][pOwnedChannel]) return SendErrorMessage(playerid, "Vous ne louez pas de canal.");

	new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1) return SendErrorMessage(playerid, "Canal introuvable.");

	new dialog_header[64] = "{FFFFFF}Nom\t{FFFFFF}ID\n", dialog_body[2048];

	format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || GetPlayerChannel(i) != RadiosInfo[id][radioChannel]) continue;

		format(dialog_body, sizeof(dialog_body), "%s%s\t%i\n", dialog_body, GetNameEx(i), i);
	}

	Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Liste des joueurs dans le canal", dialog_body, "Fermer", "");
	return 1;
}

CMD:kickoffradio(playerid, params[])
{
	if (!PlayerInfo[playerid][pOwnedChannel]) return SendErrorMessage(playerid, "Vous ne louez pas de canal.");

	new userid;

	if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/kickoffradio [ID/nom du joueur]");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (userid == playerid) return SendErrorMessage(playerid, "Vous ne pouvez pas vous exclure du canal.");

	new id = -1;

	if ((id = GetChannelID(PlayerInfo[playerid][pOwnedChannel])) == -1) return SendErrorMessage(playerid, "Canal introuvable.");
	if (RadiosInfo[id][radioChannel] != GetPlayerChannel(playerid)) return SendErrorMessage(playerid, "Vous devez être sur votre canal [CH: %i].", RadiosInfo[id][radioChannel]);
	if (GetPlayerChannel(userid) != GetPlayerChannel(playerid)) return SendErrorMessage(playerid, "Ce joueur n'est pas sur ce canal.");

	SendRadioMsg(playerid, "Vous avez exclu %s du canal.", GetNameEx(userid));
	SendRadioMsg(userid, "Vous avez été exclu du canal.");
	SetRadioChannel(userid, 0);
	return 1;
}

ALTX:r("/radio");
CMD:r(playerid, params[])
{
	if (PlayerInfo[playerid][pMutedTime] > 0) 
		return SendErrorMessage(playerid, "Votre chat local est bloqué par le système.");

	new channel = GetPlayerChannel(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO)) 
		return SendErrorMessage(playerid, "Vous n'avez pas de radio.");
	if (channel == 0) 
		return SendErrorMessage(playerid, "Vous n'êtes pas sur une fréquence.");
	if (!PlayerInfo[playerid][pAuthed]) 
		return SendErrorMessage(playerid, "Vous n'êtes pas authentifié sur cette fréquence.");

	if (channel == 911) {
		new tag[6], arg[25];
		if (!sscanf(params, "s[6]s[24]", tag, arg)) { 
			if (CompareStrings(tag, "10-27")) {
				new userid;
				if (sscanf(arg, "u", userid)) 
					return SendErrorMessage(playerid, "/r 10-27 [ID/nom du joueur]");
				if (!IsPlayerLogged(userid)) 
					return SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ : Rien trouvé pour cette requête.");

				SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ : %s, sexe : %s, âge : %i, amendes impayées : %s, recherche : %s.", GetNameEx(userid), (PlayerInfo[userid][pSex] == 1) ? ("homme") : ("femme"), GetAge(userid), (CountPlayerTickets(userid)) ? ("{FF6347}%i{9189EF}") : ("aucune"), (IsPlayerWanted(userid)) ? ("OUI") : ("non"));
				SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit : Dispatcheur, 10-27 %s.", GetNameWithMask(playerid), GetNameEx(userid));
				return 1;
			} else if (CompareStrings(tag, "10-28")) {
				new plates[10], vehicle;
				if (sscanf(arg, "s[10]", plates)) 
					return SendErrorMessage(playerid, "/r 10-28 [numéro d'immatriculation]");
				if ((vehicle = FindVehicleByPlate(plates)) == -1) 
					return SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ : Rien trouvé pour cette requête.");

				PlayCrimeReportForPlayer(playerid, playerid, 13);

				SendClientMessageEx(playerid, COLOR_POLICECHAT, "* HQ : %s [%s], propriétaire : %s, amendes impayées : %s, recherche : %s.", VehicleNames[GetVehicleModel(vehicle) - 400], VehicleInfo[vehicle][carPlate], VehicleInfo[vehicle][carOwner], (CountVehicleTickets(vehicle)) ? ("%i") : ("aucune"), (VehicleInfo[vehicle][carWanted]) ? ("OUI") : ("non"));
				SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit : Dispatcheur, 10-28 %s.", GetNameWithMask(playerid), VehicleInfo[vehicle][carPlate]);
				return 1;
			}
		}
	}

	if (sscanf(params, "s[144]", params)) 
		return SendHelpMessage(playerid, "/r [texte]");

	new mes[128];

	if (strlen(params) > MAXLEN) {
		new pos = MAXLEN;

		if (pos < MAXLEN - 1) 
			pos = MAXLEN;

		if (pTemp[playerid][pPatrol] && channel == 911) {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s : %.*s ...", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), pos, params);
		} else {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s : %.*s ...", channel, GetNameEx(playerid), pos, params);
		}

		SendRadioMessage(channel, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit : %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "... %s", params[pos]);
	} else {
		if (pTemp[playerid][pPatrol] && channel == 911) {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s : %s", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), params);
		} else {
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s : %s", channel, GetNameEx(playerid), params);
		}

		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit : %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

CMD:rr(playerid, params[])
{
	if (isnull(params)) 
		return SendHelpMessage(playerid, "/rr [texte]");
		
	if (PlayerInfo[playerid][pMutedTime] > 0) 
		return SendErrorMessage(playerid, "Votre chat local est bloqué par le système.");

	new channel = GetPlayerChannel(playerid);
	new slot = GetPlayerSlot(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO)) 
		return SendErrorMessage(playerid, "Vous n'avez pas de radio.");
		
	if (channel == 0) 
		return SendErrorRadioMessage(playerid, "Vous n'êtes pas sur une fréquence.");
		
	if (PlayerInfo[playerid][pAuthed] == 0) 
		return SendErrorRadioMessage(playerid, "Vous n'êtes pas authentifié sur cette fréquence.");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		if (pos < MAXLEN - 1) pos = MAXLEN;

		if (pTemp[playerid][pPatrol])
			format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s : %.*s ...", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), pos, params);
		else
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s : %.*s ...", channel, slot, GetNameEx(playerid), pos, params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);

		SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit : %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
		if (pTemp[playerid][pPatrol])
			format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s : %s", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), params);
		else
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s : %s", channel, slot, GetNameEx(playerid), params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(5.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit : %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

ALTX:rlow("/rl");
CMD:rlow(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rlow [texte]");
	if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Votre chat local est bloqué par le système.");

	new channel = GetPlayerChannel(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Vous n'avez pas de radio.");
	if (!channel)								return SendErrorMessage(playerid, "Vous n'êtes pas sur une fréquence.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorMessage(playerid, "Vous n'êtes pas authentifié sur cette fréquence.");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN - 1) pos = MAXLEN;
		
		if (pTemp[playerid][pPatrol])	
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s [faible]: %.*s ...", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), pos, params);
		else							
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s [faible]: %.*s ...", channel, GetNameEx(playerid), pos, params);

		SendRadioMessage(channel, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit [faible]: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(2.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
		if (pTemp[playerid][pPatrol])	
			format(mes, sizeof(mes), "**[CH: %i, S: 0] [%s] %s [faible]: %s", channel, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), params);
		else							
			format(mes, sizeof(mes), "**[CH: %i, S: 0] %s [faible]: %s", channel, GetNameEx(playerid), params);

		SendRadioMessage(channel, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit [faible]: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}

ALTX:rrlow("/rrl");
CMD:rrlow(playerid, params[])
{
	if (isnull(params))							return SendHelpMessage(playerid, "/rrlow [texte]");
	if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Votre chat local est bloqué par le système.");

	new channel = GetPlayerChannel(playerid), slot = GetPlayerSlot(playerid);

	if (!IsPlayerHasItem(playerid, INV_RADIO))	return SendErrorMessage(playerid, "Vous n'avez pas de radio.");
	if (!channel)								return SendErrorRadioMessage(playerid, "Vous n'êtes pas sur une fréquence.");
	if (!PlayerInfo[playerid][pAuthed])			return SendErrorRadioMessage(playerid, "Vous n'êtes pas authentifié sur cette fréquence.");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN - 1) pos = MAXLEN;

		if (pTemp[playerid][pPatrol])	
			format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s [faible]: %.*s ...", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), pos, params);
		else							
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s [faible]: %.*s ...", channel, slot, GetNameEx(playerid), pos, params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		format(mes, sizeof(mes), "... %s", params[pos]);
		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit [faible]: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(2.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "... %s", params[pos]);
	}
	else
	{
		if (pTemp[playerid][pPatrol])	
			format(mes, sizeof(mes), "**[CH: %i, S: %i] [%s] %s [faible]: %s", channel, slot, PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], GetNameEx(playerid), params);
		else							
			format(mes, sizeof(mes), "**[CH: %i, S: %i] %s [faible]: %s", channel, slot, GetNameEx(playerid), params);

		SendSlotRadioMessage(channel, slot, COLOR_RADIO, mes);
		SendLocalMessage(2.0, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "(Radio) %s dit [faible]: %s", GetNameWithMask(playerid), params);
	}
	return 1;
}