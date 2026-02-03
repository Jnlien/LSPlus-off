ALTX:changerspawn("/setspawn");
CMD:changerspawn(playerid, params[])
{
	new type;

	if (sscanf(params, "d", type))
		return SendHelpMessage(playerid, "/changerspawn [0 - spawn normal, 1 - maison, 2 - faction, 3 - dernière position]");
		
	if (type > 3 || type < 0)
		return SendHelpMessage(playerid, "/changerspawn [0 - spawn normal, 1 - maison, 2 - faction, 3 - dernière position]");

	if (type == 0)
	{
		Dialog_Show(playerid, SelectSpawn, DIALOG_STYLE_LIST, "{8D8DFF}Choisissez un lieu de spawn", "Los Santos Airport\nGare de Los Santos", "Choisir", "Annuler");
		return 1;
	}
	else if (type == 1)
	{
		Dialog_Show(playerid, ChangeHouseSpawn, DIALOG_STYLE_INPUT, "{8D8DFF}[Sélection de maison]", "Entrez l'ID de la maison où vous souhaitez apparaître. Vous pouvez voir l'ID avec /hlist.", "Choisir", "Annuler");
	}
	else if (type == 2)
	{
		if (!PlayerInfo[playerid][pFaction])
			return SendErrorMessage(playerid, "Vous ne faites partie d'aucune faction.");

		SendSyntaxMessage(playerid, "Vous apparaîtrez désormais au spawn de votre faction.");
		PlayerInfo[playerid][pSpawnType] = 2;
	}
	else if (type == 3)
	{
		SendSyntaxMessage(playerid, "Vous apparaîtrez désormais à votre dernière position.");
		PlayerInfo[playerid][pSpawnType] = 3;
	}

	SQL_SetInteger("users", "spawntype", PlayerInfo[playerid][pSpawnType], PlayerInfo[playerid][pID]);
	return 1;
}

ALTX:location("/street");
CMD:location(playerid, params[])
{
	new location[MAX_ZONE_NAME],
		Float:x, Float:y, Float:z;

	GetPlayerPos(playerid, x, y, z);
	GetZone(x, y, location, MAX_ZONE_NAME);

	if (IsPlayerInAnyDynamicArea(playerid)) {
		new areas[5], street_area[2], j = GetPlayerNumberDynamicAreas(playerid);
		GetPlayerDynamicAreas(playerid, areas, sizeof(areas));

		if (GetStreet(areas, j, street_area)) {
			if (street_area[1] == -1)
				return SendSpecialMessage(playerid, "%s, %s.", location, street_names[street_area[0]]);
			else
				return SendSpecialMessage(playerid, "%s, à l'intersection de %s et %s.", location, street_names[street_area[0]], street_names[street_area[1]]);
		}
	}

	SendSpecialMessage(playerid, "%s.", location);
	return 1;
}

ALTX:address("/adres", "/adress", "addres");
CMD:address(playerid, params[])
{
	new houseid;
	if (sscanf(params, "d", houseid)) 
		return SendHelpMessage(playerid, "/address [ID de la maison]");

	if (houseid < 0 || houseid >= MAX_HOUSES)
		return SendErrorMessage(playerid, "Aucune maison trouvée avec cet identifiant.");

	if (!HouseInfo[houseid][hID])
		return SendErrorMessage(playerid, "Adresse introuvable pour cette maison.");

	if (HouseInfo[houseid][hApart])
		Waypoint_Set(playerid, ApartInfo[HouseInfo[houseid][hApart]][aPosX], ApartInfo[HouseInfo[houseid][hApart]][aPosY], ApartInfo[HouseInfo[houseid][hApart]][aPosZ]);
	else
		Waypoint_Set(playerid, HouseInfo[houseid][hPosX], HouseInfo[houseid][hPosY], HouseInfo[houseid][hPosZ]);
			
	SendSyntaxMessage(playerid, "Maison (%s) marquée sur le GPS. (/removecp)", (!HouseInfo[houseid][hApart]) ? GetHouseZone(houseid) : GetApartmentZone(HouseInfo[houseid][hApart]));

	return 1;
}

