CMD:atrash(playerid, params[])
{
	if (!CheckAdmin(playerid, FOUNDER_LEVEL)) return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/atrash [create / remove / edit / goto]");
		return 1;
	}

	if (CompareStrings(option, "create"))
	{
		new type;

		if (sscanf(secoption, "i", type)) return SendHelpMessage(playerid, "/atrash create [1 - poubelle | 2 - conteneur]");
		if (type < 1 || type > 2) return SendErrorMessage(playerid, "Le type doit être entre 1 et 2.");

        if (GetPlayerInterior(playerid) != 0 ||
            GetPlayerVirtualWorld(playerid) != 0) return SendErrorMessage(playerid, "Vous devez être à l'extérieur dans le monde virtuel 0.");

        new id = Trash_CheckFreeSlot();
		if (id == -1) return SendErrorMessage(playerid, "Nombre maximal de poubelles atteint.");

		GetPlayerPos(playerid, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]);
		TrashData[id][TrashX] += 0.5;

        TrashData[id][TrashType] = (type == 1) ? (TYPE_BIN) : (TYPE_DUMPSTER);
        TrashData[id][TrashLevel] = (TrashData[id][TrashType] == TYPE_BIN) ? 1 : 2;

		new label[64];
        format(label, sizeof(label), "%s", (TrashData[id][TrashType] == TYPE_BIN) ? ("Poubelle") : ("Conteneur"));
		TrashData[id][TrashLabel] = CreateDynamic3DTextLabel(label, 0x2ECC71FF, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]+1.25, 15.0, .testlos = 1);

        new trashModel = (TrashData[id][TrashType] == TYPE_DUMPSTER) ? 1344 : 1328;
        TrashData[id][TrashObject] = CreateDynamicObject(trashModel, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ], 0.0, 0.0, TrashData[id][TrashRZ], 0, 0);

		new query[256];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `trashes` (type,x,y,z,rz) VALUES (%i,%f,%f,%f,%f)", 
            TrashData[id][TrashType],
            TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ], TrashData[id][TrashRZ]
        );
		mysql_tquery(dbHandle, query, "OnTrashInsert", "d", id);

        SetPVarInt(playerid, #edit_trash, id + 1);
		EditDynamicObject(playerid, TrashData[id][TrashObject]);

		SendSyntaxMessage(playerid, "Vous avez créé une %s [ID : %i].", (TrashData[id][TrashType] == TYPE_BIN) ? ("poubelle") : ("conteneur"), id);
		return 1;
	}

	else if (CompareStrings(option, "remove"))
	{
		new id;

		if (sscanf(secoption, "i", id)) return SendHelpMessage(playerid, "/atrash remove [ID poubelle]");
		if (id < 0 || id >= MAX_TRASHES || !TrashData[id][TrashID]) return SendErrorMessage(playerid, "Poubelle non trouvée.");

		if (IsValidDynamicObject(TrashData[id][TrashObject]))
		{
			DestroyDynamicObject(TrashData[id][TrashObject]);
			TrashData[id][TrashObject] = INVALID_OBJECT_ID;
		}

        if (IsValidDynamic3DTextLabel(TrashData[id][TrashLabel]))
		{
			DestroyDynamic3DTextLabel(TrashData[id][TrashLabel]);
			TrashData[id][TrashLabel] = Text3D:INVALID_3DTEXT_ID;
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `trashes` WHERE `id` = %i", TrashData[id][TrashID]);
		mysql_tquery(dbHandle, query);

		TrashData[id][TrashID] = 0;
		SendAdmMessageToPlayer(playerid, "Vous avez supprimé la poubelle [ID : %i].", id);
		return 1;
	}

	else if (CompareStrings(option, "edit"))
	{
		new id;

		if (sscanf(secoption, "d", id)) return SendHelpMessage(playerid, "/atrash edit [ID poubelle]");
		if (id < 0 || id > MAX_TRASHES || !TrashData[id][TrashID]) return SendErrorMessage(playerid, "Poubelle non trouvée.");
		if (Trash_Nearest(playerid) != id) return SendErrorMessage(playerid, "Vous devez être proche de la poubelle.");

		SetPVarInt(playerid, #edit_trash, id + 1);
		EditDynamicObject(playerid, TrashData[id][TrashObject]);
		return 1;
	}

	else if (CompareStrings(option, "goto"))
	{
		new id;

		if (sscanf(secoption, "i", id)) return SendHelpMessage(playerid, "/atrash goto [ID poubelle]");
		if (id < 0 || id > MAX_TRASHES || !TrashData[id][TrashID]) return SendErrorMessage(playerid, "Poubelle non trouvée.");

		SetPlayerPosEx(playerid, TrashData[id][TrashX], TrashData[id][TrashY], TrashData[id][TrashZ]);
		SetWorld(playerid, 0);
		SetInterior(playerid, 0);
		return 1;
	}
	else
	{
		SendHelpMessage(playerid, "/atrash [create / remove / edit / goto]");
	}
	return 1;
}

CMD:afactory(playerid, params[])
{
	if (!CheckAdmin(playerid, FOUNDER_LEVEL)) return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/afactory [create / remove / goto]");
		return 1;
	}

	if (CompareStrings(option, "create"))
	{
		new name[32];

		if (sscanf(secoption, "S()[32]", name)) return SendHelpMessage(playerid, "/afactory create [Nom]");
		if (strlen(name) < 4 || strlen(name) > 32) return SendErrorMessage(playerid, "Le nom doit contenir entre 4 et 32 caractères.");

		if (GetPlayerInterior(playerid) != 0 || GetPlayerVirtualWorld(playerid) != 0)
			return SendErrorMessage(playerid, "Vous devez être à l’extérieur dans le monde virtuel 0.");

		new id = Factory_CheckFreeSlot();
		if (id == -1) return SendErrorMessage(playerid, "Nombre maximal d'usines atteint.");

		GetPlayerPos(playerid, FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ]);
		format(FactoryData[id][FactoryName], 32, "%s", name);

		new label[128];
		format(label, sizeof(label), "Usine de traitement des déchets - %s\n\n{FFFFFF}Quantité actuelle de déchets traités : {F39C12}0", FactoryData[id][FactoryName]);
		FactoryData[id][FactoryLabel] = CreateDynamic3DTextLabel(label, 0x2ECC71FF, FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ] + 0.5, 15.0, .testlos = 1);
		FactoryData[id][FactoryCP] = CreateDynamicCP(FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ], 6.0);

		new query[256];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `factories` (name,x,y,z) VALUES ('%e',%f,%f,%f)", FactoryData[id][FactoryName], FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ]);
		mysql_tquery(dbHandle, query, "OnFactoryInsert", "d", id);

		SendSyntaxMessage(playerid, "Vous avez créé une usine de traitement '%s' [ID : %i].", FactoryData[id][FactoryName], id);
		return 1;
	}
	else if (CompareStrings(option, "remove"))
	{
		new id;
		
		if (sscanf(secoption, "i", id)) return SendHelpMessage(playerid, "/afactory remove [ID]");
		if (id < 0 || id >= MAX_FACTORIES || !FactoryData[id][FactoryID]) return SendErrorMessage(playerid, "ID non trouvé.");

		if (IsValidDynamicCP(FactoryData[id][FactoryCP]))
		{
			DestroyDynamicCP(FactoryData[id][FactoryCP]);
			FactoryData[id][FactoryCP] = INVALID_OBJECT_ID;
		}
        
		if (IsValidDynamic3DTextLabel(FactoryData[id][FactoryLabel]))
		{
			DestroyDynamic3DTextLabel(FactoryData[id][FactoryLabel]);
			FactoryData[id][FactoryLabel] = Text3D:INVALID_3DTEXT_ID;
		}

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `factories` WHERE `id` = %i", TrashData[id][TrashID]);
		mysql_tquery(dbHandle, query);

		FactoryData[id][FactoryCurrent] = 0;
		FactoryData[id][FactoryID] = 0;

		SendAdmMessageToPlayer(playerid, "Vous avez supprimé l'usine de traitement des déchets [ID : %i].", id);
		return 1;
	}
	else if (CompareStrings(option, "goto"))
	{
		new id;

		if (sscanf(secoption, "i", id)) return SendHelpMessage(playerid, "/afactory goto [ID]");
		if (id < 0 || id >= MAX_FACTORIES || !FactoryData[id][FactoryID]) return SendErrorMessage(playerid, "ID non trouvé.");

		SetPlayerPosEx(playerid, FactoryData[id][FactoryX], FactoryData[id][FactoryY], FactoryData[id][FactoryZ]);
		SetWorld(playerid, 0);
		SetInterior(playerid, 0);
		return 1;
	}
	else
	{
		SendHelpMessage(playerid, "/afactory [create / remove / goto]");
	}
	return 1;
}

