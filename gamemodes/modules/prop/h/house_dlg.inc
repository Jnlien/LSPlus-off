Dialog:HouseMenu(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new houseid;
	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	switch(listitem)
	{
		case 0: cmd::lock(playerid, ""); // serrure de porte
		case 1: { // Améliorations de la maison
			static dlgtxt[] = "{FFFFFF}Réparer la porte [%s] {33CC33}[$300]{FFFFFF}\n\
								Acheter une radio {33CC33}[$500]{FFFFFF}\n\
								Améliorer la porte\n\
								Améliorer la serrure\n\
								Améliorer l'alarme\n\
								Acheter une armoire {33CC33}[$600]{FFFFFF}\n\
								Acheter une trousse de secours";

			new diatxt[sizeof(dlgtxt) + 30], door[30];	

			if (!HouseInfo[houseid][hDDoor]) format(door, sizeof(door), "{9ACD32}PAS NÉCESSAIRE{FFFFFF}");
			else 							format(door, sizeof(door), "{FF6347}NÉCESSAIRE{FFFFFF}");

			format(diatxt, sizeof(diatxt), dlgtxt, door);

			Dialog_Show(playerid, HouseUpgrade, DIALOG_STYLE_LIST, "Améliorations de la maison", diatxt, "Choisir", "Retour");
		}
		case 2: cmd::furniture(playerid, ""); // Gestion des meubles
		case 3: // Locataires
		{
			new query[128];
			mysql_format(dbHandle, query, sizeof(query), "SELECT `id`, `name` FROM `users` WHERE `rent_key` = %i LIMIT %i", HouseInfo[houseid][hID], MAX_TENAT_SPACE);
			mysql_tquery(dbHandle, query, "ShowTenantsAmount", "i", playerid);
		}
		case 4: // Inventaire de la maison
		{
			if (!IsPlayerInsideHouse(playerid, houseid))		return SendErrorMessage(playerid, "Vous devez être à l'intérieur de la maison.");
				
			PrintHouseInv(playerid, houseid);
		}
		case 5: // Coffre-fort
		{
			static dlgtxt[] = "Dans le coffre-fort : $%i\n\
								{FF6347}>{FFFFFF} Déposer\n\
								{FF6347}>{FFFFFF} Retirer";

			new diatxt[sizeof(dlgtxt) + 11];		
			format(diatxt, sizeof(diatxt), dlgtxt, HouseInfo[houseid][hCash]);

			Dialog_Show(playerid, HouseSafeChoose, DIALOG_STYLE_TABLIST, "Coffre-fort", diatxt, "Choisir", "Retour");
			return 1;
		}
		case 6: cmd::clothes(playerid, ""); // Armoire à vêtements
		case 7: Dialog_Show(playerid, SellHouseChoose, DIALOG_STYLE_LIST, "{FFFFFF}Vente de la maison", "Vendre la maison à un joueur\nVendre la maison à l'État", "Choisir", "Retour");
	}
	return 1;
}

