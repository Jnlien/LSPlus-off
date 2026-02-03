CMD:checkint(playerid, params[])
{
	new houseid;

	if ((houseid = GetNearOutsideHouse(playerid)) == -1)	return SendErrorMessage(playerid, "Aucune maison à vendre à proximité.");
	if (HouseInfo[houseid][hOwnerID])						return SendErrorMessage(playerid, "Cette maison est déjà achetée.");
	
	if (!GetWorld(playerid) && !GetInterior(playerid))		GetPlayerPos(playerid, PlayerInfo[playerid][pLocationPos][0], PlayerInfo[playerid][pLocationPos][1], PlayerInfo[playerid][pLocationPos][2]);

	SetPlayerPos(playerid, HouseInfo[houseid][hExitX], HouseInfo[houseid][hExitY], HouseInfo[houseid][hExitZ]);
	SetInterior(playerid, HouseInfo[houseid][hExitInt]);
	SetWorld(playerid, HouseInfo[houseid][hExitVW]);
	pTemp[playerid][pHouseEntered] = houseid;
	
	SetPVarInt(playerid, #check_int, 30);
	SendSyntaxMessage(playerid, "Vous examinez l'intérieur. Vous avez 30 secondes.");
	return 1;
}

ALTX:house("/h");
CMD:house(playerid, params[])
{
	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/h(ouse) [paramètre]");
		SendSyntaxMessage(playerid, "buy | menu | upgrade | withdraw | deposit | sell | sellto | backdoor");

		if (!randomEx(0, 5)) {
			SendHelpMessage(playerid, "Si vous avez besoin d'ajouter un garage / modifier la position de l'entrée/sortie principale ou secondaire");
			SendHelpMessage(playerid, "Veuillez vous rendre sur le forum dans la section \"Propriétés\" !");
		}

		return 1;
	}

	if (CompareStrings(option, "buy"))
	{
		new houseid;

		if ((houseid = GetNearOutsideHouse(playerid)) == -1) return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison à vendre.");
		if (HouseInfo[houseid][hOwnerID]) return SendErrorMessage(playerid, "Cette maison appartient déjà à quelqu'un.");
		if (GetPlayerHouses(playerid) >= GetMaxPlayerHouse(playerid)) return SendErrorMessage(playerid, "Vous ne pouvez pas posséder plus de maisons.");
		if (PlayerInfo[playerid][pLevel] < HouseInfo[houseid][hLevelbuy]) return SendErrorMessage(playerid, "Vous devez être au niveau %i pour acheter cette maison.", HouseInfo[houseid][hLevelbuy]);
		if (GetCash(playerid) < HouseInfo[houseid][hPrice]) return SendNotCash(playerid);

		HouseInfo[houseid][hOwnerID] = PlayerInfo[playerid][pID];
		HouseInfo[houseid][hLocked] = 1;
		HouseInfo[houseid][hCash] = 0;
		format(HouseInfo[houseid][hOwner], MAX_PLAYER_NAME, GetNameEx(playerid));

		new money = PlayerInfo[playerid][pCash];

		GiveCash(playerid, -HouseInfo[houseid][hPrice]);
		SendSyntaxMessage(playerid, "Vous avez acheté la maison [ID: %i] pour $%i.", houseid, HouseInfo[houseid][hPrice]);

		for (new i = 0; i < MAX_GARAGES; i++)
		{
			if (!GarageInfo[i][garageID] || GarageInfo[i][garageHouse] != HouseInfo[houseid][hID]) continue;

			SendSyntaxMessage(playerid, "Votre maison possède un garage [ID: %i].", i);
			GarageInfo[i][garageOwner] = PlayerInfo[playerid][pID];
			format(GarageInfo[i][garageOwnerName], 24, "%s", GetNameEx(playerid));

			SQL_SetInteger("garage", "owner", GarageInfo[i][garageOwner], GarageInfo[i][garageID]);
			SQL_SetString("garage", "ownername", GarageInfo[i][garageOwnerName], GarageInfo[i][garageID]);
		}

		if (HouseInfo[houseid][hExtX][0] && HouseInfo[houseid][hExtY][0] && HouseInfo[houseid][hExtZ][0]) {
			SendSyntaxMessage(playerid, "Votre maison possède une entrée arrière. Vous devez définir une sortie dans l'intérieur. (/h backdoor)");
		}

		SendHelpMessage(playerid, "Utilisez /househelp pour obtenir de l'aide sur la gestion de votre maison.");
		Save_House(houseid);

		new mes[256];
		format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent(AVANT): %i | Argent(APRÈS): %i] a acheté la maison [H_MYSQL: %i] pour -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), HouseInfo[houseid][hID], HouseInfo[houseid][hPrice]);
		insert_log(mes, PROPERTY_LOG, 3, PlayerInfo[playerid][pAccID]);
		return 1;
	}
	else if (CompareStrings(option, "hmenu")) return cmd::hmenu(playerid, "");
	else if (CompareStrings(option, "upgrade"))
	{
		new houseid;

		if ((houseid = GetNearInsideHouse(playerid)) == -1) return SendErrorMessage(playerid, "Vous devez être à l'intérieur de la maison.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

		new parameters[16], arg[112];

		if (sscanf(secoption, "s[16]S()[112]", parameters, arg))
		{
			SendHelpMessage(playerid, "/house upgrade [paramètre]");
			SendSyntaxMessage(playerid, "radio {33CC33}[$500]{FFFFFF} | lock | alarm | cupboard {33CC33}[$600]{FFFFFF} | door | repairdoor | heal");
			return 1;
		}

		if (CompareStrings(parameters, "radio"))
		{
			if (HouseInfo[houseid][hRadio]) return SendErrorMessage(playerid, "Cette maison possède déjà une radio.");
			if (GetCash(playerid) < 500) return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent. Nécessaire : $500.");

			HouseInfo[houseid][hRadio] = 1;
			SQL_SetInteger("houses", "radio", HouseInfo[houseid][hRadio], HouseInfo[houseid][hID]);
			GiveCash(playerid, -500);
			SendSyntaxMessage(playerid, "Vous avez acheté une radio pour $500. Utilisez /setstation.");
			return 1;
		}

		else if (CompareStrings(parameters, "lock"))
		{
			new level;

			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/house upgrade lock [niveau (1-5)]");
				SendSyntaxMessage(playerid, "Chaque niveau coûte $250 de plus que le précédent [Niveau 1 - $250 | Niveau 5 - $1250].");
				return 1;
			}
			if (level < 1 || level > 5) return SendErrorMessage(playerid, "Le niveau du verrou doit être entre 1 et 5.");
			if (GetCash(playerid) < 250 * level) return SendErrorMessage(playerid, "Le coût du verrou de niveau %i est de $%i.", level, level * 250);
			if (HouseInfo[houseid][hLock] >= level) return SendErrorMessage(playerid, "Vous avez déjà un verrou de niveau %i.", HouseInfo[houseid][hLock]);

			HouseInfo[houseid][hLock] = level;
			SQL_SetInteger("houses", "lock", HouseInfo[houseid][hLock], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 250);
			SendSyntaxMessage(playerid, "Vous avez acheté un verrou de niveau %i pour $%i.", HouseInfo[houseid][hLock], level * 250);
			return 1;
		}

		else if (CompareStrings(parameters, "alarm"))
		{
			new level;

			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/house upgrade alarm [niveau (1-3)]");
				SendSyntaxMessage(playerid, "Chaque niveau coûte $500 de plus que le précédent [Niveau 1 - $500 | Niveau 3 - $1500].");
				return 1;
			}
			if (level < 1 || level > 3) return SendErrorMessage(playerid, "Le niveau de l'alarme doit être entre 1 et 3.");
			if (GetCash(playerid) < 500 * level) return SendErrorMessage(playerid, "Le coût de l'alarme de niveau %i est de $%i.", level, level * 500);
			if (HouseInfo[houseid][hAlarm] >= level) return SendErrorMessage(playerid, "Vous avez déjà une alarme de niveau %i.", HouseInfo[houseid][hAlarm]);

			HouseInfo[houseid][hAlarm] = level;
			SQL_SetInteger("houses", "alarm", HouseInfo[houseid][hAlarm], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 500);
			SendSyntaxMessage(playerid, "Vous avez acheté une alarme de niveau %i pour $%i.", HouseInfo[houseid][hAlarm], level * 500);
			return 1;
		}

		else if (CompareStrings(parameters, "door"))
		{
			new level;

			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/house upgrade door [niveau (1-2)]");
				SendSyntaxMessage(playerid, "Chaque niveau coûte $750 de plus que le précédent [Niveau 1 - $750 | Niveau 2 - $1500].");
				return 1;
			}
			if (level < 1 || level > 2) return SendErrorMessage(playerid, "Le niveau de la porte doit être entre 1 et 2.");
			if (GetCash(playerid) < 750 * level) return SendErrorMessage(playerid, "Le coût de la porte de niveau %i est de $%i.", level, level * 750);
			if (HouseInfo[houseid][hDoor] >= level) return SendErrorMessage(playerid, "Vous avez déjà une porte de niveau %i.", HouseInfo[houseid][hDoor]);

			HouseInfo[houseid][hDoor] = level;
			SQL_SetInteger("houses", "door", HouseInfo[houseid][hDoor], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 750);
			SendSyntaxMessage(playerid, "Vous avez acheté une porte de niveau %i pour $%i.", HouseInfo[houseid][hDoor], level * 750);
			return 1;
		}

		else if (CompareStrings(parameters, "cupboard"))
		{
			if (HouseInfo[houseid][hCupboard]) return SendErrorMessage(playerid, "Vous avez déjà une armoire.");
			if (GetCash(playerid) < 600) return SendErrorMessage(playerid, "Le coût de l'armoire est de $600.");

			HouseInfo[houseid][hCupboard] = 1;
			SQL_SetInteger("houses", "cupboard", HouseInfo[houseid][hCupboard], HouseInfo[houseid][hID]);
			GiveCash(playerid, -600);
			SendSyntaxMessage(playerid, "Vous avez acheté une armoire pour $600.");
			SendSyntaxMessage(playerid, "Tapez /clothes pour l'utiliser.");
			return 1;
		}

		else if (CompareStrings(parameters, "repairdoor"))
		{
			if (!HouseInfo[houseid][hDDoor]) return SendErrorMessage(playerid, "Votre porte d'entrée n'est pas endommagée.");
			if (GetCash(playerid) < 300) return SendErrorMessage(playerid, "Le coût de la réparation de la porte est de $300.");

			HouseInfo[houseid][hDDoor] = 0;
			SQL_SetInteger("houses", "ddoor", HouseInfo[houseid][hDDoor], HouseInfo[houseid][hID]);
			UpdateHouseDoor(houseid);
			GiveCash(playerid, -300);
			SendSyntaxMessage(playerid, "Vous avez réparé la porte d'entrée pour $300.");
			return 1;
		}

		else if (CompareStrings(parameters, "heal"))
		{
			new level;

			if (sscanf(arg, "i", level))
			{
				SendHelpMessage(playerid, "/house upgrade heal [niveau (1-5)]");
				SendSyntaxMessage(playerid, "Chaque niveau coûte $400 de plus que le précédent [Niveau 1 - $400 | Niveau 5 - $2000].");
				SendSyntaxMessage(playerid, "Chaque niveau augmente la santé restaurée de 20 HP [Niveau 1 - +20 HP | Niveau 5 - +100 HP].");
				SendSyntaxMessage(playerid, "Chaque niveau augmente le délai d'utilisation de la commande de 20 secondes [Niveau 1 - 60 sec. | Niveau 5 - 140 sec.].");
				return 1;
			}
			if (HouseInfo[houseid][hHeal]) return SendErrorMessage(playerid, "La maison possède déjà une trousse de secours.");
			if (level < 1 || level > 5) return SendErrorMessage(playerid, "Le niveau de la trousse de secours doit être entre 1 et 5.");
			if (GetCash(playerid) < level * 400) return SendErrorMessage(playerid, "Le coût de la trousse de secours de niveau %i est de $%i.", level, level * 400);

			HouseInfo[houseid][hHeal] = level;
			HouseInfo[houseid][hHealAmount] = 3;
			SQL_SetInteger("houses", "heal", HouseInfo[houseid][hHeal], HouseInfo[houseid][hID]);
			SQL_SetInteger("houses", "heal_amount", HouseInfo[houseid][hHealAmount], HouseInfo[houseid][hID]);
			GiveCash(playerid, -level * 400);
			SendSyntaxMessage(playerid, "Vous avez acheté une trousse de secours de niveau %i pour $%i.", HouseInfo[houseid][hHeal], level * 400);
			return 1;
		}
	}

	else if (CompareStrings(option, "withdraw"))
	{
		new houseid;

		if ((houseid = GetNearInsideHouse(playerid)) == -1) return SendErrorMessage(playerid, "Vous devez être à l'intérieur de la maison.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

		new amount;

		if (sscanf(secoption, "i", amount))
		{
			SendHelpMessage(playerid, "/house withdraw [montant]");
			SendSyntaxMessage(playerid, "Le coffre contient $%i.", HouseInfo[houseid][hCash]);
			return 1;
		}
		if (amount > HouseInfo[houseid][hCash] || amount < 1) return SendErrorMessage(playerid, "Le coffre ne contient pas assez d'argent.");

		HouseInfo[houseid][hCash] -= amount;
		SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
		GiveCash(playerid, amount);
		SendSyntaxMessage(playerid, "Vous avez retiré $%i du coffre. Solde restant : $%i.", amount, HouseInfo[houseid][hCash]);
		return 1;
	}

	else if (CompareStrings(option, "deposit"))
	{
		new houseid;

		if ((houseid = GetNearInsideHouse(playerid)) == -1) return SendErrorMessage(playerid, "Vous devez être à l'intérieur de la maison.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

		new amount;

		if (sscanf(secoption, "i", amount))
		{
			SendHelpMessage(playerid, "/house deposit [montant]");
			SendSyntaxMessage(playerid, "Le coffre contient $%i.", HouseInfo[houseid][hCash]);
			return 1;
		}
		if (amount > GetCash(playerid) || amount < 1) return SendNotCash(playerid);

		GiveCash(playerid, -amount);
		HouseInfo[houseid][hCash] += amount;
		SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
		SendSyntaxMessage(playerid, "Vous avez déposé $%i dans le coffre. Solde : $%i.", amount, HouseInfo[houseid][hCash]);
		return 1;
	}

	else if (CompareStrings(option, "sell"))
	{
		new houseid;

		if ((houseid = GetNearOutsideHouse(playerid)) == -1) return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

		static const str_size[] = "{FFFFFF}Voulez-vous vendre la maison pour $%i ? {FFFF00}(50%% du prix initial)";
		new mes[sizeof(str_size) - 2 + 12];
		format(mes, sizeof(mes), str_size, HouseInfo[houseid][hPrice] / 2);
		Dialog_Show(playerid, SellHouse, DIALOG_STYLE_MSGBOX, "{FFFFFF}Vente de maison", mes, "Vendre", "Annuler");
		return 1;
	}

	else if (CompareStrings(option, "sellto"))
	{
		new userid, price;

		if (sscanf(secoption, "ui", userid, price)) return SendHelpMessage(playerid, "/house sellto [ID/nom du joueur] [prix]");

		new houseid;

		if ((houseid = GetNearOutsideHouse(playerid)) == -1) return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison.");
		if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");
		if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
		if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
		if (price < 0 || price > 100000000) return SendErrorMessage(playerid, "Le prix de vente doit être compris entre $0 et $100000000.");

		pTemp[userid][pHousePrice] = price;
		pTemp[userid][pHousePlayer] = playerid;
		pTemp[userid][pHouseHouse] = houseid;
		SetPVarInt(userid, #accept_time, gettime() + 30);
		SendSyntaxMessage(playerid, "Vous avez proposé à %s d'acheter votre maison pour $%i.", GetNameEx(userid), price);
		SendSyntaxMessage(userid, "%s souhaite vous vendre une maison pour $%i. Appuyez sur Y pour acheter.", GetNameEx(playerid), price);
	}

	if (CompareStrings(option, "backdoor"))
	{
		new houseid = GetNearInsideHouse(playerid);

		if (houseid == -1) return SendErrorMessage(playerid, "Vous devez être dans votre maison !");
		if (GetInterior(playerid) != HouseInfo[houseid][hExitInt] || GetWorld(playerid) != HouseInfo[houseid][hExitVW]) {
			return SendErrorMessage(playerid, "Vous devez être dans votre maison !");
		}
		if (!HouseInfo[houseid][hExtX][0] && !HouseInfo[houseid][hExtY][0] && !HouseInfo[houseid][hExtZ][0]) {
			return SendErrorMessage(playerid, "Aucune entrée arrière n'est définie pour cette maison ! (Contactez le forum dans la section propriétés)");
		}

		new Float:posX, Float:posY, Float:posZ;
		GetPlayerPos(playerid, posX, posY, posZ);

		HouseInfo[houseid][hExtX][1] = posX;
		HouseInfo[houseid][hExtY][1] = posY;
		HouseInfo[houseid][hExtZ][1] = posZ;

		HouseInfo[houseid][hExtVW][1] = GetPlayerVirtualWorld(playerid);
		HouseInfo[houseid][hExtInt][1] = GetPlayerInterior(playerid);

		Save_House(houseid);
		SendSyntaxMessage(playerid, "Vous avez défini avec succès la position de la sortie arrière pour votre maison.");
	}

	return 1;
}

CMD:hmenu(playerid, params[])
{
	new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1) 
		return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	static const str_size[] = "%s serrure de porte\n\
								Améliorations de la maison\n\
								Gestion des meubles\n\
								Locataires\n\
								Inventaire de la maison\n\
								Coffre-fort\n\
								Armoire\n\
								Vendre la maison";
	new mes[sizeof(str_size) - 4 + 7 + 1 + 1];
	
	format(mes, sizeof(mes), str_size, (HouseInfo[houseid][hLocked] != 0) ? ("Ouvrir") : ("Fermer"));
	Dialog_Show(playerid, HouseMenu, DIALOG_STYLE_LIST, "Menu de la maison", mes, "Choisir", "Annuler");
	return 1;
}

CMD:rentroom(playerid, params[])
{
	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1) 
		return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");
		
	new userid, price;

	if (sscanf(params, "ud", userid, price)) 
		return SendHelpMessage(playerid, "/rentroom [ID/nom du joueur] [prix de location par heure]");
	if (userid == playerid) 
		return 1;	
	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return SendNotNearPlayer(playerid);
	if (price < 0 || price > 500) 
		return SendErrorMessage(playerid, "Le prix de location doit être compris entre $0 et $500.");
	if (GetPlayerHouses(userid) || PlayerInfo[userid][pRentKey]) 
		return SendErrorMessage(playerid, "Le joueur possède déjà ou loue une maison.");
	if (GetCash(userid) < price) 
		return SendErrorMessage(playerid, "Le joueur n'a pas assez d'argent.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `users` WHERE `rent_key`=%i", HouseInfo[houseid][hID]);
	mysql_tquery(dbHandle, query, "OfferRentRoom", "iii", playerid, userid, price);
	return 1;
}


