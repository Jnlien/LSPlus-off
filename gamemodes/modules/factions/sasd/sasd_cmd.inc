CMD:taser(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))	return SendErrorMessage(playerid, "Vous devez être policier ou agent pénitentiaire.");
	if (!PlayerInfo[playerid][pOnDuty])							return SendErrorMessage(playerid, "Vous devez d'abord commencer votre service.");

	if (!pTemp[playerid][pTaser])
	{
		if (PlayerInfo[playerid][pPlayerWeapon] != 0) cmd::putgun(playerid, "");
		GivePlayerWeaponEx(playerid, 23, 1, 1, 0);
		pTemp[playerid][pTaser] = 1;
		SetPlayerChatBubble(playerid, "sort un taser de sa ceinture tactique.", COLOR_PURPLE, 20.0, 4000);
		SM1(playerid, "Vous avez sorti votre taser.");
	    return 1;
	}

	ResetPlayerWeapons(playerid);

	if (PlayerInfo[playerid][pPlayerWeapon] != 0) RemovePlayerAttachedObject(playerid, ATTACH_HANDS);

	PlayerInfo[playerid][pPlayerWeapon] = 0;
	PlayerInfo[playerid][pPlayerAmmo] = 0;
	PlayerInfo[playerid][pAmmoType] = 0;
	PlayerInfo[playerid][pWepSerial] = 0;
	
    pTemp[playerid][pTaser] = 0;
	SetPlayerChatBubble(playerid, "range son taser sur sa ceinture tactique.", COLOR_PURPLE, 20.0, 4000);
	SM1(playerid, "Vous avez rangé votre taser.");
	return 1;
}

CMD:trace(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))		return SendErrorMessage(playerid, "Vous devez être membre des forces de l'ordre.");
    if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Vous devez d'abord être en service.");

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid))				return SendNotInVehicle(playerid);
	if (VehicleInfo[vehicle][carFaction] != faction)	return SendErrorMessage(playerid, "Vous devez être dans un véhicule de service.");

	new trace, targetid;

	if (sscanf(params,"d", trace))								return SendHelpMessage(playerid, "/trace [numéro de téléphone]");
	if (pTemp[playerid][pTracePlayer] != INVALID_PLAYER_ID)		return SendErrorMessage(playerid, "Vous suivez déjà un numéro (/traceoff pour arrêter).");
	if (trace == PlayerInfo[playerid][pNumber])					return 1;
    if ((targetid = GetNumberOwner(trace)) == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Numéro introuvable dans la base de données.");
    if (PlayerInfo[targetid][pJailed] != 0)						return SendErrorMessage(playerid, "Le propriétaire de ce numéro est actuellement détenu.");

	SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_POLICECHAT, "** HQ: %s %s a lancé une localisation sur le numéro %i. **", GetRankName(playerid), GetNameEx(playerid), trace);

	Dialog_Show(playerid, ShowTrace, DIALOG_STYLE_MSGBOX, "{8D8DFF}Système de localisation", "{B4B5B7}La localisation a commencé...", "Fermer", "");

	pTemp[playerid][pTracePlayer] = targetid;
	pTemp[playerid][pTraceStage] = 0;

	new Float:x, Float:y, Float:z;
	
	if (GetWorld(targetid) || GetInterior(targetid))
	{
		x = PlayerInfo[targetid][pLocationPos][0];
		y = PlayerInfo[targetid][pLocationPos][1];
		z = PlayerInfo[targetid][pLocationPos][2];
	}
	else GetPlayerPos(targetid, x, y, z);

	pTemp[playerid][pTraceDist][0] = GetDistanceBetweenPoints(x, y, z, 1459.3712, -783.0401, 93.6459);
	pTemp[playerid][pTraceDist][1] = GetDistanceBetweenPoints(x, y, z, 2667.2908, -1957.5245, 13.5469);
	pTemp[playerid][pTraceDist][2] = GetDistanceBetweenPoints(x, y, z, 923.6055, -1893.5146, 1.5319);
	return 1;
}

CMD:traceoff(playerid, params[])
{
	if (pTemp[playerid][pTracePlayer] == INVALID_PLAYER_ID)
		return SendErrorMessage(playerid, "Vous n'êtes pas dans le système de localisation.");

	if (pTemp[playerid][pTraceGZ] != -1)
		GangZoneDestroy(pTemp[playerid][pTraceGZ]);
    
	pTemp[playerid][pTraceGZ] = -1;
    pTemp[playerid][pTracePlayer] = INVALID_PLAYER_ID;
    pTemp[playerid][pTraceStage] = 0;
    
	SendSyntaxMessage(playerid, "Vous avez quitté le système de localisation.");
	return 1;
}

CMD:fine(playerid, params[])
{
	new amount, reason[64], userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsPlayerLogged(userid))								return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))				return SendNotNearPlayer(playerid);
	if (!IsFactionPolice(faction))								return SendErrorMessage(playerid, "Vous devez être policier.");
	if (sscanf(params, "uds[64]", userid, amount, reason))		return SendHelpMessage(playerid, "/fine [ID/nom du joueur] [montant de l'amende ($1-$5.000)] [motif]");
	if (!PlayerInfo[playerid][pOnDuty])							return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (strlen(reason) < 1 || strlen(reason) > 63)				return SendErrorMessage(playerid, "Le motif ne peut dépasser 63 caractères.");
	if (amount < 1 || amount > 5000)							return SendErrorMessage(playerid, "Le montant de l'amende doit être compris entre $1 et $5000.");
	if (!GivePlayerTicket(playerid, userid, amount, reason))	return SendErrorMessage(playerid, "Le joueur a déjà atteint le nombre maximal d'amendes.");

	new mes[128];
	format(mes, sizeof(mes), "a donné une contravention de $%s à %s. Motif : \"%s\".", FormatNumber(amount), GetNameWithMask(userid), reason);
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

	SM1(playerid, "Vous avez donné une contravention à %s pour \"%s\" d’un montant de $%s.", GetNameWithMask(userid), reason, FormatNumber(amount));
	SendSpecialMessage(userid, "%s %s vous a donné une amende de $%s. Motif : \"%s\".", GetRankName(playerid), GetNameWithMask(playerid), FormatNumber(amount), reason);
	SendHelpMessage(userid, "Utilisez \"/tickets\" pour voir la liste de vos amendes.");

	pTemp[playerid][pPoliceFines]++;
	return 1;
}

ALTX:vehiclefine("/vfine");
CMD:vehiclefine(playerid, params[])
{
	new amount, vehicle, vehiclenum[12], reason[64], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))									return SendErrorMessage(playerid, "Vous devez être officier de police.");
	if (sscanf(params, "s[12]is[64]", vehiclenum, amount, reason))	return SendHelpMessage(playerid, "/vehiclefine [plaque d'immatriculation (/checkplate)] [montant de l'amende ($1-$5.000)] [motif]");
	if ((vehicle = FindVehicleByPlate(vehiclenum)) == -1)			return SendErrorMessage(playerid, "Véhicule non trouvé.");
	if (!PlayerInfo[playerid][pOnDuty])								return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (0 == GetNearVehicle(playerid, 5.0, vehicle))					return SendErrorMessage(playerid, "Vous devez être à proximité du véhicule.");
	if (VehicleInfo[vehicle][carRegistered] != 1)					return SendErrorMessage(playerid, "Ce véhicule n’est pas enregistré.");
	if (strlen(reason) < 1 || strlen(reason) > 63)					return SendErrorMessage(playerid, "Le motif doit faire entre 1 et 63 caractères.");
	if (amount < 1 || amount > 5000)									return SendErrorMessage(playerid, "Le montant de l’amende doit être compris entre $1 et $5.000.");
	if (!GiveVehicleTicket(playerid, vehicle, amount, reason))		return SendErrorMessage(playerid, "Ce véhicule a déjà atteint le nombre maximal d’amendes.");

	new mes[128];
	format(mes, sizeof(mes), "a émis une contravention pour %s d’un montant de $%s.", VehicleNames[GetVehicleModel(vehicle)-400], FormatNumber(amount));
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

	new userid = GetVehicleOwner(vehicle);
	if (userid != INVALID_PLAYER_ID)
	{
		SendSpecialMessage(userid, "%s %s a émis une contravention sur votre %s d’un montant de $%s. Motif : \"%s\".", GetRankName(playerid), GetNameEx(playerid), VehicleNames[GetVehicleModel(vehicle)-400], FormatNumber(amount), reason);
		SendHelpMessage(userid, "Utilisez /vtickets pour consulter vos amendes de véhicule.");
	}

	SM1(playerid, "Vous avez émis une contravention \"%s\" sur %s pour un montant de $%s.", reason, VehicleNames[GetVehicleModel(vehicle)-400], FormatNumber(amount));

	pTemp[playerid][pPoliceFines]++;
	return 1;
}