Dialog:HouseUpgrade(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	switch(listitem)
	{
		case 0: { // Réparer la porte
			if (!HouseInfo[houseid][hDDoor])	return SendErrorMessage(playerid, "Votre porte d'entrée n'est pas endommagée.");
			if (GetCash(playerid) < 300)		return SendErrorMessage(playerid, "Le coût de la réparation de la porte est de $300.");

			HouseInfo[houseid][hDDoor] = 0;
			SQL_SetInteger("houses", "ddoor", HouseInfo[houseid][hDDoor], HouseInfo[houseid][hID]);
			UpdateHouseDoor(houseid);
			GiveCash(playerid, -300);
			SendSyntaxMessage(playerid, "Vous avez réparé la porte d'entrée pour $300.");
			return 1;
		}
		case 1: { // Acheter une radio
			if (HouseInfo[houseid][hRadio])	return SendErrorMessage(playerid, "Une radio est déjà installée dans cette maison.");
			if (GetCash(playerid) < 500)		return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent. Nécessaire : $500.");

			HouseInfo[houseid][hRadio] = 1;
			SQL_SetInteger("houses", "radio", HouseInfo[houseid][hRadio], HouseInfo[houseid][hID]);
			GiveCash(playerid, -500);
			SendSyntaxMessage(playerid, "Vous avez acheté une radio pour $500. Utilisez /setstation.");
			return 1;
		}
		case 2: { // Améliorer la porte
			static dlgtxt[] = "Niveau\tType de porte\tCaractéristique\tPrix\n\
								{C3C3C3}Niveau 1{FFFFFF}\tEn bois\t+5% de protection contre le crochetage\t{33CC33}$750\n\
								{C3C3C3}Niveau 2{FFFFFF}\tEn fer\t+15% de protection contre le crochetage\t{33CC33}$1500\n";

			Dialog_Show(playerid, HouseUpgradeDoor, DIALOG_STYLE_TABLIST_HEADERS, "Améliorer la porte", dlgtxt, "Acheter", "Retour");
		}
		case 3: { // Améliorer la serrure
			static dlgtxt[] = "Niveau\tCaractéristique\tPrix\n\
								{C3C3C3}Niveau 1{FFFFFF}\t+5% de protection contre le crochetage\t{33CC33}$250\n\
								{C3C3C3}Niveau 2{FFFFFF}\t+10% de protection contre le crochetage\t{33CC33}$500\n\
								{C3C3C3}Niveau 3{FFFFFF}\t+15% de protection contre le crochetage\t{33CC33}$750\n\
								{C3C3C3}Niveau 4{FFFFFF}\t+20% de protection contre le crochetage\t{33CC33}$1000\n\
								{C3C3C3}Niveau 5{FFFFFF}\t+25% de protection contre le crochetage\t{33CC33}$1250\n";

			Dialog_Show(playerid, HouseUpgradeLock, DIALOG_STYLE_TABLIST_HEADERS, "Améliorer la serrure", dlgtxt, "Acheter", "Retour");
		}
		case 4: { // Améliorer l'alarme
			static dlgtxt[] = "Niveau\tCaractéristique\tPrix\n\
								{C3C3C3}Niveau 1{FFFFFF}\tStandard\t{33CC33}$500\n\
								{C3C3C3}Niveau 2{FFFFFF}\tMoyenne\t{33CC33}$1000\n\
								{C3C3C3}Niveau 3{FFFFFF}\tBonne\t{33CC33}$1500\n";

			Dialog_Show(playerid, HouseUpgradeAlarm, DIALOG_STYLE_TABLIST_HEADERS, "Améliorer l'alarme", dlgtxt, "Acheter", "Retour");
		}
		case 5: { // Acheter une armoire
			if (HouseInfo[houseid][hCupboard])	return SendErrorMessage(playerid, "Vous avez déjà une armoire.");
			if (GetCash(playerid) < 600)			return SendErrorMessage(playerid, "Le coût de l'armoire est de $600.");

			HouseInfo[houseid][hCupboard] = 1;
			SQL_SetInteger("houses", "cupboard", HouseInfo[houseid][hCupboard], HouseInfo[houseid][hID]);
			GiveCash(playerid, -600);
			SendSyntaxMessage(playerid, "Vous avez acheté une armoire pour $600.");
			SendSyntaxMessage(playerid, "Utilisez /clothes pour l'utiliser.");
			return 1;
		}
		case 6: { // Acheter une trousse de secours
			static dlgtxt[] = "Niveau\tSanté\tRécupération\tPrix\n\
								{C3C3C3}Niveau 1{FFFFFF}\t+20 HP\t+60 secondes\t{33CC33}$400\n\
								{C3C3C3}Niveau 2{FFFFFF}\t+40 HP\t+80 secondes\t{33CC33}$800\n\
								{C3C3C3}Niveau 3{FFFFFF}\t+60 HP\t+100 secondes\t{33CC33}$1200\n\
								{C3C3C3}Niveau 4{FFFFFF}\t+80 HP\t+120 secondes\t{33CC33}$1600\n\
								{C3C3C3}Niveau 5{FFFFFF}\t+100 HP\t+140 secondes\t{33CC33}$2000\n";

			Dialog_Show(playerid, HouseUpgradeHeal, DIALOG_STYLE_TABLIST_HEADERS, "Acheter une trousse de secours", dlgtxt, "Acheter", "Retour");	
		}
	}

	return 1;
}

