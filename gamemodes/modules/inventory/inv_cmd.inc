ALTX:inventory("/inv");
CMD:inventory(playerid, params[])
{
	DeletePVar(playerid, #inventory_house);
	DeletePVar(playerid, #inventory_vehicle);

	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser l'inventaire pour le moment.");

	if (GetPVarInt(playerid, #pDelay)) return 1;

	new count, diatxt[64] = "Inventaire du personnage";

	new houseid;
	if ((houseid = GetNearHouse(playerid)) != -1 && HouseInfo[houseid][hOwnerID] == PlayerInfo[playerid][pID] && PlayerToPoint(1, playerid, HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
	{
		format(diatxt, sizeof(diatxt), "%s\nInventaire de la maison", diatxt);
		count++;
	}

	new vehicle = GetNearVehicle(playerid, 4.0);
	if (vehicle != 0 && (!IsADontTrunk(vehicle) || !IsAPlane(vehicle) || !IsABiker(vehicle) || !IsAHelicopter(vehicle) || !IsABoat(vehicle) || !IsABicycle(vehicle)))
	{
		if ((VehicleInfo[vehicle][carTrunkLock] && !IsPlayerInAnyVehicle(playerid)) ||
			(IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction]))
		{
			if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction])
				format(diatxt, sizeof(diatxt), "%s\nInventaire du boîte à gants", diatxt);
			else
				format(diatxt, sizeof(diatxt), "%s\nInventaire du coffre", diatxt);
			count++;
		}
	}

	vehicle = GetPlayerVehicleID(playerid);
	if (IsPlayerInAnyVehicle(playerid) && vehicle != 0 && GetPlayerHasRightDrive(playerid, vehicle))
	{
		format(diatxt, sizeof(diatxt), "%s\nInventaire de la boîte à gants", diatxt);
		count++;
	}

	if (count)
		Dialog_Show(playerid, InventoryMenu, DIALOG_STYLE_LIST, "Choix de l'inventaire :", diatxt, "Choisir", "Annuler");
	else
		PrintInv(playerid);

	return 1;
}

CMD:put(playerid, params[])
{
	if (!IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
		return SendWarningMessage(playerid, "Vous n'avez rien dans les mains.");

	if (!GetPVarInt(playerid, #attach_item))
		return SendErrorMessage(playerid, "Cette commande permet uniquement de ranger certains objets dans l'inventaire !");

	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	SetPVarInt(playerid, #attach_item, 0);

	if (GiveInvItem(playerid, pTemp[playerid][tempItem], pTemp[playerid][tempQ], pTemp[playerid][tempEx]) == -1)
		return SendErrorMessage(playerid, "Il n'y a pas de place dans l'inventaire.");

	pTemp[playerid][tempItem] = 0;
	pTemp[playerid][tempEx] = 0;
	pTemp[playerid][tempQ] = 0;

	SendSyntaxMessage(playerid, "Vous avez rangé l'objet de vos mains.");
	return 1;
}

PrintInv(playerid)
{
	new result[(MAX_INV_SLOTS+5) * 128] = "#\tObjet\tQuantité";
	if (GetPVarInt(playerid, #pDelay)) return PrintInv(playerid);

	FixInv(playerid);
	for (new i = 0; i < MAX_INV_SLOTS; i++) {
		switch (Inventory[playerid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t{C3C3C3}Emplacement vide", result, i + 1);
			}
			case 22 .. 34:
			{
				if (Inventory[playerid][i][pInvQ] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i pts]{FFFFFF}\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]);
			}
			case 78, 79:
			{
				if (Inventory[playerid][i][pInvEx] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %.1f]{FFFFFF}\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQFloat]);
			}
			case INV_GANJA, INV_MED_GANJA, 103 .. 118:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s{FFFFFF}\t%.1fg", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQFloat]);
			}
			case INV_DRUG:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[Pureté: %i%%]{FFFFFF}\t%.1fg", result, i + 1, Inventory[playerid][i][pInvName], Inventory[playerid][i][pInvEx], Inventory[playerid][i][pInvQFloat]);
			}
			case INV_GASCAN:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQ]);
			}
		}
	}

	if (PlayerInfo[playerid][pPlayerWeapon] >= 1)
	{
		format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}RANGER L'ARME", result, MAX_INV_SLOTS + 1);
		format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}JETER L'ARME", result, MAX_INV_SLOTS + 2);

		if (PlayerInfo[playerid][pPlayerWeapon] >= 22 && PlayerInfo[playerid][pPlayerWeapon] <= 34)
		{
			if (PlayerInfo[playerid][pPlayerAmmo] > 0)
				format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}RETIRER LES MUNITIONS", result, MAX_INV_SLOTS + 3);
		}
		if (PlayerInfo[playerid][pPlayerWeapon] == 23)
		{
			format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}RETIRER LE SILENCIEUX", result, MAX_INV_SLOTS + 4);
		}
	}

	if (GetPVarInt(playerid, #attach_item))
	{
		format(result, sizeof(result), "%s\n{ffffff}%i.\t{FF6347}> {FFFFFF}RANGER DE LA MAIN\n{ffffff}%i.\t{FF6347}> {FFFFFF}JETER", result, MAX_INV_SLOTS + 1, MAX_INV_SLOTS + 2);
	}

	Dialog_Show(playerid, Inventory, DIALOG_STYLE_TABLIST_HEADERS, "Inventaire", result, "Choisir", "Annuler");
	return true;
}


PrintInvEx(playerid, userid)
{
	new result[(MAX_INV_SLOTS + 1) * 64] = "#\tObjet\tQuantité";
	if (GetPVarInt(playerid, #pDelay)) return PrintInvEx(playerid, userid);

	format(result, sizeof(result), "-\t{33AA33}Argent\t$%i{FFFFFF}\n", PlayerInfo[playerid][pCash]);

	for (new i = 0; i < MAX_INV_SLOTS; i++)
	{
		switch (Inventory[playerid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Emplacement vide", result, i + 1);
			}
			case 22 .. 34:
			{
				if (Inventory[playerid][i][pInvQ] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i pts]{FFFFFF}\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]);
			}
			case 78, 79:
			{
				if (Inventory[playerid][i][pInvEx] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]);
			}
			case INV_GASCAN:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQ]);
			}
		}
	}

	new title[34];

	if (!IsAdminDuty(userid))
	{
		new mes[64];
		format(mes, sizeof(mes), "fouille %s.", GetNameWithMask(playerid));
		cmd::me(playerid, mes);

		title = "Fouille de %s";
	}
	else title = "Inventaire de %s";

	format(title, sizeof(title), title, GetNameWithMask(playerid));
	Dialog_Show(userid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	return true;
}