CMD:checkfines(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Vous devez être un policier.");
    if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
		
    new userid;

	if (sscanf(params,"u",userid))					return SendHelpMessage(playerid, "/checkfines [ID/nom du joueur]");
	if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);

	new query[255];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `userid`=%i AND `paid`=0 LIMIT %i", PlayerInfo[userid][pID], MAX_TICKETS);
	mysql_tquery(dbHandle, query, "ShowPlayersTickets", "ii", playerid, 0);

	SetPVarInt(playerid, #tickets_show, userid);
	SetPVarInt(playerid, #ticketsuse, 1);
	return 1;
}

CMD:checkvfines(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Vous devez être un policier.");
    if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");

	new vehicle;

    if (isnull(params))								return SendHelpMessage(playerid, "/checkvfines [plaque d'immatriculation (/checkplate)]");
	if (strlen(params) > 12)						return SendErrorMessage(playerid, "Cette plaque d'immatriculation n'existe pas.");
    if ((vehicle = FindVehicleByPlate(params)) == -1) return SendErrorMessage(playerid, "Le véhicule n'a pas été trouvé.");

	new query[255];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i AND `paid`=0 LIMIT %i", VehicleInfo[vehicle][carID], MAX_TICKETS);
	mysql_tquery(dbHandle, query, "ShowCarsTickets", "i", playerid);

	SetPVarInt(playerid, #tickets_show, vehicle);
	SetPVarInt(playerid, #ticketsuse, 1);
	return 1;
}

ALTX:doorram("/dram");
CMD:doorram(playerid,params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new building;
	switch(GetNearBuilding(playerid)) {
		case 0: {
			building = GetNearInsideHouse(playerid);
			if(building == -1) return 1;

			for(new i = 0; i < MAX_FURNITURE; i++)
			{
				if (!IsPlayerInRangeOfPoint(playerid, 1.5, HFurn[building][i][fPosX], HFurn[building][i][fPosY], HFurn[building][i][fPosZ]) || !HFurn[building][i][fOn] || !isHouseDoor(HFurn[building][i][fModel]))
					continue;

				if (HFurn[building][i][fOpened]) return SendWarningMessage(playerid, "La porte est déjà ouverte !");

				cmd::server_ame(playerid, "défonce la porte à coup de pied.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,0,1,1);

				SetTimerEx("DoorRamEnd", 1000, false, "iiii", 0, building, i, playerid);
				break;
			}
		}
		case 1: {
			building = GetNearInsideBiz(playerid);
			if(building == -1) return 1;

			for(new i = 0; i < MAX_FURNITURE; i++)
			{
				if (!IsPlayerInRangeOfPoint(playerid, 1.5, BFurn[building][i][fPosX], BFurn[building][i][fPosY], BFurn[building][i][fPosZ]) || !BFurn[building][i][fOn] || !isHouseDoor(BFurn[building][i][fModel]))
					continue;

				if (BFurn[building][i][fOpened]) return SendWarningMessage(playerid, "La porte est déjà ouverte !");

				cmd::server_ame(playerid, "défonce la porte à coup de pied.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,0,1,1);

				SetTimerEx("DoorRamEnd", 1000, false, "iiii", 1, building, i, playerid);
				break;
			}
		}
		case 2: {
			building = GetNearInsideGarage(playerid);
			if(building == -1) return 1;

			for(new i = 0; i < MAX_FURNITURE; i++)
			{
				if (!IsPlayerInRangeOfPoint(playerid, 1.5, GFurn[building][i][fPosX], GFurn[building][i][fPosY], GFurn[building][i][fPosZ]) || !GFurn[building][i][fOn] || !isHouseDoor(GFurn[building][i][fModel]))
					continue;

				if (GFurn[building][i][fOpened]) return SendWarningMessage(playerid, "La porte est déjà ouverte !");

				cmd::server_ame(playerid, "défonce la porte à coup de pied.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,0,1,1);

				SetTimerEx("DoorRamEnd", 1000, false, "iiii", 2, building, i, playerid);
				break;
			}
		}
	}
	return 1;
}

this::DoorRamEnd(type, building, i, playerid)
{
	switch(type)
	{
		case 0: {
			HFurn[building][i][fLocked] = 0;
			HFurn[building][i][fOpened] = 1;

			MoveDynamicObject(HFurn[building][i][fObject],
				HFurn[building][i][fPosX],
				HFurn[building][i][fPosY],
				HFurn[building][i][fPosZ],
				1.5,
				HFurn[building][i][fPosRX],
				HFurn[building][i][fPosRY],
				HFurn[building][i][fPosRZ] - 90.0);
		}
		case 1: {
			BFurn[building][i][fLocked] = 0;
			BFurn[building][i][fOpened] = 1;

			MoveDynamicObject(BFurn[building][i][fObject],
				BFurn[building][i][fPosX],
				BFurn[building][i][fPosY],
				BFurn[building][i][fPosZ],
				1.5,
				BFurn[building][i][fPosRX],
				BFurn[building][i][fPosRY],
				BFurn[building][i][fPosRZ] - 90.0);
		}
		case 2: {
			GFurn[building][i][fLocked] = 0;
			GFurn[building][i][fOpened] = 1;

			MoveDynamicObject(GFurn[building][i][fObject],
				GFurn[building][i][fPosX],
				GFurn[building][i][fPosY],
				GFurn[building][i][fPosZ],
				1.5,
				GFurn[building][i][fPosRX],
				GFurn[building][i][fPosRY],
				GFurn[building][i][fPosRZ] - 90.0);
		}
	}

	ClearAnimations(playerid);
	GameTextForPlayer(playerid, "~w~VOUS ~y~AVEZ ENFONCÉ~w~ LA PORTE", 1000, 1);
	return 1;
}

CMD:ram(playerid,params[])
{
	if (!IsPlayerFactionPolice(playerid)) return SendErrorMessage(playerid, "Vous devez être un membre des forces de l'ordre.");
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez d'abord être en service (/duty).");
		    
	new slot;
	
	if (sscanf(params, "d", slot))
	{
		SendHelpMessage(playerid, "/ram [action]");
		SendSyntaxMessage(playerid, "1 - Crocheter | 2 - Enfoncer | 3 - Poser du C4 | 4 - Forcer la porte d’un véhicule");
		return 1;
	}

	if (slot < 1 || slot > 4)						return 1;
    if (slot < 4 && PlayerInfo[playerid][pRank] < 4) return SendErrorMessage(playerid, "Cette méthode de forçage est accessible à partir du rang 4.");
	if (pTemp[playerid][pRamType]) return SendErrorMessage(playerid, "Vous êtes déjà en train de forcer une porte.");

    new location[MAX_ZONE_NAME];
	GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);

	if (slot == 4) 
	{
		new vehicle = GetNearVehicle(playerid, 2.0);
		if (!vehicle) return SendErrorMessage(playerid, "Aucun véhicule à proximité.");

		if (VehicleInfo[vehicle][carLocked] == 0)  return SendErrorMessage(playerid, "Le véhicule est déjà déverrouillé.");
		if (VehicleInfo[vehicle][carOwnerID] == 0) return SendErrorMessage(playerid, "Ce véhicule appartient à l'État.");

		cmd::server_ame(playerid, "tente de forcer la serrure du véhicule avec un crochet.");
		ApplyAnimation(playerid, "OTB", "betslp_lkabt", 4.0, 1, 0, 0, 0, 0);

		pTemp[playerid][pRamType] = 1;
		pTemp[playerid][pRamTime] = 30;
		pTemp[playerid][pRamHouse] = -1;
		pTemp[playerid][pRamBiz] = -1;
		pTemp[playerid][pRamGarage] = -1;
		pTemp[playerid][pRamVeh] = vehicle;
		return 1;
	}


	new id = -1;
	if ((id = GetNearOutsideHouse(playerid)) != -1)
	{
	    if (!HouseInfo[id][hOwnerID]) return SendErrorMessage(playerid, "La maison appartient à l'État.");
		if (!HouseInfo[id][hLocked] || HouseInfo[id][hDDoor]) return SendErrorMessage(playerid, "La porte est déjà ouverte.");

		switch(slot)
		{
			case 1:
			{
				cmd::server_ame(playerid, "force la serrure de la porte à l'aide d'un crochet.");
				ApplyAnimation(playerid,"OTB","betslp_lkabt", 4.0, 1, 0, 0, 0, 0);

				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 30;
				pTemp[playerid][pRamHouse] = id;
				pTemp[playerid][pRamBiz] = -1;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
			case 2:
			{
				cmd::server_ame(playerid, "détruit la porte avec un coup de pied.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,1,1,1);
				HouseInfo[id][hLocked] = 0;
				HouseInfo[id][hDDoor] = 1;
				Save_House(id);
				UpdateHouseDoor(id);
				GameTextForPlayer(playerid, "~w~VOUS ~y~AVEZ ENFONCÉ~w~ LA PORTE", 1000, 1);
				InfoMSG(playerid, "APPUYEZ SUR ~y~\"F\"~w~ OU SAISISSEZ ~y~/ENTER", 2);

				new query[128];
				format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] a enfoncé la porte (/ram) de la maison [H_MYSQL: %i]", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), HouseInfo[id][hID]);
				insert_log(query, RAM_LOG, 5, PlayerInfo[playerid][pAccID], 0);
			}
			case 3:
			{
				cmd::server_ame(playerid, "pose du C4 sur la porte.");
				OnPlayAnim(playerid, "BOMBER","BOM_Plant",4.0,0,0,0,0,0);
				pTemp[playerid][pRamObj] = CreateDynamicObject(1654, HouseInfo[id][hPosX], HouseInfo[id][hPosY], HouseInfo[id][hPosZ]-0.8, 0.0, 0.0, 90.0, HouseInfo[id][hPosInt], HouseInfo[id][hPosVW]);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 10;
				pTemp[playerid][pRamHouse] = id;
				pTemp[playerid][pRamBiz] = -1;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
		}
		return 1;
	}
	
	else if ((id = GetNearOutsideBiz(playerid)) != -1)
	{
		if (BizInfo[id][bizType] == BIZ_GAS_STATION || BizInfo[id][bizType] == BIZ_AUTOSALON || BizInfo[id][bizType] == BIZ_BOATSALON || BizInfo[id][bizType] == BIZ_FLYSALON || BizInfo[id][bizType] == BIZ_TUNING || BizInfo[id][bizType] == BIZ_ADVERT || BizInfo[id][bizType] == BIZ_LICENSE) return 1;
		if (!BizInfo[id][bizOwnerID])							return SendErrorMessage(playerid, "L'entreprise appartient à l'État.");
		if (!BizInfo[id][bizLocked] || BizInfo[id][bizDDoor])	return SendErrorMessage(playerid, "La porte est déjà ouverte.");

		switch(slot)
		{
			case 1:
			{
				cmd::server_ame(playerid, "force la serrure de la porte à l'aide d'un crochet.");
				ApplyAnimation(playerid,"OTB","betslp_lkabt", 4.0, 1, 0, 0, 0, 0);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 30;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = id;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
			case 2:
			{
				cmd::server_ame(playerid, "détruit la porte avec un coup de pied.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,1,1,1);
				BizInfo[id][bizLocked] = 0;
				BizInfo[id][bizDDoor] = 1;
				Save_Business(id);
				UpdateBusinessDoor(id);
				GameTextForPlayer(playerid, "~w~VOUS ~y~AVEZ ENFONCÉ~w~ LA PORTE", 1000, 1);
				InfoMSG(playerid, "APPUYEZ SUR ~y~\"F\"~w~ OU SAISISSEZ ~y~/ENTER", 2);

				new query[128];
				format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] a enfoncé la porte (/ram) de l'entreprise [B_MYSQL: %i]", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), BizInfo[id][bizID]);
				insert_log(query, RAM_LOG, 6, PlayerInfo[playerid][pAccID], 0);
			}
			case 3:
			{
				cmd::server_ame(playerid, "pose du C4 sur la porte.");
				OnPlayAnim(playerid, "BOMBER","BOM_Plant",4.0,0,0,0,0,0);
				pTemp[playerid][pRamObj] = CreateDynamicObject(1654, BizInfo[id][bizPosX], BizInfo[id][bizPosY], BizInfo[id][bizPosZ]-0.8, 0.0, 0.0, 90.0, BizInfo[id][bizPosVW], BizInfo[id][bizPosInt]);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 10;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = id;
				pTemp[playerid][pRamGarage] = -1;
				pTemp[playerid][pRamVeh] = -1;
			}
		}
		return 1;
	}

	else if ((id = GetNearOutsideGarage(playerid)) != -1)
	{
		if (!GarageInfo[id][garageOwner])									return SendErrorMessage(playerid, "Le garage appartient à l'État.");
		if (!GarageInfo[id][garageLocked] || GarageInfo[id][garageDDoor])	return SendErrorMessage(playerid, "La porte est déjà ouverte.");

		switch(slot)
		{
			case 1:
			{
				cmd::server_ame(playerid, "force la serrure de la porte à l'aide d'un crochet.");
				ApplyAnimation(playerid,"OTB","betslp_lkabt", 4.0, 1, 0, 0, 0, 0);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 30;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = -1;
				pTemp[playerid][pRamGarage] = id;
				pTemp[playerid][pRamVeh] = -1;
			}
			case 2:
			{
				cmd::server_ame(playerid, "détruit la porte avec un coup de pied.");
				ApplyAnimation(playerid,"FIGHT_E","FightKick_B",4.1,0,1,1,1,1);
				GarageInfo[id][garageLocked] = 0;
				GarageInfo[id][garageDDoor] = 1;
				Save_Garage(id);
				UpdateGarageDoor(id);
				GameTextForPlayer(playerid, "~w~VOUS ~y~AVEZ ENFONCÉ~w~ LA PORTE", 1000, 1);
				InfoMSG(playerid, "APPUYEZ SUR ~y~\"F\"~w~ OU SAISISSEZ ~y~/ENTER", 2);

				new query[128];
				format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] a enfoncé la porte (/ram) du garage [B_MYSQL: %i]", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), GarageInfo[id][garageID]);
				insert_log(query, RAM_LOG, 6, PlayerInfo[playerid][pAccID], 0);
			}
			case 3:
			{
				cmd::server_ame(playerid, "pose du C4 sur la porte.");
				OnPlayAnim(playerid, "BOMBER","BOM_Plant",4.0,0,0,0,0,0);
				pTemp[playerid][pRamObj] = CreateDynamicObject(1654, GarageInfo[id][garagePlayer_Enter][0], GarageInfo[id][garagePlayer_Enter][1], GarageInfo[id][garagePlayer_Enter][2]-0.8, 0.0, 0.0, 90.0, GarageInfo[id][garageEnterVW], GarageInfo[id][garageEnterInt]);
				pTemp[playerid][pRamType] = slot;
				pTemp[playerid][pRamTime] = 10;
				pTemp[playerid][pRamHouse] = -1;
				pTemp[playerid][pRamBiz] = id;
				pTemp[playerid][pRamGarage] = id;
				pTemp[playerid][pRamVeh] = -1;
			}
		}		
		return 1;
	}
	else if (slot == 2) cmd::doorram(playerid, "");
	
	return 1;
}

CMD:frisktrunk(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return 1;
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez d'abord être en service.");

	new vehicle = GetNearVehicle(playerid, 4.0);
	if (vehicle == 0)																															return SendErrorMessage(playerid, "Aucun véhicule à proximité.");
	if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))	return SendErrorMessage(playerid, "Ce véhicule n'a pas de coffre.");
	if (!IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carTrunkLock] == 0)																return SendErrorMessage(playerid, "Le coffre de ce véhicule est fermé.");
	if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) 									return SendErrorMessage(playerid, "Vous ne pouvez pas fouiller ce véhicule.");
	
	if (!IsNearTrunk(vehicle, playerid, 2.0)) 	return SendErrorMessage(playerid, "Vous n'êtes pas près du coffre !");

	PrintVehInv(playerid, vehicle, 1);
	return 1;
}

CMD:friskgb(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid)) return 1;
	if (!PlayerInfo[playerid][pOnDuty])	 return SendErrorMessage(playerid, "Vous devez d'abord être en service.");
	
	new vehicle = GetPlayerVehicleID(playerid);
	if (!vehicle && !IsPlayerInAnyVehicle(playerid))																										return SendErrorMessage(playerid, "Vous devez être dans un véhicule.");
	if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && VehicleInfo[vehicle][carOwnerID] != 0) return SendErrorMessage(playerid, "Vous ne pouvez pas fouiller ce véhicule.");

	PrintGBInv(playerid, vehicle, 1);
	return 1;
}

