CMD:editspawn(playerid,params[])
{
	new factionid = PlayerInfo[playerid][pFaction];

	if (!isLeader(playerid) && !GetSpawnRight(playerid)) 
		return SendErrorMessage(playerid, "Vous n'avez pas les droits pour modifier la position de spawn de la faction.");

	GetPlayerPos(playerid, Factions[factionid][fPosX], Factions[factionid][fPosY], Factions[factionid][fPosZ]);
	Factions[factionid][fSpawnInt] = GetInterior(playerid);
	Factions[factionid][fSpawnVW] = GetWorld(playerid);

	new query[128 + 56];
	mysql_format(dbHandle, query, sizeof(query), 
		"UPDATE `factions` SET `spawnx`=%f, `spawny`=%f, `spawnz`=%f, `spawnint`=%i, `spawnvw`=%i WHERE `id`=%i", 
		Factions[factionid][fPosX], Factions[factionid][fPosY], Factions[factionid][fPosZ], 
		Factions[factionid][fSpawnInt], Factions[factionid][fSpawnVW], Factions[factionid][fID]);
	mysql_tquery(dbHandle, query);

	SendSyntaxMessage(playerid, "Vous avez défini la nouvelle position de spawn pour la faction %s.", Factions[factionid][fName]);
	return 1;
}

CMD:editrankname(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], rank, name[32];

	if (!isLeader(playerid) && !GetRankNameRight(playerid))		
		return SendErrorMessage(playerid, "Vous n'avez pas les droits pour modifier les noms de rangs.");
	if (sscanf(params,"ds[31]", rank, name))						
		return SendHelpMessage(playerid, "/editrankname [rang] [nom]");
	if (strlen(name) > 31)										
		return SendErrorMessage(playerid, "Le nom du rang ne peut pas dépasser 31 caractères.");
	if (rank < 1 || rank > Factions[faction][fRanks])			
		return SendErrorMessage(playerid, "Le rang doit être entre 1 et %i.", Factions[faction][fRanks]);
	
	format(FactionRanks[faction][rank-1], 32, "%s", name);
	UpdateFactionRank(faction, rank);
	SendSyntaxMessage(playerid, "Vous avez modifié le nom du rang [ID: %i] en '%s'.", rank, name);
	return 1;
}

