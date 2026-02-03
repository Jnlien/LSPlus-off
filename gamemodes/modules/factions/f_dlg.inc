Dialog:DutyMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	    
    new faction = PlayerInfo[playerid][pFaction];
    
	if (faction == 0 || !IsFactionLegal(faction)) return 1;
        
	switch(listitem)
	{
	    case 0://duty
	    {
			if (CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendErrorMessage(playerid, "Veuillez d'abord quitter votre service d'administration.");
			if (pTemp[playerid][pTaser])              return SendErrorMessage(playerid, "Veuillez d'abord ranger votre taser.");
			if (pTemp[playerid][pPatrol])              return SendErrorMessage(playerid, "Veuillez d'abord quitter la patrouille (/callsign).");
			if (!IsPlayerAtUniform(playerid))          return SendErrorMessage(playerid, "Vous n'êtes pas dans la salle d'habillage.");
    	
	        if (!PlayerInfo[playerid][pOnDuty])
			{
				if (PlayerInfo[playerid][pPlayerWeapon]) 
				{
					if (CheckInv(playerid))	cmd::putgun(playerid, "");
					else return SendErrorMessage(playerid, "Rangez d'abord votre arme dans votre inventaire.");
				}
				
				SetPVarInt(playerid, #pDelay, 1);

				new count;
			    for(new i; i < MAX_INV_SLOTS; i++)
				{
					if (inventory_items[Inventory[playerid][i][pInvItem]][itemType] != ITEM_TYPE_WEAPON && 
						inventory_items[Inventory[playerid][i][pInvItem]][itemType] != ITEM_TYPE_AMMO) continue;

					if (count >= MAX_COP_SLOTS)
					{
						for(new j; j < MAX_INV_SLOTS; j++)
						{
							PlayerInfo[playerid][pCopItem][j] = 0;
							PlayerInfo[playerid][pCopQ][j] = 0;
							PlayerInfo[playerid][pCopEx][j] = 0;
							PlayerInfo[playerid][pCopS][j] = 0;
						}

						SetPVarInt(playerid, #pDelay, 0);
						SendAdmMessage("Attention : %s possède une grande quantité d'armes dans son inventaire. [AVERTISSEMENT]", GetNameEx(playerid));
						return SendErrorMessage(playerid, "Vous avez trop d'armes/munitions ! (Vous pouvez stocker seulement 5 objets dans la caisse)");
					}

				    PlayerInfo[playerid][pCopItem][count] = Inventory[playerid][i][pInvItem];
				    PlayerInfo[playerid][pCopQ][count] = Inventory[playerid][i][pInvQ];
					PlayerInfo[playerid][pCopEx][count] = Inventory[playerid][i][pInvEx];
					PlayerInfo[playerid][pCopS][count] = Inventory[playerid][i][pInvItem];

					count++;
				}

				ResetPlayerWeaponsEx(playerid);

				if (IsFactionPolice(faction))
				{
					SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "%s %s a pris son badge de son casier.", GetRankName(playerid), GetNameWithMask(playerid));
				}

				PlayerInfo[playerid][pOnDuty] = 1;
		 		SetPlayerColor(playerid, RemoveAlpha(Factions[faction][fColor]));
		        SendFactionMessage(faction, COLOR_POLICECHAT, "** HQ : %s %s a pris son service. **", GetRankName(playerid), GetNameEx(playerid));
				GameTextForPlayer(playerid, "~g~EN SERVICE", 5000, 3);
				
				cmd::duty(playerid, "");
				Save_User(playerid);

				SetPVarInt(playerid, #pDelay, 0);
				return 1;
			}

			ResetPlayerWeaponsEx(playerid);
			
			if (IsPlayerAttachedObjectSlotUsed(playerid, 5))	RemovePlayerAttachedObject(playerid, 5);
			if (IsPlayerAttachedObjectSlotUsed(playerid, 6))	RemovePlayerAttachedObject(playerid, 6);
    		if (IsPlayerAttachedObjectSlotUsed(playerid, 7))	RemovePlayerAttachedObject(playerid, 7);

			new copsitems = 0, playersitems = 0;
			for(new i = 0; i < MAX_INV_SLOTS; i++)
			{
				if (i < MAX_COP_SLOTS) if (PlayerInfo[playerid][pCopItem][i]) copsitems++;
				if (!Inventory[playerid][i][pInvItem]) playersitems++;
			}

			if (playersitems - copsitems >= 0)
			{
				for(new i = 0; i < MAX_COP_SLOTS; i++)
				{					
					if (PlayerInfo[playerid][pCopItem][i])
					{
						new slotid = GetInvNextSlot(playerid);
						if (slotid == -1)	continue;

						Inventory[playerid][slotid][pInvItem] = PlayerInfo[playerid][pCopItem][i];
						Inventory[playerid][slotid][pInvQ] = PlayerInfo[playerid][pCopQ][i];
						Inventory[playerid][slotid][pInvEx] = PlayerInfo[playerid][pCopEx][i];
						Inventory[playerid][slotid][pInvS] = PlayerInfo[playerid][pCopS][i];
					}
					
					PlayerInfo[playerid][pCopItem][i] = 0;
					PlayerInfo[playerid][pCopQ][i] = 0;
					PlayerInfo[playerid][pCopEx][i] = 0;
					PlayerInfo[playerid][pCopS][i] = 0;				
				}
			}
			else return SendErrorMessage(playerid, "Vous ne pouvez pas prendre d'objets de votre casier. Votre inventaire est plein.");

			if (IsFactionPolice(faction))
			{
				SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "%s %s a rangé son badge dans son casier.", GetRankName(playerid), GetNameWithMask(playerid));
			}

			SendFactionMessage(faction, IsFactionPolice(faction) ? COLOR_LIGHTBLUE : LSFD_DUTY, "** HQ : %s %s a terminé son service. **", GetRankName(playerid), GetNameEx(playerid));
			GameTextForPlayer(playerid, "~r~HORS SERVICE", 5000, 3);
			
			DestroyAllBort(playerid);
		    PlayerInfo[playerid][pChar] = 0;
			PlayerInfo[playerid][pSwatDuty] = 0;
			PlayerInfo[playerid][pOnDuty] = 0;

			CallRemoteFunction("LoadHolsters","i",playerid);
			SetPlayerColor(playerid, TEAM_HIT_COLOR);
			SetPlayerArmour(playerid, 0);
			SetPlayerSkin(playerid, PlayerInfo[playerid][pSkin]);
			cmd::duty(playerid, "");

			return Save_User(playerid);
	    }
	    case 1:
		{
			if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez être en service.");
			if (!IsPlayerAtUniform(playerid))   return SendErrorMessage(playerid, "Vous devez être dans les vestiaires.");
			if (PlayerInfo[playerid][pSwatDuty]) return SendErrorMessage(playerid, "Déposez d'abord votre équipement S.W.A.T avant de changer d'uniforme.");

			if (IsFactionPolice(faction))
			{
				new submes[16],
					mes[sizeof(police_skins)*sizeof(submes)];
				
				for (new i = 0; i < sizeof(police_skins); i++)
				{
					format(submes, sizeof(submes), "%i\tID: %i\n", police_skins[i], i);
					strcat(mes, submes);
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Sélection de l'uniforme de service", mes, "Choisir", "Fermer");
			}
			else if (IsFactionMedic(faction) || IsFactionFire(faction))
			{
				new submes[16], mes[sizeof(fd_skins)*sizeof(submes)];

				for (new i = 0; i < sizeof(fd_skins); i++)
				{
					format(submes, sizeof(submes), "%i\tID: %i\n", fd_skins[i], i);
					strcat(mes, submes);
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Sélection de l'uniforme de service", mes, "Choisir", "Fermer");
			}
			else if (IsFactionDocs(faction))
			{
				new mes[sizeof(docs_skins)*128];
				for (new i = 0; i < sizeof(docs_skins); i++)
				{
					format(mes, sizeof(mes), "%s%i\tID: %i\n", mes, docs_skins[i], docs_skins[i]);
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Sélection de l'uniforme de service", mes, "Choisir", "Fermer");
			}
			else if (IsFactionCity(faction))
			{
				new submes[128], mes[sizeof(city_skins)*sizeof(submes)];

				for (new i = 0; i < sizeof(city_skins); i++)
				{
					format(submes, sizeof(submes), "%i\tID: %i\n", city_skins[i], i);
					strcat(mes, submes);
				}
				Dialog_Show(playerid, MODEL_SELECTION_UNIFORM, DIALOG_STYLE_PREVIEW_MODEL, "Sélection de l'uniforme de service", mes, "Choisir", "Fermer");
			}
			else SendErrorMessage(playerid, "Votre faction n'a pas accès à cette fonction.");
		}
		case 2:
		{
			if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez être en service.");
			if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid)) return SendErrorMessage(playerid, "Vous devez être dans les vestiaires.");

			if (IsFactionPolice(faction))
			{
				return Dialog_Show(playerid, LSPDArmoury, DIALOG_STYLE_LIST, "Équipement & Armes", 
					"• Radio\n• Armes\n• Équipement S.W.A.T.", 
					"Choisir", "Retour");
			}
			else if (IsFactionDocs(faction))
			{   
				return Dialog_Show(playerid, DOCSGUN, DIALOG_STYLE_LIST, "Armes", \
					"• Radio\n"\
					"• Matraque\n"\
					"• Spray au poivre\n"\
					"• Desert Eagle ( .50AE .ext )\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( .50AE .ext )\n"\
					"{FFFFFF}• MP5 ( 9x19mm .ext )\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( 9x19mm .ext )\n"\
					"{FFFFFF}• Shotgun ( 12-G BIRDSHOT )\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( 12-G BIRDSHOT )\n"\
					"\t{ffc46a}- Prendre munitions non-létales ( 12-G BEANBAG )\n"\
					"{FFFFFF}• M4 ( 5.56x45mm (drum) )\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( 5.56x45mm (drum) )\n"\
					"\t{ffc46a}- Prendre munitions non-létales ( 5.56x45mm (rubber) )\n"\
					"{FFFFFF}• Sniper Rifle (7.62x51mm)\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( 7.62x51mm )\n"\
					"{FFFFFF}• Grenade à gaz\n"\
					"• Appareil photo\n"\
					"{FF6347}• Rendre tout l'équipement",\
					"Choisir", "Fermer");
			}
			else if (IsFactionCity(faction))
			{
				return Dialog_Show(playerid, CITYGUN, DIALOG_STYLE_LIST, "Armes", \
					"• Radio\n"\
					"• Desert Eagle ( .50AE .ext )\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( .50AE .ext )\n"\
					"{FFFFFF}• MP5 ( 9x19mm .ext )\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( 9x19mm .ext )\n"\
					"{FFFFFF}• M4 ( 5.56x45mm (drum) )\n"\
					"\t{ffc46a}- Prendre munitions supplémentaires ( 5.56x45mm (drum) )\n"\
					"\t{ffc46a}- Prendre munitions non-létales ( 5.56x45mm (rubber) )\n"\
					"{FF6347}• Rendre toutes les armes", \
					"Choisir", "Fermer");
			}
			else if (IsFactionFire(faction) || IsFactionMedic(faction))
			{
				return Dialog_Show(playerid, FDArmoury, DIALOG_STYLE_LIST, "Équipement", \
					"• Radio\n"\
					"• Tronçonneuse\n"\
					"• Extincteur\n"\
					"{FF6347}• Rendre tout l'équipement", \
					"Choisir", "Retour");
			}
		}
	    case 3:
		{
			if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez être en service.");
			if (!IsFactionLegal(faction)) return SendErrorMessage(playerid, "Vous devez faire partie d'une faction légale.");
			if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid)) return SendErrorMessage(playerid, "Vous devez être dans les vestiaires.");

			if (IsFactionMedic(faction) || IsFactionFire(faction)) 
				SetPlayerArmour(playerid, 50.0);
			else 
				SetPlayerArmour(playerid, 100.0);

			SendSyntaxMessage(playerid, "Vous avez enfilé un gilet pare-balles.");
			cmd::duty(playerid, "");
		}
	    case 4:
		{
			if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
			if (!IsPlayerAtUniform(playerid)) return SendErrorMessage(playerid, "Vous devez être dans les vestiaires.");

			SetPlayerHealth(playerid, 100.0);

			SendSyntaxMessage(playerid, "Votre santé a été restaurée.");
			cmd::duty(playerid, "");
		}
	}
	return 1;
}

