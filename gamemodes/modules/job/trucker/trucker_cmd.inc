CMD:tpda(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] != JOB_TRUCKER) return 1;
	if (!IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Vous devez être dans un véhicule.");

	Dialog_Show(playerid, PDAMenu, DIALOG_STYLE_LIST, "{FFFFFF}Ordinateur de bord - Menu", "{FFFFFF}Afficher les entreprises\nAfficher les commandes de toutes les entreprises\nAfficher les informations sur le navire", "Choisir", "Annuler");
	return 1;
}

CMD:trailer(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] != JOB_TRUCKER) return 1;
	if (!IsPlayerInAnyVehicle(playerid)) return 1;

	new vehicle = GetPlayerVehicleID(playerid), trailerid = GetVehicleTrailer(vehicle);

	if (!IsTruckCar(vehicle)) return 1;
	if (trailerid == 0) return SendErrorMessage(playerid, "Aucune remorque n'est attachée au véhicule.");

	if (isnull(params)) {
		SendHelpMessage(playerid, "/trailer [cargo / detach / lights / lock]");
		return 1;
	}

	if (CompareStrings(params, "cargo") || CompareStrings(params, "list")) {
		new title[52], msg[512] = "{9ACD32}Marchandise\t{9ACD32}Quantité", count;

		format(title, sizeof(title), "{FFFFFF}Remorque [Capacité : %i]", GetMaxCargoVehicle(trailerid));

		for (new i; i != MAX_TRUCK_PRODUCT; i++) {
			if (!VehicleInfo[trailerid][carCargoAmount][i]) continue;

			format(msg, sizeof(msg), "%s\n%s\t%i unités", msg, truckcargo_product[i], VehicleInfo[trailerid][carCargoAmount][i]);
			count++;
		}

		if (!count) return SendErrorMessage(playerid, "Aucune marchandise dans la remorque.");

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "Fermer", "");
		return 1;
	}

	else if (CompareStrings(params, "detach")) {
		if (GetVehicleSpeed(vehicle) > 3) return SendErrorMessage(playerid, "Vous devez être à l'arrêt.");

		DetachTrailerFromVehicle(vehicle);
		SendSyntaxMessage(playerid, "Remorque détachée avec succès.");
		return 1;
	}

	else if (CompareStrings(params, "lights")) {
		new engine, lights, alarm, doors, bonnet, boot, objective;
		
		GetVehicleParamsEx(trailerid, engine, lights, alarm, doors, bonnet, boot, objective);

		if (lights != 0) {
			GameTextForPlayer(playerid, "~w~FEUX DE LA REMORQUE ~r~ÉTEINTS", 2500, 4);
			VehicleLight(trailerid, false);
			return 1;
		}

		GameTextForPlayer(playerid, "~w~FEUX DE LA REMORQUE ~g~ALLUMÉS", 2500, 4);
		VehicleLight(trailerid, true);
		return 1;
	}

	else if (CompareStrings(params, "lock")) {
		if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) 
			return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");
		
		PlaySound(playerid, 1145);

		if (VehicleInfo[trailerid][carLocked]) 
			GameTextForPlayer(playerid, "~w~REMORQUE ~g~DÉVERROUILLÉE", 2500, 4);
		else 
			GameTextForPlayer(playerid, "~w~REMORQUE ~r~VERROUILLÉE", 2500, 4);

		ToggleVehicleLock(trailerid, (VehicleInfo[trailerid][carLocked]) ? (false) : (true));
		return 1;
	}

	else {
		SendHelpMessage(playerid, "/trailer [cargo / detach / lights / lock]");
	}
	return 1;
}