CMD:fchat(playerid, params[])
{
	if (PlayerInfo[playerid][pFaction] == 0)				return SendErrorMessage(playerid, "Vous devez faire partie d'une faction.");
    if (!isLeader(playerid) && !GetChatRight(playerid))	return SendErrorMessage(playerid, "Vous n'êtes pas le leader de votre faction.");

	new faction = PlayerInfo[playerid][pFaction];
	if (Factions[faction][fChatON] == 1)
	{
		SendFactionMessageF(faction, COLOR_LIGHTBLUE, " ** %s a désactivé le chat faction.", GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Vous avez désactivé le chat OOC faction. Désormais, seul vous pourrez écrire.");
		Factions[faction][fChatON] = 0;
	}
	else
	{
		SendFactionMessageF(faction, COLOR_LIGHTBLUE, " ** %s a activé le chat faction.", GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Vous avez activé le chat OOC faction. Tous les membres peuvent maintenant écrire.");
		Factions[faction][fChatON] = 1;
	}
	SQL_SetInteger("factions", "chaton", Factions[faction][fChatON], faction);
	return 1;
}

CMD:invite(playerid, params[])
{
    if (PlayerInfo[playerid][pFaction] == 0)						return SendErrorMessage(playerid, "Vous devez faire partie d'une faction.");
    if (!isLeader(playerid) && !GetInviteRight(playerid))			return SendErrorMessage(playerid, "Vous n'avez pas le droit d'inviter dans la faction.");
    if (Factions[PlayerInfo[playerid][pFaction]][fJoinRank] == 0)	return SendErrorMessage(playerid, "Veuillez configurer les grades et salaires dans votre faction.");

	new userid;

	if (sscanf(params, "u", userid))									return SendHelpMessage(playerid, "/invite [ID/nom du joueur]");
	if (!IsPlayerLogged(userid))										return SendNotConnected(playerid);
	if (userid == playerid)												return SendErrorMessage(playerid, "Vous êtes déjà dans une faction.");
    if (PlayerInfo[userid][pFaction] != 0)								return SendErrorMessage(playerid, "Ce joueur est déjà dans une faction.");
    if (pTemp[userid][pFactionPlayer] == playerid)						return SendErrorMessage(playerid, "Vous avez déjà proposé à ce joueur de rejoindre votre faction.");

	pTemp[userid][pFactionPlayer] = playerid;
	SetPVarInt(userid, #accept_time, gettime() + 30);

	SendSyntaxMessage(userid, "%s vous a proposé de rejoindre %s (Appuyez sur Y pour accepter)", GetNameWithMask(playerid), Factions[PlayerInfo[playerid][pFaction]][fName]);
	SendSyntaxMessage(playerid, "Vous avez proposé à %s de rejoindre votre faction.", GetNameWithMask(userid));
	return 1;
}

CMD:uninvite(playerid, params[])
{
    if (PlayerInfo[playerid][pFaction] == 0)							return SendErrorMessage(playerid, "Vous devez faire partie d'une faction.");
    if (!isLeader(playerid) && !GetUninviteRight(playerid))				return SendErrorMessage(playerid, "Vous n'avez pas les droits pour exclure un membre.");
    if (Factions[PlayerInfo[playerid][pFaction]][fJoinRank] == 0)		return SendErrorMessage(playerid, "Veuillez configurer les grades et salaires dans votre faction.");

	new userid;

	if (sscanf(params, "u", userid))									return SendHelpMessage(playerid, "/uninvite [ID/nom du joueur]");
	if (!IsPlayerLogged(userid))										return SendNotConnected(playerid);
	if (userid == playerid)												return SendErrorMessage(playerid, "Vous ne pouvez pas vous exclure vous-même.");
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "Ce joueur n'est pas dans votre faction.");

	if (!isLeader(playerid) && GetUninviteRight(playerid) && (isLeader(userid) || PlayerInfo[userid][pRank] >= PlayerInfo[playerid][pRank]))
		return SendErrorMessage(playerid, "Vous pouvez uniquement exclure un joueur ayant un grade inférieur au vôtre.");

	SendSyntaxMessage(userid, "%s vous a exclu de la faction %s.", GetNameEx(playerid), Factions[PlayerInfo[playerid][pFaction]][fName]);
	SendSyntaxMessage(playerid, "Vous avez exclu %s de votre faction.", GetNameEx(userid));

    RemovePlayerFaction(userid);
	SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_LIGHTBLUE, "[Faction] %s a été exclu par le leader %s.", GetNameEx(userid), GetNameEx(playerid));
	return 1;
}

CMD:giverank(playerid, params[])
{
    if (PlayerInfo[playerid][pFaction] == 0)									return SendErrorMessage(playerid, "Vous devez être dans une faction.");
    if (!isLeader(playerid) && !GetRankRight(playerid))							return SendErrorMessage(playerid, "Vous n'avez pas les droits pour modifier les grades des membres.");
    if (Factions[PlayerInfo[playerid][pFaction]][fJoinRank] == 0)				return SendErrorMessage(playerid, "Veuillez d'abord configurer les grades de votre faction.");

	new userid, rank;

	if (sscanf(params, "ui", userid, rank))										return SendHelpMessage(playerid, "/giverank [ID/nom du joueur] [grade]");
	if (!IsPlayerLogged(userid))												return SendNotConnected(playerid);
	if (userid == playerid)														return SendErrorMessage(playerid, "Vous ne pouvez pas modifier votre propre grade.");
    if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction])			return SendErrorMessage(playerid, "Ce joueur n'est pas dans votre faction.");
	if (rank < 1 || rank > Factions[PlayerInfo[playerid][pFaction]][fRanks])	return SendErrorMessage(playerid, "Grade entre 1 et %i.", Factions[PlayerInfo[playerid][pFaction]][fRanks]);
	if (!isLeader(playerid) && GetRankRight(playerid) && (isLeader(userid) || PlayerInfo[userid][pRank] >= PlayerInfo[playerid][pRank]))
		return SendErrorMessage(playerid, "Vous ne pouvez modifier que le grade d'un joueur inférieur au vôtre.");
	if (PlayerInfo[playerid][pRank] < rank)
		return SendErrorMessage(playerid, "Vous ne pouvez pas attribuer un grade supérieur au vôtre.");

	PlayerInfo[userid][pRank] = rank;
	SendSyntaxMessage(userid, "%s vous a attribué le grade '%s'.", GetNameEx(playerid), GetRankName(userid));
	SendSyntaxMessage(playerid, "Vous avez attribué le grade '%s' à %s.", GetRankName(userid), GetNameEx(userid));
	return 1;
}