Dialog:ScrapVehicleFaction(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	
    if (PlayerInfo[playerid][pFaction] == 0)				return SendErrorMessage(playerid, "Vous n'êtes pas dans une faction.");
	if (!IsFactionLegal(PlayerInfo[playerid][pFaction]))	return SendErrorMessage(playerid, "Vous devez appartenir à une faction gouvernementale.");

	new veh = PlayerInfo[playerid][pCarKey];

	if (!IsValidVehicle(veh))					return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
	if (!IsPlayerInVehicle(playerid, veh))		return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
	if (IsADonateModel(GetVehicleModel(veh)))	return SendErrorMessage(playerid, "Vous ne pouvez pas transférer ce véhicule.");
	if (VehicleInfo[veh][carComponent][5] != 0)	return SendErrorMessage(playerid, "Vous ne pouvez pas transférer ce véhicule tant qu'il possède du nitro.");
	    
    VehicleInfo[veh][carOwnerID] = 0;
    VehicleInfo[veh][carFaction] = PlayerInfo[playerid][pFaction];
    StopCarBoomBox(veh);
    
	new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `cars` SET `owner_id`=0, `faction`=%i WHERE `id`=%i", PlayerInfo[playerid][pFaction], VehicleInfo[veh][carID]);
	mysql_tquery(dbHandle, query);

    PlayerInfo[playerid][pCarKey] = 0;
    VehicleInfo[veh][carOwnerPlayer] = INVALID_PLAYER_ID;
    DeleteVehicleTickets(veh);

	SendSyntaxMessage(playerid, "Vous avez transféré votre véhicule %s à votre faction %s.", VehicleNames[VehicleInfo[veh][carModel]-400], Factions[PlayerInfo[playerid][pFaction]][fName]);
	return 1;
}