CMD:unrent(playerid, params[])
{
	if (PlayerInfo[playerid][pRentKey] < 1)	return SendErrorMessage(playerid, "Vous ne louez pas de maison.");

	PlayerInfo[playerid][pRentKey] = 0;
	SQL_SetInteger("users", "rent_key", 0, PlayerInfo[playerid][pID]);
	SendSyntaxMessage(playerid, "Vous avez annulé la location de la maison.");
	return 1;
}

/*
CMD:tenants(playerid, params[])
{
	new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Âû íå ðÿäîì ñ äîìîì.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Äàííûé äîì íå ïðèíàäëåæèò Âàì.");

    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `id`, `name` FROM `users` WHERE `rent_key` = %i", HouseInfo[houseid][hID]);
	mysql_tquery(dbHandle, query, "ShowTenantsAmount", "i", playerid);
	return 1;
}*/

CMD:heal(playerid, params[])
{
	new id, userid;

	if ((id = GetNearInsideHouse(playerid)) != -1 && PlayerInfo[playerid][pID] == HouseInfo[id][hOwnerID])
	{
		if (HouseInfo[id][hHeal] == 0)					return SendErrorMessage(playerid, "Il n'y a pas de trousse de secours dans la maison.");
		if (pTemp[playerid][pHealTime] > gettime())		return SendErrorMessage(playerid, "Vous pourrez utiliser la trousse de secours dans %i secondes.", pTemp[playerid][pHealTime] - gettime());
		if (sscanf(params, "u", userid))				return SendHelpMessage(playerid, "/heal [ID/nom du joueur]");
		if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
		if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
		if (PlayerInfo[userid][pInjured])				return SendErrorMessage(playerid, "Le joueur est mort/KO/blessé.");
		if (GetPlayerHealthEx(userid) >= 100.0)			return SendErrorMessage(playerid, "Le joueur est en bonne santé.");

		if (playerid != userid)
		{
			SendSyntaxMessage(playerid, "Vous avez utilisé une trousse de secours (+%i HP) sur le joueur %s. Attendez %i secondes avant de pouvoir réutiliser la commande.", (HouseInfo[id][hHeal] * 20), GetNameWithMask(userid), 40 + (HouseInfo[id][hHeal] * 20));
			SendSyntaxMessage(userid, "%s a utilisé une trousse de secours sur vous. (+%i HP)", GetNameWithMask(playerid), (HouseInfo[id][hHeal] * 20));
		}
		else
		{
			SendSyntaxMessage(playerid, "Vous avez utilisé une trousse de secours (+%i HP) sur vous-même. Attendez %i secondes avant de pouvoir réutiliser la commande.", (HouseInfo[id][hHeal] * 20), 40 + (HouseInfo[id][hHeal] * 20));
		}

		ResetDamages(userid);
		StopPlayerAnims(userid);
		UnFreezePlayer(userid);

		pTemp[playerid][pHealTime] = gettime() + 40 + (HouseInfo[id][hHeal] * 20);

		new Float:hp;
		AntiCheatGetHealth(playerid, hp);
		SetPlayerHealth(playerid, hp + float(HouseInfo[id][hHeal]) * 20.0);

		if (GetPlayerHealthEx(userid) > 100.0) SetPlayerHealth(userid, 100.0);

		HouseInfo[id][hHealAmount]--;
		if (!HouseInfo[id][hHealAmount]) HouseInfo[id][hHeal] = 0;
		Save_House(id);
		return 1;
	}

	else if (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid)) // FD Heal
	{
		if (sscanf(params, "u", userid))				return SendHelpMessage(playerid, "/heal [ID/nom du joueur]");
		if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Vous devez être en service.");
		if (!IsPlayerLogged(userid))					return SendNotConnected(playerid);
		if (userid == playerid)							return 1;
		if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
		if (pTemp[userid][pHealPlayer] == playerid)		return SendErrorMessage(playerid, "Vous avez déjà proposé des soins à ce joueur.");
		if (PlayerInfo[userid][pInjured] != 0)			return SendErrorMessage(playerid, "Ce joueur est blessé/mort.");
		if (GetPlayerHealthEx(userid) >= 100.0)			return SendErrorMessage(playerid, "Le joueur est en bonne santé.");

		pTemp[userid][pHealPlayer] = playerid;
		SetPVarInt(userid, #accept_time, gettime() + 30);
		SendSyntaxMessage(userid, "%s souhaite vous soigner. Appuyez sur Y pour accepter l'offre.", GetNameWithMask(playerid));
		SendSyntaxMessage(playerid, "Vous avez proposé des soins à %s.", GetNameWithMask(userid));
	}
	
	else return SendErrorMessage(playerid, "Vous n'êtes pas dans une maison.");

	return 1;
}