CMD:ranks(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], mes[1024] = "{FFFFFF}";

	if (!isLeader(playerid) && !GetRankRight(playerid))	
		return SendErrorMessage(playerid, "Vous n'avez pas les droits pour consulter les grades de votre faction.");

	for (new i = 0; i != Factions[faction][fRanks]; i++)
	{
		format(mes, sizeof(mes), "%s%i. %s\n", mes, i + 1, FactionRanks[faction][i]);
	}

	new title[32];
	format(title, sizeof(title), "{FFFFFF}Grades de la faction [%i]", Factions[faction][fRanks]);
	Dialog_Show(playerid, None, DIALOG_STYLE_LIST, title, mes, "Fermer", "");
	return 1;
}

CMD:editrights(playerid, params[])
{
	if (!PlayerInfo[playerid][pFaction])						
		return SendErrorMessage(playerid, "Vous n'appartenez à aucune faction.");
	
	if (!isLeader(playerid) && !GetRightsRight(playerid))	
		return SendErrorMessage(playerid, "Vous n'avez pas les droits pour modifier les permissions des grades.");

	new faction = PlayerInfo[playerid][pFaction];
	
	new faction_ranks[1024] = EOS;
	for (new i = 0; i != Factions[faction][fRanks]; i++)
	{
		if (PlayerInfo[playerid][pRank] >= i && !isLeader(playerid))
		{
			format(faction_ranks, sizeof(faction_ranks), "%s%i\t%s\n", faction_ranks, i+1, FactionRanks[faction][i]);
		}
		else if (isLeader(playerid))
		{
			format(faction_ranks, sizeof(faction_ranks), "%s%i\t%s\n", faction_ranks, i+1, FactionRanks[faction][i]);
		}
	}

	Dialog_Show(playerid, FactionSettings, DIALOG_STYLE_TABLIST, "{FFFFFF}Paramètres des grades", faction_ranks, "Suivant", "Annuler");
	return 1;
}

CMD:myrights(playerid, params[])
{
	if (!PlayerInfo[playerid][pFaction])	
		return SendErrorMessage(playerid, "Vous n'appartenez à aucune faction.");

	return ShowMyRights(playerid);
}

CMD:towcars(playerid, params[])
{
	if (!isLeader(playerid) && !GetTowRight(playerid)) 
		return SendErrorMessage(playerid, "Vous n'avez pas l'autorisation d'envoyer les véhicules inutilisés au spawn.");

	new faction = PlayerInfo[playerid][pFaction];

    SendFactionMessageF(faction, COLOR_LIGHTRED, "%s %s a respawné tous les véhicules inutilisés de la faction.", GetRankName(playerid), GetNameEx(playerid));

    new vehicle, occupied[MAX_VEHICLES char];

    foreach(new i : Player)
	{
	    if ((vehicle = GetPlayerVehicleID(i)) == 0 || VehicleInfo[vehicle][carFaction] != faction) continue;

		occupied{vehicle} = 1;
	}

	foreach(new i : Vehicle)
	{
		if (occupied{i} == 1 || VehicleInfo[i][carFaction] != faction) continue;

		RespawnVehicle(i);
	}
	return 1;
}

CMD:flist(playerid, params[])
{
	if (PlayerInfo[playerid][pFaction] == 0)	
		return SendErrorMessage(playerid, "Vous n'appartenez à aucune faction.");

	new mes[3072] = "{FFFFFF}Pseudo\t{FFFFFF}ID\t{FFFFFF}Rang";

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pFaction] != PlayerInfo[playerid][pFaction]) continue;

		format(mes, sizeof(mes), "%s\n%s\t%i\t%s", mes, GetNameEx(i), i, GetRankName(i));	
	}

	Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Membres de la faction connectés", mes, "Fermer", "");
	return 1;
}

