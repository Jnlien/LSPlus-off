ALTX:garage("/g");
CMD:garage(playerid, params[])
{
	new id = GetNearGarage(playerid);

	if (id == -1)	return SendErrorMessage(playerid, "Vous devez être à proximité d'un garage.");

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "(/g)arage [info / buy / sell / repairdoor]");
		return 1;
	}

	if (CompareStrings(option, "info"))
	{
		new dialog_header[64] = "{FFFFFF}Informations\t{FFFFFF}Valeur\n", dialog_body[1024], temp[128];

		format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

		format(temp, sizeof(temp), "ID\t%i\nPrix\t%i\nPortes\t%s\n", id, GarageInfo[id][garagePrice], (GarageInfo[id][garageLocked]) ? ("Fermées") : ("Ouvertes"));
		strcat(dialog_body, temp);

		if (GarageInfo[id][garageHouse])
		{
			format(temp, sizeof(temp), "Attaché à la maison\t%i\n", GarageInfo[id][garageHouse]);
			strcat(dialog_body, temp);
		}

		if (GarageInfo[id][garageBiz])
		{
			format(temp, sizeof(temp), "Attaché au business\t%i\n", GarageInfo[id][garageBiz]);
			strcat(dialog_body, temp);
		}

		if (GarageInfo[id][garageOwner])
		{
			format(temp, sizeof(temp), "Propriétaire\t%s", GarageInfo[id][garageOwnerName]);
			strcat(dialog_body, temp);
		}

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Informations sur le garage", dialog_body, "Fermer", "");
		return 1;
	}

	else if (CompareStrings(option, "buy"))
	{
		if (GarageInfo[id][garageOwner])	return SendErrorMessage(playerid, "Ce garage n'est pas à vendre.");
		if (GarageInfo[id][garageHouse])	return SendErrorMessage(playerid, "Ce garage est attaché à une maison.");
		if (GarageInfo[id][garageBiz])	return SendErrorMessage(playerid, "Ce garage est attaché à un business.");

		new count = 0;

		for(new i = 0; i < MAX_GARAGES; i++)
		{
			if (!GarageInfo[i][garageID] || GarageInfo[i][garageHouse] || GarageInfo[i][garageBiz] || PlayerInfo[playerid][pID] != GarageInfo[i][garageOwner]) continue;

			count++;
			break;
		}

		if (count)											return SendErrorMessage(playerid, "Vous ne pouvez pas posséder plus de garages.");
		if (GetCash(playerid) < GarageInfo[id][garagePrice])	return SendNotCash(playerid);

		new money = PlayerInfo[playerid][pCash];

		GiveCash(playerid, -GarageInfo[id][garagePrice]);
		SendSyntaxMessage(playerid, "Vous avez acheté le garage [ID: %i] pour $%i.", id, GarageInfo[id][garagePrice]);
		GarageInfo[id][garageOwner] = PlayerInfo[playerid][pID];
		format(GarageInfo[id][garageOwnerName], 24, "%s", GetNameEx(playerid));
		
		SQL_SetInteger("garage", "owner", GarageInfo[id][garageOwner], GarageInfo[id][garageID]);
		SQL_SetString("garage", "ownername", GarageInfo[id][garageOwnerName], GarageInfo[id][garageID]);

		new mes[256];
		format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent (AVANT): %s | Argent (APRÈS): %s] a acheté le garage [G_MYSQL: %i] pour -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid),
		GarageInfo[id][garageID], GarageInfo[id][garagePrice]);
		insert_log(mes, PROPERTY_LOG, 5, PlayerInfo[playerid][pAccID]);
		return 1;
	}

	else if (CompareStrings(option, "sell"))
	{
		if (GarageInfo[id][garageOwner] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Ce garage ne vous appartient pas.");
		if (GarageInfo[id][garageHouse])									return SendErrorMessage(playerid, "Ce garage est attaché à une maison.");
		if (GarageInfo[id][garageBiz])									return SendErrorMessage(playerid, "Ce garage est attaché à un business.");

		new money = PlayerInfo[playerid][pCash];
		
		GiveCash(playerid, GarageInfo[id][garagePrice]);
		SendSyntaxMessage(playerid, "Vous avez vendu le garage [ID: %i] pour $%i.", id, GarageInfo[id][garagePrice]);
		GarageInfo[id][garageOwner] = 0;
		GarageInfo[id][garageOwnerName][0] = EOS;
		SQL_SetInteger("garage", "owner", 0, GarageInfo[id][garageID]);
		SQL_SetString("garage", "ownername", "", GarageInfo[id][garageID]);

		new mes[256];
		format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent (AVANT): %i | Argent (APRÈS): %i] a vendu le garage [G_MYSQL: %i] pour -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid),
		GarageInfo[id][garageID], GarageInfo[id][garagePrice]);
		insert_log(mes, PROPERTY_LOG, 6, PlayerInfo[playerid][pAccID]);
	}

	else if (CompareStrings(option, "repairdoor"))
	{
		if (!GarageInfo[id][garageDDoor]) return SendErrorMessage(playerid, "Votre porte d'entrée n'est pas endommagée.");
		if (GetCash(playerid) < 300)		 return SendErrorMessage(playerid, "Le coût de la réparation de la porte est de $300.");

		GarageInfo[id][garageDDoor] = 0;
		SQL_SetInteger("garage", "ddoor", GarageInfo[id][garageDDoor], GarageInfo[id][garageID]);
		UpdateGarageDoor(id);
		GiveCash(playerid, -300);
		SendSyntaxMessage(playerid, "Vous avez réparé la porte d'entrée pour $300.");
		return 1;
	}
	return 1;
}