CMD:friskprop(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid)) return 1;
	if (!PlayerInfo[playerid][pOnDuty])	 return SendErrorMessage(playerid, "Vous devez d'abord être en service.");

	new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison.");
	if (HouseInfo[houseid][hOwnerID] == PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison vous appartient.");
	if (!IsPlayerInsideHouse(playerid, houseid))						return SendErrorMessage(playerid, "Vous devez être à l'intérieur de la maison.");

	PrintHouseInv(playerid, houseid, 1);
	return 1;
}

CMD:frisk(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new userid;

	if (sscanf(params,"u", userid))										 return SendHelpMessage(playerid, "/frisk [ID/nom du joueur]");
	if (!IsPlayerLogged(userid))											 return SendNotConnected(playerid);
	if (userid == playerid)												 return 1;
	if (!PlayerNearPlayer(3.0, playerid, userid))						 return SendNotNearPlayer(playerid);
	if (pTemp[userid][pFriskPlayer] == playerid)							 return SendErrorMessage(playerid, "Vous avez déjà proposé une fouille à ce joueur.");
	if (IsPlayerFactionPolice(playerid) && PlayerInfo[playerid][pOnDuty]) return PrintInvEx(playerid, userid);

	SendWarningMessage(playerid, "Attendez que le joueur accepte ou refuse votre proposition de fouille.");
	SendSyntaxMessage(userid, "Le joueur %s souhaite vous fouiller.", GetNameEx(playerid));
	SendHelpMessage(userid, "Tapez {ffc46a}/friskapprove{FFFFFF} pour accepter ou {ffc46a}/cancelfrisk{FFFFFF} pour refuser la proposition.");

	pTemp[userid][pFriskPlayer] = playerid;
	SetPVarInt(userid, #accept_time, gettime() + 30);
	return 1;
}

CMD:friskapprove(playerid, params[])
{
	if (pTemp[playerid][pFriskPlayer] == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Personne ne vous a proposé de procéder à une fouille.");

	new userid = pTemp[playerid][pFriskPlayer];

	pTemp[playerid][pFriskPlayer] = INVALID_PLAYER_ID;

	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);

	SendSyntaxMessage(playerid, "Vous avez autorisé le joueur %s à effectuer une fouille sur vous.", GetNameEx(userid));

	PrintInvEx(playerid, userid);
	return 1;
}

CMD:cancelfrisk(playerid, params[])
{
	if (pTemp[playerid][pFriskPlayer] == INVALID_PLAYER_ID)	return SendErrorMessage(playerid, "Personne ne vous a proposé de procéder à une fouille.");

	new userid = pTemp[playerid][pFriskPlayer];
	
	pTemp[playerid][pFriskPlayer] = INVALID_PLAYER_ID;
	SendSyntaxMessage(playerid, "Vous avez refusé la proposition de %s pour effectuer une fouille.", GetNameWithMask(userid));
	SendSyntaxMessage(userid, "Le joueur %s a refusé votre proposition pour effectuer une fouille.", GetNameWithMask(playerid));
	return 1;
}

CMD:setplate(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Vous devez être un policier.");

	new vehicleid = GetPlayerVehicleID(playerid);

	if (!vehicleid || VehicleInfo[vehicleid][carFaction] != PlayerInfo[playerid][pFaction])	return SendErrorMessage(playerid, "Vous n'êtes pas dans un véhicule de faction.");

	new plate[24];

	format(plate, sizeof(plate), "%s", GetVehicleNumber());

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `plate` FROM `cars` WHERE `plate` = '%e'", plate);
	mysql_tquery(dbHandle, query, "PoliceSetPlate", "iis", playerid, vehicleid, plate);
	return 1;
}

CMD:zond(playerid, params[])
{
	if (pTemp[playerid][pZond] != 0)															
		return SetPlayerZond(playerid, 0);
	
	if (!IsPlayerFactionPolice(playerid))
		return SendErrorMessage(playerid, "Vous devez être un policier ou un agent pénitentiaire.");

	if (!PlayerInfo[playerid][pSwatDuty])
		return SendErrorMessage(playerid, "Cette fonction est disponible uniquement pour le S.W.A.T.");

	SetPlayerZond(playerid, 1);
	return 1;
}

CMD:flashbang(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid)) 
		return SendErrorMessage(playerid, "Vous devez être un policier ou un agent pénitentiaire.");
	if (!PlayerInfo[playerid][pSwatDuty]) 
		return SendErrorMessage(playerid, "Cette fonction est réservée au S.W.A.T.");

	new id = -1;

	// Vérifie si le joueur est près d'une entreprise
	if ((id = GetNearOutsideBiz(playerid)) != -1 && 
		(BizInfo[id][bizType] != BIZ_AUTOSALON && BizInfo[id][bizType] != BIZ_ADVERT && 
		 BizInfo[id][bizType] != BIZ_TUNING && BizInfo[id][bizType] != BIZ_LICENSE && 
		 BizInfo[id][bizType] != BIZ_BOATSALON && BizInfo[id][bizType] != BIZ_FLYSALON))
	{
		if (BizInfo[id][bizLocked]) 
			return GameTextForPlayer(playerid, "~r~FERMÉ!", 2500, 1);

		SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_POLICECHAT, 
			"* %s %s a utilisé une grenade flashbang.", GetRankName(playerid), GetNameEx(playerid));
		cmd::me(playerid, "retire une grenade flashbang de sa ceinture et la lance à l'intérieur du bâtiment.");

		foreach (new i : Player)
		{
			if (!IsPlayerLogged(i) || !IsPlayerBizInside(i, id)) 
				continue;

			SendSyntaxMessage(i, "Vous avez été aveuglé par une grenade flashbang.");
			GameTextForPlayer(i, "~r~!!! VOUS AVEZ ÉTÉ AVEUGLÉ !!!~n~~n~~n~!!! VOUS AVEZ ÉTÉ AVEUGLÉ !!!~n~~n~~n~!!! VOUS AVEZ ÉTÉ AVEUGLÉ !!!", 12000, 3);
			SetPlayerWeather(i, 36);
			SetPlayerTime(i, 22, 0);
			StopPlayerAnims(i);
			FreezePlayer(i);
			SetPlayerDrunkLevel(i, 50000);
			pTemp[i][pFlashTime] = 15;
		}
		return 1;
	}

	// Vérifie si le joueur est près d'une maison
	if ((id = GetNearOutsideHouse(playerid)) != -1)
	{
		if (HouseInfo[id][hLocked]) 
			return GameTextForPlayer(playerid, "~r~FERMÉ!", 2500, 1);

		SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_POLICECHAT, 
			"* %s %s a utilisé une grenade flashbang.", GetRankName(playerid), GetNameEx(playerid));
		cmd::me(playerid, "retire une grenade flashbang de sa ceinture et la lance à l'intérieur du bâtiment.");

		foreach (new i : Player)
		{
			if (!IsPlayerLogged(i) || !IsPlayerInsideHouse(i, id)) 
				continue;

			SendSyntaxMessage(i, "Vous avez été aveuglé par une grenade flashbang.");
			GameTextForPlayer(i, "~r~!!! VOUS AVEZ ÉTÉ AVEUGLÉ !!!~n~~n~~n~!!! VOUS AVEZ ÉTÉ AVEUGLÉ !!!~n~~n~~n~!!! VOUS AVEZ ÉTÉ AVEUGLÉ !!!", 12000, 3);
			SetPlayerWeather(i, 36);
			SetPlayerTime(i, 22, 0);
			StopPlayerAnims(i);
			FreezePlayer(i);
			SetPlayerDrunkLevel(i, 50000);
			pTemp[i][pFlashTime] = 15;
		}
		return 1;
	}

	SendErrorMessage(playerid, "Utilisez cette commande près de l'entrée d'une maison ou d'une entreprise.");
	return 1;
}