ALTX:departments("/d");
CMD:departments(playerid, params[])
{
    if (PlayerInfo[playerid][pMutedTime] > 0)	return SendErrorMessage(playerid, "Votre chat local est bloqué par le système.");

	new faction[23];

	if (isnull(params))								    return SendHelpMessage(playerid, "(/d)epartments [message]");
	if (!IsFactionLegal(PlayerInfo[playerid][pFaction])) return SendErrorMessage(playerid, "Commande réservée aux structures gouvernementales.");
    if (!PlayerInfo[playerid][pOnDuty])				    return SendErrorMessage(playerid, "Vous devez d'abord être en service.");
    if (!IsPlayerHasItem(playerid, INV_RADIO))	  	    return SendErrorMessage(playerid, "Vous n'avez pas de radio.");

	format(faction, sizeof(faction), "%s", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]));

	for(new i = 1; i != MAX_FACTIONS; i++)
	{
	    if (Factions[i][fON] == 0 || !IsFactionLegal(i)) continue;

        if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;

			if (pos < MAXLEN-1) pos = MAXLEN;

			SendFactionMessage(i, COLOR_ALLDEPT, "** [%s] %s %s: %.*s ...", faction, GetRankName(playerid), GetNameWithMask(playerid), pos, params);
			SendFactionMessage(i, COLOR_ALLDEPT, "... %s **", params[pos]);
		}
		else
		{
			SendFactionMessage(i, COLOR_ALLDEPT, "** [%s] %s %s: %s **", faction, GetRankName(playerid), GetNameWithMask(playerid), params);
		}
	}

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;

    	SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s dit: %.*s ...", GetNameWithMask(playerid), pos, params);
		SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "... %s", params[pos]);
	}
	else
        SendLocalMessage(5.0, playerid, COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5, "(Radio) %s dit: %s", GetNameWithMask(playerid), params);
	return 1;
}

ALTX:faction("/f");
CMD:faction(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
    
	if (PlayerInfo[playerid][pMutedTime] > 0)						return SendErrorMessage(playerid, "Votre chat local est bloqué par le système.");
	if (isnull(params))												return SendHelpMessage(playerid, "(/f)action [chat OOC]");
	if (PlayerInfo[playerid][pFaction] == 0)						return SendErrorMessage(playerid, "Vous devez faire partie d'une faction.");
	if (Factions[faction][fChatON] == 0 && !isLeader(playerid))		return SendErrorMessage(playerid, "Le chat OOC de la faction est désactivé.");
	if (PlayerInfo[playerid][pSettings] & togFaction)				return SendErrorMessage(playerid, "Vous avez désactivé l'affichage des messages de faction.");

	
	new id = PlayerInfo[playerid][pFaction];

	if (IsFactionLegal(faction))
	{
		if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;

			if (pos < MAXLEN-1) pos = MAXLEN;

			SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s %s: %.*s ... )) **", Factions[id][fColor] >>> 8, GetRankName(playerid), GetNameEx(playerid), pos, params);
			SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}... %s )) **", Factions[id][fColor] >>> 8, params[pos]);
		}
		else
		{
			SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s %s: %s )) **", Factions[id][fColor] >>> 8, GetRankName(playerid), GetNameEx(playerid), params);
		}
	}
	else
	{
		if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;

			if (pos < MAXLEN-1) pos = MAXLEN;

			SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s: %.*s ... )) **", Factions[id][fColor] >>> 8, GetNameEx(playerid), pos, params);
			SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}... %s )) **", Factions[id][fColor] >>> 8, params[pos]);
		}
		else
		{
			SendFactionMessageF(PlayerInfo[playerid][pFaction], COLOR_WHITE, "{%06x}** (( %s: %s )) **", Factions[id][fColor] >>> 8, GetNameEx(playerid), params);
		}
	}
	return 1;
}