CMD:payer(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
        return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

    new userid, amount;
    if (sscanf(params, "ud", userid, amount))
        return SendHelpMessage(playerid, "/payer [ID/joueur] [montant]");

    if (userid == playerid)
        return 1;

    if (PlayerInfo[playerid][pHours] < 2)
        return SendErrorMessage(playerid, "Vous devez avoir joué au moins 2 heures sur le serveur pour utiliser cette commande.");

    if (GetPVarInt(playerid, #just_paid) > gettime())
        return SendErrorMessage(playerid, "Veuillez attendre 2 secondes avant d'envoyer de l'argent à nouveau.");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    if (amount > GetCash(playerid) || amount <= 0)
        return SendNotCash(playerid);

    if (!PlayerNearPlayer(3.0, playerid, userid))
        return SendNotNearPlayer(playerid);

    SetPVarInt(playerid, #just_paid, gettime() + 2);

    new money1 = PlayerInfo[playerid][pCash], money2 = PlayerInfo[userid][pCash];

    GiveCash(playerid, -amount);
    GiveCash(userid, amount);
    PlaySound(playerid, 1052);
    PlaySound(userid, 1052);

    SendSyntaxMessage(playerid, "Vous avez donné $%i à %s [ID:%i].", amount, GetNameWithMask(userid), userid);
    SendSyntaxMessage(userid, "Vous avez reçu $%i de %s [ID:%i].", amount, GetNameWithMask(playerid), playerid);
    SendAdmMessage("%s (ID: %i) a donné à %s (ID: %i) $%i.", GetNameEx(playerid), playerid, GetNameEx(userid), userid, amount);

    new mes[256];
    format(mes, sizeof(mes), "sort de l'argent et le donne à %s.", GetNameWithMask(userid));
    cmd::me(playerid, mes);

    format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent(Avant): %i | Argent(Après): %i] a donné de l'argent à %s [MYSQL: %i | IP: %s | Argent(Avant): %i | Argent(Après): %i] -- $%i",
        GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money1, GetCash(playerid),
        GetName(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid), money2, GetCash(userid), amount);
    insert_log(mes, MONEY_LOG, 1, playerid);

    return 1;
}

CMD:licenses(playerid, params[])
{
	new userid;
	if (sscanf(params, "u", userid))
		return SendHelpMessage(playerid, "/licenses [ID/nom du joueur]");

	if (userid == playerid)
	{
		cmd::server_ame(playerid, "regarde sa propre carte d'identité.");
		ShowLicenses(playerid, playerid);
		return 1;
	}

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (!PlayerNearPlayer(8.0, playerid, userid))
		return SendNotNearPlayer(playerid);

	new mes[128];
	format(mes, sizeof(mes), "montre sa carte d'identité à %s.", GetNameWithMask(userid));
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
	ShowLicenses(playerid, userid);
	return 1;
}

CMD:id(playerid, params[])
{
	if (isnull(params))
		return SendHelpMessage(playerid, "/id [ID/nom du joueur]");

	if (isNumeric(params))
	{
	    new userid = strval(params);
	    if (!IsPlayerLogged(userid))
			return SendErrorMessage(playerid, "Aucun résultat trouvé.");

	    SendClientMessageEx(playerid, COLOR_GREY, "[ID: %i] %s [%s]", userid, GetNameEx(userid), GetAccName(userid));
	    return 1;
	}

	new count;
	foreach (new i : Player)
	{
	    if (!IsPlayerLogged(i) || strfind(GetName(i), params, true) == -1) continue;

        SendClientMessageEx(playerid, COLOR_GREY, "[ID: %i] %s [%s]", i, GetNameEx(i), GetAccName(i));
        count = 1;
	}

	if (!count)
		SendErrorMessage(playerid, "Aucun résultat trouvé.");

	return 1;
}

CMD:admins(playerid, params[])
{
    SendClientMessage(playerid, COLOR_WHITE, "|_______LISTE DES ADMINS_______|");

    foreach (new i : Player)
    {
        if (!IsPlayerLogged(i)) continue;

        if (PlayerInfo[i][pAdmin] >= MODERATOR_LEVEL && !PlayerInfo[i][pAdminHide])
        {
            if (!IsAdminDuty(i)) SendClientMessageEx(playerid, COLOR_LIGHTRED, " * {C7C7C7}%s | %s [ID: %i]", GetAdminRank(i), GetAccName(i), i);
            else SendClientMessageEx(playerid, COLOR_GREEN2, " * {FFFFFF}%s | %s [ID: %i] (En service)", GetAdminRank(i), GetAccName(i), i);
            
        }
    }

    return 1;
}

CMD:testers(playerid, params[])
{
	SendClientMessage(playerid, COLOR_WHITE, "|_______LISTE DES TESTEURS_______|");

	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pHelper] < 1) continue;

		if (!IsTesterDuty(i))
			SendClientMessageEx(playerid, COLOR_LIGHTRED, " * {C7C7C7}Helper | %s [ID: %i] [%s]", GetNameEx(i), i, GetAccName(i));
		else
			SendClientMessageEx(playerid, COLOR_GREEN2, " * {FFFFFF}Helper | %s [ID:%i] [%s] (En service)", GetNameEx(i), i, GetAccName(i));
	}

	return 1;
}