CMD:callsign(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))
		return SendErrorMessage(playerid, "Vous devez être un policier.");

	// Si le joueur est déjà dans un patrouille
	if (pTemp[playerid][pPatrol])
	{
		new id = pTemp[playerid][pPatrol] - 1;

		pTemp[playerid][pPatrol] = 0;

		if (id < 0 || id >= MAX_PATROL || !PatrolInfo[GetPatrolID(playerid)][id][patrulExists])
			return 1;

		if (PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] == playerid)
			PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] = INVALID_PLAYER_ID;

		if (PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] == playerid)
			PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] = INVALID_PLAYER_ID;

		if (PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] == INVALID_PLAYER_ID &&
			PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] == INVALID_PLAYER_ID)
		{
			PatrolInfo[GetPatrolID(playerid)][id][patrulExists] = 0;
		}

		SendFactionMessage(
			PlayerInfo[playerid][pFaction],
			COLOR_POLICECHAT,
			"[DISP] %s %s a quitté la patrouille sous l'indicatif %s",
			GetRankName(playerid),
			GetNameEx(playerid),
			PatrolInfo[GetPatrolID(playerid)][id][patrulName]
		);
		return 1;
	}

	if (!PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");

	if (isnull(params))
		return SendHelpMessage(playerid, "/callsign [indicatif]");

	// Vérifie si l'indicatif existe déjà
	for (new i = 0; i < MAX_PATROL; i++)
	{
		if (!PatrolInfo[GetPatrolID(playerid)][i][patrulExists] ||
			!CompareStrings(params, PatrolInfo[GetPatrolID(playerid)][i][patrulName]))
			continue;

		if (PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0] != INVALID_PLAYER_ID &&
			PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1] != INVALID_PLAYER_ID)
		{
			return SendErrorMessage(playerid, "Cette patrouille est déjà complète.");
		}

		if (PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0] == INVALID_PLAYER_ID)
			PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0] = playerid;
		else
			PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1] = playerid;

		pTemp[playerid][pPatrol] = i + 1;
		SendFactionMessage(
			PlayerInfo[playerid][pFaction],
			COLOR_POLICECHAT,
			"[DISP] %s %s a rejoint la patrouille sous l'indicatif %s",
			GetRankName(playerid),
			GetNameEx(playerid),
			PatrolInfo[GetPatrolID(playerid)][i][patrulName]
		);
		return 1;
	}

	// Création d'une nouvelle patrouille
	new id = -1;
	for (new i = 0; i < MAX_PATROL; i++)
	{
		if (PatrolInfo[GetPatrolID(playerid)][i][patrulExists])
			continue;

		id = i;
		break;
	}

	if (id == -1)
		return SendErrorMessage(playerid, "Le nombre maximum de patrouilles a été atteint.");

	pTemp[playerid][pPatrol] = id + 1;
	PatrolInfo[GetPatrolID(playerid)][id][patrulExists] = 1;
	format(PatrolInfo[GetPatrolID(playerid)][id][patrulName], 32, "%s", params);
	PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][0] = playerid;
	PatrolInfo[GetPatrolID(playerid)][id][patrulOfficer][1] = INVALID_PLAYER_ID;

	SendFactionMessage(
		PlayerInfo[playerid][pFaction],
		COLOR_POLICECHAT,
		"[DISP] %s %s a commencé une patrouille sous l'indicatif %s",
		GetRankName(playerid),
		GetNameEx(playerid),
		PatrolInfo[GetPatrolID(playerid)][id][patrulName]
	);
	return 1;
}

CMD:meg(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerFactionPolice(playerid))
		return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])
		return SendErrorMessage(playerid, "Disponible uniquement dans un véhicule LSPD/SASD.");

	new type;
	if (sscanf(params, "i", type))
	{
		new _mes[556] = 
			"1. Ici la Police de Los Santos, restez où vous êtes !\n"\
			"2. Ici Buzzard 1, nous subissons des tirs au sol !\n"\
			"3. Quatre suspects dans une berline bleue retournant vers South Central.\n"\
			"4. Mayday, mayday ! Buzzard 1 est en train de s'écraser !\n"\
			"5. Ici la LSPD, ne bougez pas... HEY ! QU'EST-CE QUE VOUS FAITES ? TROP BAS, VOUS ALLEZ TOUS NOUS TUER !\n"\
			"6. LSPD, arrêtez immédiatement... êtes-vous fou ? Vous allez tous nous tuer !\n"\
			"7. Ici la Police de Los Santos, tout le monde reste où il est !";

		Dialog_Show(playerid, MPCarVoice, DIALOG_STYLE_LIST, "{8D8DFF}Mégaphone", _mes, "Choisir", "Annuler");
		return SendHelpMessage(playerid, "/meg [ID du son 1 - 7]");
	}

	MegaphoneSoundVoice(playerid, type);
	return 1;
}

CMD:revoke(playerid, params[])
{
	new userid, type[24], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))					
		return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty])				
		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (sscanf(params, "us[24]", userid, type))		
		return SendHelpMessage(playerid, "/revoke [ID/nom du joueur] [carlic | flylic | weapons]");
	if (userid == playerid)							
		return 1;
	if (!IsPlayerLogged(userid))						
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(4.0, playerid, userid))	
		return SendNotNearPlayer(playerid);

	if (CompareStrings(type, "carlic"))
	{
		if (!PlayerInfo[userid][pCarLic])	
			return SendErrorMessage(playerid, "Le joueur ne possède pas de permis de conduire.");

		new mes[128];
		format(mes, sizeof(mes), "retire le permis de conduire de %s.", GetNameWithMask(userid));
		cmd::me(playerid, mes);

		SendSyntaxMessage(userid, "Votre permis de conduire a été retiré par %s.", GetNameWithMask(playerid));
		PlayerInfo[userid][pCarLic] = 0;
		SQL_SetInteger("users", "carlic", 0, PlayerInfo[userid][pID]);
		return 1;
	}

	else if (CompareStrings(type, "flylic"))
	{
		if (!PlayerInfo[userid][pFlyLic])	
			return SendErrorMessage(playerid, "Le joueur ne possède pas de licence de vol.");

		new mes[128];
		format(mes, sizeof(mes), "retire la licence de vol de %s.", GetNameWithMask(userid));
		cmd::me(playerid, mes);

		SendSyntaxMessage(userid, "Votre licence de vol a été retirée par %s.", GetNameWithMask(playerid));
		PlayerInfo[userid][pFlyLic] = 0;
		SQL_SetInteger("users", "flylic", 0, PlayerInfo[userid][pID]);
		return 1;
	}

	else if (CompareStrings(type, "weapons"))
	{
		new count = -1;
		for (new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
			{
				count = i;
				break;
			}
		}
		if (count == -1 && !PlayerInfo[playerid][pPlayerWeapon])	
			return SendErrorMessage(playerid, "Le joueur ne possède pas d'arme.");

		new mes[128];
		format(mes, sizeof(mes), "confisque les armes de %s.", GetNameWithMask(userid));
		cmd::me(playerid, mes);

		SendWarningMessage(userid, "Toutes vos armes ont été confisquées par %s.", GetNameWithMask(playerid));
		ResetPlayerWeaponsEx(userid, 0);
		return 1;
	}
	else 
	{
		SendHelpMessage(playerid, "/revoke [ID/nom du joueur] [carlic | flylic | weapons]");
	}
	return 1;
}

CMD:detain(playerid, params[])
{
	new userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))
		return SendErrorMessage(playerid, "Vous devez être policier ou agent pénitentiaire.");

	if (sscanf(params, "u", userid))
		return SendHelpMessage(playerid, "/detain [ID/nom du joueur]");

	if (userid == playerid)
		return 1;

	if (!PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");

	if (!IsPlayerInAnyVehicle(playerid))
		return SendNotInVehicle(playerid);

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (!PlayerNearPlayer(10.0, playerid, userid))
		return SendNotNearPlayer(playerid);

	if (!IsPlayerInAnyVehicle(userid) || VehicleInfo[GetPlayerVehicleID(userid)][carFaction] != PlayerInfo[playerid][pFaction])
		return SendErrorMessage(playerid, "Le joueur n'est pas dans un véhicule spécial.");

	new mes[128];

	if (!IsPlayerFreeze(userid))
	{
		format(mes, sizeof(mes), "bloque toutes les portes pour %s.", GetNameWithMask(userid));
		FreezePlayer(userid);
	}
	else
	{
		format(mes, sizeof(mes), "%s débloque toutes les portes.", GetNameWithMask(userid));
		UnFreezePlayer(userid);
	}

	cmd::me(playerid, mes);
	return 1;
}