Dialog:HouseUpgradeDoor(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	listitem += 1;

	if (HouseInfo[houseid][hDoor] >= listitem)	return SendErrorMessage(playerid, "Vous avez déjà une porte de niveau %i.", HouseInfo[houseid][hDoor]);
	if (GetCash(playerid) < 750 * listitem)		return SendErrorMessage(playerid, "Vous n'avez pas $%i.", listitem * 750);

	HouseInfo[houseid][hDoor] = listitem;
	SQL_SetInteger("houses", "door", HouseInfo[houseid][hDoor], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 750);
	SendSyntaxMessage(playerid, "Vous avez acheté une porte de niveau %i pour $%i.", HouseInfo[houseid][hDoor], listitem * 750);
	return 1;
}

Dialog:HouseUpgradeLock(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	listitem += 1;

	if (HouseInfo[houseid][hLock] >= listitem)	return SendErrorMessage(playerid, "Vous avez déjà un verrou de niveau %i.", HouseInfo[houseid][hLock]);
	if (GetCash(playerid) < 250 * listitem)		return SendErrorMessage(playerid, "Vous n'avez pas $%i.", listitem * 250);
	
	HouseInfo[houseid][hLock] = listitem;
	SQL_SetInteger("houses", "lock", HouseInfo[houseid][hLock], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 250);
	SendSyntaxMessage(playerid, "Vous avez acheté un verrou de niveau %i pour $%i.", HouseInfo[houseid][hLock], listitem * 250);
	return 1;
}

Dialog:HouseUpgradeAlarm(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	listitem += 1;

	if (HouseInfo[houseid][hAlarm] >= listitem)	return SendErrorMessage(playerid, "Vous avez déjà une alarme de niveau %i.", HouseInfo[houseid][hAlarm]);
	if (GetCash(playerid) < 500 * listitem)		return SendErrorMessage(playerid, "Vous n'avez pas $%i.", listitem * 500);

	HouseInfo[houseid][hAlarm] = listitem;
	SQL_SetInteger("houses", "alarm", HouseInfo[houseid][hAlarm], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 500);
	SendSyntaxMessage(playerid, "Vous avez acheté une alarme de niveau %i pour $%i.", HouseInfo[houseid][hAlarm], listitem * 500);
	return 1;
}

Dialog:HouseUpgradeHeal(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	listitem += 1;
	
	if (HouseInfo[houseid][hHeal] >= listitem)	return SendErrorMessage(playerid, "Une trousse de secours est déjà installée dans cette maison.");
	if (GetCash(playerid) < listitem * 400)		return SendErrorMessage(playerid, "Vous n'avez pas $%i.", listitem * 400);

	HouseInfo[houseid][hHeal] = listitem;
	HouseInfo[houseid][hHealAmount] = 3;
	SQL_SetInteger("houses", "heal", HouseInfo[houseid][hHeal], HouseInfo[houseid][hID]);
	SQL_SetInteger("houses", "heal_amount", HouseInfo[houseid][hHealAmount], HouseInfo[houseid][hID]);
	GiveCash(playerid, -listitem * 400);
	SendSyntaxMessage(playerid, "Vous avez acheté une trousse de secours de niveau %i pour $%i.", HouseInfo[houseid][hHeal], listitem * 400);
	return 1;
}

