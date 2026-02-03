DisplayCargoGroups(playerid)
{
	new maxpages = floatround(MAX_TRUCK_CARGO / MAX_DIALOG_LIST, floatround_ceil);
	if (pTemp[playerid][pDialogList] < 0 || (maxpages < pTemp[playerid][pDialogList] + 1)) return 1;

	new 
		start_itter = MAX_DIALOG_LIST * pTemp[playerid][pDialogList],
		end_itter = (maxpages == pTemp[playerid][pDialogList] + 1) ? MAX_TRUCK_CARGO : (start_itter + MAX_DIALOG_LIST + 1),
		count,
		str[64 + (64 + 2 + 2 + 2 + 2 + 2) * MAX_DIALOG_LIST + (35 + 26)] = "{FFFFFF}Entreprise\t{FFFFFF}ID\t{FFFFFF}Groupe\n";

	if (pTemp[playerid][pDialogList]) format(str, sizeof(str), "%s{bccd32}Page précédente{FFFFFF}\n", str);

	for (new i = start_itter; i < end_itter; i++)
	{
		if (!TruckCargo[i][tOn]) continue;

		if (count == MAX_DIALOG_LIST) {
			strcat(str, "{bccd32}Page suivante");
		} else {
			format(str, sizeof(str), "%s%s\t%i\t%i\n", str, TruckCargo[i][tName], i, TruckCargo[i][tPack]);
			count++;
		}
	}

	if (!count) return Dialog_Show(playerid, None, DIALOG_STYLE_LIST, "{FFFFFF}Liste des groupes d'entreprises", "Aucune entreprise à afficher...", ">>>", "");

	Dialog_Show(playerid, CargoGroupsDialog, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Liste des groupes d'entreprises", str, ">>>", "");
	return 1;
}

Dialog:CargoGroupsDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (pTemp[playerid][pDialogList]) {		//if not first page
		if (listitem == 0) {
			pTemp[playerid][pDialogList]--;
			return DisplayCargoGroups(playerid);
		} else if (listitem == MAX_DIALOG_LIST+1) {
			pTemp[playerid][pDialogList]++;
			return DisplayCargoGroups(playerid);
		}
	} else { 								//if it's a first page
		if (listitem == MAX_DIALOG_LIST) {
			pTemp[playerid][pDialogList]++;
			return DisplayCargoGroups(playerid);
		}
	}

	return 1;
}

Dialog:CargoList(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Vous devez être en dehors d'un véhicule.");

	new vehicle = GetPVarInt(playerid, #cargo_vehicle);

	if (vehicle == 0) return SendErrorMessage(playerid, "Vous devez être près d'un véhicule.");
	if (!IsNearTrunk(vehicle, playerid, 2.0)) return SendErrorMessage(playerid, "Vous devez être près du coffre du véhicule.");
	if (VehicleInfo[vehicle][carLocked] == 1) return SendErrorMessage(playerid, "Le véhicule est verrouillé.");
	if (VehicleInfo[vehicle][carTrunkLock] == 0) return SendErrorMessage(playerid, "Le coffre de ce véhicule est fermé.");
				
	listitem = pTemp[playerid][pCargoListed][listitem];

	if (VehicleInfo[vehicle][carCargoAmount][listitem] == 0) return SendErrorMessage(playerid, "Ce produit n'est pas présent dans le véhicule.");
	if (!IsTakeProduct(listitem)) return SendErrorMessage(playerid, "Ce produit ne peut pas être pris à la main.");
	if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) || pTemp[playerid][pCargoID] != 0) return SendErrorMessage(playerid, "Vos mains sont déjà occupées.");

	cmd::liftup(playerid, "");
	
	InfoMSG(playerid, "~w~Pour vendre ~y~la caisse~w~, approchez-vous du pickup avec le nom du produit~n~que vous avez choisi et appuyez sur ~w~'~h~~r~~k~~VEHICLE_ENTER_EXIT~~w~'.", 10);

	SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2912, 5, 0.102000, 0.306000, -0.228999, -1.100001, 14.499999, -1.300000, 1.000000, 1.000000, 1.000000);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	pTemp[playerid][pCargoID] = listitem + 1;
	VehicleInfo[vehicle][carCargoAmount][listitem]--;

	new amount;

	for (new i; i != MAX_TRUCK_PRODUCT; i++)
	{
		amount += VehicleInfo[vehicle][carCargoAmount][i];
	}

	switch (GetVehicleModel(vehicle))
	{
		case 600, 543, 605, 422, 478, 554: DestroyDynamicObject(VehicleInfo[vehicle][carCargoObj][amount]);
	}

	Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
	return 1;
}

