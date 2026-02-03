CMD:takejob(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return 1;

	new i = Job_Nearest(playerid);
	if (i == -1)
		return 1;

	new title[32+22];
	format(title, sizeof(title), "%s", GetJobName(JobsInfo[i][jobType]));

	static const strdlg[] = "%s\nPostuler pour le travail\nAide sur les commandes\n\n{FFC46A}Démissionner";
	new mes[sizeof(strdlg) + 36];

	if (JobsInfo[i][jobType] == JOB_PIZZA)
	{
		format(mes, sizeof(mes), strdlg, "Peut être combiné avec un autre travail");
		format(title, sizeof(title), "%s - Travail secondaire", title);
	}
	else
	{
		format(mes, sizeof(mes), strdlg, "Ne peut pas être combiné avec un autre travail");
		format(title, sizeof(title), "%s - Travail principal", title);
	}

	Dialog_Show(playerid, TakeJobMenu, DIALOG_STYLE_TABLIST_HEADERS, title, mes, "Choisir", "Annuler");
	return 1;
}

CMD:quitjob(playerid, params[])
{
	new job = PlayerInfo[playerid][pJob];

	if (job == JOBLESS)
		return SendErrorMessage(playerid, "Vous êtes sans emploi.");
	if (PlayerInfo[playerid][pPremium] < 2 && PlayerInfo[playerid][pCareer] < 2)
		return SendErrorMessage(playerid, "Vous pouvez quitter votre travail seulement après deux heures.");
	if (pTemp[playerid][pJob])
		return SendErrorMessage(playerid, "Finissez votre travail en cours, rendez votre véhicule de travail (/unrcj).");

	PlayerInfo[playerid][pJob] = 0;
	PlayerInfo[playerid][pCareer] = 0;

	pTemp[playerid][pJob] = 0;
	pTemp[playerid][pJobTime] = 0;

	SQL_SetInteger("users", "job", 0, PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "career", 0, PlayerInfo[playerid][pID]);
	
	SendSyntaxMessage(playerid, "Vous avez quitté votre travail de %s%s", GetJobName(job), (job == JOB_TRUCKER) ? (". Votre rang est réinitialisé.") : ("."));
	return 1;
}

CMD:quitsidejob(playerid, params[])
{
	new job = PlayerInfo[playerid][pSideJob];

	if (job == JOBLESS)
		return SendErrorMessage(playerid, "Vous n'avez pas de travail secondaire.");

	SendSyntaxMessage(playerid, "Vous avez quitté le travail %s.", GetJobName(job));
	PlayerInfo[playerid][pSideJob] = 0;
	return 1;
}

CMD:jobhelp(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] == JOBLESS)
		return SendErrorMessage(playerid, "Vous êtes sans emploi.");

	new mes[512];
	SendSyntaxMessage(playerid, "[TRANSPORT] Louer un véhicule de travail - /rentcarjob (/rcj), rendre - /unrentcarjob (/unrcj).");

	if (PlayerInfo[playerid][pSideJob] == JOB_PIZZA)
		SendSyntaxMessage(playerid, "[LIVREUR DE PIZZAS] Livrer des pizzas - /pizzajob. Tapez /quitsidejob pour quitter ce travail.");

	if (PlayerInfo[playerid][pJob] == JOB_CARJACKER)
	{
		format(mes, sizeof(mes), "{ffffff}Vous êtes voleur de voitures. Heures de travail : %i.\n\n/delivercar - Démontage de pièces précieuses.\n/dropoff - Se débarrasser du véhicule.\n/leavemission - Abandonner la mission.\n\n/vbl (n)oisy - Effraction bruyante.\n/vbl (q)uite - Effraction discrète.\n\n/en(gine) - Commencer à forcer le moteur.\n/uns(cramble) - Décoder le mot lors du forçage.", PlayerInfo[playerid][pCareer]);
	}
	else if (PlayerInfo[playerid][pJob] == JOB_TAXI)
	{
		format(mes, sizeof(mes), "{ffffff}Vous êtes chauffeur de taxi. Heures de travail : %i.\n\n/taxi duty - Commencer/terminer votre service.\n/taxi fare - Modifier votre tarif.\n/taxi accept - Accepter une course.\n/address - Marquer une adresse sur la carte.\n/street - Connaitre votre rue actuelle.", PlayerInfo[playerid][pCareer]);
	}
	else if (PlayerInfo[playerid][pJob] == JOB_MECHANIC)
	{
		format(mes, sizeof(mes), "{ffffff}Vous êtes mécanicien. Heures de travail : %i.\n\n/refill - Faire le plein d'un véhicule.\n/repair - Réparer un véhicule.\n/paintcar - Peindre un véhicule.\n/buycomp - Acheter des composants.\n/mycomp - Vérifier les composants du véhicule.\n/attach - Remorquer un véhicule.", PlayerInfo[playerid][pCareer]);
	}
	else if (PlayerInfo[playerid][pJob] == JOB_TRUCKER)
	{
		format(mes, sizeof(mes), "{ffffff}Vous êtes chauffeur routier. Heures de travail : %i.\n\n/cargo - Gérer la marchandise.\n/trailer - Gérer la remorque.\n/tpda - Menu routier.\n/industry - Informations sur l'entreprise.\n\n", PlayerInfo[playerid][pCareer]);
		
		switch(PlayerInfo[playerid][pCareer])
		{
		    case 0..11:
		        strcat(mes, "Véhicules disponibles : Picador, Sadler, Bobcat, Walton, Yosemite.");
		    case 12..23:
		        strcat(mes, "Véhicules disponibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito.");
		    case 24..31:
		        strcat(mes, "Véhicules disponibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.");
		    case 32..47:
		    {
				strcat(mes, "Véhicules disponibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar.");
		    }
		    case 48..58:
		    {
				strcat(mes, "Véhicules disponibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed.");
		    }
		    default:
		    {
				strcat(mes, "Véhicules disponibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\n");
                strcat(mes, "Benson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed, Remorques.");
		    }
		}
	}

	Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Commandes du métier", mes, "Retour", "Fermer");
	return 1;
}