Dialog:HouseSafeChoose(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");
	if (!listitem)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	if (listitem == 1)
	{
		Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Déposer de l'argent dans le coffre-fort", "Entrez le montant d'argent que\nvous souhaitez déposer dans le coffre-fort:\n\t\t(jusqu'à $25.000.000)", "Choisir", "Annuler");
	}
	
	if (listitem == 2)
	{
		static dlgtxt[] = "Entrez le montant d'argent que\nvous souhaitez retirer du coffre-fort:\n\t\t(jusqu'à $%s)";

		new diatxt[sizeof(dlgtxt) + 10];

		format(diatxt, sizeof(diatxt), dlgtxt, FormatNumber(HouseInfo[houseid][hCash]));
		Dialog_Show(playerid, HouseWithDraw, DIALOG_STYLE_INPUT, "Retirer de l'argent du coffre-fort", diatxt, "Choisir", "Annuler");
	}
	
	return 1;
}

Dialog:HouseDeposit(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	new amount;
	if (sscanf(inputtext, "i", amount))			 return Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Déposer de l'argent dans le coffre-fort", "Entrez le montant d'argent que\nvous souhaitez déposer dans le coffre-fort:\n\t\t(jusqu'à $25.000.000)", "Choisir", "Annuler");
	if (amount > GetCash(playerid) || amount < 1) return Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Déposer de l'argent dans le coffre-fort", "VOUS N'AVEZ PAS ASSEZ D'ARGENT!\n\nEntrez le montant d'argent que\nvous souhaitez déposer dans le coffre-fort:\n\t\t(jusqu'à $25.000.000)", "Choisir", "Annuler");

	GiveCash(playerid, -amount);
	HouseInfo[houseid][hCash] += amount;
	SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
	SendSyntaxMessage(playerid, "Vous avez déposé $%i dans le coffre-fort. Solde: $%i.", amount, HouseInfo[houseid][hCash]);
	return 1;
}

Dialog:HouseWithDraw(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1)				return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	new amount;
	if (sscanf(inputtext, "i", amount))			 		 return Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Déposer de l'argent dans le coffre-fort", "Entrez le montant d'argent que\nvous souhaitez déposer dans le coffre-fort:\n\t\t(jusqu'à $25.000.000)", "Choisir", "Annuler");
	if (amount > HouseInfo[houseid][hCash] || amount < 1) return Dialog_Show(playerid, HouseDeposit, DIALOG_STYLE_INPUT, "Déposer de l'argent dans le coffre-fort", "IL N'Y A PAS ASSEZ D'ARGENT DANS LE COFFRE !\n\nEntrez le montant d'argent que\nvous souhaitez déposer dans le coffre-fort:\n\t\t(jusqu'à $25.000.000)", "Choisir", "Annuler");

	HouseInfo[houseid][hCash] -= amount;
	SQL_SetInteger("houses", "cash", HouseInfo[houseid][hCash], HouseInfo[houseid][hID]);
	GiveCash(playerid, amount);
	SendSyntaxMessage(playerid, "Vous avez retiré $%i du coffre-fort. Solde restant: $%i.", amount, HouseInfo[houseid][hCash]);

	return 1;
}

Dialog:SellHouseChoose(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	switch(listitem)
	{
		case 0:
		{
			Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de la maison", "{ffffff}Vente de la maison. Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		}
		case 1:
		{
			if (!IsPlayerOutsideHouse(playerid, houseid))	return SendErrorMessage(playerid, "Vous devez être à l'extérieur de la maison.");
				
			static const str_size[] = "{FFFFFF}Voulez-vous vendre la maison pour $%i ? {FFFF00}(50% de la valeur)";
			new mes[sizeof(str_size) - 2 + 12];

			format(mes, sizeof(mes), str_size, HouseInfo[houseid][hPrice]/2/*+GetFurniturePrice(0, houseid)*/, '%');
			Dialog_Show(playerid, SellHouse, DIALOG_STYLE_MSGBOX, "Vente de la maison", mes, "Vendre", "Annuler");
		}
	}

	return 1;
}

Dialog:BuyInterior(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
		
	new houseid = strval(inputtext);

	if (houseid < 0 || houseid >= MAX_HOUSES || !HouseInfo[houseid][hID])
	{
		Dialog_Show(playerid, BuyInterior, DIALOG_STYLE_INPUT, "{FFFFFF}Commande d'intérieur", "{FFFFFF}Entrez l'ID de la maison dont vous souhaitez changer l'intérieur :\nCette maison doit vous appartenir !", "Suivant", "Fermer");
		return SendErrorMessage(playerid, "Maison introuvable.");
	}
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])
	{
		Dialog_Show(playerid, BuyInterior, DIALOG_STYLE_INPUT, "{FFFFFF}Commande d'intérieur", "{FFFFFF}Entrez l'ID de la maison dont vous souhaitez changer l'intérieur :\nCette maison doit vous appartenir !", "Suivant", "Fermer");
		return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");
	}
	
	SetPlayerBuyInterior(playerid, 1, houseid);
	return 1;
}

Dialog:BuyInteriorAccept(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowMenuForPlayer(buy_interior, playerid);

	new houseid = pTemp[playerid][pInteriorHouse] - 1;

	if (houseid < 0 || houseid >= MAX_HOUSES || !HouseInfo[houseid][hID])	return SetPlayerBuyInterior(playerid, 0);
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])
	{
		SetPlayerBuyInterior(playerid, 0);
		return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");
	}
	if (GetCash(playerid) < 750)
	{
		ShowMenuForPlayer(buy_interior, playerid);
		return SendNotCash(playerid);
	}
	
	GiveCash(playerid, -750);
	SendSyntaxMessage(playerid, "Vous avez changé l'intérieur de votre maison [ID: %i] pour $750.", houseid);
	SetHouseInterior(houseid, pTemp[playerid][pInteriorID]);
	SetPlayerBuyInterior(playerid, 0);
	
	new paycheck;

	for (new i; i < MAX_FURNITURE; i++)
	{
		if (!HFurn[houseid][i][fID] || !HFurn[houseid][i][fOn]) continue;
		paycheck += (HFurn[houseid][i][fMarketPrice] - (HFurn[houseid][i][fMarketPrice] & 2)) / 2;
		HFurn[houseid][i][fID] = 0;
		HFurn[houseid][i][fOn] = 0;
		DestroyDynamicObject(HFurn[houseid][i][fObject]);
	}

	HouseInfo[houseid][hFurniture] = 0;
	HouseInfo[houseid][hOutsideFurniture] = 0;
	GiveCash(playerid, paycheck);

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `furnitures` WHERE `building` = %i AND `bType` = 0", HouseInfo[houseid][hID]);
	mysql_tquery(dbHandle, query);
	return 1;
}