CMD:blockpm(playerid, params[])
{
    if (PlayerInfo[playerid][pPremium] < 1) return SendErrorMessage(playerid, "Cette fonction est disponible uniquement pour les comptes Premium.");
	
	new userid;

	if (sscanf(params,"u", userid))	return SendHelpMessage(playerid, "/blockpm [ID/Nom du joueur]");
	if (userid == playerid)			return 1;
	if (!IsPlayerLogged(userid)) 	return SendNotConnected(playerid);

    if (people_blocking[playerid][userid] == 1)
    {
		SendSyntaxMessage(playerid, "Vous avez retiré %s [ID: %i] de votre liste noire de MP.", GetNameEx(userid), userid);
		people_blocking[playerid][userid] = 0;
		return 1;
	}

    SendSyntaxMessage(playerid, "Vous avez ajouté %s [ID: %i] à votre liste noire de MP.", GetNameEx(userid), userid);
	people_blocking[playerid][userid] = 1;
	return 1;
}

CMD:blocklist(playerid, params[])
{
	if (PlayerInfo[playerid][pPremium] < 1) return SendErrorMessage(playerid, "Cette fonction est disponible uniquement pour les comptes Premium.");

	SendClientMessage(playerid, COLOR_LIGHTRED, "|______LISTE NOIRE RCRP______|");
	foreach(new i : Player)
	{
		if (people_blocking[playerid][i] == 0) continue;

		SendClientMessageEx(playerid, COLOR_INFO, "[ID: %i] %s", i, GetNameEx(i));
	}
	return 1;
}

CMD:time(playerid, params[])
{
	new hour, minute, mes[128];

	gettime(hour, minute);
	format(mes, sizeof(mes), "~w~%02d:%02d~n~Prochain salaire dans : %02i min", hour, minute, 60 - PlayerInfo[playerid][pPayDayTime]);

	if (PlayerInfo[playerid][pMutedTime] > 0) 
		format(mes, sizeof(mes), "%s~n~~r~Temps restant en mute : %i", mes, PlayerInfo[playerid][pMutedTime]);

	if (PlayerInfo[playerid][pJailed] == PALOMINO_JAIL)
	{
		format(mes, sizeof(mes), "~y~Temps de prison restant : %i~n~%s", PlayerInfo[playerid][pJailTime], mes);
		GameTextForPlayer(playerid, mes, 5000, 1);
		ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
		return 1;
	}
	else if (PlayerInfo[playerid][pIsolateTime] != 0)
	{
		format(mes, sizeof(mes), "~y~Temps d'isolement restant : %i~n~%s", PlayerInfo[playerid][pIsolateTime], mes);
		GameTextForPlayer(playerid, mes, 5000, 1);
		ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
		return 1;
	}
	else if (PlayerInfo[playerid][pJailed] == PRISON_JAIL)
	{
		format(mes, sizeof(mes), "~y~Temps de prison fédérale restant : %i~n~%s", PlayerInfo[playerid][pJailTime], mes);
		GameTextForPlayer(playerid, mes, 5000, 1);
		ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
		return 1;
	}
	else
	{
		GameTextForPlayer(playerid, mes, 5000, 1);
		cmd::server_ame(playerid, "regarde l'heure sur sa montre.");
		
		if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) 
			ApplyAnimation(playerid,"COP_AMBIENT","Coplook_watch",4.1,0,0,0,0,0,1);
	}
	return 1;
}

CMD:coin(playerid, params[])
{
	SendNearbyMessage(Range_Normal, playerid, COLOR_WHITE, "* %s lance une pièce et obtient %s.", GetNameWithMask(playerid), (random(2) == 0) ? ("pile") : ("face"));
	return 1;
}

CMD:dice(playerid, params[])
{
	if (!IsPlayerHasItem(playerid, INV_DICE))
		return SendErrorMessage(playerid, "Vous n'avez pas de dés.");

	SendNearbyMessage(Range_Normal, playerid, COLOR_WHITE, "* %s lance un dé et obtient %i.", GetNameWithMask(playerid), random(6) + 1);
	return 1;
}