Dialog:BortListChoose(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bortlist(playerid, "");

    new bort = GetPVarInt(playerid, #select_bort) - 1;

    DeletePVar(playerid, #select_bort);
	if (bort < 0) return SendErrorMessage(playerid, "Objet introuvable.");

	if (BortInfo[playerid][bort][bortUsing]) return SendErrorMessage(playerid, "Cet objet est actuellement utilisé!");

	if (listitem == 0)
	{
	    if (BortInfo[playerid][bort][bortOn] == 0) return SendErrorMessage(playerid, "Objet introuvable.");

		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(BortInfo[playerid][bort][bortObject], x, y, z);

		if (GetPlayerDistanceFromPoint(playerid, x, y, z) > 10.0) return SendErrorMessage(playerid, "L'objet est trop loin de vous.");
		if (IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");
    	if (!PlayerInfo[playerid][pOnDuty]) return SendErrorMessage(playerid, "Vous devez d'abord être en service.");

		EditDynamicObject(playerid, BortInfo[playerid][bort][bortObject]);
		pTemp[playerid][pEditBort] = bort + 1;
	    return 1;
	}

    if (!BortInfo[playerid][bort][bortOn]) return SendErrorMessage(playerid, "Objet introuvable.");

	BortInfo[playerid][bort][bortOn] = 0;
	BortInfo[playerid][bort][bortUsing] = 0;

	if (IsValidDynamicObject(BortInfo[playerid][bort][bortObject]))
	{
		DestroyDynamicObject(BortInfo[playerid][bort][bortObject]);
 		BortInfo[playerid][bort][bortObject] = INVALID_OBJECT_ID;
	}

	SendSyntaxMessage(playerid, "L'objet '%s' a été supprimé.", BortInfo[playerid][bort][bortName]);

	for (new i = 0; i != MAX_BORTES; i++)
	{
		if (BortInfo[playerid][i][bortOn] == 0) continue;
		cmd::bortlist(playerid, "");
		break;
	}

	return 1;
}

Dialog:BortList(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (listitem == 0)
	{
		SendSyntaxMessage(playerid, "Tous les objets ont été supprimés.");
	    DestroyAllBort(playerid);
	    return 1;
	}
	
	new count;

	for (new i = 0; i != MAX_BORTES; i ++)
	{
		if (BortInfo[playerid][i][bortOn] == 0) continue;
		
  		if (count == listitem-1)
      	{
       		Dialog_Show(playerid, BortListChoose, DIALOG_STYLE_LIST, BortInfo[playerid][i][bortName], "Modifier l'objet\nSupprimer l'objet", "Choisir", "Retour");
         	SetPVarInt(playerid, #select_bort, i+1);
			return 1;
   		}
    	count++;
	}
	SendErrorMessage(playerid, "Objet introuvable.");
	return 1;
}

Dialog:FactionSettings(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	return ShowRightsForRank(playerid, listitem);
}

Dialog:FactionSettings_Set(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new faction = PlayerInfo[playerid][pFaction], rank = GetPVarInt(playerid, "Faction:RankEdit")-1;

	switch(listitem)
	{
		case 0:	FRank[faction][rank][rInvite] = !FRank[faction][rank][rInvite];
		case 1: FRank[faction][rank][rUninvite] = !FRank[faction][rank][rUninvite];
		case 2: FRank[faction][rank][rRank] = !FRank[faction][rank][rRank];
		case 3: FRank[faction][rank][r_eRank] = !FRank[faction][rank][r_eRank];
		case 4: FRank[faction][rank][rSpawn] = !FRank[faction][rank][rSpawn];
		case 5: FRank[faction][rank][rChat] = !FRank[faction][rank][rChat];
		case 6: FRank[faction][rank][rTow] = !FRank[faction][rank][rTow];
		case 7: FRank[faction][rank][r_eRights] = !FRank[faction][rank][r_eRights];
		
	}

	UpdateFactionRank(faction, rank+1);
	ShowRightsForRank(playerid, rank);
	DeletePVar(playerid, "Faction:EditRank");
	return 1;
}