ALTX:rentcarjob("/rcj");
CMD:rentcarjob(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return 1;
	
	if (pTemp[playerid][pJob])
		return SendErrorMessage(playerid, "Vous avez déjà un véhicule de travail ! Utilisez /unrentcarjob (/unrcj) pour le rendre.");
	if (!IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "Vous devez être dans un véhicule de travail !");
	
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPizzaCar(vehicle) && PlayerInfo[playerid][pJob] == JOBLESS)
		return SendErrorMessage(playerid, "Vous êtes sans emploi.");
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
		return SendErrorMessage(playerid, "Vous devez être au volant du véhicule.");

	if (VehicleInfo[vehicle][carRent])
		return SendErrorMessage(playerid, "Ce véhicule est déjà utilisé.");
	if (pTemp[playerid][pRentveh])
		return SendErrorMessage(playerid, "Annulez d'abord votre location actuelle avec /unrentcarjob (/unrcj) !");

	if (IsTruckCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TRUCKER && GetAvailableTruck(playerid, GetVehicleModel(vehicle)))
	{
		pTemp[playerid][pJob] = 1;
		pTemp[playerid][pJobTime] = gettime();
	}
	else if (IsTaxiCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TAXI)
	{
		pTemp[playerid][pJob] = 1;
		pTemp[playerid][pJobTime] = gettime();
	}
	else if (IsMechanicCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_MECHANIC)
	{
		pTemp[playerid][pJob] = 1;
	}
	else if (IsTrashMasterCar(vehicle) && PlayerInfo[playerid][pJob] == JOB_TRASHMASTER)
	{
		pTemp[playerid][pJob] = 1;
		Trash_InitPlayer(playerid);
	}
	else if (IsPizzaCar(vehicle) && PlayerInfo[playerid][pSideJob] == JOB_PIZZA)
	{
		pTemp[playerid][pJob] = 1;
	}
	else
	{
		SendErrorMessage(playerid, "Type de véhicule incorrect !");
		if (PlayerInfo[playerid][pJob] == JOB_TRUCKER)
			SendSyntaxMessage(playerid, "Utilisez /jobhelp pour connaître les véhicules disponibles !");
		return 1;
	}

	pTemp[playerid][pRentveh] = vehicle;
	VehicleInfo[vehicle][carRent] = PlayerInfo[playerid][pID];

	SendHelpMessage(playerid, "Utilisez /jobhelp pour voir les commandes disponibles.");
	SendSyntaxMessage(playerid, "Vous avez pris un véhicule de travail %s. Commandes disponibles : /unrentcarjob (/unrcj), /lock.", VehicleNames[GetVehicleModel(vehicle)-400]);
	SendSyntaxMessage(playerid, "Attention ! Si vous n'utilisez pas votre véhicule pendant 10 minutes, il sera supprimé.");

	return 1;
}

ALTX:unrentcarjob("/unrcj");
CMD:unrentcarjob(playerid, params[])
{
	if (!pTemp[playerid][pRentveh])
		return 1;

	SendSyntaxMessage(playerid, "Vous avez rendu le véhicule de travail.");
	EndJob(playerid);
	return 1;
}


EndJob(playerid) {
	// Ñäåëàòü òàê, ÷òîáû íå âûãîíÿëî è íåëüçÿ áûëî çàâåñòè äâèãàòåëü
	new vehicle = pTemp[playerid][pRentveh];
	
	if (PlayerInfo[playerid][pJob] == JOB_TRASHMASTER && pTemp[playerid][pJob])
	{
		Trash_ResetPlayer(playerid, 1);
		EndTrashJob(playerid);
	}
	
	pTemp[playerid][pRentveh] = 0;
    VehicleInfo[vehicle][carRent] = 0;

	pTemp[playerid][pJob] = 0;
	pTemp[playerid][pJobTime] = 0;

    RespawnVehicle(vehicle);
	return 1;
}