CMD:aindustry(playerid, params[])
{
	if (!CheckAdmin(playerid, FOUNDER_LEVEL))	return SendNotAdmin(playerid);

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/aindustry [create / remove / edit / goto / list]");
		return 1;
	}

	if (CompareStrings(option, "create"))
	{
		new type, name[64];

		if (sscanf(secoption, "is[64]", type, name))
			return SendHelpMessage(playerid, "/aindustry create [0 - matière première | 1 - production | 2 - navire | 3 - exigence pour production] [nom de l'entreprise]");

		if (type < 0 || type > 3)
			return SendErrorMessage(playerid, "Le type de production doit être entre 0 et 3.");

		if (strlen(name) < 4 || strlen(name) > 63)
			return SendErrorMessage(playerid, "Le nom doit contenir entre 4 et 63 caractères.");

		new cargoid = -1;

		for (new i = 0; i < MAX_TRUCK_CARGO; i++)
		{
			if (!TruckCargo[i][tOn]) continue;
			cargoid = i;
			break;
		}

		if (cargoid == -1)
			return SendErrorMessage(playerid, "Le nombre maximum d'entreprises a été atteint.");

		GetPlayerPos(playerid, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ]);

		TruckCargo[cargoid][tType] = type;
		TruckCargo[cargoid][tStorageSize] = 200;
		TruckCargo[cargoid][tProductID] =
		TruckCargo[cargoid][tProductAmount] =
		TruckCargo[cargoid][tPrice] =
		TruckCargo[cargoid][tStorage] =
		TruckCargo[cargoid][tPack] =
		TruckCargo[cargoid][tGps] =
		TruckCargo[cargoid][tLocked] = 0;

		format(TruckCargo[cargoid][tName], 64, "%s", name);

		new mes[128];
		format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock %i/%i\nPrix : $%i/u.", truckcargo_product[TruckCargo[cargoid][tProductID]], TruckCargo[cargoid][tStorage], TruckCargo[cargoid][tStorageSize], TruckCargo[cargoid][tPrice]);

		TruckCargo[cargoid][tLabel] = CreateDynamic3DTextLabel(mes, 0xFFFFFFFF, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ] + 0.8, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0, -1, 100.0);
		TruckCargo[cargoid][tPickup] = CreateDynamicPickup(1318, 1, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ], 0, 0, -1, 100.0);
		TruckCargo[cargoid][tOn] = 1;

		new query[256];
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `truck_cargo` (x, y, z, type, name, storage_size) VALUES (%f, %f, %f, %i, '%e', 200)",
			TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ], type, name);
		mysql_tquery(dbHandle, query, "OnTruckCargoInsert", "d", cargoid);

		SendSyntaxMessage(playerid, "Vous avez créé une entreprise [ Nom: %s | ID: %i | Type: %s ].", TruckCargo[cargoid][tName], cargoid, TruckCargo[cargoid][tPack]);
		return 1;
	}

	else if (CompareStrings(option, "remove"))
	{
		new cargoid, query[128];

		if (sscanf(secoption, "i", cargoid))
			return SendHelpMessage(playerid, "/aindustry remove [ID de l'entreprise]");

		if (cargoid < 0 || cargoid >= MAX_TRUCK_CARGO || !TruckCargo[cargoid][tOn])
			return SendErrorMessage(playerid, "Entreprise introuvable.");

		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `truck_cargo` WHERE `id` = %i", TruckCargo[cargoid][tID]);
		mysql_tquery(dbHandle, query);

		if (IsValidDynamic3DTextLabel(TruckCargo[cargoid][tLabel]))
		{
			DestroyDynamic3DTextLabel(TruckCargo[cargoid][tLabel]);
			TruckCargo[cargoid][tLabel] = Text3D:INVALID_3DTEXT_ID;
		}

		if (IsValidDynamicPickup(TruckCargo[cargoid][tPickup]))
			DestroyDynamicPickup(TruckCargo[cargoid][tPickup]);

		TruckCargo[cargoid][tOn] =
		TruckCargo[cargoid][tID] = 0;

		SendSyntaxMessage(playerid, "Entreprise [ID : %i] supprimée.", cargoid);
		return 1;
	}

	else if (CompareStrings(option, "edit"))
	{
		new id, parameters[16], arg[112];

		if (sscanf(secoption, "is[12]S()[112]", id, parameters, arg))
		{
			SendHelpMessage(playerid, "/aindustry edit [ID entreprise] [storage / size / price / unit / group / locked / gps]");
			return 1;
		}

		if (id < 0 || id >= MAX_TRUCK_CARGO || !TruckCargo[id][tOn])
			return SendErrorMessage(playerid, "Entreprise introuvable.");

		new mes[128];

		if (CompareStrings(parameters, "storage"))
		{
			new storage;

			if (sscanf(arg, "i", storage))
				return SendHelpMessage(playerid, "/aindustry edit [ID entreprise] storage [quantité de produits en stock]");
			if (storage < 0 || storage > TruckCargo[id][tStorageSize])
				return SendErrorMessage(playerid, "Quantité entre 0 et %i.", TruckCargo[id][tStorageSize]);

			TruckCargo[id][tStorage] = storage;
			SQL_SetInteger("truck_cargo", "storage", storage, TruckCargo[id][tID]);

			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock %i/%i\nPrix: $%i/u", truckcargo_product[TruckCargo[id][tProductID]], storage, TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
			UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);

			SendSyntaxMessage(playerid, "Stock de l'entreprise [ID: %i] modifié à %i.", id, storage);
			return 1;
		}
		else if (CompareStrings(parameters, "locked"))
		{
			new locked;
			if (sscanf(arg, "i", locked))
				return SendHelpMessage(playerid, "/aindustry edit [ID entreprise] locked [0 - ouvert | 1 - fermé]");
			if (locked < 0 || locked > 1)
				return SendErrorMessage(playerid, "Valeur entre 0 et 1.");

			TruckCargo[id][tLocked] = locked;
			SQL_SetInteger("truck_cargo", "locked", locked, TruckCargo[id][tID]);

			SendSyntaxMessage(playerid, "Vous avez %s l'entreprise [ID: %i].", locked ? "fermé" : "ouvert", id);
			return 1;
		}
		else if (CompareStrings(parameters, "type"))
		{
			new type;
			if (sscanf(arg, "i", type))
				return SendHelpMessage(playerid, "/aindustry edit [ID entreprise] type [0 - matière | 1 - production | 2 - bateau | 3 - exigence]");
			if (type < 0 || type > 3)
				return SendErrorMessage(playerid, "Type entre 0 et 3.");

			TruckCargo[id][tType] = type;
			SQL_SetInteger("truck_cargo", "type", type, TruckCargo[id][tID]);

			SendSyntaxMessage(playerid, "Type de l'entreprise [ID: %i] défini à %i.", id, type);
			return 1;
		}
		else if (CompareStrings(parameters, "gps"))
		{
			new visible;
			if (sscanf(arg, "i", visible))
				return SendHelpMessage(playerid, "/aindustry edit [ID entreprise] gps [0 - masqué | 1 - visible]");
			if (visible < 0 || visible > 1)
				return SendErrorMessage(playerid, "Valeur entre 0 et 1.");

			TruckCargo[id][tGps] = visible;
			SQL_SetInteger("truck_cargo", "gps", visible, TruckCargo[id][tID]);

			SendSyntaxMessage(playerid, "Vous avez %s l'entreprise [ID: %i] dans /tpda.", visible ? "rendu visible" : "masqué", id);
			return 1;
		}
		else if (CompareStrings(parameters, "size"))
		{
			new size;
			if (sscanf(arg, "i", size))
				return SendHelpMessage(playerid, "/aindustry edit [ID entreprise] size [taille]");
			if (size <= 0 || size > 10000)
				return SendErrorMessage(playerid, "Taille entre 1 et 10000.");

			TruckCargo[id][tStorageSize] = size;
			SQL_SetInteger("truck_cargo", "storage_size", size, TruckCargo[id][tID]);

			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock %i/%i\nPrix: $%i/u", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], size, TruckCargo[id][tPrice]);
			UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);

			SendSyntaxMessage(playerid, "Taille du stock de l'entreprise [ID: %i] modifiée à %i.", id, size);
			return 1;
		}
		else if (CompareStrings(parameters, "price"))
		{
			new price;
			if (sscanf(arg, "i", price))
				return SendHelpMessage(playerid, "/aindustry edit [ID entreprise] price [prix]");
			if (price < 1 || price > 10000)
				return SendErrorMessage(playerid, "Prix entre $1 et $10000.");

			TruckCargo[id][tPrice] = price;
			SQL_SetInteger("truck_cargo", "price", price, TruckCargo[id][tID]);

			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock %i/%i\nPrix: $%i/u", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], price);
			UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);

			SendSyntaxMessage(playerid, "Prix unitaire de l'entreprise [ID: %i] modifié à $%i.", id, price);
			return 1;
		}
		else if (CompareStrings(parameters, "unit"))
		{
			new unit;
			if (sscanf(arg, "i", unit))
				return SendHelpMessage(playerid, "/aindustry edit [ID entreprise] unit [unités par heure]");
			if (unit < -100 || unit > 100)
				return SendErrorMessage(playerid, "Valeur entre -100 et 100.");

			TruckCargo[id][tProductAmount] = unit;
			SQL_SetInteger("truck_cargo", "product_amount", unit, TruckCargo[id][tID]);

			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock %i/%i\nPrix: $%i/u", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
			UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);

			SendSyntaxMessage(playerid, "Production horaire de l'entreprise [ID: %i] modifiée à %i.", id, unit);
			return 1;
		}
		else if (CompareStrings(parameters, "group"))
		{
			new group;
			if (sscanf(arg, "i", group))
			{
				SendHelpMessage(playerid, "/aindustry edit [ID entreprise] group [groupe]");
				SendSyntaxMessage(playerid, "Groupe actuel de l'entreprise : %i.", TruckCargo[id][tPack]);
				return 1;
			}
			if (group < 0 || group > MAX_TRUCK_PACK)
				return SendErrorMessage(playerid, "Groupe entre 0 et %i.", MAX_TRUCK_PACK);

			TruckCargo[id][tPack] = group;
			SQL_SetInteger("truck_cargo", "pack", group, TruckCargo[id][tID]);

			SendSyntaxMessage(playerid, "Groupe de l'entreprise [ID: %i] modifié à %i.", id, group);
			return 1;
		}
		else
		{
			SendHelpMessage(playerid, "/aindustry edit [ID entreprise] [storage / size / price / unit / group / locked / gps]");
			return 1;
		}
	}
	
	else if (CompareStrings(option, "goto"))
	{
		new cargoid;

		if (sscanf(secoption, "i", cargoid))
			return SendHelpMessage(playerid, "/aindustry goto [ID de l'entreprise]");
		if (cargoid < 0 || cargoid >= MAX_TRUCK_CARGO || !TruckCargo[cargoid][tOn])
			return SendErrorMessage(playerid, "Entreprise introuvable.");

		SetPlayerPosEx(playerid, TruckCargo[cargoid][tPosX], TruckCargo[cargoid][tPosY], TruckCargo[cargoid][tPosZ]);
		SetInterior(playerid, 0);
		SetWorld(playerid, 0);
		SendSpecialMessage(playerid, "Vous avez été téléporté à l'entreprise %s [ID: %i].", TruckCargo[cargoid][tName], cargoid);
		return 1;
	}

	else if (CompareStrings(option, "list"))
	{
		pTemp[playerid][pDialogList] = 0;
		DisplayCargoGroups(playerid);
	}
	return 1;
}