Dialog:SellHouseTo(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new houseid;

	if ((houseid = GetNearHouse(playerid)) == -1)					return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'une maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	if (houseid < 0 || houseid >= MAX_HOUSES || HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return 1;
		
	new userid, price;

	if (sscanf(inputtext, "ui", userid, price))
	{
		Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de la maison", "{ffffff}Vente de la maison. Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendErrorMessage(playerid, "Format incorrect.");
	}
	if (userid == playerid)
	{
		Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de la maison", "{ffffff}Vente de la maison. Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendErrorMessage(playerid, "Vous ne pouvez pas vendre une maison à vous-même.");
	}
	if (!IsPlayerLogged(userid))
	{
		Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de la maison", "{ffffff}Vente de la maison. Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendNotConnected(playerid);
	}
	
	if (!PlayerNearPlayer(3.0, playerid, userid))
	{
		Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de la maison", "{ffffff}Vente de la maison. Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendNotNearPlayer(playerid);
	}
	
	if (price < 0 || price > 100000000)
	{
		Dialog_Show(playerid, SellHouseTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de la maison", "{ffffff}Vente de la maison. Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendErrorMessage(playerid, "Le prix de vente doit être compris entre $0 et $100000000.");
	}
	
	new mes[64];
	format(mes, sizeof(mes), "sellto %s", inputtext);
	cmd::house(playerid, mes);
	return 1;
}

Dialog:SellHouse(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new houseid;

	if ((houseid = GetNearOutsideHouse(playerid)) == -1)			return SendErrorMessage(playerid, "Aucune maison à proximité.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	if (PlayerInfo[playerid][pSpawnPoint] == HouseInfo[houseid][hID]) {
		PlayerInfo[playerid][pSpawnType] = 0;
		PlayerInfo[playerid][pSpawnPoint] = 1;
		SendHelpMessage(playerid, "Votre point de spawn a été automatiquement changé pour un emplacement par défaut à Palomino Creek. (/settings)");
	}

	new money = PlayerInfo[playerid][pCash], cash;

	GiveCash(playerid, HouseInfo[houseid][hPrice] / 2);
	PlaySound(playerid, 1052);
	SendSyntaxMessage(playerid, "Vous avez vendu votre maison avec succès pour $%i.", HouseInfo[houseid][hPrice] / 2);

	if (HouseInfo[houseid][hCash] > 0) 
	{
		SendSyntaxMessage(playerid, "Après la vente de la maison, $%i restaient dans le coffre-fort et ont été retirés avec succès.", HouseInfo[houseid][hCash]);
		GiveCash(playerid, HouseInfo[houseid][hCash]);
		cash = HouseInfo[houseid][hCash];
	}

	new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent(AVANT): %i | Argent(APRÈS): %i] a vendu une maison [H_MYSQL: %i] pour -- $%i + $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), 
	HouseInfo[houseid][hID], HouseInfo[houseid][hPrice] / 2, cash);
	insert_log(mes, PROPERTY_LOG, 1, PlayerInfo[playerid][pAccID]);

	Sell_House(houseid);
	return 1;
}