ALTX:factionattach("/fa");
CMD:factionattach(playerid, params[])
{
	if (GetNextToySlot(playerid) == -1)
		return SendErrorMessage(playerid, "Tous les emplacements d'objets sont occupés.");

	if (!IsFactionLegal(PlayerInfo[playerid][pFaction]))
		return 1;

	if (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid))
	{
		new mes[sizeof(attach_fd_model)*64];
		for (new i = 0; i < sizeof(attach_fd_model); i++)
		{
			format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t~w~%s\n", mes, attach_fd_model[i], attach_fd_name[i]); 
		}
		Dialog_Show(playerid, MODEL_SELECTION_FA, DIALOG_STYLE_PREVIEW_MODEL, "Accessoires de faction", mes, "Choisir", "Fermer");
	}
	else if (IsPlayerFactionCity(playerid))
	{
		new mes[sizeof(attach_city_model)*64];
		for (new i = 0; i < sizeof(attach_city_model); i++)
		{
			format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t~w~%s\n", mes, attach_city_model[i], attach_city_name[i]); 
		}
		Dialog_Show(playerid, MODEL_SELECTION_FA, DIALOG_STYLE_PREVIEW_MODEL, "Accessoires de faction", mes, "Choisir", "Fermer");
	}
	else if (IsPlayerFactionPolice(playerid))
	{
		new mes[sizeof(attach_police_model)*64];
		for (new i = 0; i < sizeof(attach_police_model); i++)
		{
			format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t~w~%s\n", mes, attach_police_model[i], attach_police_name[i]); 
		}
		Dialog_Show(playerid, MODEL_SELECTION_FA, DIALOG_STYLE_PREVIEW_MODEL, "Accessoires de faction", mes, "Choisir", "Fermer");
	}

	return 1;
}

CMD:changebadges(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
	if (Factions[faction][fType] == FACTION_MAFIA || Factions[faction][fType] == FACTION_GANG)
		return SendErrorMessage(playerid, "Vous devez être le leader d'une faction gouvernementale.");
	if (PlayerInfo[playerid][pRank] < 11)
		return SendErrorMessage(playerid, "Disponible à partir du rang 11.");

	new userid, badge;
        
	if (sscanf(params, "ui", userid, badge))
		return SendHelpMessage(playerid, "/changebadges [ID/Nom du joueur] [numéro]");
	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);
	if (badge < 1 || badge > 9999999)
		return SendErrorMessage(playerid, "Le numéro de badge doit être entre 1 et 9999999.");

	PlayerInfo[userid][pBadgeNumber] = badge;
	SendSyntaxMessage(playerid, "Vous avez changé le numéro de badge de %s en [%i].", GetNameEx(userid), badge);
	SendSyntaxMessage(userid, "%s a changé votre numéro de badge en [%i].", GetNameEx(playerid), badge);
	return 1;
}

CMD:carfix(playerid, params[])
{
	if (IsPlayerAtCarfix(playerid) == 0)		
		return SendErrorMessage(playerid, "Vous devez être dans un garage ou sur une base d'organisation.");

	new vehicle = GetPlayerVehicleID(playerid);

	if (!vehicle || VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])	
		return SendErrorMessage(playerid, "Vous n'êtes pas dans un véhicule de votre organisation.");
		
	FixVehicle(vehicle);
	VehicleInfo[vehicle][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(vehicle));
	SendSyntaxMessage(playerid, "Le véhicule a été entièrement réparé.");
	return 1;
}

ALTX:bortlist("/bl");
CMD:bortlist(playerid, params[])
{
    new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionLegal(faction))		
		return SendErrorMessage(playerid, "Vous devez être policier, agent pénitentiaire ou pompier.");
    if (IsPlayerInAnyVehicle(playerid))	
		return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");
    if (!PlayerInfo[playerid][pOnDuty])	
		return SendErrorMessage(playerid, "Vous devez être en service.");

    new count, str[512] = "- Supprimer tous les objets\n";

    for (new i = 0; i != MAX_BORTES; i++)
	{
		if (BortInfo[playerid][i][bortOn] == 0) continue;

        format(str, sizeof(str), "%s%i: %s\n", str, i + 1, BortInfo[playerid][i][bortName]);
        count++;
	}

	if (!count) 
		return SendErrorMessage(playerid, "Vous n'avez aucun objet de route créé.");

	Dialog_Show(playerid, BortList, DIALOG_STYLE_LIST, "{FFFFFF}Liste des objets", str, "Choisir", "Annuler");
	return 1;
}