CMD:buydrugs(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
	
	if (!PlayerInfo[playerid][pFaction])
		return SendErrorMessage(playerid, "Vous n'appartenez à aucune faction officielle.");
	if (PlayerInfo[playerid][pSideJob] != JOB_DRUGDEALER)
		return SendErrorMessage(playerid, "Vous n'avez pas accès à l'achat de drogues.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA)
		return 1;

	if (!PlayerToPoint(2.0, playerid, 2705.23, -2550.89, 13.634))
	{
	    Waypoint_Set(playerid, 2705.23, -2550.89, 13.634);
	    return SendErrorMessage(playerid, "Vous devez être dans les docks pour acheter.");
	}

    Dialog_Show(playerid, BuyDrugs, DIALOG_STYLE_TABLIST, "Achat de drogues", "Graines de cannabis\t50$ par unité\nLSD\t150$ par gramme\nHéroïne\t100$ par gramme\nCocaïne\t200$ par gramme\nÉphédrine\t50$ par unité", "Acheter", "Annuler");
	return 1;
}

CMD:buygun(playerid, params[])
{
    new bizid = -1;

	if ((bizid = GetNearInsideBiz(playerid)) != -1 && BizInfo[bizid][bizType] == BIZ_AMMO)
		return BuyGunMenu(playerid);

	new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction])
		return SendErrorMessage(playerid, "Vous n'appartenez à aucune faction officielle.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER)
		return SendErrorMessage(playerid, "Vous n'avez pas accès à l'achat d'armes.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA)
		return 1;

	if (!PlayerToPoint(2.0, playerid, 2705.23, -2550.89, 13.634))
	{
	    Waypoint_Set(playerid, 2705.23, -2550.89, 13.634);
	    return SendErrorMessage(playerid, "Vous devez être dans les docks pour acheter.");
	}

	new title[56];
	format(title, sizeof(title), "Votre faction possède %i point(s).", Factions[faction][fPoint]);
	Dialog_Show(playerid, BuyGunMain, DIALOG_STYLE_LIST, title, "- Armes\n- Munitions", "Choisir", "Annuler");
	return 1;
}

CMD:stealradio(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);

    if (!vehicle || !IsDriver(playerid))
		return SendErrorMessage(playerid, "Vous devez être assis sur le siège conducteur.");
	if (!VehicleInfo[vehicle][carOwnerID])
		return SendErrorMessage(playerid, "La radio ne peut être volée que depuis un véhicule personnel.");
    if (!VehicleInfo[vehicle][carRadio])
		return SendErrorMessage(playerid, "Il n'y a pas de radio dans ce véhicule.");
    if (IsPlayerHasItem(playerid, INV_CARSOUND))
		return SendErrorMessage(playerid, "Vous possédez déjà une radio dans votre inventaire.");
    if (GiveInvItem(playerid, INV_CARSOUND, 1) == -1)
		return SendErrorMessage(playerid, "Votre inventaire est plein.");

	VehicleInfo[vehicle][carRadio] = 0;
	
	new mes[128];
	format(mes, sizeof(mes), "retire la radio du véhicule %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
	cmd::me(playerid, mes);
	return 1;
}

CMD:factionjob(playerid, params[])
{
	if (!PlayerInfo[playerid][pFaction])
		return SendErrorMessage(playerid, "Vous devez faire partie d'une faction officielle gang/mafia.");
	if (Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_GANG && Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_MAFIA)
		return SendErrorMessage(playerid, "Vous devez être membre d'un gang ou d'une mafia officielle.");
    if (!isLeader(playerid))
		return SendErrorMessage(playerid, "Cette commande est réservée uniquement au leader du gang ou de la mafia.");

	new userid, level;

	if (sscanf(params, "ud", userid, level))
		return SendHelpMessage(playerid, "/factionjob [ID/Nom du joueur] [0 - retirer l'accès | 1 - accès achat drogue | 2 - accès achat armes]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);
	if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction])
		return SendErrorMessage(playerid, "Ce joueur n'appartient pas à votre organisation.");
    if (level > 2 || level < 0)
		return SendErrorMessage(playerid, "Le paramètre doit être entre 0 et 2.");
		
	if (!level)
	{
	    if (PlayerInfo[userid][pSideJob] == 0)
			return SendErrorMessage(playerid, "Le joueur n'avait aucun accès de travail.");

		SendSyntaxMessage(userid, "Le leader de votre faction %s vous a retiré l'accès au métier %s.", GetNameEx(playerid), GetJobName(PlayerInfo[userid][pSideJob]));
		SendSyntaxMessage(playerid, "Vous avez retiré l'accès au métier %s de %s.", GetJobName(PlayerInfo[userid][pSideJob]), GetNameEx(userid));
        PlayerInfo[userid][pSideJob] = 0;
		return 1;
	}
	
    PlayerInfo[userid][pSideJob] = level + 3;
	SendSyntaxMessage(userid, "Le leader de votre faction %s vous a donné l'accès au métier %s.", GetNameEx(playerid), GetJobName(PlayerInfo[userid][pSideJob]));
	SendSyntaxMessage(playerid, "Vous avez donné l'accès au métier %s à %s.", GetJobName(PlayerInfo[userid][pSideJob]), GetNameEx(userid));
	return 1;
}