Dialog:ChangeHouseSpawn(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new houseid = strval(inputtext);

	if (houseid < 0 || houseid >= MAX_HOUSES) 
		return SendErrorMessage(playerid, "Maison introuvable.");
	if (!HouseInfo[houseid][hID] || (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID] && HouseInfo[houseid][hID] != PlayerInfo[playerid][pRentKey])) 
		return SendErrorMessage(playerid, "Vous ne résidez pas dans cette maison.");

	SendSyntaxMessage(playerid, "Vous allez maintenant apparaître chez vous : %s.", (!HouseInfo[houseid][hApart]) ? (GetHouseZone(houseid)) : (GetApartmentZone(HouseInfo[houseid][hApart])));
	PlayerInfo[playerid][pSpawnType] = 1;
	PlayerInfo[playerid][pSpawnPoint] = HouseInfo[houseid][hID];
	
	SQL_SetInteger("users", "spawntype", PlayerInfo[playerid][pSpawnType], PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "spawnpoint", PlayerInfo[playerid][pSpawnPoint], PlayerInfo[playerid][pID]);
	return 1;
}

Dialog:ChangeHouseInterior(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new index, houseid = GetPVarInt(playerid, "house_change");

	if (houseid < 0 || houseid >= MAX_HOUSES || !HouseInfo[houseid][hID])	return SendErrorMessage(playerid, "Maison introuvable.");

	for (new i = 0; i < MAX_INTERIORS; i++)
	{
		if (!AdminTeles[i][telesOn] || HouseInfo[houseid][hClass] != AdminTeles[i][telesType]) continue;

		if (index == listitem)
		{
			index = i;
			break;
		}
		index++;
	}

	SetHouseInterior(houseid, index);
	SendAdmMessageToPlayer(playerid, "Maison [ID: %i | INT: %s].", houseid, AdminTeles[index][telesMapName]);
	return 1;
}

Dialog:EvictTenant(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");

	new 
		dlgmes[70],
		name[MAX_PLAYER_NAME]
	;

	if (!listitem) 
		Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FFFFFF}Entrez l'ID/nom du joueur que vous souhaitez loger dans votre maison", "Suivant", "Retour");
	else 
		listitem--;

	SetPVarInt(playerid, #Temp, listitem);

	format(name, sizeof(name), "%s", GetNameFromBD(pTemp[playerid][pListed][listitem]));		
	format(dlgmes, sizeof(dlgmes), "{FFFFFF}Voulez-vous vraiment expulser %s ?", name);
	
	ConfirmDialog(playerid, "Confirmation", dlgmes, "EvictTenantConfirm");
	return 1;
}

Dialog:HouseRequestRent(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::hmenu(playerid, "");
	
	new userid;
	if (sscanf(inputtext, "u", userid)) 
		return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FFFFFF}Entrez l'ID/nom du joueur que vous souhaitez loger dans votre maison", "Suivant", "Retour");
	
	if (userid == playerid) 
		return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FF6347}ID JOUEUR INCORRECT !\n{FFFFFF}Entrez l'ID/nom du joueur que vous souhaitez loger dans votre maison", "Suivant", "Retour");
	
	if (!IsPlayerLogged(userid)) 
		return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FF6347}ID JOUEUR INCORRECT !\n{FFFFFF}Entrez l'ID/nom du joueur que vous souhaitez loger dans votre maison", "Suivant", "Retour");
	
	if (!PlayerNearPlayer(3.0, playerid, userid)) 
		return Dialog_Show(playerid, HouseRequestRent, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FF6347}LE JOUEUR DOIT ÊTRE PROCHE !\n{FFFFFF}Entrez l'ID/nom du joueur que vous souhaitez loger dans votre maison", "Suivant", "Retour");
	
	if (GetPlayerHouses(userid) || PlayerInfo[userid][pRentKey]) 
		return SendErrorMessage(playerid, "Le joueur possède déjà ou loue une maison.");

	SetPVarInt(playerid, #RentUserid, userid);
	Dialog_Show(playerid, HouseRequestRent2, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FFFFFF}Entrez le montant du loyer :", "Suivant", "Retour");
	return 1;
}