CMD:mask(playerid, params[])
{
	if (!IsPlayerHasItem(playerid, INV_MASK))
		return SendErrorMessage(playerid, "Vous n'avez pas de masque.");
	if (PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous ne pouvez pas mettre un masque en service.");

	if (!maskOn{playerid})
	{
		RemoveInvItem(playerid, INV_MASK, 1);

	    cmd::server_ame(playerid, "* Met son masque.");
		GameTextForPlayer(playerid, "~w~VOUS ~g~AVEZ MIS ~w~VOTRE MASQUE", 4000, 4);
		maskOn{playerid} = true;
		ShowPlayerName(playerid, false);
		format(PlayerInfo[playerid][pMaskname], 32, "Masque_%i", randomEx(100000, 999999));
		cNametag[playerid] = CreateDynamic3DTextLabel("Met son masque...", 0xFFFFFFFF, 0.0, 0.0, 0.1, NT_DISTANCE, .attachedplayer = playerid, .testlos = 1);
	}
	else
	{
		GiveInvItem(playerid, INV_MASK, 1);

	    cmd::server_ame(playerid, "* Retire son masque.");
		GameTextForPlayer(playerid, "~w~VOUS ~r~AVEZ RETIRÉ ~w~VOTRE MASQUE", 4000, 4);
		RemovePlayerMask(playerid);
	}
	return 1;
}

CMD:checkafk(playerid, params[])
{
	new id;
	
	if (sscanf(params, "u", id))	return SendHelpMessage(playerid, "/checkafk [ID/nom du joueur]");
	if (!IsPlayerLogged(id))		return SendNotConnected(playerid);

	if (IsPlayerAFK(id))
		SendClientMessageEx(playerid, COLOR_LIGHTRED, "[ ! ] Le joueur %s [ID: %i] est en AFK depuis %i secondes.", GetNameEx(id), id, pTemp[id][pDarkAFK]-2);
	else
		SendClientMessageEx(playerid, COLOR_LIGHTRED, "[ ! ] Le joueur %s [ID: %i] n'est pas en AFK.", GetNameEx(id), id);
		
	return 1;
}

CMD:showprescribe(playerid, params[])
{
    if (PlayerInfo[playerid][pPrescribe] < gettime())
        return SendErrorMessage(playerid, "Vous n'avez pas d'ordonnance pour la marijuana médicale.");

    new userid;

	if (sscanf(params, "u", userid))
		return SendHelpMessage(playerid, "/showprescribe [ID/nom du joueur]");
	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))
		return SendNotNearPlayer(playerid);
		
    SendClientMessage(userid, COLOR_GREEN, "__________Ordonnance Médicale__________");
	SendClientMessageEx(userid, COLOR_WHITE, "Délivrée à: %s Date de naissance: %s", GetNameEx(playerid), PlayerInfo[playerid][pBirthdate]);
	SendClientMessage(userid, COLOR_WHITE, "Établie par: All Saints General Hospital.");
	SendClientMessage(userid, COLOR_WHITE, "Autorisation d'achat, de possession et d'usage de marijuana médicale.");
	SendClientMessage(userid, COLOR_GREEN, "_________________________________");
	
	new mes[128];
	format(mes, sizeof(mes), "montre l'ordonnance à %s.", GetNameWithMask(userid));
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
	return 1;
}

CMD:stats(playerid, params[])
{
	ShowStats(playerid, playerid);
	return 1;
}

/*CMD:charity(playerid, params[])
{
	new amount;

	if (sscanf(params,"d",amount))
		return SendHelpMessage(playerid, "/charity [montant]");
	if (amount > GetCash(playerid) || amount <= 0)
		return SendNotCash(playerid);

	PlaySound(playerid, 1052);
	SendSyntaxMessage(playerid, "Vous avez donné $%i en charité.", amount);
	AMSG(COLOR_LIGHTRED, "Attention: %s a donné (/charity) $%i.", GetNameEx(playerid), amount);

	new money = PlayerInfo[playerid][pCash];
	GiveCash(playerid, -amount);

	new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent avant: %i | Argent après: %i] a donné (/charity) -- $%i",
		GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), amount);
    insert_log(mes, MONEY_LOG, 5, PlayerInfo[playerid][pAccID]);
	return 1;
}*/

CMD:myprisonnumber(playerid, params[])
{
	if (PlayerInfo[playerid][pJailed] != PRISON_JAIL)
		return SendErrorMessage(playerid, "Vous n'êtes pas en prison.");

	SendClientMessageEx(playerid, COLOR_GRAD2, "Nom: %s | Numéro de prison: %s%i", GetNameEx(playerid), (PlayerInfo[playerid][pIsolateTime] != 0) ? ("B") : ("A"), PlayerInfo[playerid][pPrisoned] + 100);
	return 1;
}


/*CMD:helpme(playerid, params[])
{
	if (isnull(params))
		return SendHelpMessage(playerid, "/helpme [texte de la question]");

	SendSyntaxMessage(playerid, "Votre message a été envoyé aux testeurs ! Merci de patienter pour une réponse.");

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;

		if (pos < MAXLEN-1) pos = MAXLEN;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i)) || (PlayerInfo[i][pSettings] & togService))
				continue;
				
			SendClientMessageEx(i, 0x00CC99AA, "[QUESTION] %s [ID: %i]: %.*s ...", GetNameEx(playerid), playerid, pos, params);
			SendClientMessageEx(i, 0x00CC99AA, "... %s", params[pos]);
		}
	}
	else
	{
	    foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i)) || (PlayerInfo[i][pSettings] & togService))
				continue;

			SendClientMessageEx(i, 0x00CC99AA, "[QUESTION] %s [ID: %i]: %s", GetNameEx(playerid), playerid, params);
		}
	}
	return 1;
}*/