PrintVehInv(playerid, vehid, frisk = 0)
{
	new result[(MAX_TRUNK_INVENTORY + 1) * 64] = "#\tObjet\tQuantité";
	if (GetPVarInt(playerid, #pDelay)) return PrintVehInv(playerid, vehid, frisk);

	for (new i = 0; i < MAX_TRUNK_INVENTORY; i++)
	{
		switch (TrunkInventory[vehid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Emplacement vide", result, i + 1);
			}
			case 22 .. 34:
			{
				if (TrunkInventory[vehid][i][pInvQ] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i pts]{FFFFFF}\t1", result, i + 1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName], inventory_items[TrunkInventory[vehid][i][pInvEx]][itemName], TrunkInventory[vehid][i][pInvQ]);
			}
			case 78, 79:
			{
				if (TrunkInventory[vehid][i][pInvEx] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i + 1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName], inventory_items[TrunkInventory[vehid][i][pInvEx]][itemName], TrunkInventory[vehid][i][pInvQ]);
			}
			case INV_GASCAN:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i + 1, inventory_items[TrunkInventory[vehid][i][pInvItem]][itemName], TrunkInventory[vehid][i][pInvQ]);
			}
		}
	}

	new title[64];

	if (!frisk)
	{
		if (VehicleInfo[vehid][carFaction] && VehicleInfo[vehid][carFaction] == PlayerInfo[playerid][pFaction])
			title = "Compartiment de %s";
		else
			title = "Coffre de %s";

		format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400]);
		Dialog_Show(playerid, InventoryOther, DIALOG_STYLE_TABLIST_HEADERS, title, result, "Choisir", "Annuler");
	}
	else
	{
		if (!IsAdminDuty(playerid))
		{
			new mes[64];
			format(mes, sizeof(mes), "fouille le coffre de %s.", VehicleNames[GetVehicleModel(vehid) - 400]);
			cmd::me(playerid, mes);

			title = "Fouille du coffre de %s";
			format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400]);
		}
		else
		{
			title = "Coffre de %s | Propriétaire : %s";
			if (VehicleInfo[vehid][carOwnerID])
				format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400], VehicleInfo[vehid][carOwner]);
			else
				format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400], GetInitials(Factions[VehicleInfo[vehid][carFaction]][fName]));
		}

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	}

	if (VehicleInfo[vehid][vCorp] > 0 && CorpInfo[VehicleInfo[vehid][vCorp]][cUsed] == 1)
		SendWarningMessage(playerid, "Il y a un cadavre dans le coffre de ce véhicule.");

	return 1;
}

