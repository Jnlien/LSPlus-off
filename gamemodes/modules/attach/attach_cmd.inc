CMD:attmenu(playerid, params[])
{
	new option[24], secoption, thirdoption, fourthoption;

	if (sscanf(params, "s[24]D(-1)D(-1)D(-1)", option, secoption, thirdoption, fourthoption))
	{
		SendSyntaxMessage(playerid, "/attmenu [commande]");
		SendHelpSyntaxMessage(playerid, "liste, acheter, edit, supprimer");
		return 1;
	}

	new vehicle = GetPlayerVehicleID(playerid);

	if (!vehicle || !IsDriver(playerid))
		return SendErrorMessage(playerid, "Vous devez être assis à la place du conducteur.");
	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])
		return SendErrorMessage(playerid, "Vous n'êtes pas propriétaire de ce véhicule.");

	if (CompareStrings(option, "acheter"))
	{
		new slot, mes[sizeof(car_attach)*64];

		if (sscanf(params, "{s[7]}d", slot))
			return SendSyntaxMessage(playerid, "/attmenu acheter [ID du slot]");
		if (VehicleInfo[vehicle][carAttachObj][slot] != 0)
			return SendErrorMessage(playerid, "Ce slot est déjà utilisé.");

		SetPVarInt(playerid, #veh_editor, vehicle);
		SetPVarInt(playerid, #veh_slot, slot);

		for (new i = 0; i < sizeof(car_attach); i++)
		{
			format(mes, sizeof(mes), "%s%i(0.0, 0.0, 90.0, 1.0)\t~g~~h~$250\n", mes, car_attach[i]);
		}

		Dialog_Show(playerid, MODEL_SELECTION_OBJATT, DIALOG_STYLE_PREVIEW_MODEL, "Liste des objets attachables au véhicule", mes, "Choisir", "Fermer");
		return 1;
	}
	else if (CompareStrings(option, "supprimer"))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))
			return SendSyntaxMessage(playerid, "/attmenu supprimer [ID du slot]");
		if (VehicleInfo[vehicle][carAttach][slot] == INVALID_OBJECT_ID)
			return SendErrorMessage(playerid, "Ce slot est vide.");

		VehicleInfo[vehicle][carAttachObj][slot] = 0;
		VehicleInfo[vehicle][carAttachPosX][slot] = 0.0;
		VehicleInfo[vehicle][carAttachPosY][slot] = 0.0;
		VehicleInfo[vehicle][carAttachPosZ][slot] = 0.0;
		VehicleInfo[vehicle][carAttachRotX][slot] = 0.0;
		VehicleInfo[vehicle][carAttachRotY][slot] = 0.0;
		VehicleInfo[vehicle][carAttachRotZ][slot] = 0.0;
		DestroyDynamicObject(VehicleInfo[vehicle][carAttach][slot]);
		VehicleInfo[vehicle][carAttach][slot] = INVALID_OBJECT_ID;
		SendSyntaxMessage(playerid, "Vous avez supprimé l'objet avec succès.");
		Save_CarAttach(vehicle, slot);
		return 1;
	}
	else if (CompareStrings(option, "edit"))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))
			return SendSyntaxMessage(playerid, "/attmenu edit [ID du slot]");
		if (VehicleInfo[vehicle][carAttach][slot] == INVALID_OBJECT_ID)
			return SendErrorMessage(playerid, "Ce slot est vide.");

		new Float:x, Float:y, Float:z;
		AntiCheatGetVehiclePos(vehicle, x, y, z);

		SetPVarInt(playerid, #veh_editor, vehicle);
		SetPVarInt(playerid, #veh_slot, slot);
		SetPVarInt(playerid, #veh_obj, VehicleInfo[vehicle][carAttachObj][slot]);
		SetPVarInt(playerid, "Vehicle:AttEdit", 1);

		DestroyDynamicObject(VehicleInfo[vehicle][carAttach][slot]);
		VehicleInfo[vehicle][carAttach][slot] = INVALID_OBJECT_ID;
		VehicleInfo[vehicle][carAttach][slot] = CreateDynamicObject(VehicleInfo[vehicle][carAttachObj][slot], x, y, z + 2.0, 0.0, 0.0, 0.0);
		EditDynamicObject(playerid, VehicleInfo[vehicle][carAttach][slot]);
		return 1;
	}
	else if (CompareStrings(option, "liste"))
	{
		new message[1024] = "{FFFFFF}Slot\t{FFFFFF}ID de l'objet";

		for (new i = 0; i < MAX_TRUNK_INVENTORY; i++)
		{
			if (VehicleInfo[vehicle][carAttach][i] == INVALID_OBJECT_ID)
			{
				format(message, sizeof(message), "%s\n%i\tAucun", message, i);
			}
			else
			{
				format(message, sizeof(message), "%s\n%i\t%d", message, i, VehicleInfo[vehicle][carAttachObj][i]);
			}
		}
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Liste des objets attachés", message, "Fermer", "");
	}
	else
	{
		SendSyntaxMessage(playerid, "/attmenu [commande]");
		SendHelpSyntaxMessage(playerid, "liste, acheter, edit, supprimer");
	}
	return 1;
}

ALTX:clothing("/acc");
CMD:clothing(playerid, params[])
{
    if (GetPVarInt(playerid, #edit_attach) == 1 || GetPVarInt(playerid, #buy_attach) != 0)
        return SendErrorMessage(playerid, "Terminez l'achat ou l'édition des vêtements.");

	new count, string[750];

	for (new i; i < MAX_PLAYER_ATTACH; i++)
	{
	    if (AttachInfo[playerid][i][aID] == 0) continue;
	    
		format(string, sizeof(string), "%s\n{FFFFFF}%i: %s %s", string, i + 1, AttachInfo[playerid][i][aName], (AttachInfo[playerid][i][aUsed]) ? ("{30A90F}[Utilisé]") : ("{AFAFAF}[Non utilisé]")
		);
	    count++;
	}

	if (!count)
		return SendErrorMessage(playerid, "Vous ne possédez aucun vêtement.");

    Dialog_Show(playerid, Clothing, DIALOG_STYLE_LIST, "{FF8000}Sélection du slot", string, "Choisir", "Fermer");
	return 1;
}


/*CMD:coh(playerid, params[]) // Commande pour mettre de l'argent en mains
{
    if (GetPVarInt(playerid, #money_hand) && (
        IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) ||
        IsPlayerAttachedObjectSlotUsed(playerid, 8)
    ))
    {
        SetPVarInt(playerid, #money_hand, 0);
        RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
        RemovePlayerAttachedObject(playerid, 8);
        return 1;
    }

    if (GetCash(playerid) < 300) return SendNotCash(playerid);
    if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
        return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

    new type;
    if (sscanf(params, "d", type))
        return SendHelpMessage(playerid, "/coh [1 - main droite | 2 - main gauche]");

    if (type == 1)
        SetPlayerAttachedObject(playerid, ATTACH_HANDS, 1212, 6, 0.062999, 0.004999, 0.013000, -106.300003, -3.699995, -84.700004, 0.766000, 1.000000, 0.773000);
    else if (type == 2)
        SetPlayerAttachedObject(playerid, 8, 1212, 5, 0.083999, -0.005000, -0.015000, -106.300003, -3.699995, -84.700004, 0.766000, 1.000000, 0.829999);
    else
        return SendHelpMessage(playerid, "/coh [1 - main droite | 2 - main gauche]");

    SetPVarInt(playerid, #money_hand, 1);
    return 1;
}*/