CMD:report(playerid, params[])
{
	if (isnull(params))
		return SendHelpMessage(playerid, "/report [texte de la plainte]");
		
	if (pTemp[playerid][pReportTime] > gettime())
		return SendSpecialMessage(playerid, "Vous pourrez envoyer un autre report dans %i secondes.", pTemp[playerid][pReportTime] - gettime());

    pTemp[playerid][pReportTime] = gettime() + 60;
	SendSyntaxMessage(playerid, "Votre report a été transmis à l'administration du serveur. Veuillez patienter pour une réponse.");

    if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		if (pos < MAXLEN-1) pos = MAXLEN;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService))
				continue;

			SendClientMessageEx(i, COLOR_YELLOWG, "[REPORT] %s [ID: %i]: %.*s ...", GetNameEx(playerid), playerid, pos, params);
			SendClientMessageEx(i, COLOR_YELLOWG, "... %s", params[pos]);
		}
	}
	else
	{
	    foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService))
				continue;

			SendClientMessageEx(i, COLOR_YELLOWG, "[REPORT] %s [ID: %i] a envoyé un report: %s", GetNameEx(playerid), playerid, params);
		}
	}
	return 1;
}

CMD:irc(playerid, params[])
{
	if (isnull(params))
		return SendHelpMessage(playerid, "/irc [texte de la demande]");

	if (request_time > gettime())
		return SendSpecialMessage(playerid, "Veuillez attendre %i secondes.", request_time - gettime());

	request_time = gettime() + 60;
	SendSyntaxMessage(playerid, "Votre demande a été envoyée à l'administration du serveur. Veuillez patienter pour une réponse.");

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		if (pos < MAXLEN-1) pos = MAXLEN;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService))
				continue;

			SendClientMessageEx(i, 0x00CC99AA, "[AIDE] %s (ID: %i): %.*s ...", GetNameEx(playerid), playerid, pos, params);
			SendClientMessageEx(i, 0x00CC99AA, "... %s", params[pos]);
		}
	}
	else
	{
	    foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1) || (PlayerInfo[i][pSettings] & togService))
				continue;

			SendClientMessageEx(i, 0x00CC99AA, "[AIDE] %s (ID: %i) a demandé de l'aide : %s", GetNameEx(playerid), playerid, params);
		}
	}
	return 1;
}

CMD:sell(playerid, params[])
{
	new bizid;

	if ((bizid = GetNearInsideBiz(playerid)) != -1)
	{
		if (BizInfo[bizid][bizType] == BIZ_PAWN)
		{
			Dialog_Show(playerid, SellPawn, DIALOG_STYLE_LIST, "Penderie", "• Vendre un autoradio", "Vendre", "Annuler");
		}
	}
	return 1;
}