PrintGBInv(playerid, vehid, frisk = 0)
{
	new result[(MAX_CAR_INVENTORY + 1) * 64] = "#\tObjet\tQuantité";
	if (GetPVarInt(playerid, #pDelay)) return PrintGBInv(playerid, vehid, frisk);

	for (new i = 0; i < MAX_CAR_INVENTORY; i++)
	{
		switch (CarInventory[vehid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Emplacement vide", result, i + 1);
			}
			case 22 .. 34:
			{
				if (CarInventory[vehid][i][pInvQ] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i pts]{FFFFFF}\t1", result, i + 1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName], inventory_items[CarInventory[vehid][i][pInvEx]][itemName], CarInventory[vehid][i][pInvQ]);
			}
			case 78, 79:
			{
				if (CarInventory[vehid][i][pInvEx] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i + 1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName], inventory_items[CarInventory[vehid][i][pInvEx]][itemName], CarInventory[vehid][i][pInvQ]);
			}
			case INV_GASCAN:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i + 1, inventory_items[CarInventory[vehid][i][pInvItem]][itemName], CarInventory[vehid][i][pInvQ]);
			}
		}
	}

	new title[64];

	if (!frisk)
	{
		title = "Boîte à gants de %s";
		format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400]);
		Dialog_Show(playerid, InventoryOther, DIALOG_STYLE_TABLIST_HEADERS, title, result, "Choisir", "Annuler");
	}
	else
	{
		if (!IsAdminDuty(playerid))
		{
			new mes[64];
			format(mes, sizeof(mes), "fouille la boîte à gants de %s.", VehicleNames[GetVehicleModel(vehid) - 400]);
			cmd::me(playerid, mes);

			title = "Fouille de la boîte à gants de %s";
			format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400]);
		}
		else
		{
			title = "Boîte à gants de %s | Propriétaire : %s";
			if (VehicleInfo[vehid][carOwnerID])
				format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400], VehicleInfo[vehid][carOwner]);
			else
				format(title, sizeof(title), title, VehicleNames[GetVehicleModel(vehid) - 400], GetInitials(Factions[VehicleInfo[vehid][carFaction]][fName]));
		}

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	}

	return 1;
}

PrintHouseInv(playerid, houseid, frisk = 0)
{
	new result[(MAX_HOUSE_INVENTORY + 1) * 64] = "#\tObjet\tQuantité";
	if (GetPVarInt(playerid, #pDelay)) return PrintHouseInv(playerid, houseid, frisk);

	for (new i = 0; i < MAX_HOUSE_INVENTORY; i++)
	{
		switch (HouseInventory[houseid][i][pInvItem])
		{
			case 0:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i\t{C3C3C3}Emplacement vide", result, i + 1);
			}
			case 22 .. 34:
			{
				if (HouseInventory[houseid][i][pInvQ] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i pts]{FFFFFF}\t1", result, i + 1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName], inventory_items[HouseInventory[houseid][i][pInvEx]][itemName], HouseInventory[houseid][i][pInvQ]);
			}
			case 78, 79:
			{
				if (HouseInventory[houseid][i][pInvEx] == 0)
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", result, i + 1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName]);
				else
					format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i]{FFFFFF}\t1", result, i + 1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName], inventory_items[HouseInventory[houseid][i][pInvEx]][itemName], HouseInventory[houseid][i][pInvQ]);
			}
			case INV_GASCAN:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s {33FF66}(20L)\t1", result, i + 1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
			}
			default:
			{
				format(result, sizeof(result), "%s\n{ffffff}%i.\t%s\t%i", result, i + 1, inventory_items[HouseInventory[houseid][i][pInvItem]][itemName], HouseInventory[houseid][i][pInvQ]);
			}
		}
	}

	if (!frisk)
		Dialog_Show(playerid, InventoryOther, DIALOG_STYLE_TABLIST_HEADERS, "Planque", result, "Choisir", "Annuler");
	else
	{
		new title[42];
		if (!IsAdminDuty(playerid))
		{
			cmd::me(playerid, "fouille la maison.");
			title = "Fouille de la maison de %s";
		}
		else title = "Maison de %s";

		format(title, sizeof(title), title, HouseInfo[houseid][hOwner]);
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, result, ">>", "");
	}

	return 1;
}