CMD:cargo(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] != JOB_TRUCKER)	return 1;

	new parametrs[8],	arg[4];
	    
	if (sscanf(params, "s[8]S()[4]", parametrs, arg))
	{
		SendHelpMessage(playerid, "/cargo [list | place | putdown | pickup | buy | sell]");
		return 1;
	}

	new mes[128];

	if (CompareStrings(parametrs, "list"))
	{
		new vehicle = GetPlayerVehicleID(playerid);
		
		if (!vehicle && (vehicle = GetNearBootVehicle(playerid)) == 0) 
			return SendErrorMessage(playerid, "Vous devez être dans un véhicule ou près d'un coffre.");
		if (IsTruckCar(vehicle)) 
			return SendSyntaxMessage(playerid, "Utilisez /trailer cargo pour voir la liste des marchandises chargées.");
		if (!IsTruckerJob(vehicle)) 
			return SendErrorMessage(playerid, "Ce véhicule n'est pas adapté au transport de marchandises.");
		if (ValidTruckForPlayer(playerid, vehicle) == 0) 
			return SendErrorMessage(playerid, "Votre compétence de travail ne vous permet pas d'utiliser ce véhicule. Tapez /jobhelp pour voir la liste des véhicules disponibles.");
		if (VehicleInfo[vehicle][carLocked] == 1) 
			return SendErrorMessage(playerid, "Le véhicule est verrouillé.");

		new title[52], msg[512] = "{9ACD32}Marchandise\t{9ACD32}Quantité", count;

		format(title, sizeof(title), "%s (Capacité : %i)", VehicleNames[GetVehicleModel(vehicle)-400], GetMaxCargoVehicle(vehicle));

		for (new i; i != MAX_TRUCK_PRODUCT; i++)
		{
			pTemp[playerid][pCargoListed][i] = 0;
		}

		for (new i; i != MAX_TRUCK_PRODUCT; i++)
		{
			if (VehicleInfo[vehicle][carCargoAmount][i] == 0) continue;
			
			pTemp[playerid][pCargoListed][count] = i;
			format(msg, sizeof(msg), "%s\n%s\t%i unités", msg, truckcargo_product[i], VehicleInfo[vehicle][carCargoAmount][i]);
			count++;
		}

		if (!count) 
			return SendErrorMessage(playerid, "Aucune marchandise dans le véhicule.");

		Dialog_Show(playerid, CargoList, DIALOG_STYLE_TABLIST_HEADERS, title, msg, "Prendre", "Annuler");
		SetPVarInt(playerid, #cargo_vehicle, vehicle);
		return 1;
	}
	
	else if (CompareStrings(parametrs, "place"))
	{
		if (pTemp[playerid][pCargoID] == 0) 
			return SendErrorMessage(playerid, "Vous n'avez rien dans les mains.");
		
		new vehicle;

		if ((vehicle = GetNearBootVehicle(playerid)) == 0) 
			return SendErrorMessage(playerid, "Vous devez être près du coffre d'un véhicule.");
		if (IsTruckerJob(vehicle) == 0) 
			return SendErrorMessage(playerid, "Ce véhicule n'est pas adapté au transport de marchandises.");
		if (ValidTruckForPlayer(playerid, vehicle) == 0) 
			return SendErrorMessage(playerid, "Votre compétence de travail ne vous permet pas d'utiliser ce véhicule. Tapez /jobhelp pour voir la liste des véhicules disponibles.");
		if (VehicleInfo[vehicle][carLocked] == 1) 
			return SendErrorMessage(playerid, "Le véhicule est verrouillé.");
		if (VehicleInfo[vehicle][carTrunkLock] == 0) 
			return SendErrorMessage(playerid, "Le coffre de ce véhicule est fermé.");	    
		if (!IsValidProductVehicle(vehicle, pTemp[playerid][pCargoID]-1)) 
			return SendErrorMessage(playerid, "Cette marchandise n'est pas compatible avec ce véhicule.");

		new amount;

		for (new i; i != MAX_TRUCK_PRODUCT; i++)
		{
			amount += VehicleInfo[vehicle][carCargoAmount][i];
		}

		if (amount >= GetMaxCargoVehicle(vehicle)) 
			return SendErrorMessage(playerid, "Pas de place dans le véhicule.");

		switch (GetVehicleModel(vehicle))
		{
			case 600, 543, 605, 422, 478, 554: 
				VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(2912, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
			case 443: 
				VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(3593, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
		}

		switch (GetVehicleModel(vehicle))
		{
			case 600: 
				AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, picador_attach[amount][0], picador_attach[amount][1], picador_attach[amount][2], 0.0, 0.0, 0.0);
			case 543, 605: 
				AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, sadler_attach[amount][0], sadler_attach[amount][1], sadler_attach[amount][2], 0.0, 0.0, 0.0);
			case 422: 
				AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, bobcat_attach[amount][0], bobcat_attach[amount][1], bobcat_attach[amount][2], 0.0, 0.0, 0.0);
			case 478: 
				AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, walton_attach[amount][0], walton_attach[amount][1], walton_attach[amount][2], 0.0, 0.0, 0.0);
			case 554: 
				AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, yosemite_attach[amount][0], yosemite_attach[amount][1], yosemite_attach[amount][2], 0.0, 0.0, 0.0);
		}
		
		Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
		cmd::putdown(playerid, "");

		if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) 
			RemovePlayerAttachedObject(playerid, ATTACH_HANDS);

		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		VehicleInfo[vehicle][carCargoAmount][pTemp[playerid][pCargoID]-1]++;
		pTemp[playerid][pCargoID] = 0;
		return 1;
	}

	else if (CompareStrings(parametrs, "putdown"))
	{
		if (pTemp[playerid][pCargoID] == 0) 
			return SendErrorMessage(playerid, "Vous n'avez rien dans les mains.");

		new id = -1;
		for (new i; i != MAX_CARGO_OBJ; i++)
		{
			if (CargoObject[i][oOn] != 0) continue;

			id = i;
			break;
		}

		if (id == -1) 
			return SendErrorMessage(playerid, "Nombre maximum de caisses déposées atteint.");

		CargoObject[id][oOn] = 1;
		CargoObject[id][oProduct] = pTemp[playerid][pCargoID];
		cmd::putdown(playerid, "");
		if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) 
			RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		pTemp[playerid][pCargoID] = 0;

		new Float: x, Float: y, Float: z;

		GetXYInFrontOfPlayer(playerid, x, y, z, 1.5);
		CargoObject[id][oZ] -= 2.0;
		CargoObject[id][oObj] = CreateDynamicObject(2912, x, y, z, 0.0, 0.0, 0.0, GetWorld(playerid), GetInterior(playerid));
		CargoObject[id][oX] = x,
		CargoObject[id][oY] = y,
		CargoObject[id][oZ] = z,
		CargoObject[id][oInt] = GetInterior(playerid),
		CargoObject[id][oVW] = GetWorld(playerid);

		format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]", truckcargo_product[CargoObject[id][oProduct]-1]);

		CargoObject[id][oLabel] = CreateDynamic3DTextLabel(mes, 0xFFFFFFFF, x, y, z+0.1, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, GetWorld(playerid), GetInterior(playerid), -1, 100.0);
		return 1;
	}

	else if (CompareStrings(parametrs, "pickup"))
	{
		if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) || pTemp[playerid][pCargoID] != 0) 
			return SendErrorMessage(playerid, "Vos mains sont déjà occupées.");

		new id;
		if ((id = IsPlayerNearCargo(playerid)) == -1) 
			return SendErrorMessage(playerid, "Aucune caisse à proximité à ramasser.");

		CargoObject[id][oOn] = 0;
		pTemp[playerid][pCargoID] = CargoObject[id][oProduct];
		cmd::liftup(playerid, "");
		SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2912, 5, 0.102000, 0.306000, -0.228999, -1.100001, 14.499999, -1.300000, 1.000000, 1.000000, 1.000000);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

		if (IsValidDynamicObject(CargoObject[id][oObj])) 
			DestroyDynamicObject(CargoObject[id][oObj]);

		if (IsValidDynamic3DTextLabel(CargoObject[id][oLabel]))
		{
			DestroyDynamic3DTextLabel(CargoObject[id][oLabel]);
			CargoObject[id][oLabel] = Text3D:INVALID_3DTEXT_ID;
		}
		return 1;
	}

	else if (CompareStrings(parametrs, "buy"))
	{
		if (IsPlayerInAnyVehicle(playerid))
		{
			new id = -1;
			if ((id = TruckCargo_Nearest(playerid, 10.0)) == -1)	return SendErrorMessage(playerid, "Aucune entreprise à proximité.");
			if (TruckCargo[id][tLocked] == 1)					return SendErrorMessage(playerid, "L'entreprise est fermée.");
			
			new vehicle = GetPlayerVehicleID(playerid), trailerid = GetVehicleTrailer(vehicle);

			if (GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 554 || GetVehicleModel(vehicle) == 499 || GetVehicleModel(vehicle) == 414 || GetVehicleModel(vehicle) == 456 || GetVehicleModel(vehicle) == 455)
				trailerid = vehicle;
			
			if (trailerid == 0) 													return SendErrorMessage(playerid, "Attachez une remorque au véhicule.");
			if (ValidTruckForPlayer(playerid, trailerid) == 0)					return SendErrorMessage(playerid, "Votre compétence de travail ne vous permet pas d'utiliser ce véhicule. Tapez /jobhelp pour voir la liste des véhicules disponibles.");
			if (VehicleInfo[trailerid][carLocked] == 1) 							return SendErrorMessage(playerid, "Le véhicule est verrouillé.");
			if (IsTakeProduct(TruckCargo[id][tProductID]))						return SendErrorMessage(playerid, "Ce produit ne peut être acheté qu'en dehors du véhicule.");
			if (!IsValidProductVehicle(trailerid, TruckCargo[id][tProductID]))	return SendErrorMessage(playerid, "Ce produit n'est pas compatible avec ce véhicule.");

			new capacity = strval(arg), content = GetMaxCargoVehicle(trailerid);
				
			if (isnull(arg))
			{
				SendHelpMessage(playerid, "/cargo buy [quantité de marchandise]");
				switch(TruckCargo[id][tProductID])
				{
					case TRUCKER_BRICKS: SendClientMessageEx(playerid, -1, "Capacité : %i palettes de briques.", content/6);
					case TRUCKER_WOODS: SendClientMessageEx(playerid, -1, "Capacité : %i lots de bois.", content/18);
					default: SendClientMessageEx(playerid, -1, "Capacité : %i unités.", content);
				}
				return 1;
			}

			if (capacity < 1 || capacity > GetMaxCargoVehicle(trailerid))	return SendErrorMessage(playerid, "Quantité incorrecte : 1 - %i.", GetMaxCargoVehicle(trailerid));
			if (TruckCargo[id][tStorage] < capacity)							return SendErrorMessage(playerid, "Il n'y a pas %i unités en stock.", capacity);
			if (GetCash(playerid) < TruckCargo[id][tPrice] * capacity)		return SendNotCash(playerid);
			
			new amount, prodid = -1;

			for(new i; i != MAX_TRUCK_PRODUCT; i++)
			{
				if (VehicleInfo[trailerid][carCargoAmount][i] == 0) continue;
					
				amount += VehicleInfo[trailerid][carCargoAmount][i];
				prodid = i;
			}

			switch(TruckCargo[id][tProductID])
			{
				case TRUCKER_BRICKS:
				{
					if (amount + capacity > content/6)	return SendErrorMessage(playerid, "Pas assez de place pour %i unités.", capacity);
				}
				case TRUCKER_WOODS:
				{
					if (amount + capacity > content/18)	return SendErrorMessage(playerid, "Pas assez de place pour %i unités.", capacity);
				}
				default:
				{
					if (amount + capacity > content)		return SendErrorMessage(playerid, "Pas assez de place pour %i unités.", capacity);
				}
			}

			if (amount && prodid != TruckCargo[id][tProductID])	return SendErrorMessage(playerid, "Le véhicule contient déjà \"%s\", vous ne pouvez charger que ce produit.", truckcargo_product[prodid]);


			if (GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 554)
			{
				for(new i = 0; i != capacity; i++)
				{
					switch(GetVehicleModel(vehicle))
					{
						case 443:
						{
							VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(3593, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
							AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, paker_attach[amount][0], paker_attach[amount][1], paker_attach[amount][2], paker_attach[amount][3], 0.0, 0.0);
						}
						case 578:
						{
							VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject((TruckCargo[id][tProductID] == TRUCKER_WOODS)?(18609):(1685), 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
							if (TruckCargo[id][tProductID] == TRUCKER_WOODS)
							{
								AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, dft_attach[0], dft_attach[1], dft_attach[2], 0.0, 0.0, dft_attach[3]);
							}
							else if (TruckCargo[id][tProductID] == TRUCKER_BRICKS)
							{
								AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, dft_attach_brick[amount][0], dft_attach_brick[amount][1], dft_attach_brick[amount][2], 0.0, 0.0, 0.0);
							}
						}
						case 554:
						{
							VehicleInfo[vehicle][carCargoObj][amount] = CreateDynamicObject(1685, 0.0, 0.0, 0.0, -100.0, 0.0, 0.0);
							AttachDynamicObjectToVehicle(VehicleInfo[vehicle][carCargoObj][amount], vehicle, yosemite_attach_brick[0], yosemite_attach_brick[1], yosemite_attach_brick[2], 0.0, 0.0, 0.0);
						}
					}
					amount++;
				}
			}
			Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
			VehicleInfo[trailerid][carCargoAmount][TruckCargo[id][tProductID]] += capacity;
			GiveCash(playerid, -TruckCargo[id][tPrice] * capacity);
			TruckCargo[id][tStorage] -= capacity;
			SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock : %i / %i\nPrix : $%i / unité.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
			UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
			return 1;
		}

		new id = -1;
		if ((id = TruckCargo_Nearest(playerid, 1.5)) == -1)												return SendErrorMessage(playerid, "Aucune entreprise à proximité.");
		if (TruckCargo[id][tLocked] == 1)																return SendErrorMessage(playerid, "L'entreprise est fermée.");
		if (!IsTakeProduct(TruckCargo[id][tProductID]))													return SendErrorMessage(playerid, "Ce produit ne peut être acheté qu'en véhicule.");
		if (!TruckCargo[id][tStorage])																	return SendErrorMessage(playerid, "Le stock de l'entreprise est vide.");
		if (GetCash(playerid) < TruckCargo[id][tPrice])													return SendNotCash(playerid);
		if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) || pTemp[playerid][pCargoID] != 0)	return SendErrorMessage(playerid, "Vos mains sont déjà occupées.");

		cmd::liftup(playerid, "");

		InfoMSG(playerid, "~w~Vous pouvez poser ~y~la caisse~w~ au sol ou dans le coffre~n~de votre véhicule, ~w~ainsi que la ramasser en appuyant sur ~w~'~h~~r~~k~~GROUP_CONTROL_BWD~~w~'.", 10);

		SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2912, 5, 0.102000, 0.306000, -0.228999, -1.100001, 14.499999, -1.300000, 1.000000, 1.000000, 1.000000);
		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
		GiveCash(playerid, -TruckCargo[id][tPrice]);
		pTemp[playerid][pCargoID] = TruckCargo[id][tProductID]+1;
		TruckCargo[id][tStorage]--;
		SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
		format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock : %i / %i\nPrix : $%i / unité.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
		UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
		return 1;
	}
	else if (CompareStrings(parametrs, "sell"))
	{
		if (IsPlayerInAnyVehicle(playerid))
		{
			new vehicle = GetPlayerVehicleID(playerid), trailerid = GetVehicleTrailer(vehicle);

			if (GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 554 || GetVehicleModel(vehicle) == 499 || GetVehicleModel(vehicle) == 414 || GetVehicleModel(vehicle) == 456) 
			{
				trailerid = vehicle;
			}

			if (trailerid == 0)									return SendErrorMessage(playerid, "Attachez une remorque au véhicule.");
			if (ValidTruckForPlayer(playerid, trailerid) == 0)	return SendErrorMessage(playerid, "Votre compétence de travail ne vous permet pas d'utiliser ce véhicule. Tapez /jobhelp pour voir la liste des véhicules disponibles.");
			if (VehicleInfo[trailerid][carLocked] == 1)			return SendErrorMessage(playerid, "Le véhicule est verrouillé.");

			new capacity = strval(arg);

			if (isnull(arg))													return SendHelpMessage(playerid, "/cargo sell [quantité de marchandise]");
			if (capacity < 1 || capacity > GetMaxCargoVehicle(trailerid))	return SendErrorMessage(playerid, "Quantité de marchandise entre 1 et %i.", GetMaxCargoVehicle(trailerid));

			new amount, prodid = -1;

			for(new i; i != MAX_TRUCK_PRODUCT; i++)
			{
				if (VehicleInfo[trailerid][carCargoAmount][i] == 0 || IsTakeProduct(i))	continue;

				amount += VehicleInfo[trailerid][carCargoAmount][i];
				prodid = i;
				break;
			}

			if (amount < capacity)	return SendErrorMessage(playerid, "Il n'y a pas assez de marchandise dans le véhicule.");

			new id = -1;

			if ((id = TruckCargo_Nearest(playerid, 15.0)) != -1)
			{
				if (TruckCargo[id][tLocked] == 1)										return SendErrorMessage(playerid, "L'entreprise est fermée.");
				if (TruckCargo[id][tStorage] + capacity > TruckCargo[id][tStorageSize])	return SendErrorMessage(playerid, "Le stock de l'entreprise est plein.");
				if (TruckCargo[id][tProductID] != prodid)								return SendErrorMessage(playerid, "Cette marchandise ne peut pas être vendue à cette entreprise.");

				if (GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 554)
				{
					for(new i = 0; i != capacity; i++)
					{
						DestroyDynamicObject(VehicleInfo[vehicle][carCargoObj][amount-1]);
						amount--;
					}
				}

				Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
				VehicleInfo[trailerid][carCargoAmount][prodid] -= capacity;
				GiveCash(playerid, TruckCargo[id][tPrice] * capacity);
				TruckCargo[id][tStorage] += capacity;
				SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
				format(mes, sizeof(mes), "~w~+$%i", TruckCargo[id][tPrice] * capacity);
				GameTextForPlayer(playerid, mes, 1000, 1);
				format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock : %i / %i\nPrix : $%i / unité.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
				UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
				return 1;
			}
			else if ((id = GetNearOutsideBiz(playerid, 15.0)) != -1)
			{
				if (BizInfo[id][bizProduct] + (biz_prod_point[BizInfo[id][bizType]] * capacity) > 400)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'entreprise.");
				if (BizInfo[id][bizPrice] == 0)															return SendErrorMessage(playerid, "Cette entreprise n'achète pas de marchandise.");
				if (biz_prod_types[BizInfo[id][bizType]] != prodid)										return SendErrorMessage(playerid, "Cette marchandise ne convient pas à cette entreprise.");

				if (GetVehicleModel(vehicle) == 443 || GetVehicleModel(vehicle) == 578 || GetVehicleModel(vehicle) == 554)
				{
					for(new i = 0; i != capacity; i++)
					{
						DestroyDynamicObject(VehicleInfo[vehicle][carCargoObj][amount-1]);
						amount--;
					}
				}

				Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
				VehicleInfo[trailerid][carCargoAmount][prodid] -= capacity;
				BizInfo[id][bizProduct] += (biz_prod_point[BizInfo[id][bizType]] * capacity);
				GiveCash(playerid, BizInfo[id][bizBuyPrice] * capacity);
				GiveBizCash(id, -BizInfo[id][bizBuyPrice] * capacity);

				if (BizInfo[id][bizProduct] >= 400)	BizInfo[id][bizBuyPrice] = 0;

				Save_Business(id);
				format(mes, sizeof(mes), "~w~+$%i", BizInfo[id][bizBuyPrice] * capacity);
				GameTextForPlayer(playerid, mes, 1000, 1);
				return 1;
			}
			else 
			{
				SendErrorMessage(playerid, "Vous devez être près d'une entreprise ou d'un business !");
			}
			return 1;
		}

		if (pTemp[playerid][pCargoID] == 0)	return SendErrorMessage(playerid, "Vous n'avez rien dans les mains.");

		new id = -1;

		if ((id = TruckCargo_Nearest(playerid, 1.5)) != -1)
		{
			if (TruckCargo[id][tLocked] == 1)								return SendErrorMessage(playerid, "L'entreprise est fermée.");
			if (TruckCargo[id][tStorage] + 1 > TruckCargo[id][tStorageSize])	return SendErrorMessage(playerid, "Le stock de l'entreprise est plein.");
			if (TruckCargo[id][tProductID] != pTemp[playerid][pCargoID]-1)	return SendErrorMessage(playerid, "Cette marchandise ne peut pas être vendue à cette entreprise.");

			cmd::putdown(playerid, "");

			if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
			
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
			GiveCash(playerid, TruckCargo[id][tPrice]);
			pTemp[playerid][pCargoID] = 0;
			format(mes, sizeof(mes), "~w~+$%i", TruckCargo[id][tPrice]);
			GameTextForPlayer(playerid, mes, 1000, 1);
			TruckCargo[id][tStorage]++;
			SQL_SetInteger("truck_cargo", "storage", TruckCargo[id][tStorage], TruckCargo[id][tID]);
			format(mes, sizeof(mes), "[{FFFF00}%s{FFFFFF}]\nStock : %i / %i\nPrix : $%i / unité.", truckcargo_product[TruckCargo[id][tProductID]], TruckCargo[id][tStorage], TruckCargo[id][tStorageSize], TruckCargo[id][tPrice]);
			UpdateDynamic3DTextLabelText(TruckCargo[id][tLabel], 0xFFFFFFFF, mes);
		}

		else if ((id = GetNearOutsideBiz(playerid)) != -1)
		{
			if (BizInfo[id][bizProduct] + biz_prod_point[BizInfo[id][bizType]]  > 400)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'entreprise.");
			if (BizInfo[id][bizPrice] == 0)												return SendErrorMessage(playerid, "Cette entreprise n'achète pas de marchandise.");
			if (biz_prod_types[BizInfo[id][bizType]] != pTemp[playerid][pCargoID]-1)		return SendErrorMessage(playerid, "Cette marchandise ne peut pas être vendue ici.");
		
			cmd::putdown(playerid, "");
			
			if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
			
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
			BizInfo[id][bizProduct] += biz_prod_point[BizInfo[id][bizType]];
			GiveCash(playerid, BizInfo[id][bizBuyPrice]);
			GiveBizCash(id, -BizInfo[id][bizBuyPrice]);
			
			if (BizInfo[id][bizProduct] >= 400)	BizInfo[id][bizBuyPrice] = 0;
			Save_Business(id);
			pTemp[playerid][pCargoID] = 0;
			return 1;
		}
		else SendErrorMessage(playerid, "Vous devez être près d'une entreprise ou d'un business !");
	}
	return 1;
}

CMD:industry(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] != JOB_TRUCKER) 
		return 1;

	new id = -1;

	if ((id = TruckCargo_Nearest(playerid)) == -1) 
		return SendErrorMessage(playerid, "Aucune entreprise à proximité.");

	Show_Industry(playerid, id);
	return 1;
}