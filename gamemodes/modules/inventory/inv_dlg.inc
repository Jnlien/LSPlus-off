Dialog:Inventory(playerid, response, listitem, inputtext[])
{
	if (!response)										return 1;
	if (GetPVarInt(playerid, "pDelay"))					return cmd::inventory(playerid, "");

	new title[56], mes[256];

	if (listitem == MAX_INV_SLOTS) // SECTION : RANGER UNE ARME
	{
		if (GetPVarInt(playerid, #attach_item)) 	return cmd::put(playerid, "");
	    else 										return cmd::putgun(playerid, "");
	}
	else if (listitem == MAX_INV_SLOTS + 1) // SECTION : JETER UNE ARME
	{
		if (GetPVarInt(playerid, #attach_item))	return cmd::drop(playerid, "");
	    else 									return cmd::dropgun(playerid, "");
	}
	else if (listitem == MAX_INV_SLOTS + 2) // SECTION : RANGER MUNITIONS
	{
		if (PlayerInfo[playerid][pPlayerAmmo] < 1) return cmd::silencer(playerid, "");
		
	    if (!CheckInv(playerid))					return SendErrorMessage(playerid, "Votre inventaire est plein.");
		if (!PlayerInfo[playerid][pPlayerAmmo])		return SendErrorMessage(playerid, "L'arme n'a pas de chargeur.");

		new wep = CheckPlayerWeaponEx(playerid);
		if (!wep) return SendAdmMessage("Le joueur %s[ID:%i] est suspecté d'utiliser un cheat d'arme !", GetNameEx(playerid), playerid);

		if (wep >= 22 && wep <= 34) {
			new am = CheckAmmo(PlayerInfo[playerid][pAmmoType]);
			if (PlayerInfo[playerid][pPlayerAmmo] > am) PlayerInfo[playerid][pPlayerAmmo]=am;
		} 

		if (GiveInvItem(playerid, PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pPlayerAmmo]) == -1)
			return SendErrorMessage(playerid, "Votre inventaire est plein.");
			
		SendWarningMessage(playerid, "Vous avez déchargé %s - %s, %i munitions.", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);

		new msg[64];
		format(msg, sizeof(msg), "* %s retire le chargeur de l'arme.", GetNameWithMask(playerid));
		SetPlayerChatBubble(playerid, msg, COLOR_PURPLE, 20.0, 4000);

		PlayerInfo[playerid][pPlayerAmmo]=0;
		PlayerInfo[playerid][pAmmoType]=0;
		ResetPlayerWeapons(playerid);	
		
		return true;
	}
	else if (listitem == MAX_INV_SLOTS + 3) // SILENCIEUX
	{
		return cmd::silencer(playerid, "");
	}

	if (Inventory[playerid][listitem][pInvItem] == 0)	return cmd::inventory(playerid, "");

	if (inventory_items[Inventory[playerid][listitem][pInvItem]][itemSplit])
		if ((inventory_items[Inventory[playerid][listitem][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][listitem][pInvItem]][itemType] == ITEM_TYPE_SEEDS) && (IsPlayerHasItem(playerid, 78) || IsPlayerHasItem(playerid, 79)))
			mes = "Utiliser l'objet\nDonner l'objet\nJeter l'objet\nDétruire l'objet\nDiviser\nFusionner\nMettre dans un sac/boîte";
		else
			mes = "Utiliser l'objet\nDonner l'objet\nJeter l'objet\nDétruire l'objet\nDiviser\nFusionner";
	else
	{
		if (inventory_items[Inventory[playerid][listitem][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
		{
			if (Inventory[playerid][listitem][pInvEx]) 
				format(mes, sizeof(mes), "Retirer %s (%f)\nDonner l'objet\nJeter l'objet\nDétruire l'objet", inventory_items[Inventory[playerid][listitem][pInvEx]][itemName], Inventory[playerid][listitem][pInvQFloat]);
			else mes = "Déposer un objet\nDonner l'objet\nJeter l'objet\nDétruire l'objet";

			if (!GetPVarInt(playerid, #attach_item)) format(mes, sizeof(mes), "%s\nPrendre en main", mes);
		}
		else
			mes = "Utiliser l'objet\nDonner l'objet\nJeter l'objet\nDétruire l'objet";
	}

    new houseid = -1;
	if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
	{
	    SetPVarInt(playerid, #inventory_house, houseid+1);
 		format(mes, sizeof(mes), "%s\n{ffff00}Déposer dans la maison (ID: %i)", mes, houseid);
	}

	new vehicle = GetNearVehicle(playerid, 4.0);
	if (vehicle != 0 && (!IsADontTrunk(vehicle) || !IsAPlane(vehicle) || !IsABiker(vehicle) || !IsAHelicopter(vehicle) || !IsABoat(vehicle) || !IsABicycle(vehicle)))
	{
		if ((VehicleInfo[vehicle][carTrunkLock] && !IsPlayerInAnyVehicle(playerid)) || (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction]))
		{
			SetPVarInt(playerid, #inventory_vehicle, vehicle);
			format(mes, sizeof(mes), "%s\n{ffff00}Déposer dans %s (ID: %i)", mes, VehicleNames[GetVehicleModel(vehicle)-400], vehicle);
		}
	}

	if (GetPVarInt(playerid, #inventory_vehicle) == 0 && inventory_items[Inventory[playerid][listitem][pInvItem]][itemSize] == ITEM_SIZE_SMALL)
	{
		vehicle = GetPlayerVehicleID(playerid);
		if (IsPlayerInAnyVehicle(playerid) && vehicle != 0 && VehicleInfo[vehicle][carOwnerID] == PlayerInfo[playerid][pID])
		{
			format(mes, sizeof(mes), "%s\n{ffff00}Ranger dans la boîte à gants %s (ID: %i)", mes, VehicleNames[GetVehicleModel(vehicle)-400], vehicle);	
		}
	}

	SetPVarInt(playerid, #inventory_choose, listitem);
	format(title, sizeof(title), "{FFFFFF}Inventaire > %s", inventory_items[Inventory[playerid][listitem][pInvItem]][itemName]);
    Dialog_Show(playerid, InventoryChoose, DIALOG_STYLE_LIST, title, mes, "Sélectionner", "Retour");

	return 1;
}

Dialog:InventoryChoose(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::inventory(playerid, "");

	new slot = GetPVarInt(playerid, #inventory_choose);

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || Inventory[playerid][slot][pInvItem] == 0)	return cmd::inventory(playerid, "");
	
	if (listitem > 3 || (listitem == 0 && inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT))
	{
		new listadd;
		if (inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit])
			if ((inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_SEEDS) && (IsPlayerHasItem(playerid, 78) || IsPlayerHasItem(playerid, 79)))
				listadd = 3;
			else
				listadd = 2;
		else 
		{
			if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
			{
				if (listitem == 4)
				{
					if (!GetPVarInt(playerid, #attach_item))	listadd = 6; //Âçÿòü â ðóêè								
				}
				else
				{
					if (Inventory[playerid][slot][pInvEx]) 	listadd = 4; // Äîñòàòü
					else 									listadd = 5; // Ïîëîæèòü
				}											
			}
			else listadd = 0;
		}

		switch(listitem)
		{
			case 0:
			{
				switch(listadd)
				{
					case 4: listitem = 8; // Äîñòàòü
					case 5: listitem = 9; // Ïîëîæèòü
				}
			}
			case 4:
			{
				switch(listadd)
				{
					case 0: listitem = 4; // ÁÀÃÀÆÍÈÊ
					case 2: listitem = 5; // ÐÀÇÄÅËÈÒÜ
					case 3: listitem = 5; // ÐÀÇÄÅËÈÒÜ
					case 6: listitem = 10; // ÂÇßÒÜ Â ÐÓÊÈ
				}
			}
			case 5:
			{
				switch(listadd)
				{
					case 2: listitem = 6; // ÎÁÚÅÄÈÍÈÒÜ
					case 3: listitem = 6; // ÎÁÚÅÄÈÍÈÒÜ
				}
			}
			case 6:
			{
				switch(listadd)
				{
					case 2: listitem = 4; //ÁÀÃÀÆÍÈÊ
					case 3: listitem = 7; // ÏÎËÎÆÈÒÜ Â
				}
			}
			case 7:
			{
				listitem = 4;// ÁÀÃÀÆÍÈÊ
			}
		}
	}

	switch(listitem)
	{
	    case 0: CallLocalFunction("OnPlayerUseItem", "ii", playerid, slot);
		case 1: Dialog_Show(playerid, GiveItem, DIALOG_STYLE_INPUT, "{FFFFFF}Donner un objet", "{ffffff}Entrez le nom ou l'ID du joueur à qui vous souhaitez donner l'objet :", "Suivant", "Retour");
		case 2:
		{
			if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_AMMO || inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
			{
				if (CheckAdmin(playerid, ADMIN_LEVEL_1) || IsTester(playerid)) return SendErrorMessage(playerid, "Quittez d'abord le mode admin.");
				if (PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Cette fonction est indisponible en service.");
			}

			if (Inventory[playerid][slot][pInvQ])	
			{
				if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
				{
					if (ItemDrop_Drop(playerid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)
						return SendErrorMessage(playerid, "Le nombre maximum d'objets au sol a été atteint.");

					switch(inventory_items[Inventory[playerid][slot][pInvItem]][itemType]) {
						case 22..34: SendSyntaxMessage(playerid, "Vous avez jeté %s [%s / %i mun.]. Tapez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], inventory_items[Inventory[playerid][slot][pInvEx]][itemName], inventory_items[Inventory[playerid][slot][pInvQ]][itemName]);
						default: SendSyntaxMessage(playerid, "Vous avez jeté %s. Tapez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
					}
				}
				else if (inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][slot][pInvItem]][itemType] == ITEM_TYPE_REACT)
				{
					// Bloc pour DROGUES ou RÉACTIFS à compléter si nécessaire
				}
				else
				{
					if (ItemDrop_Drop(playerid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)
						return SendErrorMessage(playerid, "Le nombre maximum d'objets au sol a été atteint.");

					SendSyntaxMessage(playerid, "Vous avez jeté %s [%i unités]. Tapez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
				}
			}
			else
			{
				if (ItemDrop_Drop(playerid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)
					return SendErrorMessage(playerid, "Le nombre maximum d'objets au sol a été atteint.");

				SendSyntaxMessage(playerid, "Vous avez jeté %s. Tapez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);			
			}
			RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
		}
		case 3:
		{
			new mes[128];
			format(mes, sizeof(mes), "{ffffff}Voulez-vous vraiment détruire l'objet - %s ?", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
			Dialog_Show(playerid, DeleteItem, DIALOG_STYLE_MSGBOX, "{FFFFFF}Détruire l'objet", mes, "Oui", "Annuler");
		}
		case 4:
		{
		    new mes[128];

		    if (GetPVarInt(playerid, #inventory_house) != 0)
			{
				if (Inventory[playerid][slot][pInvQ] > 1 && inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit]) 
				{
					format(mes, sizeof(mes), "{ffffff}Objet : %s (Quantité : %i)\n\nEntrez la quantité que vous souhaitez placer dans la cache de la maison :", 
					inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
				}
				else
				{
					new 
						houseid = -1,
						i = GetNextHouseTrunkSlot(houseid),
						item = Inventory[playerid][slot][pInvItem]
					;

					if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
					{
						if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous ne pouvez pas placer une arme dans la cache pendant votre service.");

						if (i == -1)	return SendErrorMessage(playerid, "Il n'y a pas de place dans la maison.");
						if (item == 0) 	return cmd::inventory(playerid, "");
						
						SM2(playerid, "Vous avez placé %s dans la cachette de la maison.", inventory_items[item][itemName]);

						HouseInventory[houseid][i][pInvItem] 	= Inventory[playerid][slot][pInvItem];
						HouseInventory[houseid][i][pInvQ]		= Inventory[playerid][slot][pInvQ];
						HouseInventory[houseid][i][pInvEx]		= Inventory[playerid][slot][pInvEx];
						HouseInventory[houseid][i][pInvS]		= Inventory[playerid][slot][pInvS];
						HouseInventory[houseid][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
						format(HouseInventory[houseid][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
						
						RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
						
						format(mes, sizeof(mes), "place %s dans la cachette de la maison.", inventory_items[item][itemName]);
						SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

						Save_HouseInventory(houseid);
						return cmd::inventory(playerid, "");
					}
				}
			}
			else if (GetPVarInt(playerid, #inventory_vehicle) != 0)
			{
				if (Inventory[playerid][slot][pInvQ] > 1 && inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit]) 
				{
   			    	format(mes, sizeof(mes), "{ffffff}Objet : %s (Quantité : %i)\n\nEntrez la quantité que vous souhaitez placer dans le coffre :", 
					inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
				}
				else 
				{
					new vehicle = GetPVarInt(playerid, #inventory_vehicle);

					new item = Inventory[playerid][slot][pInvItem];
					if (item == 0) return cmd::inventory(playerid, "");

					if (vehicle > 0 && GetNearVehicle(playerid, 5.0, vehicle) != 0)
					{
						if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))		return SendErrorMessage(playerid, "Ó ýòîãî òðàíñïîðòà íåò áàãàæíèêà.");
						if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])										return SendErrorMessage(playerid, "Âàì íåäîñòóïåí áàãàæíèê ñëóæåáíîãî òðàíñïîðòà.");
						if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0) 									return SendErrorMessage(playerid, "Íåëüçÿ êëàñòü îðóæèå â ëè÷íûé òðàíñïîðò íà äåæóðñòâå.");
						if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) 	return SendErrorMessage(playerid, "Âû äîëæíû ñòîÿòü ó áàãàæíèêà.");
						if (!IsPlayerInAnyVehicle(playerid) && !VehicleInfo[vehicle][carTrunkLock]) 																		return SendErrorMessage(playerid, "Áàãàæíèê ýòîãî òðàíñïîðòíîãî ñðåäñòâà çàêðûò.");

						new i = GetNextVehicleTrunkSlot(vehicle);
						if (i == -1)	return SendErrorMessage(playerid, "Il n'y a pas de place dans le véhicule.");

						TrunkInventory[vehicle][i][pInvItem] 	= Inventory[playerid][slot][pInvItem];
						TrunkInventory[vehicle][i][pInvQ] 		= Inventory[playerid][slot][pInvQ];
						TrunkInventory[vehicle][i][pInvEx]		= Inventory[playerid][slot][pInvEx];
						TrunkInventory[vehicle][i][pInvS] 		= Inventory[playerid][slot][pInvS];
						TrunkInventory[vehicle][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
						format(TrunkInventory[vehicle][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
						
						RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
						
						SM2(playerid, "Vous avez placé %s dans %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);

						format(mes, sizeof(mes), "place %s dans le %s du %s.", inventory_items[item][itemName], (VehicleInfo[vehicle][carFaction]) ? ("coffre-fort") : ("coffre"), VehicleNames[GetVehicleModel(vehicle)-400]);
						SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

						Save_CarInventory(vehicle);
						return cmd::inventory(playerid, "");
					}
				}
			}
			else if (GetPVarInt(playerid, #inventory_vehicle) == 0 && IsPlayerInAnyVehicle(playerid))
			{
				new vehicle = GetPlayerVehicleID(playerid);
				if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID] && vehicle == 0) return cmd::inventory(playerid, "");

				if (Inventory[playerid][slot][pInvQ] > 1 && inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit]) 
				{
   			    	format(mes, sizeof(mes), "{ffffff}Objet : %s (Quantité : %i)\n\nIndiquez la quantité que vous souhaitez placer dans le coffre :", 
					inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
				}
				else 
				{
					new 
						i = GetNextVehicleGBSlot(vehicle),
						item = Inventory[playerid][slot][pInvItem]
					;

					if (vehicle == 0) 	return cmd::inventory(playerid, "");
					if (item == 0) 		return cmd::inventory(playerid, "");
					if (i == -1) return SendErrorMessage(playerid, "Il n'y a pas de place dans le véhicule.");

					if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])
						return SendErrorMessage(playerid, "Vous n'avez pas accès à la boîte à gants de ce véhicule.");

					if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0)
						return SendErrorMessage(playerid, "Impossible de stocker une arme dans un véhicule personnel pendant le service.");

					
					CarInventory[vehicle][i][pInvItem]		= Inventory[playerid][slot][pInvItem];
					CarInventory[vehicle][i][pInvQ]			= Inventory[playerid][slot][pInvQ];
					CarInventory[vehicle][i][pInvEx] 		= Inventory[playerid][slot][pInvEx];
					CarInventory[vehicle][i][pInvS]			= Inventory[playerid][slot][pInvS];
					CarInventory[vehicle][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
					format(CarInventory[vehicle][i][pInvName],16, "%s", Inventory[playerid][slot][pInvName]);
						
					RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
						
					SM2(playerid, "Vous avez placé %s dans la boîte à gants du véhicule %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);

					format(mes, sizeof(mes), "place %s dans la boîte à gants du %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);
					SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

					Save_CarInventory(vehicle);
					return cmd::inventory(playerid, "");
				}
			}
			else return SendErrorMessage(playerid, "Quelque chose s'est mal passé !");
			
			Dialog_Show(playerid, PutItemInProperty, DIALOG_STYLE_INPUT, "{FFFFFF}Déposer l'objet", mes, "Déposer", "Annuler");
			return 1;
		}
		case 5: // Diviser
		{
			if (!inventory_items[Inventory[playerid][slot][pInvItem]][itemFloat]) {
				if (Inventory[playerid][slot][pInvQ] < 2)
					return SendErrorMessage(playerid, "Pour diviser, la quantité de l'objet doit être supérieure à 1.");
				
				new divide_string[128];
				format(divide_string, sizeof(divide_string), "{FFFFFF}Combien souhaitez-vous séparer de %s {ffc46a}(%i){FFFFFF} ?", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);
				Dialog_Show(playerid, DivideInvInt, DIALOG_STYLE_INPUT, "Division de l'objet", divide_string, "Continuer", "Retour");
			} else {
				if (Inventory[playerid][slot][pInvQFloat] < 0.2)
					return SendErrorMessage(playerid, "Pour diviser, la quantité de l'objet doit être supérieure à 0.2g.");
				
				new divide_string[128];
				format(divide_string, sizeof(divide_string), "{FFFFFF}Combien de grammes souhaitez-vous séparer de %s {ffc46a}(%.1fg){FFFFFF} ?", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQFloat]);
				Dialog_Show(playerid, DivideInvFloat, DIALOG_STYLE_INPUT, "Division de l'objet", divide_string, "Continuer", "Retour");
			}

			return 1;
		}
		case 6: // Fusionner
		{
			FixInv(playerid);

			SendHelpMessage(playerid, "Sélectionnez l'objet avec lequel vous souhaitez fusionner %s (%i).", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ]);

			new dlgstr[20 * 128] = "#\tObjet\tQuantité";

			new ccount;
			for(new i = 0; i < MAX_INV_SLOTS; i++) 
			{
				switch(Inventory[playerid][i][pInvItem])
				{
					case 0:
						format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i\t{C3C3C3}Emplacement vide", dlgstr, i+1);
					case 22 .. 34:
					{
						if (Inventory[playerid][i][pInvQ] == 0)
							format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s {C3C3C3}[vide]\t1", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]); 
						else
							format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s {C3C3C3}[%s / %i mun]{FFFFFF}\t1", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], inventory_items[Inventory[playerid][i][pInvEx]][itemName], Inventory[playerid][i][pInvQ]); 
					}
					case INV_GANJA, INV_MED_GANJA, 103 .. 118:
					{
						format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s{FFFFFF}\t%.1fg", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQFloat]);
					}
					case INV_DRUG:
					{
						format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s {C3C3C3}[Pureté: %i%%]{FFFFFF}\t%.1fg", dlgstr, i+1, Inventory[playerid][i][pInvName], Inventory[playerid][i][pInvEx], Inventory[playerid][i][pInvQFloat]);
					}
					default:
						format(dlgstr, sizeof(dlgstr), "%s\n{ffffff}%i.\t%s\t%i", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName], Inventory[playerid][i][pInvQ]);    
				}
				ccount++;
			}

			if (!ccount)
				return SendErrorMessage(playerid, "Aucun objet compatible trouvé !");

			Dialog_Show(playerid, CombInv, DIALOG_STYLE_TABLIST_HEADERS, "Inventaire > Fusion", dlgstr, "Sélectionner", "Annuler");
			return 1;
		}
		case 7: // Placer dans
		{
			new dlgstr[32*MAX_INV_SLOTS];

			format(dlgstr, sizeof(dlgstr), "Sélectionnez où vous souhaitez placer '%s':{FFFFFF}\n", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

			new ccount;
			for(new i = 0; i < MAX_INV_SLOTS; i++) 
			{
				if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
				{
					if (!Inventory[playerid][i][pInvEx])
					{
						ccount++;
						format(dlgstr, sizeof(dlgstr), "%s%i. %s\n", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
					}
				}
			}
			if (!ccount) return SendErrorMessage(playerid, "Aucun objet approprié trouvé !");

			Dialog_Show(playerid, InventoryPut, DIALOG_STYLE_TABLIST_HEADERS, "Inventaire", dlgstr, "Sélectionner", "Annuler");
			return 1;
		}
		case 8: // sortir
		{
			if (!Inventory[playerid][slot][pInvEx]) return 1;

			if (!CheckInv(playerid))
			{
				cmd::inventory(playerid, "");
				return SendErrorMessage(playerid, "Votre inventaire est plein.");
			}

			if (GiveInvItem(playerid, Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvQ]) == -1)
				return SendErrorMessage(playerid, "Votre inventaire est plein.");

			SendSyntaxMessage(playerid, "Vous avez sorti %s (%i) de %s.", inventory_items[Inventory[playerid][slot][pInvEx]][itemName], Inventory[playerid][slot][pInvQ], inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

			Inventory[playerid][slot][pInvEx] = 0;
			Inventory[playerid][slot][pInvQ] = 1;

			return 1;
		}
		case 9: // déposer
		{
			new dlgstr[32*MAX_INV_SLOTS];

			format(dlgstr, sizeof(dlgstr), "Choisissez l'objet que vous souhaitez déposer dans '%s':{FFFFFF}\n", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

			new ccount;
			for(new i = 0; i < MAX_INV_SLOTS; i++) 
			{
				if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_SEEDS)
				{
					ccount++;
					if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_DRUGS){
						format(dlgstr, sizeof(dlgstr), "%s%i. %s\n", dlgstr, i+1, Inventory[playerid][i][pInvName]);
					} else {
						format(dlgstr, sizeof(dlgstr), "%s%i. %s\n", dlgstr, i+1, inventory_items[Inventory[playerid][i][pInvItem]][itemName]);
					}
				}
			}

			if (!ccount) return SendErrorMessage(playerid, "Aucun objet compatible n'a été trouvé !");

			Dialog_Show(playerid, InventoryPut2, DIALOG_STYLE_TABLIST_HEADERS, "Inventaire", dlgstr, "Choisir", "Annuler");
			return 1;
		}
		case 10:
		{
			if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
				return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

			if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");
			if (GetPlayerVehicleID(playerid) != 0)	return SendErrorMessage(playerid, "Vous devez être en dehors d'un véhicule.");
			
			if (Inventory[playerid][slot][pInvItem] == 78)
			{
				SetPlayerAttachedObject(playerid, ATTACH_HANDS, 19571, 6, -0.157999, -0.002000, -0.169000, -21.999990, 87.000022, 0.000000, 1.186000, 1.000000, 1.170000);
				SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
			}
			else if (Inventory[playerid][slot][pInvItem] == 79)
			{
				SetPlayerAttachedObject(playerid, ATTACH_HANDS, inventory_items[Inventory[playerid][slot][pInvItem]][itemModel], 6, 0.204, 0.012, 0.026, 0.0, -101.3, -10.5, 0.838, 0.5, 0.597);
			}

			SetPVarInt(playerid, #attach_item, 1);

			pTemp[playerid][tempItem] = Inventory[playerid][slot][pInvItem];
			pTemp[playerid][tempEx] = Inventory[playerid][slot][pInvEx];
			pTemp[playerid][tempQ] = Inventory[playerid][slot][pInvQ];

			InfoMSG(playerid, "Vous pouvez ranger ou jeter cet objet depuis vos mains\n~y~Dans l'inventaire~w~, ou avec la commande ~y~/PUT~w~ ou ~y~/DROP~w~", 5);
			SendSyntaxMessage(playerid, "Vous avez pris %s en main.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

			RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			return 1;
		}
	}
	return 1;
}

Dialog:InventoryPut2(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new count,
		slot = GetPVarInt(playerid, #inventory_choose);

	for(new i = 0; i < MAX_INV_SLOTS; i++) 
	{
		if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_DRUGS || inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_SEEDS)
		{
			if (listitem == count)	
			{
				count = i;
				break;
			}
			else count++;
		}
	}

	Inventory[playerid][slot][pInvEx] = Inventory[playerid][count][pInvItem];
	Inventory[playerid][slot][pInvQ] = Inventory[playerid][count][pInvQ];

	SendSyntaxMessage(playerid, "Vous avez rangé avec succès %s (%i) dans %s.", inventory_items[Inventory[playerid][slot][pInvEx]][itemName], Inventory[playerid][slot][pInvQ], inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);

	RemoveInvItem(playerid, Inventory[playerid][count][pInvItem], 0, count);
	return 1;
}


Dialog:InventoryPut(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new count,
		slot = GetPVarInt(playerid, #inventory_choose);

	for(new i = 0; i < MAX_INV_SLOTS; i++) 
	{
		if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] == ITEM_TYPE_INSIDEABLE_OBJECT)
		{
			if (!Inventory[playerid][i][pInvEx])
			{
				if (listitem == count)	
				{
					count = i;
					break;
				}
				else count++;
			}
		}
	}

	Inventory[playerid][count][pInvEx] = Inventory[playerid][slot][pInvItem];
	Inventory[playerid][count][pInvQ] = Inventory[playerid][slot][pInvQ];

	SendSyntaxMessage(playerid, "Vous avez rangé avec succès %s (%i) dans %s.", inventory_items[Inventory[playerid][count][pInvEx]][itemName], Inventory[playerid][count][pInvQ], inventory_items[Inventory[playerid][count][pInvItem]][itemName]);

	RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
	return 1;
}

Dialog:CombInv(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot1 = GetPVarInt(playerid, #inventory_choose),
		slot2 = listitem;

	if (Inventory[playerid][slot1][pInvItem] != Inventory[playerid][slot2][pInvItem])
	{
		SendErrorMessage(playerid, "Vous ne pouvez combiner que des objets identiques.");
		return cmd::inventory(playerid, "");
	}

	if (slot1 == slot2)
	{
		SendErrorMessage(playerid, "Vous ne pouvez pas combiner un objet avec lui-même.");
		return cmd::inventory(playerid, "");
	}
	
	switch(Inventory[playerid][slot1][pInvItem])
	{
		case INV_GANJA, INV_MED_GANJA, 103 .. 118:
		{
			if (Inventory[playerid][slot1][pInvQFloat] + Inventory[playerid][slot2][pInvQFloat] > 999.0) {
				SendErrorMessage(playerid, "Le poids total des objets combinés ne doit pas dépasser 999g.");
				return cmd::inventory(playerid, "");
			}

			Inventory[playerid][slot2][pInvQFloat] += Inventory[playerid][slot1][pInvQFloat];
			SendSyntaxMessage(playerid, "Vous avez combiné avec succès %s (%.1fg).", inventory_items[Inventory[playerid][slot2][pInvItem]][itemName], Inventory[playerid][slot2][pInvQFloat]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot1][pInvItem], 0.0, slot1);
		}
		case INV_DRUG:
		{
			if (!CompareStrings(Inventory[playerid][slot2][pInvName], Inventory[playerid][slot2][pInvName])) {
				SendErrorMessage(playerid, "Vous ne pouvez pas combiner deux drogues différentes. (Noms différents !)");
				return cmd::inventory(playerid, "");
			}
			if (Inventory[playerid][slot1][pInvEx] != Inventory[playerid][slot2][pInvEx]) {
				SendErrorMessage(playerid, "Vous ne pouvez pas combiner deux drogues différentes. (Pureté différente !)");
				return cmd::inventory(playerid, "");
			}
			if (Inventory[playerid][slot1][pInvQFloat] + Inventory[playerid][slot2][pInvQFloat] > 999.0) {
				SendErrorMessage(playerid, "Le poids total des objets combinés ne doit pas dépasser 999g.");
				return cmd::inventory(playerid, "");
			}

			Inventory[playerid][slot2][pInvQFloat] += Inventory[playerid][slot1][pInvQFloat];
			SendSyntaxMessage(playerid, "Vous avez combiné avec succès %s {C3C3C3}[OAS: %i%%]{FFFFFF} (%.1fg).", Inventory[playerid][slot2][pInvName], Inventory[playerid][slot2][pInvEx], Inventory[playerid][slot2][pInvQFloat]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot1][pInvItem], 0.0, slot1);
		}
		default: {
			if ((Inventory[playerid][slot1][pInvQ] + Inventory[playerid][slot2][pInvQ] > 100) && (inventory_items[Inventory[playerid][slot1][pInvQ]][itemType] == ITEM_TYPE_SEEDS || inventory_items[Inventory[playerid][slot1][pInvQ]][itemType] == ITEM_TYPE_DRUGS))
			{
				SendErrorMessage(playerid, "La quantité totale des objets combinés ne doit pas dépasser 100.");
				return cmd::inventory(playerid, "");
			} 
			else if (Inventory[playerid][slot1][pInvQ] + Inventory[playerid][slot2][pInvQ] > 10)
			{
				SendErrorMessage(playerid, "La quantité totale des objets combinés ne doit pas dépasser 10.");
				return cmd::inventory(playerid, "");
			}

			Inventory[playerid][slot2][pInvQ] += Inventory[playerid][slot1][pInvQ];
			SendSyntaxMessage(playerid, "Vous avez combiné avec succès %s (%i).", inventory_items[Inventory[playerid][slot2][pInvItem]][itemName], Inventory[playerid][slot2][pInvQ]);

			RemoveInvItem(playerid, Inventory[playerid][slot1][pInvItem], 0, slot1);
		}
	}

	return 1;
}

Dialog:DivideInvInt(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot1 = GetPVarInt(playerid, #inventory_choose);
	
	if (CheckInv(playerid) == 0) return SendErrorMessage(playerid, "Votre inventaire est plein.");
	if (strval(inputtext) <= 0 || strval(inputtext) >= Inventory[playerid][slot1][pInvQ]) 
		return SendErrorMessage(playerid, "Valeur incorrecte.");

	SendSyntaxMessage(playerid, "Vous avez divisé avec succès {ffc46a}%s{ffffff} en %i et %i.", inventory_items[Inventory[playerid][slot1][pInvItem]][itemName], strval(inputtext), Inventory[playerid][slot1][pInvQ] - strval(inputtext) );
	
	Inventory[playerid][slot1][pInvQ] -= strval(inputtext);

	new str[1] = EOS;
	GiveInvItem(playerid, Inventory[playerid][slot1][pInvItem], strval(inputtext), 0, 0, str, 1);
	return 1;
}


Dialog:DivideInvFloat(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot1 = GetPVarInt(playerid, #inventory_choose);

	if (CheckInv(playerid) == 0) return SendErrorMessage(playerid, "Votre inventaire est plein.");

	if (floatstr(inputtext) < 0.1 || floatstr(inputtext) >= Inventory[playerid][slot1][pInvQFloat])
		return SendErrorMessage(playerid, "Valeur invalide.");

	SendSyntaxMessage(playerid, "Vous avez divisé avec succès {ffc46a}%s{ffffff} en %.1fg et %.1fg.", inventory_items[Inventory[playerid][slot1][pInvItem]][itemName], floatstr(inputtext), Inventory[playerid][slot1][pInvQFloat] - floatstr(inputtext));

	Inventory[playerid][slot1][pInvQFloat] -= floatstr(inputtext);

	new str[1] = EOS;
	GiveFloatInvItem(playerid, Inventory[playerid][slot1][pInvItem], floatstr(inputtext), 0, 0, str, 1);

	return 1;
}

Dialog:PutItemInProperty(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::inventory(playerid, "");

    new 
		slot = GetPVarInt(playerid, #inventory_choose), 
		houseid = GetPVarInt(playerid, #inventory_house)-1, 
		vehicle = GetPVarInt(playerid, #inventory_vehicle),
		item = Inventory[playerid][slot][pInvItem]
	;

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || item == 0)	return cmd::inventory(playerid, "");
    if (strval(inputtext) < 1 || Inventory[playerid][slot][pInvQ] < strval(inputtext))	return SendErrorMessage(playerid, "Vous n'avez pas cette quantité.");

	if (GetPVarInt(playerid, #inventory_vehicle) != 0)
	{
		if (vehicle > 0 && GetNearVehicle(playerid, 5.0, vehicle) != 0)
		{
			if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))		
				return SendErrorMessage(playerid, "Ce véhicule n'a pas de coffre.");
			
			if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])		
				return SendErrorMessage(playerid, "Le coffre de ce véhicule ne vous est pas accessible.");
			
			if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0)		
				return SendErrorMessage(playerid, "Vous ne pouvez pas déposer d'arme dans un véhicule personnel en service.");
			
			if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) 	
				return SendErrorMessage(playerid, "Vous devez être à côté du coffre.");
			
			if (!IsPlayerInAnyVehicle(playerid) && !VehicleInfo[vehicle][carTrunkLock]) 																	
				return SendErrorMessage(playerid, "Le coffre de ce véhicule est fermé.");
			
			new i = GetNextVehicleTrunkSlot(vehicle);
			if (i == -1) return SendErrorMessage(playerid, "Il n'y a pas de place dans le véhicule.");

			TrunkInventory[vehicle][i][pInvItem] 	= Inventory[playerid][slot][pInvItem];
			TrunkInventory[vehicle][i][pInvQ] 		= Inventory[playerid][slot][pInvQ];
			TrunkInventory[vehicle][i][pInvEx] 		= Inventory[playerid][slot][pInvEx];
			TrunkInventory[vehicle][i][pInvS] 		= Inventory[playerid][slot][pInvS];
			TrunkInventory[vehicle][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
			format(TrunkInventory[vehicle][i][pInvName], 16, "%s", Inventory[playerid][slot][pInvName]);
						
			Inventory[playerid][slot][pInvQ] -= strval(inputtext);
						
			SM2(playerid, "Vous avez déposé %s dans %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);

			new mes[128];
			format(mes, sizeof(mes), "dépose %s (%i) dans le %s %s.", inventory_items[item][itemName], strval(inputtext), 
				(VehicleInfo[vehicle][carFaction]) ? ("compartiment") : ("coffre"), VehicleNames[GetVehicleModel(vehicle)-400]);
			SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

			Save_CarInventory(vehicle);
			return cmd::inventory(playerid, "");
		}
	}
    else if (GetPVarInt(playerid, #inventory_vehicle) == 0 && IsPlayerInAnyVehicle(playerid))
	{
		vehicle = GetPlayerVehicleID(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID] && vehicle == 0)
			return cmd::inventory(playerid, "");

		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0)
			return SendErrorMessage(playerid, "Vous ne pouvez pas déposer d'arme dans un véhicule personnel en service.");

		new i = GetNextVehicleGBSlot(vehicle);
		if (i == -1)
			return SendErrorMessage(playerid, "Il n'y a pas de place dans le véhicule.");

		CarInventory[vehicle][i][pInvItem]		= Inventory[playerid][slot][pInvItem];
		CarInventory[vehicle][i][pInvQ]			= Inventory[playerid][slot][pInvQ];
		CarInventory[vehicle][i][pInvEx] 		= Inventory[playerid][slot][pInvEx];
		CarInventory[vehicle][i][pInvS]			= Inventory[playerid][slot][pInvS];
		CarInventory[vehicle][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
		format(CarInventory[vehicle][i][pInvName], 16, "%s", Inventory[playerid][slot][pInvName]);

		Inventory[playerid][slot][pInvQ] -= strval(inputtext);

		SM2(playerid, "Vous avez déposé %s (%i) dans la boîte à gants de %s.", inventory_items[item][itemName], strval(inputtext), VehicleNames[GetVehicleModel(vehicle)-400]);

		new mes[128];
		format(mes, sizeof(mes), "dépose %s dans la boîte à gants de %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		Save_CarInventory(vehicle);
		return cmd::inventory(playerid, "");
	}
	else if (GetPVarInt(playerid, #inventory_house) != 0)
	{
		new i = GetNextHouseTrunkSlot(houseid);

		if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES &&
			PlayerToPoint(1, playerid, HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
		{
			if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty])
				return SendErrorMessage(playerid, "Vous ne pouvez pas déposer d'arme dans une cachette en service.");

			if (i == -1)
				return SendErrorMessage(playerid, "Il n'y a pas de place dans la maison.");

			SM2(playerid, "Vous avez déposé %s dans la cachette de la maison.", inventory_items[item][itemName]);

			HouseInventory[houseid][i][pInvItem] 	= Inventory[playerid][slot][pInvItem];
			HouseInventory[houseid][i][pInvQ]		= Inventory[playerid][slot][pInvQ];
			HouseInventory[houseid][i][pInvEx]		= Inventory[playerid][slot][pInvEx];
			HouseInventory[houseid][i][pInvS]		= Inventory[playerid][slot][pInvS];
			HouseInventory[houseid][i][pInvQFloat]	= Inventory[playerid][slot][pInvQFloat];
			format(HouseInventory[houseid][i][pInvName], 16, "%s", Inventory[playerid][slot][pInvName]);

			Inventory[playerid][slot][pInvQ] -= strval(inputtext);

			new mes[128];
			format(mes, sizeof(mes), "dépose %s (%i) dans la cachette de la maison.", inventory_items[item][itemName], strval(inputtext));
			SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

			Save_HouseInventory(houseid);
			return cmd::inventory(playerid, "");
		}
	}
	else 
	{
		SendErrorMessage(playerid, "Oups.. Quelque chose s'est mal passé. Veuillez réessayer l'opération.");
		return cmd::inventory(playerid, "");
	}

	return 1;
}

Dialog:InventoryOther(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new houseid = -1;

	if ((houseid = GetNearInsideHouse(playerid)) != -1 && houseid >= 0 && houseid < MAX_HOUSES && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
	{
        if (!HouseInventory[houseid][listitem][pInvItem]) return 1;

		new item = HouseInventory[houseid][listitem][pInvItem];
		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty])
			return SendErrorMessage(playerid, "Vous ne pouvez pas prendre une arme dans une maison en service.");

		if (GivePlayerInvItem(playerid, item,
			HouseInventory[houseid][listitem][pInvQ],
			HouseInventory[houseid][listitem][pInvQFloat],
			HouseInventory[houseid][listitem][pInvEx],
			HouseInventory[houseid][listitem][pInvS],
			HouseInventory[houseid][listitem][pInvName]) == -1)
			return SendErrorMessage(playerid, "Il n'y a pas de place dans l'inventaire.");

		SendSyntaxMessage(playerid, "Vous avez pris %s depuis la cachette de la maison.", inventory_items[item][itemName]);

		HouseInventory[houseid][listitem][pInvName] = EOS;
		HouseInventory[houseid][listitem][pInvQFloat] = 0.0;
		HouseInventory[houseid][listitem][pInvItem] =
		HouseInventory[houseid][listitem][pInvQ] =
		HouseInventory[houseid][listitem][pInvEx] =
		HouseInventory[houseid][listitem][pInvS] = 0;

		Save_HouseInventory(houseid);

		new mes[128];
		format(mes, sizeof(mes), "prend %s depuis la cachette de la maison.", inventory_items[item][itemName]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		return 1;
	}
	
	new vehicle = GetPlayerVehicleID(playerid);

	if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carOwnerID] == PlayerInfo[playerid][pID])
	{
		if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])
			return SendErrorMessage(playerid, "La boîte à gants de ce véhicule ne vous est pas accessible.");

		if (!CarInventory[vehicle][listitem][pInvItem])
			return 1;

		new item = CarInventory[vehicle][listitem][pInvItem];

		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pFaction] &&
			VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && PlayerInfo[playerid][pOnDuty])
			return SendErrorMessage(playerid, "Vous ne pouvez pas prendre une arme d'un véhicule non-faction en service.");

		if (GivePlayerInvItem(playerid, item,
			CarInventory[vehicle][listitem][pInvQ],
			CarInventory[vehicle][listitem][pInvQFloat],
			CarInventory[vehicle][listitem][pInvEx],
			CarInventory[vehicle][listitem][pInvS],
			CarInventory[vehicle][listitem][pInvName]) == -1)
			return SendErrorMessage(playerid, "Il n'y a pas de place dans l'inventaire.");

		CarInventory[vehicle][listitem][pInvName] = EOS;
		CarInventory[vehicle][listitem][pInvQFloat] = 0.0;
		CarInventory[vehicle][listitem][pInvItem] =
		CarInventory[vehicle][listitem][pInvS] =
		CarInventory[vehicle][listitem][pInvEx] =
		CarInventory[vehicle][listitem][pInvQ] = 0;

		Save_CarInventory(vehicle);

		new mes[128];
		format(mes, sizeof(mes), "prend %s de la boîte à gants de %s.", inventory_items[item][itemName], VehicleNames[GetVehicleModel(vehicle)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
		return 1;
	}


	if ((vehicle = GetNearVehicle(playerid, 5.0)) != 0)
	{
		new item = TrunkInventory[vehicle][listitem][pInvItem];

	    if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))
			return SendErrorMessage(playerid, "Ce véhicule n'a pas de coffre.");

		if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])
			return SendErrorMessage(playerid, "Le coffre de ce véhicule ne vous est pas accessible.");

		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pOnDuty] && VehicleInfo[vehicle][carOwnerID] != 0)
			return SendErrorMessage(playerid, "Vous ne pouvez pas déposer une arme dans un véhicule personnel en service.");

		if (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction])
			return SendErrorMessage(playerid, "Vous devez être à côté du coffre.");

		if (!IsPlayerInAnyVehicle(playerid) && !VehicleInfo[vehicle][carTrunkLock])
			return SendErrorMessage(playerid, "Le coffre de ce véhicule est fermé.");

		if (!TrunkInventory[vehicle][listitem][pInvItem])
			return cmd::inventory(playerid, "");

		if (ValidWeaponForInventory(item) && PlayerInfo[playerid][pFaction] &&
			VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction] && PlayerInfo[playerid][pOnDuty])
			return SendErrorMessage(playerid, "Vous ne pouvez pas prendre une arme d’un véhicule non-faction en service.");

		if (GivePlayerInvItem(playerid, item,
			TrunkInventory[vehicle][listitem][pInvQ],
			TrunkInventory[vehicle][listitem][pInvQFloat],
			TrunkInventory[vehicle][listitem][pInvEx],
			TrunkInventory[vehicle][listitem][pInvS],
			TrunkInventory[vehicle][listitem][pInvName]) == -1)
			return SendErrorMessage(playerid, "Il n'y a pas de place dans l'inventaire.");

		if (IsValidDynamicObject(VehicleInfo[vehicle][carAttach][listitem]))
			DestroyDynamicObject(VehicleInfo[vehicle][carAttach][listitem]);

		TrunkInventory[vehicle][listitem][pInvName] = EOS;
		TrunkInventory[vehicle][listitem][pInvQFloat] = 0.0;
		TrunkInventory[vehicle][listitem][pInvItem] =
		TrunkInventory[vehicle][listitem][pInvS] =
		TrunkInventory[vehicle][listitem][pInvEx] =
		TrunkInventory[vehicle][listitem][pInvQ] = 0;

		Save_CarInventory(vehicle);

		new mes[128];
		format(mes, sizeof(mes), "prend %s depuis le %s de %s.", inventory_items[item][itemName], (VehicleInfo[vehicle][carFaction]) ? ("boîte à gants") : ("coffre"), VehicleNames[GetVehicleModel(vehicle)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

	    return 1;
	}
	
	cmd::inventory(playerid, "");
	return 1;
}

Dialog:DeleteItem(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot = GetPVarInt(playerid, #inventory_choose);

	if (slot < 0 || slot > MAX_INV_SLOTS - 1 || Inventory[playerid][slot][pInvItem] == 0)
		return cmd::inventory(playerid, "");

	if (Inventory[playerid][slot][pInvItem] == 21 && maskOn{playerid})
		cmd::mask(playerid, "");

	SendLocalMessage(Range_Normal, playerid, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5, "** (( %s a supprimé l'objet %s de son inventaire. ))", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]
	);

	RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);

	return 1;
}

Dialog:GiveItem(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::inventory(playerid, "");

    new slot = GetPVarInt(playerid, #inventory_choose);

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || Inventory[playerid][slot][pInvItem] == 0)	return cmd::inventory(playerid, "");

	new userid;

    if (sscanf(inputtext, "u", userid))
		return Dialog_Show(playerid, GiveItem, DIALOG_STYLE_INPUT, "{FFFFFF}Donner un objet", "{ffffff}Entrez le nom ou l'ID du joueur à qui vous souhaitez donner l'objet :", "Suivant", "Retour");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (!PlayerNearPlayer(3.0, playerid, userid))
		return SendNotNearPlayer(playerid);

	if (playerid == userid)
		return SendErrorMessage(playerid, "Vous ne pouvez pas vous donner un objet à vous-même.");


	if (inventory_items[Inventory[playerid][slot][pInvItem]][itemSplit] == 0 || Inventory[playerid][slot][pInvQFloat] == 0.1)
	{
		switch(Inventory[playerid][slot][pInvItem])
		{
			case 22..34:
			{
				if (GiveInvItem(userid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)
					return SendErrorMessage(playerid, "Il n'y a pas de place dans l'inventaire.");

				new str[128];
				format(str, sizeof(str), "sort %s et le donne à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
				SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

				SM1(userid, "%s vous donne %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
				SM1(playerid, "Vous avez donné %s à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
					
				RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			}
			case INV_GANJA, INV_MED_GANJA, 103 .. 118:
			{
				if (GiveFloatInvItem(userid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQFloat]) == -1)
					return SendErrorMessage(playerid, "L'inventaire du joueur est plein.");

				new str[128];
				format(str, sizeof(str), "sort %s et le donne à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
				SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

				SM1(userid, "%s vous donne %s (%.1fg).", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQFloat]);
				SM1(playerid, "Vous avez donné %s (%.1fg) à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQFloat], GetNameWithMask(userid));
				
				RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			}
			case INV_DRUG:
			{
				if (GiveDrugInvItem(userid, Inventory[playerid][slot][pInvName], Inventory[playerid][slot][pInvQFloat], Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1)
					return SendErrorMessage(playerid, "L'inventaire du joueur est plein.");

				new str[128];
				format(str, sizeof(str), "sort %s et le donne à %s.", Inventory[playerid][slot][pInvName], GetNameWithMask(userid));
				SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

				SM1(userid, "%s vous donne %s (%.1fg).", GetNameWithMask(playerid), Inventory[playerid][slot][pInvName], Inventory[playerid][slot][pInvQFloat]);
				SM1(playerid, "Vous avez donné %s (%.1fg) à %s.", Inventory[playerid][slot][pInvName], Inventory[playerid][slot][pInvQFloat], GetNameWithMask(userid));
				
				RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);
			}
			default:
			{
				if (GiveInvItem(userid, Inventory[playerid][slot][pInvItem], Inventory[playerid][slot][pInvQ]) == -1)
					return SendErrorMessage(playerid, "L'inventaire du joueur est plein.");

				RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], 0, slot);

				new str[128];
				format(str, sizeof(str), "sort %s et le donne à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
				SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

				SM1(userid, "%s vous donne %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName]);
				SM1(playerid, "Vous avez donné %s à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			}
		}
		return 1;
	}

	SetPVarInt(playerid, #give_item, userid);

	new mes[256];
	format(mes, sizeof(mes), "{ffffff}Objet : %s (Quantité : %i)\n\nEntrez la quantité que vous souhaitez donner au joueur %s :", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));

	Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Donner un objet", mes, "Donner", "Annuler");
	return 1;
}

Dialog:GiveQuantity(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::inventory(playerid, "");

	new slot = GetPVarInt(playerid, #inventory_choose), userid = GetPVarInt(playerid, #give_item);
	
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
	if (playerid == userid) return SendErrorMessage(playerid, "Vous ne pouvez pas vous donner un objet à vous-même.");

	if (slot < 0 || slot > MAX_INV_SLOTS-1 || Inventory[playerid][slot][pInvItem] == 0) return cmd::inventory(playerid, "");

	switch(Inventory[playerid][slot][pInvItem])
	{
		case 22..34:
		{
			return 1;
		}
		case INV_GANJA, INV_MED_GANJA, 103 .. 118:
		{
			format(inputtext, 5, "%.1f", floatstr(inputtext));
			new Float:amount = floatstr(inputtext);

			if (amount < 0.1 || Inventory[playerid][slot][pInvQFloat] < amount)
			{
				SendErrorMessage(playerid, "Vous n'avez pas cette quantité.");

				new mes[256];
				format(mes, sizeof(mes), "{ffffff}Objet : %s (Quantité : %i)\n\nEntrez la quantité que vous souhaitez donner au joueur %s:", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));
				Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Donner un objet", mes, "Donner", "Annuler");
				return 1;
			}

			if (GiveFloatInvItem(userid, Inventory[playerid][slot][pInvItem], amount) == -1) return SendErrorMessage(playerid, "L'inventaire du joueur est plein.");

			new str[128];
			format(str, sizeof(str), "sort %s et le donne à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			SM1(userid, "%s vous donne %s (%.1fg).", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], amount);
			SM1(playerid, "Vous avez donné %s (%.1fg) à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], amount, GetNameWithMask(userid));

			new query[256];
			format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] a donné %s (Quantité: %i) à %s [MYSQL: %i | IP: %s]", GetNameEx(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], amount, GetNameEx(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid));
			insert_log(query, ITEM_LOG, 3, PlayerInfo[playerid][pAccID], PlayerInfo[userid][pAccID]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], amount, slot);
		}
		case INV_DRUG:
		{
			format(inputtext, 5, "%.1f", floatstr(inputtext));
			new Float:amount = floatstr(inputtext);

			if (amount < 0.1 || Inventory[playerid][slot][pInvQFloat] < amount)
			{
				SendErrorMessage(playerid, "Vous n'avez pas cette quantité.");

				new mes[256];
				format(mes, sizeof(mes), "{ffffff}Objet : %s (Quantité : %i)\n\nEntrez la quantité que vous souhaitez donner au joueur %s:", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));
				Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Donner un objet", mes, "Donner", "Annuler");
				return 1;
			}

			if (GiveDrugInvItem(userid, Inventory[playerid][slot][pInvName], amount, Inventory[playerid][slot][pInvEx], Inventory[playerid][slot][pInvS]) == -1) return SendErrorMessage(playerid, "L'inventaire du joueur est plein.");

			new str[128];
			format(str, sizeof(str), "sort %s et le donne à %s.", Inventory[playerid][slot][pInvName], GetNameWithMask(userid));
			SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			SM1(userid, "%s vous donne %s (%.1fg).", GetNameWithMask(playerid), Inventory[playerid][slot][pInvName], amount);
			SM1(playerid, "Vous avez donné %s (%.1fg) à %s.", Inventory[playerid][slot][pInvName], amount, GetNameWithMask(userid));

			new query[256];
			format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] a donné %s [drogue] (Quantité: %i / Pureté: %i%%) à %s [MYSQL: %i | IP: %s]", GetNameEx(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), Inventory[playerid][slot][pInvName], amount, Inventory[playerid][slot][pInvEx], GetNameEx(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid));
			insert_log(query, ITEM_LOG, 3, PlayerInfo[playerid][pAccID], PlayerInfo[userid][pAccID]);

			RemoveFloatInvItem(playerid, Inventory[playerid][slot][pInvItem], amount, slot);
		}
		default:
		{
			if (strval(inputtext) < 1 || Inventory[playerid][slot][pInvQ] < strval(inputtext))
			{
				SendErrorMessage(playerid, "Vous n'avez pas cette quantité.");

				new mes[256];
				format(mes, sizeof(mes), "{ffffff}Objet : %s (Quantité : %i)\n\nEntrez la quantité que vous souhaitez donner au joueur %s:", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], Inventory[playerid][slot][pInvQ], GetNameWithMask(userid));
				Dialog_Show(playerid, GiveQuantity, DIALOG_STYLE_INPUT, "Donner un objet", mes, "Donner", "Annuler");
				return 1;
			}

			if (GiveInvItem(userid, Inventory[playerid][slot][pInvItem], strval(inputtext)) == -1) return SendErrorMessage(playerid, "L'inventaire du joueur est plein.");

			new str[128];
			format(str, sizeof(str), "sort %s et le donne à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], GetNameWithMask(userid));
			SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

			SM1(userid, "%s vous donne %s [quantité: %i].", GetNameWithMask(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], strval(inputtext));
			SM1(playerid, "Vous avez donné %s [quantité: %i] à %s.", inventory_items[Inventory[playerid][slot][pInvItem]][itemName], strval(inputtext), GetNameWithMask(userid));

			new query[128];
			format(query, sizeof(query), "%s [MYSQL: %i | IP: %s] a donné %s (Quantité: %i) à %s [MYSQL: %i | IP: %s]", GetNameEx(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), inventory_items[Inventory[playerid][slot][pInvItem]][itemName], strval(inputtext), GetNameEx(userid), PlayerInfo[userid][pID], q_GetPlayerIP(userid));
			insert_log(query, ITEM_LOG, 3, PlayerInfo[playerid][pAccID], PlayerInfo[userid][pAccID]);

			RemoveInvItem(playerid, Inventory[playerid][slot][pInvItem], strval(inputtext), slot);
		}
	}
	return 1;
}

Dialog:InventoryMenu(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;
	
	switch(listitem)
	{
		case 0: return PrintInv(playerid);
		case 1:
		{
			new houseid;
			if ((houseid = GetNearHouse(playerid)) != -1 && HouseInfo[houseid][hOwnerID] == PlayerInfo[playerid][pID] && PlayerToPoint(1, playerid,HouseInfo[houseid][hCheckPosX], HouseInfo[houseid][hCheckPosY], HouseInfo[houseid][hCheckPosZ]))
			{
				return PrintHouseInv(playerid, houseid);
			}

			new vehicle = GetNearVehicle(playerid, 4.0);
			if (vehicle != 0 && (!IsADontTrunk(vehicle) || !IsAPlane(vehicle) || !IsABiker(vehicle) || !IsAHelicopter(vehicle) || !IsABoat(vehicle) || !IsABicycle(vehicle)))
			{
				if ((VehicleInfo[vehicle][carTrunkLock] && !IsPlayerInAnyVehicle(playerid)) || (IsPlayerInAnyVehicle(playerid) && VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction]))
				{
					return PrintVehInv(playerid, vehicle);
				}
			}
			
			vehicle = GetPlayerVehicleID(playerid);
			if (IsPlayerInAnyVehicle(playerid) && vehicle != 0 && VehicleInfo[vehicle][carOwnerID] == PlayerInfo[playerid][pID])
			{
				return PrintGBInv(playerid, vehicle);
			}
		}
	}

	SendErrorMessage(playerid, "Oups.. Quelque chose s'est mal passé. Veuillez réessayer l'opération.");
	return 1;
}