CMD:bort(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionLegal(faction))		
		return SendErrorMessage(playerid, "Vous devez être policier, agent pénitentiaire ou pompier.");

    if (IsPlayerInAnyVehicle(playerid))	
		return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");

    if (!PlayerInfo[playerid][pOnDuty])	
		return SendErrorMessage(playerid, "Vous devez être en service.");

	if (IsFactionMedic(faction) || IsFactionFire(faction))
    {
		new mes[sizeof(bort_list_fd) * 128];
		for (new i = 0; i < sizeof(bort_list_fd); i++)
		{
			format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t%s\n", mes, bort_list_fd[i], bort_names_fd[i]); 
		}
		Dialog_Show(playerid, MODEL_SELECTION_BORT, DIALOG_STYLE_PREVIEW_MODEL, "_", mes, "Choisir", "Fermer");
	}
	else if (IsFactionCity(faction))
	{
		new mes[sizeof(bort_list_city) * 128];
		for (new i = 0; i < sizeof(bort_list_city); i++)
		{
			format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t%s\n", mes, bort_list_city[i], bort_names_city[i]); 
		}
		Dialog_Show(playerid, MODEL_SELECTION_BORT, DIALOG_STYLE_PREVIEW_MODEL, "_", mes, "Choisir", "Fermer");
	}
	else
	{
		new mes[sizeof(bort_list) * 128];
		for (new i = 0; i < sizeof(bort_list); i++)
		{
			format(mes, sizeof(mes), "%s%i(0.0, 0.0, 0.0, 1.0)\t%s\n", mes, bort_list[i], bort_names[i]); 
		}
		Dialog_Show(playerid, MODEL_SELECTION_BORT, DIALOG_STYLE_PREVIEW_MODEL, "_", mes, "Choisir", "Fermer");
	}

	return 1;
}

ALTX:megaphone("/m");
CMD:megaphone(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid);

    if (!IsFactionLegal(faction))															
		return SendErrorMessage(playerid, "Vous devez appartenir à une faction gouvernementale.");

	if (vehicle == 0 || VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])	
		return SendErrorMessage(playerid, "Vous devez être dans un véhicule de votre faction.");

	if (isnull(params))	
		return SendHelpMessage(playerid, "Utilisation : /megaphone [message]");

	SendNearbyMessage(Range_VLong, playerid, COLOR_YELLOWG, "[ %s %s :o< %s ]", GetRankName(playerid), GetNameWithMask(playerid), params);
	return 1;
}

CMD:mdc(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);
	
	if (IsPlayerPhoneCreated(playerid))	
		return SendErrorMessage(playerid, "Rangez votre téléphone avant d'utiliser le MDC.");

	if (!IsPlayerFactionPolice(playerid) && !IsPlayerFactionMedic(playerid) && !IsPlayerFactionFire(playerid))	
		return SendErrorMessage(playerid, "Vous devez faire partie de la police, des pompiers ou des secours.");

	if (VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && !IsPlayerInRangeOfPoint(playerid, 50.0, 2781.0559, -1056.2823, 1028.4969) && !IsPlayerInRangeOfPoint(playerid, 50.0, 2874.6665, -940.6707, 1035.9259))
	{
		return SendErrorMessage(playerid, "Accessible uniquement au QG ou dans un véhicule de service.");
	}

	if (IsPlayerFactionPolice(playerid))
	{
		if (!PlayerInfo[playerid][pOnDuty])	
			return SendErrorMessage(playerid, "Vous devez être en service pour utiliser le MDC.");

		if (IsPlayerUseMDC(playerid))
			return MDC_Hide(playerid);

		if (pTemp[playerid][pCCTV])
			return 1; // Impossible pendant l'utilisation de la vidéosurveillance.

		MdcInfo[playerid][mdcPage] = PAGE_NONE;

		MDC_Show(playerid);

		SelectTextDraw(playerid, 255); // Activer la sélection du MDC.
		return 1;
	}

	if (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid))
	{
		if (!PlayerInfo[playerid][pOnDuty])	
			return SendErrorMessage(playerid, "Vous devez être en service pour utiliser le système.");

		return Get_Calls(playerid);
	}
	return 1;
}

CMD:showbadge(playerid, params[])
{
	new userid;
	
	if (!PlayerInfo[playerid][pFaction] || !IsFactionLegal(PlayerInfo[playerid][pFaction]))	
		return SendErrorMessage(playerid, "Vous ne faites pas partie d'une force de l'ordre.");

	if (sscanf(params, "u", userid))	
		return SendHelpMessage(playerid, "/showbadge [ID/Nom du joueur]");

	if (!IsPlayerLogged(userid))	
		return SendNotConnected(playerid);

	if (!PlayerNearPlayer(3.0, playerid, userid))	
		return SendNotNearPlayer(playerid);

	SendClientMessage(userid, COLOR_POLICECHAT, "_______________________");
	SendClientMessageEx(userid, COLOR_WHITE, Factions[PlayerInfo[playerid][pFaction]][fName]);
	SendClientMessageEx(userid, COLOR_WHITE, "%s - %s", GetRankName(playerid), GetNameWithMask(playerid));
	SendClientMessageEx(userid, COLOR_WHITE, "Badge n° : %i", PlayerInfo[playerid][pBadgeNumber]);
	SendClientMessage(userid, COLOR_POLICECHAT, "_______________________");

	SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "* %s montre son badge à %s.", GetNameWithMask(playerid), GetNameWithMask(userid));
	return 1;
}