CMD:grantbuild(playerid, params[])
{
	new userid;

	if (sscanf(params, "u", userid)) 
		return SendHelpMessage(playerid, "/grantbuild [ID/nom du joueur]");
	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);
	if (userid == playerid) 
		return 1;
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return SendNotNearPlayer(playerid);

	new houseid;

	if ((houseid = GetNearInsideHouse(playerid)) == -1) 
		return SendErrorMessage(playerid, "Vous devez être à l'intérieur de la maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	if (pTemp[userid][pBuildHouse] == HouseInfo[houseid][hID])
	{
		SendSyntaxMessage(playerid, "Vous avez retiré l'accès au mobilier pour le joueur %s.", GetNameWithMask(userid));
		SendSyntaxMessage(userid, "%s vous a retiré l'accès au mobilier.", GetNameWithMask(playerid));
		pTemp[userid][pBuildHouse] = 0;
		return 1;
	}

	SendSyntaxMessage(playerid, "Vous avez accordé l'accès au mobilier au joueur %s.", GetNameWithMask(userid));
	SendSyntaxMessage(userid, "%s vous a accordé l'accès au mobilier pour un travail collaboratif.", GetNameWithMask(playerid));
	pTemp[userid][pBuildHouse] = HouseInfo[houseid][hID];
	return 1;
}