CMD:arrest(playerid, params[])
{
	new userid, time, reason[128], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction)) 
		return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (sscanf(params, "uds[128]", userid, time, reason)) 
		return SendHelpMessage(playerid, "/arrest [ID/nom du joueur] [durée (minutes)] [motif]");
	if (!PlayerInfo[playerid][pOnDuty]) 
		return SendErrorMessage(playerid, "Vous devez être en service.");

	new arrestid = 0;

	if ((arrestid = GetArrestID(playerid)) == 0) 
		return SendErrorMessage(playerid, "Vous devez être près d'une cellule !");
	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return SendNotNearPlayer(playerid);
	if (userid == playerid) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas effectuer cette action sur vous-même.");
	if (PlayerInfo[userid][pJailed]) 
		return SendErrorMessage(playerid, "Le joueur est déjà en détention.");
	if (time < 1 || time > 60) 
		return SendErrorMessage(playerid, "La durée de l'arrestation doit être comprise entre 1 et 60 minutes.");

	Add_Arrest(playerid, userid, time);
	PlayerInfo[userid][pJailed] = arrestid;
	PlayerInfo[userid][pJailTime] = time * 60;
	PlayerInfo[userid][pArrested]++;
	SQL_SetInteger("users", "arrested", PlayerInfo[userid][pArrested], PlayerInfo[userid][pID]);
	AddCriminalRecord(userid, reason, PlayerInfo[playerid][pUsername], PlayerInfo[playerid][pFaction], 1);

	PutPlayerInPrison(userid);
	SendErrorMessage(userid, "Vous avez été arrêté. Durée : %s minutes.", FormatNumber(time));
	pTemp[playerid][pPoliceJails]++;
	return 1;
}

CMD:prison(playerid, params[])
{
	return SendErrorMessage(playerid, "La prison est actuellement désactivée !");
	/*new userid, time, reason[128], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))		
		return SendErrorMessage(playerid, "Vous devez être policier ou agent pénitentiaire.");
	if (sscanf(params, "uds[128]", userid, time, reason))			
		return SendHelpMessage(playerid, "/prison [ID/nom du joueur] [durée (minutes)] [motif]");
	if (!PlayerInfo[playerid][pOnDuty])								
		return SendErrorMessage(playerid, "Vous devez être en service.");
	if (!PlayerToPoint(5.0, playerid, 241.6535, -315.2582, 947.9460))	
		return SendErrorMessage(playerid, "Vous devez être dans la prison.");
	if (!IsPlayerLogged(userid))										
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid))					
		return SendNotNearPlayer(playerid);
	if (time < 1 || time > 1000)										
		return SendErrorMessage(playerid, "La durée doit être comprise entre 1 et 1000 minutes.");
	if (userid == playerid)											
		return 1;

	PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));
	PlayerInfo[userid][pJailed] = PRISON_JAIL;
	PlayerInfo[userid][pJailTime] = time * 60;
	PlayerInfo[userid][pIsolateTime] = 0;

	SendSyntaxMessage(userid, "Vous avez été emprisonné. Durée : %s minutes.", FormatNumber(time));
	SendSyntaxMessage(userid, "Votre numéro de détenu est A%i (/myprisonnumber).", PlayerInfo[userid][pPrisoned] + 100);
	return 1;*/
}

CMD:checkplate(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))
		return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
		return SendErrorMessage(playerid, "Vous devez être à pied à proximité d'un véhicule.");

	new vehicle;

	if ((vehicle = GetNearVehicle(playerid, 2.5)) == 0)
		return SendErrorMessage(playerid, "Aucun véhicule à proximité.");

	if (VehicleInfo[vehicle][carRegistered] == 0)
		SendSyntaxMessage(playerid, "Le numéro d'immatriculation du véhicule %s n'est pas enregistré.", VehicleNames[GetVehicleModel(vehicle) - 400]);
	else
		SendSyntaxMessage(playerid, "Le numéro d'immatriculation du véhicule %s est %s.", VehicleNames[GetVehicleModel(vehicle) - 400], VehicleInfo[vehicle][carPlate]);

	return 1;
}

CMD:cuff(playerid, params[])
{
	new userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))					
		return SendErrorMessage(playerid, "Vous devez être policier ou agent pénitentiaire.");

	if (sscanf(params, "u", userid))					
		return SendHelpMessage(playerid, "/cuff [ID/nom du joueur]");

	if (!PlayerInfo[playerid][pOnDuty])				
		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");

	if (!IsPlayerLogged(userid))						
		return SendNotConnected(playerid);

	if (!PlayerNearPlayer(3.0, playerid, userid))	
		return SendNotNearPlayer(playerid);

	if (userid == playerid)							
		return 1;

	if (pTemp[userid][pCuffed])						
		return SendErrorMessage(playerid, "Le joueur est déjà menotté.");

	if (pTemp[userid][pTaserTime])					
		return SendErrorMessage(playerid, "Le joueur est assommé ou inconscient.");

	GameTextForPlayer(userid, "~r~Menotté", 2500, 3);
	pTemp[userid][pCuffed] = 1;

	SendSyntaxMessage(userid, "%s vous a menotté.", GetNameWithMask(playerid));
	SendSyntaxMessage(playerid, "Vous avez menotté %s.", GetNameWithMask(userid));

	SetPlayerAttachedObject(userid, ATTACH_HANDS, 19418, 6, 0.003999, 0.028000, -0.018000, -12.900005, 0.000000, -65.100006, 0.878000, 1.890999, 1.652001);

	SetPlayerSpecialAction(userid, SPECIAL_ACTION_CUFFED);

	return 1;
}

CMD:uncuff(playerid, params[])
{
	new userid, faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction)) 
		return SendErrorMessage(playerid, "Vous devez être policier ou agent pénitentiaire.");
	if (sscanf(params, "u", userid)) 
		return SendHelpMessage(playerid, "/uncuff [ID/nom du joueur]");
	if (!PlayerInfo[playerid][pOnDuty]) 
		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return SendNotNearPlayer(playerid);
	if (userid == playerid) 
		return 1;
	if (!pTemp[userid][pCuffed]) 
		return SendErrorMessage(playerid, "Le joueur n'est pas menotté.");

	pTemp[userid][pCuffed] = 0;
	SendSyntaxMessage(userid, "%s vous a retiré les menottes.", GetNameWithMask(playerid));
	SendSyntaxMessage(playerid, "Vous avez retiré les menottes de %s.", GetNameWithMask(userid));
	RemovePlayerAttachedObject(userid, ATTACH_HANDS);
	SetPlayerSpecialAction(userid, SPECIAL_ACTION_NONE);
	return 1;
}

ALTX:chase("/clist", "/sitleader");
CMD:chase(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
	if (!IsFactionPolice(faction))		return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	
	new userid;
	if (sscanf(params, "u", userid))					return SendHelpMessage(playerid, "/chase [ID/nom du joueur]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)								return 1;
	if (GetPVarInt(playerid, #Chase_Time) > 1)			return SendErrorMessage(playerid, "Une poursuite est déjà en cours pour %s [ID:%i].", GetNameWithMask(userid), userid);
	if (!PlayerNearPlayer(15.0, playerid, userid))		return SendNotNearPlayer(playerid);

	SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s a commencé une poursuite contre %s [ID:%i].", GetRankName(playerid), GetNameEx(playerid), GetNameWithMask(userid), userid);

	StartChase(userid);	
	return 1;
}

CMD:siren(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid), model = GetVehicleModel(vehicle), siren_id;

	if (!IsFactionLegal(faction)) 
		return SendErrorMessage(playerid, "Vous devez être policier, agent pénitentiaire, ambulancier ou membre du gouvernement.");
	if (VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) 
		return SendErrorMessage(playerid, "Vous devez être dans un véhicule de service.");
	if (sscanf(params, "i", siren_id)) 
		return SendHelpMessage(playerid, "/siren [1-3]");
	if (siren_id < 1 || siren_id > 3) 
		return SendErrorMessage(playerid, "ID de sirène invalide : 1-3.");

	if (siren_id == 1)
	{
		if (IsValidDynamicObject(VehicleInfo[vehicle][carSiren][0]))
		{
			SendSyntaxMessage(playerid, "Sirène retirée.");
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][0]);
			VehicleInfo[vehicle][carSiren][0] = INVALID_OBJECT_ID;
			return 1;
		}

		if (model != 560 && model != 541 && model != 525 && model != 426 && model != 579 && model != 415 && model != 554 && model != 546 && model != 566 && model != 561 && model != 547 && model != 482)
			return SendErrorMessage(playerid, "La sirène peut être installée uniquement sur Sultan/Bullet/Towtruck/Premier/Huntley/Cheetah/Yosemite/Intruder/Tahoma/Stratum/Primo/Burrito.");

		VehicleInfo[vehicle][carSiren][0] = CreateDynamicObject((model != 525) ? (18646) : (19419), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
		switch (model)
		{
			case 560: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.571924, 0.068632, 0.860007, 0.000000, 0.000000, -181.148086);
			case 541: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.442310, -0.091429, 0.670001, 0.000000, -2.299999, -178.874786);
			case 426: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.570416, -0.012622, 0.890005, 0.000000, 0.000000, -91.267646);
			case 525: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.0, -0.485, 1.36, 0.000000, 0.000000, 180.0);
			case 579: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.640568, -0.021741, 1.260003, 0.000000, 0.000000, -90.153976);
			case 415: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.424245, -0.241412, 0.640021, 3.000001, -3.499999, -181.499969);
			case 554: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.734659, 0.117240, 1.059349, -4.199999, 0.000000, -87.859664);
			case 546: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.591740, -0.153342, 0.870005, 0.000000, 0.000000, -269.351074);
			case 566: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.660138, 0.024635, 0.869999, 0.000000, 0.000000, -88.731002);
			case 561: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.428671, 0.080074, 0.850004, -2.799999, 0.000000, -88.665367);
			case 547: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.530513, 0.002143, 0.916606, -3.899999, 0.000000, -89.768531);
			case 482: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][0], vehicle, 0.515804, 0.702674, 0.950006, 0.000000, 0.000000, -92.038864);
		}

		Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
		return SendSyntaxMessage(playerid, "Sirène installée.");
	}

	else if (siren_id == 2)
	{
		if (IsValidDynamicObject(VehicleInfo[vehicle][carSiren][1]))
		{
			SendSyntaxMessage(playerid, "Sirène retirée.");
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][1]);
			VehicleInfo[vehicle][carSiren][1] = INVALID_OBJECT_ID;
			return 1;
		}

		if (model != 560 && model != 426 && model != 579 && model != 566 && model != 561 && model != 547 && model != 546)
			return SendErrorMessage(playerid, "La sirène peut être installée uniquement sur Sultan/Premier/Huntley/Tahoma/Stratum/Primo/Intruder.");

		VehicleInfo[vehicle][carSiren][1] = CreateDynamicObject(19419, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
		switch (model)
		{
			case 560: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.000000, -1.289999, 0.394999, 0.000000, 0.000000, 0.000000);
			case 426: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.000000, -1.669998, 0.349999, 0.000000, 0.000000, 0.000000);
			case 579: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.005192, -2.445963, 0.521951, 0.000000, 0.000000, -359.884796);
			case 566: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.009878, -1.681756, 0.400007, 0.000000, 0.000000, -0.010757);
			case 561: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, -0.004864, -2.317730, 0.249995, 0.000000, 0.000000, -0.192703);
			case 547: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, -0.001344, -1.571307, 0.400003, 0.000000, 0.000000, -0.468620);
			case 546: AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][1], vehicle, 0.014445, -1.771190, 0.340004, 0.000000, 0.000000, -359.851013);
		}
		Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
		return SendSyntaxMessage(playerid, "Sirène installée.");
	}

	else if (siren_id == 3)
	{
		if (IsValidDynamicObject(VehicleInfo[vehicle][carSiren][2]))
		{
			SendSyntaxMessage(playerid, "Sirène retirée.");
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][2]);
			VehicleInfo[vehicle][carSiren][2] = INVALID_OBJECT_ID;
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][3]);
			VehicleInfo[vehicle][carSiren][3] = INVALID_OBJECT_ID;
			DestroyDynamicObject(VehicleInfo[vehicle][carSiren][4]);
			VehicleInfo[vehicle][carSiren][4] = INVALID_OBJECT_ID;
			return 1;
		}

		if (model != 541 && model != 415 && model != 560)
			return SendErrorMessage(playerid, "La sirène peut être installée uniquement sur Bullet/Cheetah/Sultan.");

		if (model == 541)
		{
			VehicleInfo[vehicle][carSiren][2] = CreateDynamicObject(19620, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][2], vehicle, 0.000000, 0.000000, 0.659999, 0.000000, 0.000000, -359.823394);
			VehicleInfo[vehicle][carSiren][3] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][3], vehicle, 0.006984, -2.280384, -0.190001, 0.000000, 0.000000, -359.824523);
			VehicleInfo[vehicle][carSiren][4] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][4], vehicle, -0.007443, 2.430286, -0.070001, 0.000000, 0.000000, -180.024566);
		}
		if (model == 415)
		{
			VehicleInfo[vehicle][carSiren][2] = CreateDynamicObject(19620, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][2], vehicle, 0.000475, -0.270263, 0.630008, 0.000000, 0.000000, -359.899139);
			VehicleInfo[vehicle][carSiren][3] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][3], vehicle, -0.005538, 2.660761, -0.210004, 0.000000, 0.000000, -180.099121);
			VehicleInfo[vehicle][carSiren][4] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][4], vehicle, 0.003255, -2.460503, -0.140003, 0.000000, 0.000000, 0.266403);
		}
		if (model == 560)
		{
			VehicleInfo[vehicle][carSiren][2] = CreateDynamicObject(19620, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][2], vehicle, -0.000927, 0.110103, 0.870014, 0.000000, 0.000000, -359.517333);
			VehicleInfo[vehicle][carSiren][3] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][3], vehicle, 0.004800, -2.370894, 0.105430, 0.000000, 0.000000, -359.912292);
			VehicleInfo[vehicle][carSiren][4] = CreateDynamicObject(19797, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, -1, -1, 300.0);
			AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carSiren][4], vehicle, -0.003252, 2.561954, 0.050001, 0.000000, 0.000000, -179.912200);
		}

		Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
		return SendSyntaxMessage(playerid, "Sirène installée.");
	}
	return 1;
}