CMD:pull(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid), userid, seat;

	if (!IsPlayerFactionPolice(playerid) &&
		!IsPlayerFactionMedic(playerid) &&
		!IsPlayerFactionFire(playerid))						
		return SendErrorMessage(playerid, "Vous devez être policier, pompier ou médecin.");

    if (!vehicle || IsABoat(vehicle) || IsABiker(vehicle))	
		return SendErrorMessage(playerid, "Vous devez être dans un véhicule adapté.");

	new maxseat = GetMaxPassengers(GetVehicleModel(vehicle));

	if (sscanf(params, "ud", userid, seat))			
		return SendHelpMessage(playerid, "/pull [ID/Nom du joueur] [siège (1-%i)]", maxseat);

	if (!IsPlayerLogged(userid))						
		return SendNotConnected(playerid);

	if (seat > maxseat || seat < 1)					
		return SendSyntaxMessage(playerid, "Le numéro de siège doit être entre 1 et %i.", maxseat);

	if (userid == playerid)							
		return 1;

	if (!PlayerNearPlayer(10.0, playerid, userid))	
		return SendNotNearPlayer(playerid);

	if (PlayerInfo[userid][pInjured])				
		return SendErrorMessage(playerid, "Vous ne pouvez pas déplacer un joueur blessé.");

	if (IsPlayerInAnyVehicle(userid))				
		return SendErrorMessage(playerid, "Le joueur est déjà dans un véhicule.");

    if (!PutPlayerInVehicle(userid, vehicle, seat))	
		return SendErrorMessage(playerid, "Impossible d'installer le joueur dans le véhicule.");

	if (IsFactionMedic(faction) || IsFactionFire(faction))
	{
		StopPlayerAnims(userid);
		SendSyntaxMessage(playerid, "Vous avez installé %s dans le véhicule médical.", GetNameEx(userid));
		SendSyntaxMessage(userid, "%s vous a installé dans un véhicule médical.", GetNameEx(playerid));
	}
	else
	{
	    SendSyntaxMessage(playerid, "Vous avez placé %s dans le véhicule.", GetNameEx(userid));
		SendSyntaxMessage(userid, "%s vous a placé dans un véhicule.", GetNameEx(playerid));
	}
	return 1;
}

/*
CMD:pullout(playerid, params[])
{
	new userid;

	if (sscanf(params,"u", userid))					return SendHelpMessage(playerid, "/pullout [ID/Nom]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)							return 1;
	if (!PlayerNearPlayer(10.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (!IsPlayerInAnyVehicle(userid))				return SendErrorMessage(playerid, "Ce joueur n'est pas dans un véhicule.");
	if (PlayerInfo[userid][pInjured] == 0 && pTemp[userid][pTaserTime] == 0)
		return SendErrorMessage(playerid, "Le joueur n'est ni blessé, ni mort, ni paralysé.");

	new vehid = GetPlayerVehicleID(userid);
	if (VehicleInfo[vehid][carLock] != 0)			
		return SendErrorMessage(playerid, "Le véhicule est verrouillé. Utilisez /ram !");

	new mes[128];
	format(mes, sizeof(mes), "tire %s hors du véhicule.", GetNameWithMask(userid));
	cmd::me(playerid, mes);

	SavePlayerPos(playerid);
	SetPlayerPosEx(userid, PlayerInfo[playerid][pPosX], PlayerInfo[playerid][pPosY], PlayerInfo[playerid][pPosZ]);
	SavePlayerPos(userid);

	ApplyAnimation(userid, "PED", "KO_SHOT_STOM", 4.0, 1, 0, 0, 0, 0, 1);

	UnFreezePlayer(userid);
	return 1;
}
*/