Dialog:HouseRequestRent2(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;
	
	new price, userid = GetPVarInt(playerid, #RentUserid);
	if (sscanf(inputtext, "i", price)) 
		return Dialog_Show(playerid, HouseRequestRent2, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FFFFFF}Entrez le montant du loyer :", "Suivant", "Retour");
	if (price < 0 || price > 500) 
		return Dialog_Show(playerid, HouseRequestRent2, DIALOG_STYLE_INPUT, "Nouveau locataire", "{FF6347}Le montant du loyer doit être compris entre $0 et $500 !\n{FFFFFF}Entrez le montant du loyer :", "Suivant", "Retour");
	if (GetCash(userid) < price) 
		return SendErrorMessage(playerid, "Le joueur n'a pas assez d'argent.");

	new houseid;
	if ((houseid = GetNearInsideHouse(playerid)) == -1) 
		return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre maison.");
	if (HouseInfo[houseid][hOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Cette maison ne vous appartient pas.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `users` WHERE `rent_key`=%i", HouseInfo[houseid][hID]);
	mysql_tquery(dbHandle, query, "OfferRentRoom", "iii", playerid, userid, price);

	return 1;
}

Dialog:StartBreakingLock(playerid, response, listitem, inputtext[])
{
	new houseid = BreakingHouse[playerid];
	
	BreakingHouse[playerid] = 0;
	
	if (!response)									  return 1;
	if (!IsPlayerOutsideHouse(playerid, houseid, 3.0)) return SendErrorMessage(playerid, "Vous devez être près de l'entrée de la maison.");
	if (HouseInfo[houseid][hOwnerID] == 0)   		  return SendErrorMessage(playerid, "Seules les maisons achetées peuvent être cambriolées.");
	if (HouseInfo[houseid][hLocked] == 0)   			  return SendErrorMessage(playerid, "Les portes de la maison sont ouvertes.");
	if (IsPlayerCrouching(playerid) == false)		  return SendErrorMessage(playerid, "Vous devez vous accroupir.");
 
	if (pTemp[playerid][JackingType] == 3) {
		if (!IsPlayerHasItem(playerid, INV_PICKLOCK)) return SendErrorMessage(playerid, "Vous devez avoir un crochet pour forcer la serrure.");
		
		new htime;
		
		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = 250 + HouseInfo[houseid][hLock] * 250;
			case 1: htime = 250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.05);
			case 2: htime = 250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.15);
		}

		JackingLock[playerid] = htime;	
		RemovePlayerItem(playerid, INV_PICKLOCK, 1);

	} else if (pTemp[playerid][JackingType] == 4) {
		if (!CheckHanditem(playerid, 18634)) return SendErrorMessage(playerid, "Vous devez avoir un pied-de-biche en main.");
		PlaySound(playerid, 1138);
		HouseAlarm(houseid, true);

		new htime;
		
		switch(HouseInfo[houseid][hDoor]) {
			case 0: htime = 250 + HouseInfo[houseid][hLock] * 250;
			case 1: htime = 250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.05);
			case 2: htime = 250 + HouseInfo[houseid][hLock] * 250 + floatround((250 + HouseInfo[houseid][hLock] * 250) * 0.15);
		}

		JackingLock[playerid] = htime;	

	} else return 1;
	
	BreakingHouse[playerid] = houseid;
	cmd::me(playerid, "sort quelque chose et commence à s'occuper de la serrure de la porte d'entrée.");
	return 1;
}