CMD:licenseweapon(playerid, params[])
{
	new userid, price;

	if (sscanf(params, "ui", userid, price)) 
		return SendHelpMessage(playerid, "/licenseweapon [ID/nom du joueur] [prix]");
	if (IsPlayerFactionPolice(playerid)) 
		return SendErrorMessage(playerid, "Vous devez être membre du LSPD.");
	if (PlayerInfo[playerid][pAuthwep] != 1) 
		return SendErrorMessage(playerid, "Vous n'avez pas l'autorisation de délivrer des licences d'armes.");
	if (price < 1 || price > 10000) 
		return SendErrorMessage(playerid, "Le prix doit être compris entre $1 et $10 000.");
	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pWepLic] != 0) 
		return SendErrorMessage(playerid, "Le joueur possède déjà une licence d'armes.");
	if (pTemp[userid][pWepPlayer] == playerid) 
		return SendErrorMessage(playerid, "Vous avez déjà proposé une licence à ce joueur.");

	pTemp[userid][pWepPlayer] = playerid;
	pTemp[userid][pWepPrice] = price;
	SetPVarInt(userid, #accept_time, gettime() + 30);
	SendSyntaxMessage(userid, "%s %s vous propose d'acheter une licence d'armes pour $%i. (Appuyez sur Y pour accepter)", GetRankName(playerid), GetNameEx(playerid), price);
	SendSyntaxMessage(playerid, "Vous avez proposé à %s d'acheter une licence d'armes pour $%i.", GetNameEx(userid), price);
	return 1;
}

CMD:authweapon(playerid, params[])
{
	new userid;

	if (sscanf(params, "u", userid)) 
		return SendHelpMessage(playerid, "/authweapon [ID/nom du joueur]");
	if (IsPlayerFactionPolice(playerid)) 
		return SendErrorMessage(playerid, "Vous devez être membre du LSPD.");
	if (!isLeader(playerid)) 
		return SendErrorMessage(playerid, "Vous n'êtes pas le leader de la faction.");
	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction]) 
		return SendErrorMessage(playerid, "Le joueur n'appartient pas à votre organisation.");

	if (PlayerInfo[userid][pAuthwep] == 0)
	{
		PlayerInfo[userid][pAuthwep] = 1;
		SendSyntaxMessage(userid, "%s %s vous a accordé l'autorisation de délivrer des licences d'armes.", GetRankName(playerid), GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Vous avez accordé l'autorisation de délivrer des licences d'armes à %s.", GetNameEx(userid));
		return 1;
	}

	PlayerInfo[userid][pAuthwep] = 0;
	SendSyntaxMessage(userid, "%s %s vous a retiré l'autorisation de délivrer des licences d'armes.", GetRankName(playerid), GetNameEx(playerid));
	SendSyntaxMessage(playerid, "Vous avez retiré l'autorisation de délivrer des licences d'armes à %s.", GetNameEx(userid));
	return 1;
}

CMD:authswat(playerid, params[])
{
	new userid;

	if (sscanf(params, "u", userid)) 
		return SendHelpMessage(playerid, "/authswat [ID/nom du joueur]");
	if (!IsPlayerFactionPolice(playerid)) 
		return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!isLeader(playerid)) 
		return SendErrorMessage(playerid, "Vous n'êtes pas le leader de la faction.");
	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pFaction] != PlayerInfo[playerid][pFaction]) 
		return SendErrorMessage(playerid, "Le joueur n'appartient pas à votre organisation.");

	if (PlayerInfo[userid][pSwat] == 0)
	{
		PlayerInfo[userid][pSwat] = 1;
		SendSyntaxMessage(userid, "%s %s vous a accordé les pouvoirs d'un membre du groupe d'intervention S.W.A.T.", GetRankName(playerid), GetNameEx(playerid));
		SendSyntaxMessage(playerid, "Vous avez accordé les pouvoirs S.W.A.T. à %s.", GetNameEx(userid));
		return 1;
	}

	PlayerInfo[userid][pSwat] = 0;
	SendSyntaxMessage(userid, "%s %s vous a retiré les pouvoirs d'un membre du groupe d'intervention S.W.A.T.", GetRankName(playerid), GetNameEx(playerid));
	SendSyntaxMessage(playerid, "Vous avez retiré les pouvoirs S.W.A.T. à %s.", GetNameEx(userid));
	return 1;
}

CMD:impound(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionPolice(faction))                return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty])           return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (!IsATowTruck(vehicle))                    return SendErrorMessage(playerid, "Vous devez être dans un dépanneur.");
	if (!IsTrailerAttachedToVehicle(vehicle))     return SendErrorMessage(playerid, "Aucune remorque n'est attachée au dépanneur.");

	new trailer = GetVehicleTrailer(vehicle);

	if (!VehicleInfo[trailer][carOwnerID])                                return SendErrorMessage(playerid, "Seuls les véhicules personnels peuvent être envoyés à la fourrière.");
	if (!PlayerToPoint(300.0, playerid, -1209.9501, -1064.3761, 128.2656)) return SendErrorMessage(playerid, "Vous êtes trop loin de la fourrière.");

	DetachTrailerFromVehicle(vehicle);
	GetVehiclePos(trailer, VehicleInfo[trailer][carParkX], VehicleInfo[trailer][carParkY], VehicleInfo[trailer][carParkZ]);
	GetVehicleZAngle(trailer, VehicleInfo[trailer][carParkA]);

	SendSyntaxMessage(playerid, "%s a été envoyé à la fourrière.", VehicleNames[GetVehicleModel(trailer) - 400]);
	VehicleEngine(trailer, false);

	VehicleInfo[trailer][Impounded] = 1;

	Save_Car(trailer);

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pCarKey] != trailer) continue;

		SendSyntaxMessage(i, "Votre %s a été attaché et envoyé à la fourrière par %s %s.", VehicleNames[GetVehicleModel(trailer) - 400], GetRankName(playerid), GetNameEx(playerid));
		break;
	}
	return 1;
}

CMD:ptow(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionPolice(faction))			return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (!IsATowTruck(vehicle))				return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse.");
	
	if (IsTrailerAttachedToVehicle(vehicle))
	{
		DetachTrailerFromVehicle(vehicle);
		return SendErrorMessage(playerid, "Le véhicule précédemment attaché a été détaché.");
	}

	new vehid;

	if (sscanf(params, "i", vehid))														return SendHelpMessage(playerid, "/ptow [vehicleID]");
	if (vehid < 1 || vehid >= MAX_VEHICLES)												return 1;
	if (GetNearVehicle(playerid, 14.0, vehid) == 0)										return SendErrorMessage(playerid, "Le véhicule doit être à proximité de votre dépanneuse.");
	if (IsABoat(vehid) || IsAPlane(vehid) || IsAHelicopter(vehid) || IsABiker(vehid))	return SendErrorMessage(playerid, "Ce type de véhicule ne peut pas être attaché.");
		
	AttachTrailerToVehicle(vehid, vehicle);
	SendSyntaxMessage(playerid, "Vous avez attaché %s. Utilisez /ptow pour le détacher.", VehicleNames[GetVehicleModel(vehid) - 400]);
	return 1;
}

