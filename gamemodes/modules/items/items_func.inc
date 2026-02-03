this::OnPlayerUseItem(playerid, slotid)
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed]) return SendErrorMessage(playerid, "Âû íå ìîæåòå èñïîëüçîâàòü èíâåíòàğü ïğÿìî ñåé÷àñ.");
    if (slotid < 0 || slotid > MAX_INV_SLOTS-1) return 1;
    
    new itemid = Inventory[playerid][slotid][pInvItem];
	
    switch(itemid)
    {
        case INV_SIGN: cmd::carsign(playerid, "");
        case INV_HEAL:
        {
            if (!PlayerInfo[playerid][pInjured])
                return SendErrorMessage(playerid, "Utilisable uniquement en cas de blessure ou d'état critique.");

            if (GetPVarInt(playerid, #heal_time) > gettime())
                return SendErrorMessage(playerid, "Vous pouvez utiliser une trousse de soin une fois par minute.");

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health >= 100.0)
                return SendErrorMessage(playerid, "Votre santé est déjà au maximum.");

            if (health + 15.0 > 100.0)
                SetPlayerHealth(playerid, 100.0);
            else
                SetPlayerHealth(playerid, health + 15.0);

            SetPVarInt(playerid, #heal_time, gettime() + 60);

            Inventory[playerid][slotid][pInvQ]--;
            if (!Inventory[playerid][slotid][pInvQ])
                Inventory[playerid][slotid][pInvItem] = 0;
        }

        case INV_GANJA, INV_MED_GANJA, INV_DRUG:
        {
            if (pTemp[playerid][pDrugTime])
                return SendErrorMessage(playerid, "Vous avez déjà consommé de la drogue récemment.");

            UseDrug(playerid, itemid, slotid);
        }

        case INV_BOOMBOX:   PlaceBoombox(playerid);
        case INV_RADIO:
        {
            new mes[550] = "/r - parler à la radio.\n";
            strcat(mes, "/rr - parler à la radio sur le slot.\n");
            strcat(mes, "/(rl)ow - parler à la radio à voix basse.\n");
            strcat(mes, "/(rrl)ow - parler à la radio à voix basse sur le slot.\n");
            strcat(mes, "/auth - s'authentifier sur une fréquence radio.\n");
            strcat(mes, "/(ch)annel set - changer de fréquence radio.\n");
            strcat(mes, "/(ch)annel rent - louer une fréquence radio.\n");
            strcat(mes, "/setslot - changer de slot radio.\n");
            strcat(mes, "/(ch)annel leave - quitter la fréquence.\n");
            strcat(mes, "/(ch)annel sell - vendre la fréquence (50% remboursé).\n");
            strcat(mes, "/newauth - changer le mot de passe de la fréquence.\n");
            strcat(mes, "/kickoffradio - exclure quelqu’un de la fréquence.\n");
            strcat(mes, "/radiomembers - afficher les membres de la fréquence.");

            Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Radio", mes, "X", "");
        }
        case INV_MASK:      cmd::mask(playerid, "");
        case INV_LIGHTER: SendHelpMessage(playerid, "Utilisez une cigarette dans l'inventaire pour fumer.");
        case INV_CIGAR:
        {
            if (!IsPlayerHasItem(playerid, INV_LIGHTER)) return SendErrorMessage(playerid, "Vous n'avez pas de briquet.");
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            Inventory[playerid][slotid][pInvQ]--;

            if (Inventory[playerid][slotid][pInvQ] == 0)
                Inventory[playerid][slotid][pInvItem] = 0;

            cmd::server_ame(playerid, "sort un cigare de l'étui et l'allume avec un briquet.");
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_SMOKE_CIGGY);
            SM2(playerid, "Vous avez allumé une cigarette.");

            InfoMSG(playerid, "Appuyez sur \"~g~Clic gauche~w~\" pour ~y~utiliser~w~.\nAppuyez sur \"~r~ENTRÉE~w~\" pour ~y~retirer~w~ l'objet.", 5);
        }
        case INV_DICE: SendNearbyMessage(Range_Normal, playerid, COLOR_WHITE, "* %s lance les dés et obtient %i.", GetNameWithMask(playerid), random(6) + 1);
        case INV_PEN:       cmd::note(playerid, "");
        case INV_SILENCER:  cmd::silencer(playerid, "");
        case INV_OPTICS:    cmd::optics(playerid, "");
        case INV_GASCAN:
        {
            if (IsPlayerInAnyVehicle(playerid))
                return SendErrorMessage(playerid, "Vous devez être hors du véhicule.");

            new vehicle;
            if ((vehicle = GetNearVehicle(playerid, 2.5)) == 0)
                return SendErrorMessage(playerid, "Aucun véhicule à proximité.");

            if (VehicleInfo[vehicle][carFuel] + 20.0 > GetVehicleFuelCapacity(GetVehicleModel(vehicle)))
                return SendErrorMessage(playerid, "Le réservoir du véhicule est déjà plein.");

            VehicleInfo[vehicle][carFuel] += 20.0;

            SM2(playerid, "Votre véhicule a été ravitaillé à l'aide du jerrican.");
            cmd::me(playerid, "remplit le véhicule avec un jerrican.");

            Inventory[playerid][slotid][pInvQ]--;

            if (Inventory[playerid][slotid][pInvQ] == 0)
                Inventory[playerid][slotid][pInvItem] = 0;
        }
        case INV_BURGER:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            InfoMSG(playerid, "Appuyez sur ~g~LKM~w~ pour ~y~utiliser~w~.~n~Appuyez sur ~r~ENTRÉE~w~ pour ~y~supprimer~w~ l'objet.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 10 > 100.0) SetPlayerHealth(playerid, 100.0);
            else SetPlayerHealth(playerid, health + 10.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());

            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);

            RemovePlayerItem(playerid, INV_BURGER, 1);
            cmd::server_ame(playerid, "mange un burger.");
        }
        case INV_PIZZA:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            InfoMSG(playerid, "Appuyez sur ~g~LKM~w~ pour ~y~utiliser~w~.~n~Appuyez sur ~r~ENTRÉE~w~ pour ~y~supprimer~w~ l'objet.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 15.0 > 100.0) SetPlayerHealth(playerid, 100.0);
            else SetPlayerHealth(playerid, health + 15.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());

            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);

            RemovePlayerItem(playerid, INV_PIZZA, 1);
            cmd::server_ame(playerid, "mange une pizza.");
        }
        case INV_CHICKEN:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            InfoMSG(playerid, "Appuyez sur ~g~LKM~w~ pour ~y~utiliser~w~.~n~Appuyez sur ~r~ENTRÉE~w~ pour ~y~supprimer~w~ l'objet.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 7.0 > 100.0) SetPlayerHealth(playerid, 100.0);
            else SetPlayerHealth(playerid, health + 7.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());

            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);

            RemovePlayerItem(playerid, INV_CHICKEN, 1);
            cmd::server_ame(playerid, "mange du poulet.");
        }
        case INV_HOTDOG:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            InfoMSG(playerid, "Appuyez sur ~g~LKM~w~ pour ~y~utiliser~w~.~n~Appuyez sur ~r~ENTRÉE~w~ pour ~y~supprimer~w~ l'objet.", 5);

            new Float:health;
            AntiCheatGetHealth(playerid, health);

            if (health + 5.0 > 100.0) SetPlayerHealth(playerid, 100.0);
            else SetPlayerHealth(playerid, health + 5.0);

            SetPVarInt(playerid, #use_food, 5);
            SetPVarInt(playerid, #time_food, gettime());

            SetPlayerAttachedObject(playerid, ATTACH_HANDS, 2703, 6, 0.055999, 0.028000, 0.008000, 2.599998, 0.000000, 16.599998, 0.844000, 0.871000, 0.870000);

            RemovePlayerItem(playerid, INV_HOTDOG, 1);
            cmd::server_ame(playerid, "mange un hot-dog.");
        }
        case INV_WATER, INV_SODA, INV_COLA:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            InfoMSG(playerid, "Appuyez sur ~g~LKM~w~ pour ~y~utiliser~w~.~n~Appuyez sur ~r~ENTRÉE~w~ pour ~y~supprimer~w~ l'objet.", 5);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
            RemovePlayerItem(playerid, itemid, 1);

            if (itemid == INV_WATER)     cmd::server_ame(playerid, "boit de l'eau.");
            else if (itemid == INV_SODA) cmd::server_ame(playerid, "boit du soda.");
            else if (itemid == INV_COLA) cmd::server_ame(playerid, "boit du cola.");
        }
        case INV_BEER:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            InfoMSG(playerid, "Appuyez sur ~g~LKM~w~ pour ~y~utiliser~w~.~n~Appuyez sur ~r~ENTRÉE~w~ pour ~y~supprimer~w~ l'objet.", 5);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_BEER);
            RemovePlayerItem(playerid, itemid, 1);
            cmd::server_ame(playerid, "boit de la bière.");
        }
        case INV_VODKA, INV_WHISKEY, INV_BRANDY, INV_WINE:
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            InfoMSG(playerid, "Appuyez sur ~g~LKM~w~ pour ~y~utiliser~w~.~n~Appuyez sur ~r~ENTRÉE~w~ pour ~y~supprimer~w~ l'objet.", 5);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_WINE);
            RemovePlayerItem(playerid, itemid, 1);

            if (itemid == INV_VODKA)         cmd::server_ame(playerid, "boit de la vodka.");
            else if (itemid == INV_WHISKEY)  cmd::server_ame(playerid, "boit du whisky.");
            else if (itemid == INV_BRANDY)   cmd::server_ame(playerid, "boit du brandy.");
            else                             cmd::server_ame(playerid, "boit du vin.");
        }

        case 1 .. 18, 22 .. 34, 41 .. 43, 46: // ARMES
        {
            if (PlayerInfo[playerid][pPlayerWeapon] != 0)
                cmd::putgun(playerid, "");

            if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
                return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

            if (PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])
                return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction actuellement.");

            PlayerInfo[playerid][pPlayerWeapon] = Inventory[playerid][slotid][pInvItem];
            PlayerInfo[playerid][pPlayerAmmo]   = Inventory[playerid][slotid][pInvQ];
            PlayerInfo[playerid][pAmmoType]     = Inventory[playerid][slotid][pInvEx];
            PlayerInfo[playerid][pWepSerial]    = Inventory[playerid][slotid][pInvS];

            GivePlayerWeaponEx(playerid, Inventory[playerid][slotid][pInvItem], Inventory[playerid][slotid][pInvQ], Inventory[playerid][slotid][pInvEx], Inventory[playerid][slotid][pInvS]);
            SM2(playerid, "Vous avez sorti %s de l'inventaire.", inventory_items[Inventory[playerid][slotid][pInvItem]][itemName]);

            new mes[64];
            if (PlayerInfo[playerid][pPlayerWeapon] >= 22 && PlayerInfo[playerid][pPlayerWeapon] <= 34)
                format(mes, sizeof(mes), "* %s sort une arme.", GetNameEx(playerid));
            else
                format(mes, sizeof(mes), "* %s sort %s.", GetNameEx(playerid), inventory_items[Inventory[playerid][slotid][pInvItem]][itemName]);

            SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);
            
            RemoveInvItem(playerid, Inventory[playerid][slotid][pInvItem], 0, slotid);
        }

        case 82 .. 99:
        {
            if (PlayerInfo[playerid][pPlayerWeapon] != 0 && PlayerInfo[playerid][pPlayerAmmo] != 0)
            {
                SendErrorMessage(playerid, "Votre arme est déjà chargée.");
                return PrintInv(playerid);
            }

            if (PlayerInfo[playerid][pPlayerWeapon] == 0)
            {
                SendErrorMessage(playerid, "Vous n'avez pas d'arme en main.");
                return PrintInv(playerid);
            }

            if (CompatAmmo(PlayerInfo[playerid][pPlayerWeapon], itemid) == 0)
            {
                SendErrorMessage(playerid, "Ces munitions ne sont pas compatibles avec votre arme.");
                return PrintInv(playerid);
            }

            PlayerInfo[playerid][pAmmoType] = itemid;
            PlayerInfo[playerid][pPlayerAmmo] = Inventory[playerid][slotid][pInvQ];

            GivePlayerWeaponEx(playerid,
                PlayerInfo[playerid][pPlayerWeapon],
                PlayerInfo[playerid][pPlayerAmmo],
                PlayerInfo[playerid][pAmmoType],
                PlayerInfo[playerid][pWepSerial]
            );

            SM1(playerid, "Vous avez rechargé %s - %s (%i cartouches).", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);

            new mes[64];
            format(mes, sizeof(mes), "* %s recharge %s",
                GetNameEx(playerid),
                inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName]
            );
            SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 30.0, 10000);

            RemoveInvItem(playerid, itemid, 0, slotid);
            ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);
        }
	}
	return 1;
}