CMD:payday(playerid, params[])
{
	new userid;
	if (!CheckAdmin(playerid, FOUNDER_LEVEL))
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid))
		return SendHelpMessage(playerid, "/payday [ID/Nom du joueur]");

	if (userid == INVALID_PLAYER_ID)
		return SendErrorMessage(playerid, "Le joueur n'a pas été trouvé.");

	PayDay(userid);

	SendAdmMessageToPlayer(userid, "L'administrateur %s vous a attribué un PayDay.", GetAccName(playerid));
	SendAdmMessageToPlayer(playerid, "Vous avez attribué un PayDay à %s.", GetNameEx(userid));
	return 1;
}

CMD:paydayall(playerid, params[])
{
	if (!CheckAdmin(playerid, FOUNDER_LEVEL))	
		return SendNotAdmin(playerid);

	for(new i = 0; i < MAX_PLAYERS; i++) PayDay(i);
	
	SendAdmMessageToAll("L'administrateur %s a attribué un PayDay à tous les joueurs.", GetAccName(playerid));
	return 1;
}

CMD:testmsgs(playerid, params[]) 
{
    if (!CheckAdmin(playerid, FOUNDER_LEVEL))		
        return SendNotAdmin(playerid);

    new type, str[64];
    if (sscanf(params, "is[64]", type, str))		
        return SendHelpMessage(playerid, "/testmsgs [type] [texte]");
    if (type < 0 || type > 1)					
        return 1;

    switch(type)
    {
        case 0: JobMessage(playerid, str, 10);
        case 1: InfoMSG(playerid, str, 10);
    }
    
    return 1;
}