CMD:apb(playerid, params[])
{
	new action[24], secoption[153], tmp[2][128], faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction))		return SendErrorMessage(playerid, "Vous devez être un officier de police.");
	if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Vous devez être en service (/duty).");

	if (sscanf(params, "s[24]S()[153]", action, secoption))
	{
		SendHelpMessage(playerid, "/apb [action]");
		SendSyntaxMessage(playerid, "create / clear / show / edit / list");
		return 1;
	}

	if (CompareStrings(action, "list"))
	{
		new count;
	   
		for (new i = 0; i != MAX_APBS; i++)
		{
			if (ApbInfo[i][apbOn] == 0) continue;

			if (strlen(ApbInfo[i][apbCharge]) > 64)
				SendClientMessageEx(playerid, COLOR_LIGHTRED, "APB %i:{FFFFFF} %.64s ... (%s)", i, ApbInfo[i][apbCharge], ApbInfo[i][apbSuspect]);
			else
				SendClientMessageEx(playerid, COLOR_LIGHTRED, "APB %i:{FFFFFF} %s (%s)", i, ApbInfo[i][apbCharge], ApbInfo[i][apbSuspect]);

			count++;
		}

		if (!count)	return SendErrorMessage(playerid, "Aucune note APB trouvée.");

		return 1;
	}
	else if (CompareStrings(action, "show"))
	{
		new number;

		if (sscanf(secoption, "i", number))	return SendHelpMessage(playerid, "/apb show [numéro APB]");
		if (!ApbInfo[number][apbOn])			return SendErrorMessage(playerid, "Note APB introuvable.");

		SendClientMessage(playerid, -1, "____________________________________");
		SendClientMessageEx(playerid, COLOR_LIGHTRED, "Suspect :{FFFFFF} %s", ApbInfo[number][apbSuspect]);
		
		if (strlen(ApbInfo[number][apbCharge]) > 64)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTRED, "Description :{FFFFFF} %.64s ...", ApbInfo[number][apbCharge]);
			SendClientMessageEx(playerid, -1, "... %s", ApbInfo[number][apbCharge][64]);
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_LIGHTRED, "Description :{FFFFFF} %s", ApbInfo[number][apbCharge]);
		}

		SendClientMessageEx(playerid, COLOR_LIGHTRED, "Ajouté par :{FFFFFF} %s, %s.", ApbInfo[number][apbOfficer], Factions[ApbInfo[number][apbFaction]][fShortName]);
		SendClientMessage(playerid, -1, "____________________________________");
	}
	else if (CompareStrings(action, "clear"))
	{
		new number;

		if (sscanf(secoption, "i", number))	return SendHelpMessage(playerid, "/apb clear [numéro APB]");
		if (!ApbInfo[number][apbOn])			return SendErrorMessage(playerid, "Note APB introuvable.");

		SendSyntaxMessage(playerid, "Vous avez effacé la note APB numéro %i.", number);
		SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "* HQ : %s %s a supprimé la note APB numéro %i.", GetRankName(playerid), GetNameEx(playerid), number);

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `apbs` WHERE `id` = %i", ApbInfo[number][apbID]);
		mysql_tquery(dbHandle, query);

		ApbInfo[number][apbOn] = 0;
		ApbInfo[number][apbID] = 0;
	}
	else if (CompareStrings(action, "edit"))
	{
		new number, info[153];

		if (sscanf(secoption, "is[153]", number, info))	return SendHelpMessage(playerid, "/apb edit [numéro APB] [suspect/description]");
		if (!ApbInfo[number][apbOn])						return SendErrorMessage(playerid, "Note APB introuvable.");
		if (strfind(info, "/", true) == -1)				return SendErrorMessage(playerid, "Symbole \"/\" introuvable.");
		
		SendSyntaxMessage(playerid, "Vous avez modifié la note APB [NUM : %i].", number);
		strexplode(tmp, info, "/");
		format(ApbInfo[number][apbSuspect], MAX_PLAYER_NAME, "%s", tmp[0]);
		format(ApbInfo[number][apbCharge], 128, "%s", tmp[1]);
		ApbInfo[number][apbFaction] = faction;

		SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "* HQ : %s %s a modifié la note APB numéro %i.", GetRankName(playerid), GetNameEx(playerid), number);
		
		new query[128 + 128];
		mysql_format(dbHandle, query, sizeof(query), "UPDATE `apbs` SET `charge` = '%s', `suspect` = '%e' WHERE `id` = %i", tmp[1], tmp[0], ApbInfo[number][apbID]);
		mysql_tquery(dbHandle, query);
		return 1;
	}
	else if (CompareStrings(action, "create"))
	{
		new info[153];

		if (sscanf(secoption, "s[153]", info))	return SendHelpMessage(playerid, "/apb create [suspect/description]");
		if (strfind(info, "/", true) == -1)		return SendErrorMessage(playerid, "Symbole \"/\" introuvable.");

		new apb = -1;

		for (new i = 0; i != MAX_APBS; i++)
		{
			if (ApbInfo[i][apbOn] != 0) continue;

			apb = i;
			break;
		}

		if (apb == -1)	return SendErrorMessage(playerid, "Aucun espace disponible pour une nouvelle note APB.");

		SendSyntaxMessage(playerid, "Vous avez créé une note APB [NUM : %i].", apb);
		strexplode(tmp, info, "/");
		format(ApbInfo[apb][apbOfficer], MAX_PLAYER_NAME, "%s", GetNameEx(playerid));
		format(ApbInfo[apb][apbSuspect], MAX_PLAYER_NAME, "%s", tmp[0]);
		format(ApbInfo[apb][apbCharge], 128, "%s", tmp[1]);
		format(ApbInfo[apb][apbDate], 32, "%s", GetFullDate());

		ApbInfo[apb][apbFaction] = faction;

		SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "* HQ : %s %s a ajouté une nouvelle note APB, numéro %i.", GetRankName(playerid), GetNameEx(playerid), apb);
		
		new query[128 + 128];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `apbs` (faction, charge, suspect, officer, date) VALUES (%i, '%s', '%s', '%s', '%s')", faction, tmp[1], tmp[0], GetNameEx(playerid), GetFullDate());
		mysql_tquery(dbHandle, query, "ApbCreate", "i", apb);
		return 1;
	}
	return 1;
}

CMD:toll(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
		
	if (isnull(params))
	{
		SendHelpMessage(playerid, "/toll [paramètre]");
		SendClientMessage(playerid, COLOR_GRAD2, "close/open - Ferme/Ouvre toutes les barrières.");
		SendClientMessage(playerid, COLOR_GRAD2, "0 - Ouvre/ferme les barrières de l'autoroute à Las Venturas.");
		SendClientMessage(playerid, COLOR_GRAD2, "1 - Blueberry (pont droit) - Ouvre/ferme les barrières sur le pont droit à Blueberry.");
		SendClientMessage(playerid, COLOR_GRAD2, "2 - Blueberry (pont gauche) - Ouvre/ferme les barrières sur le pont gauche à Blueberry.");
		return 1;
	}
	
	if (L_a_RequestAllowedCop > gettime())	
		return SendErrorMessage(playerid, "Vous devez attendre %i secondes.", L_a_RequestAllowedCop - gettime());
		
	if (CompareStrings(params, "open"))
	{
		if (PlayerInfo[playerid][pRank] < 10)	
			return SendErrorMessage(playerid, "Cette fonction est disponible à partir du rang 10.");
		
		for (new i; i != MAX_TOLLS; i++)
		{
			aTolls[i][E_tLocked] = 0;
		}
		
		SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s a ouvert toutes les barrières. **", GetRankName(playerid), GetNameEx(playerid));
		L_a_RequestAllowedCop = gettime() + TOLL_DELAY_COP;
		return 1;
	}
	else if (CompareStrings(params, "close"))
	{
		if (PlayerInfo[playerid][pRank] < 10)	
			return SendErrorMessage(playerid, "Cette fonction est disponible à partir du rang 10.");
			
		for (new i; i != MAX_TOLLS; i++)
		{
			aTolls[i][E_tLocked] = 1;
			Toll_CloseToll(i);
		}
		
		SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s a fermé toutes les barrières. **", GetRankName(playerid), GetNameEx(playerid));
		L_a_RequestAllowedCop = gettime() + TOLL_DELAY_COP;
		return 1;
	}
	
	new id = strval(params);
	
	if (id < 0 || id > 4)				
		return SendHelpMessage(playerid, "/toll [numéro de la barrière]");
	if (PlayerInfo[playerid][pRank] < 4)	
		return SendErrorMessage(playerid, "Cette fonction est disponible à partir du rang 4.");
		
	if (!IsPlayerInRangeOfPoint(playerid, TOLL_OPEN_DISTANCE, toll_gate_pos[id][0], toll_gate_pos[id][1], toll_gate_pos[id][2]) && 
		!IsPlayerInRangeOfPoint(playerid, TOLL_OPEN_DISTANCE, toll_gate_pos[id+1][0], toll_gate_pos[id+1][1], toll_gate_pos[id+1][2]))
	{
		return SendErrorMessage(playerid, "Vous devez être à proximité de ces barrières.");
	}
			
	new toll_name[3][64] = {"de l'autoroute Las Venturas", "sur le pont droit à Blueberry", "sur le pont gauche à Blueberry"};

	if (aTolls[id][E_tLocked] == 0)
	{
		aTolls[id][E_tLocked] = 1;
		Toll_CloseToll(id);
		aTolls[id+1][E_tLocked] = 1;
		Toll_CloseToll(id+1);
		SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s a fermé les barrières %s. **", GetRankName(playerid), GetNameEx(playerid), toll_name[id]);
	}
	else
	{
		aTolls[id][E_tLocked] = 0;
		aTolls[id+1][E_tLocked] = 0;
		SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** HQ: %s %s a ouvert les barrières %s. **", GetRankName(playerid), GetNameEx(playerid), toll_name[id]);
	}

	L_a_RequestAllowedCop = gettime() + TOLL_DELAY_COP;
	return 1;
}