Dialog:SelectIndustry(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::tpda(playerid, "");

    listitem = GetPVarInt(playerid, #select_industry);

    if (listitem < 0 || listitem > MAX_TRUCK_CARGO || TruckCargo[listitem][tOn] == 0) return 1;

	if (TruckCargo[listitem][tPack] == 0)
	{
	    Waypoint_Set(playerid, TruckCargo[listitem][tPosX], TruckCargo[listitem][tPosY], TruckCargo[listitem][tPosZ]);
    	PlayerPlaySound(playerid, 21001, 0.0, 0.0, 0.0);
	    return 1;
	}
	
	new count, mes[512];

	for(new i; i != MAX_TRUCK_CARGO; i++)
 	{
		pTemp[playerid][pPDAListed][i] = -1;

		if (TruckCargo[i][tOn] == 0 || TruckCargo[i][tPack] == 0 || TruckCargo[i][tPack] != TruckCargo[listitem][tPack])	continue;

		pTemp[playerid][pPDAListed][count] = i;
		format(mes, sizeof(mes), "%s\n{9ACD32}- %s {808080}(%s $%i / unité, %i / %i)", mes, (TruckCargo[i][tType] == 3) ? ("achat") : ("vente"), truckcargo_product[TruckCargo[i][tProductID]], TruckCargo[i][tPrice], TruckCargo[i][tStorage], TruckCargo[i][tStorageSize]);
		count++;
	}

	Dialog_Show(playerid, SelectIndustry3, DIALOG_STYLE_LIST, TruckCargo[listitem][tName], mes, "GPS", "Annuler");
	return 1;
}

Dialog:SelectIndustry2(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::tpda(playerid, "");

	listitem = pTemp[playerid][pPDAListed][listitem];

    if (listitem == -1 || TruckCargo[listitem][tOn] == 0) return 1;

    Show_Industry(playerid, listitem, 1);
	return 1;
}

Dialog:SelectIndustry3(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::tpda(playerid, "");

    listitem = pTemp[playerid][pPDAListed][listitem];

    if (listitem < 0 || listitem > MAX_TRUCK_CARGO || TruckCargo[listitem][tOn] == 0) return 1;

	Waypoint_Set(playerid, TruckCargo[listitem][tPosX], TruckCargo[listitem][tPosY], TruckCargo[listitem][tPosZ]);
   	PlayerPlaySound(playerid, 21001, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:PDABiz(playerid, response, listitem, inputtext[])
{
	if (!response)		
	{
		NullPagesVar(playerid);
		return cmd::tpda(playerid, "");
	}
	if (listitem == 0 && GetPVarInt(playerid, #pda_list))			return ShowBizMenuPDA(playerid, GetPVarInt(playerid, #pda_list)-1, PAGE_PREV);	
    else if (listitem == 10 && !GetPVarInt(playerid, #pda_list))		return ShowBizMenuPDA(playerid, GetPVarInt(playerid, #pda_list)+1, PAGE_NEXT);
	else if (listitem == 11 && GetPVarInt(playerid, #pda_list))		return ShowBizMenuPDA(playerid, GetPVarInt(playerid, #pda_list)+1, PAGE_NEXT);
	
	if (pTemp[playerid][pCargoListed][listitem-1] == -1) return 1;

	new bizid = pTemp[playerid][pCargoListed][listitem-1];

	if (BizInfo[bizid][bizID] == 0) return 1;

	NullPagesVar(playerid);
	Waypoint_Set(playerid, BizInfo[bizid][bizPosX], BizInfo[bizid][bizPosY], BizInfo[bizid][bizPosZ]);
	PlayerPlaySound(playerid, 21001, 0.0, 0.0, 0.0);
	SendSyntaxMessage(playerid, "L'entreprise est marquée sur la carte avec un point rouge. (/removecp)");
	return 1;
}

Dialog:PDAMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem)
	{
		case 0:
		{
			new count, mes[1512];
				
			for (new i; i != MAX_TRUCK_CARGO; i++)
			{
				pTemp[playerid][pPDAListed][i] = -1;

				if (TruckCargo[i][tOn] == 0 || TruckCargo[i][tType] == 2 || TruckCargo[i][tType] == 3 || TruckCargo[i][tGps] == 0) continue;
				
				pTemp[playerid][pPDAListed][count] = i;
				format(mes, sizeof(mes), "%s\n%s (%s, {9ACD32}%s)", mes, TruckCargo[i][tName], truckcargo_type[TruckCargo[i][tType]], (TruckCargo[i][tLocked] == 0) ? ("ouvert") : ("fermé"));
				count++;
			}

			if (count) 	Dialog_Show(playerid, SelectIndustry2, DIALOG_STYLE_LIST, "{FFFFFF}Ordinateur de bord - Entreprises", mes, "Choisir", "Retour");
			else 		Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Ordinateur de bord - Entreprises", "Aucune entreprise trouvée.", "Fermer", "");
		}
		case 1: NullPagesVar(playerid), ShowBizMenuPDA(playerid, 0, PAGE_NEXT);
		case 2:
		{
			new mes[1024], str_len;

			format(mes, sizeof(mes), "{FFFFFF}Bienvenue au {9ACD32}Port de Los Santos!\n\n\
									{FFFFFF}Le navire est actuellement {9ACD32}%s{FFFFFF}.\n\n\
									{9ACD32}Informations sur le navire:{FFFFFF}\n\
									Le navire est arrivé à :\t\t\t\t%s\n\
									Le navire partira à :\t\t\t\t%s\n\
									Le prochain arrivera à :\t\t\t%s\n\
									{808080}* Les horaires ci-dessus peuvent être approximatifs.\n\n",
			(ship_docked == 1) ? ("au quai") : ("en route"),
			get_date("%hh:%ii", ship_arrived), 
			get_date("%hh:%ii", ship_depart), 
			get_date("%hh:%ii", ship_next));

			format(mes, sizeof(mes), "%s{9ACD32}Vente :\n\
									{808080}Le navire n'apporte pas de marchandises à vendre. Il\nachète uniquement des produits locaux pour exportation.\n\n{9ACD32}Demandé :\n\
									{808080}Produit\t\t\tPrix\t\tEn stock (Requis){FFFFFF}", mes);
			

			for (new i; i != MAX_TRUCK_CARGO; i++)
			{
				if (TruckCargo[i][tOn] == 0 || TruckCargo[i][tType] != 2) continue;

				str_len = strlen(truckcargo_product[TruckCargo[i][tProductID]]);
				format(mes, sizeof(mes), "%s\n%s%s\t\t$%i\t\t%i unité(s) {808080}(%i){FFFFFF}", 
					mes, 
					truckcargo_product[TruckCargo[i][tProductID]], 
					(str_len < 6 || TruckCargo[i][tProductID] == 15 || TruckCargo[i][tProductID] == 17 || TruckCargo[i][tProductID] == 10) ? ("\t") : (""),
					TruckCargo[i][tPrice], 
					TruckCargo[i][tStorage], 
					TruckCargo[i][tStorageSize]);
			}
			Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Ordinateur de bord - Port de Los Santos", mes, "X", "");
		}
	}
	return 1;
}