IsPlayerHasItem(playerid, itemid, amount = 0)
{
	if (!IsPlayerLogged(playerid) || !itemid) return 0;
    
    new result = -1;
	for(new i = 0; i != MAX_INV_SLOTS; i++)
	{
	    if (amount > 0 && Inventory[playerid][i][pInvItem] == itemid && Inventory[playerid][i][pInvQ] >= amount) 
        { 
            result = i;
            break; 
        }  
	    else if (amount < 1 && Inventory[playerid][i][pInvItem] == itemid) 
        {
            result = i;
            break; 
        }
	}

    if (result == -1) return 0;
    else return 1;
}

RemovePlayerItem(playerid, itemid, amount = -1)
{
    for(new i; i != MAX_INV_SLOTS; i++)
	{
	    if (Inventory[playerid][i][pInvItem] != itemid) continue;

	    if (amount == -1)
	    {
	        Inventory[playerid][i][pInvItem] = 0;
	        Inventory[playerid][i][pInvQ] = 0;
			return 1;
		}

	    Inventory[playerid][i][pInvQ] -= amount;

		if (Inventory[playerid][i][pInvQ] < 1)
		{
		    Inventory[playerid][i][pInvItem] = 0;
	        Inventory[playerid][i][pInvQ] = 0;
  		}
		return 1;
	}
	return 0;
}