CMD:buy(playerid, params[])
{
	new string[1024], bizid = -1;

	if ((bizid = GetNearInsideBiz(playerid)) != -1)
	{
	    if (BizInfo[bizid][bizType] == BIZ_BUILD)
	    {
	        Dialog_Show(playerid, BuyInterior, DIALOG_STYLE_INPUT, "{FFFFFF}Commander un intérieur", "{FFFFFF}Entrez l'ID de la maison dont vous voulez changer l'intérieur :\nLa maison doit vous appartenir ! (Utilisez /hlist)", "Suivant", "Fermer");
	    }

		if (BizInfo[bizid][bizType] == BIZ_PAWN)
		{
			SendHelpMessage(playerid, "/sell pour vendre des objets.");
		}
	    else if (BizInfo[bizid][bizType] == BIZ_STORE || BizInfo[bizid][bizType] == BIZ_PHARMACY || BizInfo[bizid][bizType] == BIZ_BAR)
	    {
			new type; 
			switch(BizInfo[bizid][bizType])
			{
				case BIZ_STORE: type = 4;		
				case BIZ_BAR: type = 5;			
				case BIZ_PHARMACY: type = 6;	 
			} 
			if (!type)	return SendErrorMessage(playerid, "Erreur système ! (pl_cmd.inc:493)");

			new diatxt[LIMIT_MAX_PRODUCTS*128];
			for(new i = 0; i < max_product[type]; i++)
			{
				format(diatxt, sizeof(diatxt), "%s%i(0.0, 0.0, 63.0, 0.8)\t~w~%s~n~~g~~h~$%i\n", diatxt, model_order[type][i], name_order[type][i], BizInfo[bizid][bizProductPrice][i]);
			}

			new title[56];
			format(title, sizeof(title), "%s____-____Sélectionnez un produit", BizInfo[bizid][bizName]);

			Dialog_Show(playerid, StoreDialog, DIALOG_STYLE_PREVIEW_MODEL, title, diatxt, "Acheter", "Annuler");
	    }
	    else if (BizInfo[bizid][bizType] == BIZ_BARBER)
	    {
	        if (GetPVarInt(playerid, #buy_attach))	return SendErrorMessage(playerid, "Veuillez terminer l'opération actuelle.");
			if (GetNextToySlot(playerid) == -1)		return SendWarningMessage(playerid, "Tous vos emplacements pour accessoires sont occupés.");

			for(new x; x < sizeof(BarberShop); x++)
			{
				format(string, sizeof(string), "%s%s - $%i\n", string, BarberShop[x][toyName], BarberShop[x][toyPrice]);
			}

			Dialog_Show(playerid, BarberDialog, DIALOG_STYLE_LIST, "{FFFFFF}Sélectionnez un produit", string, "Choisir", "Annuler");
		}
		else if (BizInfo[bizid][bizType] == BIZ_PIZZA || BizInfo[bizid][bizType] == BIZ_CLUCKIN || BizInfo[bizid][bizType] == BIZ_BURGER || BizInfo[bizid][bizType] == BIZ_DONUT)
		{
			for(new i; i != 4; i++)
			{
			    TextDrawShowForPlayer(playerid, TD_restaurant[i]);
			}

			new mes[128], type = BizInfo[bizid][bizType] - BIZ_PIZZA;

			for(new i; i != max_product[type]; i++)
			{
			    format(mes, sizeof(mes), "%s~n~~r~Santé : +%i~n~~r~Faim réduite~n~~p~Prix : $%i~n~", name_order[type][i], 30 + (i * 10), BizInfo[bizid][bizProductPrice][i]);
				PlayerTextDrawSetString(playerid, TD_restaurant_prod[playerid][i], mes);
				PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i]);
	            PlayerTextDrawSetPreviewModel(playerid, TD_restaurant_prod[playerid][i + 4], model_order[type][i]);
	            PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i + 4]);
			}

			SelectTextDraw(playerid, 0x00cb77FF);
			SetPVarInt(playerid, #restaurant, 1);
		}
		else if (BizInfo[bizid][bizType] == BIZ_PHONES)
		{
			new phonebuy_names[9][16] = {"Noir", "Bleu", "Rouge", "Vert", "Orange", "Bleu foncé", "Vert foncé", "Rose", "Jaune"};

			new diatxt[sizeof(phone_model)*128];
			for(new i = 0; i < sizeof(phone_model); i++)
			{
				format(diatxt, sizeof(diatxt), "%s%i(180.0, 45.0, 0.0, 0.8)\t%s téléphone~n~~g~~h~$1000\n", diatxt, phone_model[i], phonebuy_names[i]);
			}

			Dialog_Show(playerid, ComputerStore, DIALOG_STYLE_PREVIEW_MODEL, "Sélectionnez un téléphone", diatxt, "Choisir", "Annuler");
		}
		else if (BizInfo[bizid][bizType] == BIZ_AMMO)
		{
			cmd::buygun(playerid, "");
		}
		else if (pTemp[playerid][pBizEntered] != -1 && BizInfo[pTemp[playerid][pBizEntered]][bizType] == BIZ_BARBER)
		{
			if (GetPVarInt(playerid, #buy_attach))	return SendErrorMessage(playerid, "Veuillez terminer l'opération actuelle.");
			if (GetNextToySlot(playerid) == -1)		return SendErrorMessage(playerid, "Tous vos emplacements sont pleins.");

			for(new x; x < sizeof(BarberShop); x++)
			{
				format(string, sizeof(string), "%s%s - $%i\n", string, BarberShop[x][toyName], BarberShop[x][toyPrice]);
			}
			Dialog_Show(playerid, BarberDialog, DIALOG_STYLE_LIST, "{FFFFFF}Sélectionnez un produit", string, "Choisir", "Annuler");
		}
		else if (BizInfo[bizid][bizType] == BIZ_ATTACH)
		{
			if (GetPVarInt(playerid, #buy_attach))	return SendErrorMessage(playerid, "Veuillez terminer l'opération actuelle.");
			if (GetNextToySlot(playerid) == -1)		return SendErrorMessage(playerid, "Tous vos emplacements sont pleins.");

			Dialog_Show(playerid, BizAttachDialog, DIALOG_STYLE_LIST, "{FFFFFF}Sélectionnez une catégorie", "1. Instruments de musique\n2. Montres\n3. Outils\n4. Équipements sportifs\n5. Accessoires", "Choisir", "Annuler");
		}
	}
	
	return 1;
}

CMD:buyclothes(playerid, params[])
{
    if (PlayerInfo[playerid][pOnDuty])	
        return SendErrorMessage(playerid, "Vous devez être hors service pour acheter des vêtements.");

	new id;

	if ((id = GetNearInsideBiz(playerid)) == -1)	
        return SendErrorMessage(playerid, "Vous n'êtes pas dans un magasin de vêtements.");
	if (BizInfo[id][bizType] != BIZ_CLOTHES)		
        return SendErrorMessage(playerid, "Vous n'êtes pas dans un magasin de vêtements.");
	if (GetCash(playerid) < 100)					
        return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent. ($100 requis)");

	if (PlayerInfo[playerid][pSex] == 1) // Homme
	{
		new mes[sizeof(male_skins)*16];
		for(new i = 0; i < sizeof(male_skins); i++)
		{
			format(mes, sizeof(mes), "%s%i\tID: %i\n", mes, male_skins[i], male_skins[i]); 
		}
		Dialog_Show(playerid, MODEL_SEL_BUYCLOTHES, DIALOG_STYLE_PREVIEW_MODEL, "Achat de vêtements :", mes, "Choisir", "Fermer");
	}
	else // Femme
	{
		new mes[sizeof(female_skins)*16];
		for(new i = 0; i < sizeof(female_skins); i++)
		{
			format(mes, sizeof(mes), "%s%i\tID: %i\n", mes, female_skins[i], female_skins[i]); 
		}
		Dialog_Show(playerid, MODEL_SEL_BUYCLOTHES, DIALOG_STYLE_PREVIEW_MODEL, "Achat de vêtements :", mes, "Choisir", "Fermer");
	}
	return 1;
}

CMD:fire(playerid, params[])
{
	if (!FireOnline() || !PoliceOnline())
		return SendErrorMessage(playerid, "Il doit y avoir au moins %i policiers et %i pompiers en service !", NEED_COPS_TO_CRIME, NEED_FIRE_TO_CRIME);

	if (!IsPlayerHasItem(playerid, INV_LIGHTER)) 
		return SendErrorMessage(playerid, "Vous n'avez pas de briquet.");
	if (!IsPlayerHasItem(playerid, INV_GASCAN))  
		return SendErrorMessage(playerid, "Vous n'avez pas de jerricane.");
	if (pTemp[playerid][pFireTime] > 0)
		return SendErrorMessage(playerid, "Vous êtes déjà en train d'allumer un incendie.");

	new type = GetNearBuilding(playerid, 0.0, 2.0), building;
	switch(type)
	{
		case 0:
		{
			building = GetNearOutsideHouse(playerid);
			if (building == -1 || (gettime() - HouseInfo[building][hFireTimer] <= 0))
				return SendErrorMessage(playerid, "Essayez plus tard !");
		}
		case 1:
		{
			building = GetNearOutsideGarage(playerid);
			if (building == -1 || (gettime() - BizInfo[building][bFireTimer] <= 0))
				return SendErrorMessage(playerid, "Essayez plus tard !");
		}
		case 2:
		{
			building = GetNearOutsideBiz(playerid);
			if (building == -1 || (gettime() - GarageInfo[building][gFireTimer] <= 0))
				return SendErrorMessage(playerid, "Essayez plus tard !");
		}
		default: 
			return SendErrorMessage(playerid, "Approchez-vous d'une entrée de bâtiment.");
	}

	if (building == -1)
		return SendErrorMessage(playerid, "Approchez-vous d'une entrée de bâtiment.");

	pTemp[playerid][pFireTime] = 30;
	SetPVarInt(playerid, #firetype, type);
	SetPVarInt(playerid, #firebuildID, building);

	SendSyntaxMessage(playerid, "Vous avez commencé à allumer un incendie. Utilisez {FF6347}/stop{FFFFFF} pour annuler.");

	RemovePlayerItem(playerid, INV_GASCAN, 1);

	SavePlayerPos(playerid);
	FreezePlayer(playerid);
	ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.0, 1, 0, 0, 0, 0, 0);
	return 1;
}

CMD:offclothes(playerid, params[])
{
	new race;

	if (PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous ne devez pas être en service.");

	if (PlayerInfo[playerid][pSkin] != GetPlayerSkin(playerid)) 
	{
	    if (!PlayerInfo[playerid][pSkin])
			return SendErrorMessage(playerid, "Vous ne pouvez pas revenir à votre tenue d'origine, car elle est inconnue.");

	    SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
	    SendSyntaxMessage(playerid, "Vous avez remis vos vêtements d'origine.");
	    return 1;
	}

	if (sscanf(params, "d", race))
		return SendHelpMessage(playerid, "/offclothes [1-2] (1 = peau foncée, 2 = peau claire)");
	if (race < 1 || race > 2)
		return SendErrorMessage(playerid, "Type de peau : 1 = foncé / 2 = clair");

	switch (PlayerInfo[playerid][pSex])
	{
		case 1: // Homme
		{
			switch (race)
			{
				case 1: 
				{
					SetPlayerSkin(playerid, 18);
					SendSyntaxMessage(playerid, "Vous avez retiré vos vêtements.");
				}
				case 2:
				{
					switch (random(3))
					{
						case 0: SetPlayerSkin(playerid, 45);
						case 1: SetPlayerSkin(playerid, 97);
						case 2: SetPlayerSkin(playerid, 154);
					}
					SendSyntaxMessage(playerid, "Vous avez retiré vos vêtements.");
				}
			}
		}
		case 2: // Femme
		{
			switch (race)
			{
				case 1:
				{
					SetPlayerSkin(playerid, 139);
					SendSyntaxMessage(playerid, "Vous avez retiré vos vêtements.");
				}
				case 2:
				{
					switch (random(3))
					{
						case 0: SetPlayerSkin(playerid, 251);
						case 1: SetPlayerSkin(playerid, 140);
						case 2: SetPlayerSkin(playerid, 138);
					}
					SendSyntaxMessage(playerid, "Vous avez retiré vos vêtements.");
				}
			}
		}
	}
	return 1;
}

CMD:setstyle(playerid, params[])
{
	new option, secoption;

	if (sscanf(params, "iI(-1)", option, secoption))
		return SendHelpMessage(playerid, "/setstyle (1 - Style de marche | 2 - Style de parole | 3 - Style de combat | 4 - Style du compteur)");

	switch (option)
	{
		case 1: // Style de marche
		{
			new style;
			if (sscanf(params, "{d}d", style))
				return SendHelpMessage(playerid, "/setstyle 1 [style de marche (0-17)]");
			if (style < 0 || style > 17)
				return SendErrorMessage(playerid, "Le style de marche doit être entre 0 et 17.");

			PlayerInfo[playerid][pWalk] = style;
			if (style == 0)
				SendSyntaxMessage(playerid, "Vous avez désactivé votre style de marche.");
			else
				SendSyntaxMessage(playerid, "Style de marche changé en %i.", style);
			return 1;
		}
		case 2: // Style de parole
		{
			new style;
			if (sscanf(params, "{d}d", style))
				return SendHelpMessage(playerid, "/setstyle 2 [style de parole (0-9)]");
			if (style < 0 || style > 9)
				return SendErrorMessage(playerid, "Le style de parole doit être entre 0 et 9.");

			PlayerInfo[playerid][pTalk] = style;
			if (style == 0)
				SendSyntaxMessage(playerid, "Vous avez désactivé votre animation de parole.");
			else
				SendSyntaxMessage(playerid, "Style de parole changé en %i.", style);
			return 1;
		}
		case 3: // Style de combat
		{
			if (PlayerInfo[playerid][pPremium] < 1)
				return SendErrorMessage(playerid, "Changer de style de combat est réservé aux comptes Premium.");

			new style;
			if (sscanf(params, "{i}i", style))
				return SendHelpMessage(playerid, "/setstyle 3 [style de combat (1-5)]");
			if (style < 1 || style > 5)
				return SendErrorMessage(playerid, "Le style de combat doit être entre 1 et 5.");

			SetPlayerFightingStyle(playerid, fight_id[style-1]);
			PlayerInfo[playerid][pFightStyle] = fight_id[style-1];
			SendSyntaxMessage(playerid, "Style de combat changé en %s.", fight_name[style-1]);
			return 1;
		}
		case 4: // Style du HUD
		{
			if (PlayerInfo[playerid][pPremium] < 1)
				return SendErrorMessage(playerid, "Changer le style du compteur est réservé aux comptes Premium.");

			new style;
			if (sscanf(params, "{d}d", style))
				return SendHelpMessage(playerid, "/setstyle 4 [style de compteur (1-4)]");
			if (style < 1 || style > 4)
				return SendErrorMessage(playerid, "Le style du compteur doit être entre 1 et 4.");

			PlayerInfo[playerid][pHud] = style - 1;
			HideHudTextDraw(playerid);
			if (!(PlayerInfo[playerid][pSettings] & togHUD))
				ShowHudTextDraw(playerid);

			SendSyntaxMessage(playerid, "Style du compteur changé en %i.", style);
			return 1;
		}
		default:
			return SendErrorMessage(playerid, "Option invalide. 1 = Marche, 2 = Parole, 3 = Combat, 4 = Compteur.");
	}
	return 1;
}