CMD:cache(playerid, params[])
{
	new houseid;

	if ((houseid = GetNearInsideHouse(playerid)) == -1) 
		return SendErrorMessage(playerid, "Vous devez être à l'intérieur de la maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	GetPlayerPos(playerid, HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]);

	Save_House(houseid);
	SendSyntaxMessage(playerid, "Vous avez modifié l'emplacement de la cachette.");
	return 1;
}

ALTX:hbreaklock("/hbl", "/hbreak");
CMD:hbreaklock(playerid, params[])
{
	if (!PoliceOnline())	return SendErrorMessage(playerid, "Il doit y avoir au moins %i policiers en jeu.", NEED_COPS_TO_CRIME);

	new houseid = GetNearHouse(playerid);
	if (houseid == -1)						 		  return SendErrorMessage(playerid, "Vous devez être près de l'entrée d'une maison.");
	if (!IsPlayerOutsideHouse(playerid, houseid, 3.0)) return SendErrorMessage(playerid, "Vous devez être près de l'entrée d'une maison.");
	if (HouseInfo[houseid][hOwnerID] == 0)   		  return SendErrorMessage(playerid, "Seules les maisons achetées peuvent être cambriolées.");
	if (HouseInfo[houseid][hLocked] == 0)   			  return SendErrorMessage(playerid, "Les portes de la maison sont déjà ouvertes.");
	if (JackingLock[playerid] != 0)         			  return SendErrorMessage(playerid, "Vous êtes déjà en train de forcer une porte.");
	if (IsPlayerCrouching(playerid) == false) 		  return SendErrorMessage(playerid, "Vous devez être accroupi.");

	foreach(new i : Player) {
		if (BreakingHouse[i] == houseid) return SendErrorMessage(playerid, "Quelqu'un est déjà en train de forcer les portes de cette maison.");
	}

	new param[10];
	if (sscanf(params, "s[10]", param)) {
		SendHelpMessage(playerid, "/hbl [type] ou hbreaklock [type]");
		SendSyntaxMessage(playerid, "(q)uite - effraction silencieuse avec un crochet. (plus long)");
		SendSyntaxMessage(playerid, "(n)oisy - effraction bruyante avec un pied-de-biche, active l'alarme si présente. (plus rapide)");
		return 1;
	}	

	if (CompareStrings(param, "q") || CompareStrings(param, "quite"))
	{
		if (!IsPlayerHasItem(playerid, INV_PICKLOCK)) return SendErrorMessage(playerid, "Vous devez avoir un crochet.");

		pTemp[playerid][JackingType] = 3;
		BreakingHouse[playerid] = houseid;

		new htime;

		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = 250 + HouseInfo[houseid][hLock] * 250;
			case 1: htime = 250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.05);
			case 2: htime = 250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.15);
		}

		static const str_size[] = "{"COL_GREY"}Vous voulez commencer à forcer les portes de la maison ? Cela prendra %i secondes.";
		new mes[sizeof(str_size) + 6 + 70];
		format(mes, sizeof(mes), str_size, (!HouseInfo[houseid][hApart]) ? (GetHouseZone(houseid)) : (GetApartmentZone(HouseInfo[houseid][hApart])), htime);
		Dialog_Show(playerid, StartBreakingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Effraction des portes", mes, "Commencer", "Annuler");
	}
	else if (CompareStrings(param, "n") || CompareStrings(param, "noisy"))
	{
		if (!CheckHanditem(playerid, 18634)) return SendErrorMessage(playerid, "Vous devez avoir un pied-de-biche dans les mains. (/clothing)");
		pTemp[playerid][JackingType] = 4;
		BreakingHouse[playerid] = houseid;

		new htime;

		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.6);
			case 1: htime = floatround((250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.05)) * 0.6);
			case 2: htime = floatround((250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.15)) * 0.6);
		}

		static const str_size[] = "{"COL_GREY"}Vous voulez commencer à forcer les portes de la maison %s, %i? Cela prendra %i secondes.";
		new mes[sizeof(str_size) + 76];
		format(mes, sizeof(mes), str_size, (!HouseInfo[houseid][hApart]) ? (GetHouseZone(houseid)) : (GetApartmentZone(HouseInfo[houseid][hApart])), htime);
		Dialog_Show(playerid, StartBreakingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Effraction des portes", mes, "Commencer", "Annuler");
	} else {
		SendHelpMessage(playerid, "/hbl [type] ou hbreaklock [type]");
		SendSyntaxMessage(playerid, "(q)uite - effraction silencieuse avec un crochet. (plus long)");
		SendSyntaxMessage(playerid, "(n)oisy - effraction bruyante avec un pied-de-biche, active l'alarme si présente. (plus rapide)");
		return 1;
	}

	return 1;
}