this::LoadHolsters(playerid)
{
	/*if (GetPVarInt(playerid, "Backpack") == 1) 
    {
		if (IsPlayerAttachedObjectSlotUsed(playerid, 7)) RemovePlayerAttachedObject(playerid, 6);
		SetPlayerAttachedObject(playerid, 6, 371, 1, 0.0, -0.14, 0.0, 0.0, 90, 359.5022, 1, 1, 1);
		return 1;
	}*/

    new count;
    for(new w = 0; w < MAX_WEAP-1; w++)
    {
        count = -1;
        for(new p = count+1; p < MAX_INV_SLOTS-1; p++)
        {
            if (inventory_items[Inventory[playerid][p][pInvItem]][itemType] == ITEM_TYPE_WEAPON)
            {
                if (GetWeaponType(Inventory[playerid][p][pInvItem]) == w)
                {
                    count = p;
                    break;
                }
            }
        }
        if (count != -1)
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, w + 5))
                RemovePlayerAttachedObject(playerid, w + 5);

            if (0 == PlayerInfo[playerid][pHideWeap][w] && !IsPlayerAttachedObjectSlotUsed(playerid, w + 5))
            {
                SetPlayerAttachedObject(playerid, w + 5, 
                    inventory_items[Inventory[playerid][count][pInvItem]][itemModel], 
                    PlayerInfo[playerid][pWeaponBone][w], 
                    PlayerInfo[playerid][pWeaponX][w], 
                    PlayerInfo[playerid][pWeaponY][w], 
                    PlayerInfo[playerid][pWeaponZ][w], 
                    PlayerInfo[playerid][pWeaponRX][w], 
                    PlayerInfo[playerid][pWeaponRY][w], 
                    PlayerInfo[playerid][pWeaponRZ][w], 
                    1.0, 1.0, 1.0);
                //SM1(playerid, "[slot: %i]: %s [item id: %i]", w, inventory_items[PlayerInfo[playerid][pInvItem][count]][itemName], count+1);
            }
        }
        else 
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, w + 5)) RemovePlayerAttachedObject(playerid, w + 5);
        }
    }

	return 1;
}

GetWeaponType(weaponid)
{
    switch(weaponid)
    {
        case 2,3,5,6,7,8,15:    return 0; //WEAPON_TYPE_HAND
        case 22,23,24,28,29,32: return 1; //WEAPON_TYPE_SEC
        case 25,26,30,31,33,34: return 2; //WEAPON_TYPE_PRIM
    }
    return -1;
}
    