ALTX:government("/gov");
CMD:government(playerid, params[])
{
	if (PlayerInfo[playerid][pMutedTime] > 0) 
		return SendErrorMessage(playerid, "Votre chat local est bloqué par le système.");
	if (isnull(params)) 
		return SendHelpMessage(playerid, "(/gov)ernment [message]");

	new faction = PlayerInfo[playerid][pFaction], rank = PlayerInfo[playerid][pRank];

	if (!faction || !IsFactionLegal(faction)) 
		return SendErrorMessage(playerid, "Vous ne faites pas partie d'une structure gouvernementale.");
	if (!isLeader(playerid) && rank != Factions[faction][fRanks] - 1) 
		return SendErrorMessage(playerid, "Accessible uniquement au leader.");
	if (PlayerInfo[playerid][pSettings] & togGov) 
		return SendErrorMessage(playerid, "Activez d'abord l'affichage des annonces gouvernementales.");

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || (PlayerInfo[i][pSettings] & togGov)) 
			continue;

		if (strlen(params) > MAXLEN)
		{
			new pos = MAXLEN;
			if (pos < MAXLEN - 1) pos = MAXLEN;

			SendClientMessageEx(i, COLOR_LSPD, "[Annonce Gouvernementale] %.*s ...", pos, params);
			SendClientMessageEx(i, COLOR_LSPD, "[Annonce Gouvernementale] ... %s", params[pos]);
		}
		else
		{
			SendClientMessageEx(i, COLOR_LSPD, "[Annonce Gouvernementale] %s", params);
		}
	}
	return 1;
}

CMD:duty(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!faction || !IsFactionLegal(faction))								return 1;
	if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))	return SendErrorMessage(playerid, "Vous n'êtes pas dans le vestiaire ou dans un véhicule de service.");

	Dialog_Show(playerid, DutyMenu, DIALOG_STYLE_LIST, "{FFFFFF}Service", "- Prendre son service\n- Uniforme\n- Équipement\n- Recharger le gilet pare-balles\n- Recharger la santé", "Choisir", "Annuler");
	return 1;
}

CMD:color(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsPlayerFactionPolice(playerid))	return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty])		return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (PlayerInfo[playerid][pRank] < 5)	return SendErrorMessage(playerid, "Cette fonction n'est pas disponible pour vous.");

	if (GetPlayerColor(playerid) != RemoveAlpha(Factions[faction][fColor]))
	{
		SetPlayerColor(playerid, RemoveAlpha(Factions[faction][fColor]));
		SendSyntaxMessage(playerid, "Vous avez changé la couleur de votre pseudo.");
	}
	else
	{
		SetPlayerColor(playerid, TEAM_HIT_COLOR);
		SendSyntaxMessage(playerid, "Vous avez changé la couleur de votre pseudo.");
	}
	return 1;
}

CMD:lift(playerid, params[])
{
	if (!IsPlayerInRangeOfPoint(playerid, 2.0, 2268.0640, 2448.2449, 3.5313) &&
		!IsPlayerInRangeOfPoint(playerid, 2.0, 2781.0559, -1056.2823, 1028.4969) &&
		!IsPlayerInRangeOfPoint(playerid, 2.0, 2874.6665, -940.6707, 1035.9259) &&
		!IsPlayerInRangeOfPoint(playerid, 2.0, 2872.8813, -1072.9934, 1037.1782))
		return 1;

	Dialog_Show(playerid, LSPDElevator, DIALOG_STYLE_LIST, "{FFFFFF}Ascenseur", "[0] Garage\n[1] Premier étage\n[2] Deuxième étage\n[3] Salle de sport", "Choisir", "Annuler");
	return 1;
}

CMD:ane(playerid, params[])
{
	new
		vehicle = GetPlayerVehicleID(playerid),
		userid;

	if (!IsPlayerFactionPolice(playerid))
		return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && !PlayerToPoint(5.0, playerid, 96.8945, -76.5486, 1001.6760))
		return SendErrorMessage(playerid, "Disponible uniquement dans un poste de police ou un véhicule LSPD/SASD.");
	if (sscanf(params, "i", userid))
		return SendHelpMessage(playerid, "/ane [numéro d'appel]");
	if (!IsPlayerLogged(userid) || pTemp[userid][pCallOn] != 1)
		return SendErrorMessage(playerid, "Aucun appel trouvé avec ce numéro.");

	if (pTemp[playerid][pPatrol])
		SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_LSPD, "* %s %s [%s] a pris l'appel d'urgence #%i.", GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], userid);
	else
		SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_LSPD, "* %s %s a pris l'appel d'urgence #%i.", GetRankName(playerid), GetNameEx(playerid), userid);

	Waypoint_Set(playerid, pTemp[userid][pCallPosX], pTemp[userid][pCallPosY], pTemp[userid][pCallPosZ], 3.0);
	pTemp[userid][pCallOn] = 0;

	pTemp[playerid][pPoliceCalls]++;
	return 1;
}

ALTX:spike("/sp");
CMD:spike(playerid, params[])
{
	if (!IsPlayerFactionPolice(playerid)) return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez être en service (/duty).");

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/spike [paramètre]");
		SendSyntaxMessage(playerid, "create / remove / remall");
		return 1;
	}

	if (CompareStrings(option, "create"))
	{
		if (GetWorld(playerid) != 0 || GetInterior(playerid) != 0) return SendErrorMessage(playerid, "Vous devez être à l'extérieur.");
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");

		new Float:x, Float:y, Float:z, Float:a, location[MAX_ZONE_NAME];

		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, a);

		if (CreateStrip(x, y, z, a) == -1) return SendErrorMessage(playerid, "Nombre maximum de herses atteint.");

		GetPlayer2DZone(playerid, location, MAX_ZONE_NAME);

		SendFactionMessage(FACTION_POLICE, COLOR_LSPD, "* HQ: %s %s a placé des herses sur la route %s.", GetRankName(playerid), GetNameEx(playerid), location);
		return 1;
	}

	if (CompareStrings(option, "remove"))
	{
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");

		for (new i = 0; i < MAX_SPIKE; i++)
		{
			if (!SpikeInfo[i][sCreated] || !IsPlayerInRangeOfPoint(playerid, 4.0, SpikeInfo[i][sX], SpikeInfo[i][sY], SpikeInfo[i][sZ])) continue;

			SpikeInfo[i][sCreated] = 0;
			SpikeInfo[i][sX] = SpikeInfo[i][sY] = SpikeInfo[i][sZ] = 0.0;

			if (IsValidDynamicObject(SpikeInfo[i][sObject]))
			{
				DestroyDynamicObject(SpikeInfo[i][sObject]);
				SpikeInfo[i][sObject] = INVALID_OBJECT_ID;
			}

			SendSyntaxMessage(playerid, "Vous avez retiré les herses [ID: %i].", i);
			break;
		}
		return 1;
	}

	else if (CompareStrings(option, "remall"))
	{
		for (new i = 0; i < MAX_SPIKE; i++)
		{
			if (!SpikeInfo[i][sCreated]) continue;

			SpikeInfo[i][sCreated] = 0;
			SpikeInfo[i][sX] = SpikeInfo[i][sY] = SpikeInfo[i][sZ] = 0.0;

			if (IsValidDynamicObject(SpikeInfo[i][sObject]))
			{
				DestroyDynamicObject(SpikeInfo[i][sObject]);
				SpikeInfo[i][sObject] = INVALID_OBJECT_ID;
			}
		}

		SendFactionMessage(FACTION_POLICE, COLOR_LSPD, "* HQ: Toutes les herses ont été retirées par %s %s.", GetRankName(playerid), GetNameEx(playerid));
		return 1;
	}

	else
	{
		SendHelpMessage(playerid, "/spike [paramètre]");
		SendSyntaxMessage(playerid, "create / remove / remall");
	}
	return 1;
}

CMD:prisonlist(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction) || !IsFactionDocs(faction))
		return SendErrorMessage(playerid, "Vous devez être un policier ou un agent pénitentiaire.");

	if (IsFactionPolice(faction))
	{
		SendClientMessage(playerid, COLOR_GREEN, "Liste des détenus dans le poste de police :");

		foreach (new i : Player)
		{
			if (!IsPlayerLogged(i) || PlayerInfo[i][pJailed] != PALOMINO_JAIL)
				continue;

			SendClientMessageEx(playerid, COLOR_GRAD2, "%s | Temps restant : %s secondes.", GetNameEx(i), FormatNumber(PlayerInfo[i][pJailTime]));
		}
		return 1;
	}

	/* Exemple pour d'autres factions comme SASD :
	else if (faction == SASD)
	{
		SendClientMessage(playerid, COLOR_GREEN, "Liste des détenus dans le poste du San Andreas Sheriff's Department :");
		foreach (new i : Player)
		{
			if (!IsPlayerLogged(i) || PlayerInfo[i][pJailed] != SASD_JAIL)
				continue;

			SendClientMessageEx(playerid, COLOR_GRAD2, "%s | Temps restant : %s secondes.", GetNameEx(i), FormatNumber(PlayerInfo[i][pJailTime]));
		}
		return 1;
	}
	*/

	return 1;
}

ALTX:repaircam("/rcam");
CMD:repaircam(playerid)
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionPolice(faction)) 				return SendErrorMessage(playerid, "Vous devez être un policier.");
	if (!IsPlayerHasItem(playerid, INV_TOOLS)) 	return SendErrorMessage(playerid, "Vous n'avez pas d'outils.");

	new camid = GetNearestCamera(playerid, 10.0);

	if (camid == -1) 				return SendErrorMessage(playerid, "Vous devez être près d'une caméra de surveillance.");
	if (!CCTVInfo[camid][tvBreak]) 	return SendErrorMessage(playerid, "Cette caméra n'a pas besoin de réparation.");

	SetCameraBreakState(camid, 0);
	SendSyntaxMessage(playerid, "Vous avez réparé la caméra de surveillance [ID : %i | %s].", camid, CCTVInfo[camid][tvName]);
	return 1;
}

/*
CMD:rgun(playerid)
{
	if (!IsPlayerFactionPolice(playerid)) 
		return SendErrorMessage(playerid, "Vous devez être un policier.");

	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new dropid = -1;
	if ((dropid = ItemDrop_Nearest(playerid)) == -1) 
		return SendErrorMessage(playerid, "Aucune arme trouvée au sol à proximité.");
	
	if (ItemDrop[dropid][dropID] == 0) 
		return SendErrorMessage(playerid, "Aucune arme trouvée au sol à proximité.");
	
	if (inventory_items[ItemDrop[dropid][dropID]][itemType] != ITEM_TYPE_WEAPON) 
		return SendErrorMessage(playerid, "Aucune arme trouvée au sol à proximité.");

	ItemDrop_Delete(dropid);
	return 1;
}
*/