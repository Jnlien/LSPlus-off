CMD:reload(playerid, params[])
{
	if (PlayerInfo[playerid][pPlayerWeapon] && PlayerInfo[playerid][pPlayerAmmo])
		return SendErrorMessage(playerid, "Votre arme est déjà chargée.");

    if (!PlayerInfo[playerid][pPlayerWeapon])
    	return SendErrorMessage(playerid, "Vous n'avez pas d'arme en main.");
	
	new slotid = -1;
	for (new i = 0; i < MAX_INV_SLOTS; i++)
	{
		if (CompatAmmo(PlayerInfo[playerid][pPlayerWeapon], Inventory[playerid][i][pInvItem]))
		{
			slotid = i;
			break;
		}
	}
	if (slotid == -1)
		return SendErrorMessage(playerid, "Vous n'avez pas les munitions compatibles pour cette arme.");

	PlayerInfo[playerid][pAmmoType] = Inventory[playerid][slotid][pInvItem];
	PlayerInfo[playerid][pPlayerAmmo] = Inventory[playerid][slotid][pInvQ];

	GivePlayerWeaponEx(playerid,
		PlayerInfo[playerid][pPlayerWeapon],
		PlayerInfo[playerid][pPlayerAmmo],
		PlayerInfo[playerid][pAmmoType],
		PlayerInfo[playerid][pWepSerial]
	);

	SM1(playerid, "Vous avez rechargé %s - %s (%i munitions).", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);

	new mes[64];
	format(mes, sizeof(mes), "* %s recharge %s", GetNameEx(playerid), inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName]);
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 30.0, 10000);

	RemoveInvItem(playerid, Inventory[playerid][slotid][pInvItem], 0, slotid);

	ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);

	return 1;
}

CMD:weapon(playerid, params[])
{
	new option[24], secoption, thirdoption, fourthoption;

	if (sscanf(params, "s[24]D(-1)D(-1)D(-1)", option, secoption, thirdoption, fourthoption)) 
	{
		SendHelpMessage(playerid, "/weapon [edit / bone / hide / reset]");
		SendSyntaxMessage(playerid, "Utilisez {ffc46a}/weaponhelp{FFFFFF} pour plus d'informations.");
		return 1;
	}

	if (!strcmp(option, "bone", true))
	{
		new slot, bone, type;
		if (sscanf(params, "{s[7]}ddD(-1)", slot, bone, type))
		{
			SendHelpMessage(playerid, "/weapon bone [slot] [os]");
			SendSyntaxMessage(playerid, "Slot  - [ 1 - Arme de poing | 2 - Arme secondaire | 3 - Arme principale ]");
			SendSyntaxMessage(playerid, "Os    - [ 1 - Colonne vertébrale | 3 - Épaule gauche | 4 - Épaule droite | 5 - Main gauche | 6 - Main droite ]");
			SendSyntaxMessage(playerid, "Os    - [ 7 - Hanche droite | 8 - Hanche gauche | 9 - Jambe gauche | 10 - Jambe droite ]");
			return 1;
		}

		if (slot > 3 || slot < 1) return SendErrorMessage(playerid, "Le slot doit être compris entre 1 et 3.");
		if (IsPlayerHoldingWeapon(playerid)) return SendErrorMessage(playerid, "Rangez votre arme dans l'inventaire.");
		if (bone < 1 || bone == 2 || bone > 10) return SendErrorMessage(playerid, "L'ID de l'os doit être entre 1 et 10 (sauf 2).");
		if (GetPVarInt(playerid, #edit_weapon) != 0) return SendErrorMessage(playerid, "Vous modifiez déjà une position d'arme.");

		new weaponid = -1;
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (GetWeaponType(Inventory[playerid][i][pInvItem]) == slot-1)
			{
				weaponid = Inventory[playerid][i][pInvItem];
				break;
			}
		}
		if (weaponid == -1) return SendErrorMessage(playerid, "Vous n'avez aucune arme correspondante dans cet emplacement.");

		SetPVarInt(playerid, #edit_weapon, slot);
		SetPlayerAttachedObject(playerid, 4 + slot, inventory_items[weaponid][itemModel], PlayerInfo[playerid][pWeaponBone][slot-1]);
		EditAttachedObject(playerid, 4 + slot);
		
		return 1;
	}
	else if (!strcmp(option, "edit", true))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))				
			return SendHelpMessage(playerid, "/weapon adjust [Slot (1 - Arme de poing | 2 - Arme secondaire | 3 - Arme principale)]");

		if (slot > 3 || slot < 1)						
			return SendErrorMessage(playerid, "Le slot doit être compris entre 1 et 3.");

		if (IsPlayerHoldingWeapon(playerid))				
			return SendErrorMessage(playerid, "Rangez votre arme dans l'inventaire d'abord.");

		if (GetPVarInt(playerid, #edit_weapon) != 0)		
			return SendErrorMessage(playerid, "Vous êtes déjà en train d'éditer une arme.");

		new weaponid = -1;
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (GetWeaponType(Inventory[playerid][i][pInvItem]) == slot-1)
			{
				weaponid = Inventory[playerid][i][pInvItem];
				break;
			}
		}

		if (weaponid == -1)	
			return SendErrorMessage(playerid, "Aucune arme correspondante dans ce slot dans votre inventaire.");

		SetPVarInt(playerid, #edit_weapon, slot);
		EditAttachedObject(playerid, 4 + slot);

		return 1;
	}
	else if (!strcmp(option, "reset", true))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))
			return SendHelpMessage(playerid, "/weapon reset [Slot (1 - Arme de poing | 2 - Arme secondaire | 3 - Arme principale)]");

		if (slot > 3 || slot < 1)	
			return SendErrorMessage(playerid, "Le slot doit être compris entre 1 et 3.");

		if (slot == 1)
		{
			PlayerInfo[playerid][pWeaponBone][0] = 1;
			PlayerInfo[playerid][pWeaponX][0] = 0.323999;
			PlayerInfo[playerid][pWeaponY][0] = -0.144999;
			PlayerInfo[playerid][pWeaponZ][0] = -0.166;
			PlayerInfo[playerid][pWeaponRX][0] = 0.0;
			PlayerInfo[playerid][pWeaponRY][0] = -72.0;
			PlayerInfo[playerid][pWeaponRZ][0] = 0.0;
			RemovePlayerAttachedObject(playerid, 5);
			Save_AttachWeapon(playerid, 0);
			SendSyntaxMessage(playerid, "Vous avez réinitialisé la position par défaut de l’arme de poing.");
		}
		else if (slot == 2)
		{
			PlayerInfo[playerid][pWeaponBone][1] = 8;
			PlayerInfo[playerid][pWeaponX][1] = -0.239;
			PlayerInfo[playerid][pWeaponY][1] = -0.062999;
			PlayerInfo[playerid][pWeaponZ][1] = 0.079999;
			PlayerInfo[playerid][pWeaponRX][1] = -97.2;
			PlayerInfo[playerid][pWeaponRY][1] = -8.1;
			PlayerInfo[playerid][pWeaponRZ][1] = -1.7;
			RemovePlayerAttachedObject(playerid, 6);
			Save_AttachWeapon(playerid, 1);
			SendSyntaxMessage(playerid, "Vous avez réinitialisé la position par défaut de l’arme secondaire.");
		}
		else if (slot == 3)
		{
			PlayerInfo[playerid][pWeaponBone][2] = 1;
			PlayerInfo[playerid][pWeaponX][2] = 0.106999;
			PlayerInfo[playerid][pWeaponY][2] = 0.207;
			PlayerInfo[playerid][pWeaponZ][2] = -0.079;
			PlayerInfo[playerid][pWeaponRX][2] = 179.1;
			PlayerInfo[playerid][pWeaponRY][2] = 143.1;
			PlayerInfo[playerid][pWeaponRZ][2] = -0.099999;
			RemovePlayerAttachedObject(playerid, 7);
			Save_AttachWeapon(playerid, 2);
			SendSyntaxMessage(playerid, "Vous avez réinitialisé la position par défaut de l’arme principale.");
		}
		CallRemoteFunction("LoadHolsters","i",playerid);

		return 1;
	}
	else if (!strcmp(option, "hide", true))
	{
		new slot;

		if (sscanf(params, "{s[7]}d", slot))
			return SendHelpMessage(playerid, "/weapon hide [Slot (1 - Arme de poing | 2 - Arme secondaire | 3 - Arme principale)]");

		if (slot > 3 || slot < 1)
			return SendErrorMessage(playerid, "Le slot doit être compris entre 1 et 3.");

		if (GetPVarInt(playerid, #edit_weapon) != 0)
			return SendErrorMessage(playerid, "Vous êtes déjà en train d’éditer la position d’une arme.");

		new weaponid = -1;
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (GetWeaponType(Inventory[playerid][i][pInvItem]) == slot-1)
			{
				weaponid = Inventory[playerid][i][pInvItem];
				break;
			}
		}
		if (weaponid == -1)
			return SendErrorMessage(playerid, "Vous n’avez aucune arme correspondant à ce slot dans votre inventaire.");

		if (IsWeaponDontHide(weaponid))
			return SendErrorMessage(playerid, "Vous ne pouvez pas dissimuler cette arme.");

		PlayerInfo[playerid][pHideWeap][slot-1] = (PlayerInfo[playerid][pHideWeap][slot-1]) ? (0) : (1);

		if (!PlayerInfo[playerid][pHideWeap][slot-1])
			SendSyntaxMessage(playerid, "Vous avez sorti l’arme. / Slot : %i /", slot);
		else
		{
			SendSyntaxMessage(playerid, "Vous avez dissimulé l’arme. / Slot : %i /", slot);
			RemovePlayerAttachedObject(playerid, 4 + slot);
		}


		CallRemoteFunction("LoadHolsters","i",playerid);
		return 1;
	}
	else
	{
		SendHelpMessage(playerid, "/weapon [edit / bone / hide / reset]");
	}
	return 1;
}

CMD:optics(playerid, params[])
{
    if (PlayerHoldingWeapon(playerid, 33)) // Arme sans viseur
	{
		if (!IsPlayerHasItem(playerid, INV_OPTICS))
			return SendErrorMessage(playerid, "Vous n'avez pas de viseur optique.");

		RemovePlayerItem(playerid, INV_OPTICS, 1);

		if (PlayerInfo[playerid][pPlayerAmmo])
			ResetPlayerWeapons(playerid);
		else
			RemovePlayerAttachedObject(playerid, 9);

		GivePlayerWeaponEx(playerid, 34, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);
		ApplyAnimation(playerid, "RIFLE", "rifle_load", 4.1, 0, 0, 0, 0, 0, 1);

		SendSyntaxMessage(playerid, "Vous avez équipé un viseur optique.");
		return 1;
	}
	else if (PlayerHoldingWeapon(playerid, 34)) // Arme avec viseur
	{
		if (GiveInvItem(playerid, INV_OPTICS, 1))
			return SendErrorMessage(playerid, "Il n'y a pas assez de place dans votre inventaire pour le viseur optique.");

		if (PlayerInfo[playerid][pPlayerAmmo])
			ResetPlayerWeapons(playerid);
		else
			RemovePlayerAttachedObject(playerid, 9);

		GivePlayerWeaponEx(playerid, 33, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);
		ApplyAnimation(playerid, "RIFLE", "rifle_load", 4.1, 0, 0, 0, 0, 0, 1);

		SendSyntaxMessage(playerid, "Vous avez retiré le viseur optique.");
		return 1;
	}

	SendErrorMessage(playerid, "Vous devez tenir un fusil avec ou sans viseur pour utiliser cette commande.");

	return 1;
}

CMD:silencer(playerid, params[])
{
    if (pTemp[playerid][pTaser]) 
		return SendErrorMessage(playerid, "Rangez le taser.");

	if (PlayerInfo[playerid][pPlayerWeapon] == 22) // Pistolet normal
	{
		if (!IsPlayerHasItem(playerid, INV_SILENCER)) 
			return SendErrorMessage(playerid, "Vous n'avez pas de silencieux.");

		SendSyntaxMessage(playerid, "Vous avez équipé un silencieux.");

		if (PlayerInfo[playerid][pPlayerAmmo])
			ResetPlayerWeapons(playerid);
		else
			RemovePlayerAttachedObject(playerid, 9);

		RemovePlayerItem(playerid, INV_SILENCER, 1);

		GivePlayerWeaponEx(playerid, 23, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);

		if (!IsPlayerInAnyVehicle(playerid))
			ApplyAnimation(playerid, "COLT45", "colt45_reload", 4.1, 0, 0, 0, 0, 0, 1);
		return 1;
	}
	else if (PlayerInfo[playerid][pPlayerWeapon] == 23) // Pistolet avec silencieux
	{
		if (GiveInvItem(playerid, INV_SILENCER, 1) == -1)
			return SendErrorMessage(playerid, "Pas assez de place dans l'inventaire pour le silencieux.");

		SendSyntaxMessage(playerid, "Vous avez retiré le silencieux.");

		if (PlayerInfo[playerid][pPlayerAmmo])
			ResetPlayerWeapons(playerid);
		else
			RemovePlayerAttachedObject(playerid, 9);

		GivePlayerWeaponEx(playerid, 22, PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]);

		if (!IsPlayerInAnyVehicle(playerid))
			ApplyAnimation(playerid, "COLT45", "colt45_reload", 4.1, 0, 0, 0, 0, 0, 1);
		return 1;
	}

	SendErrorMessage(playerid, "Vous devez avoir un pistolet avec ou sans silencieux en main.");
	return 1;
}

ALTX:takeprime("/tap");
CMD:takeprime(playerid, params[])
{
   	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])  
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (PlayerInfo[playerid][pPlayerWeapon])
		return SendErrorMessage(playerid, "Vous avez déjà une arme en main !");

	new foundid = -1, 
		str[64];

	// Vérifie s'il y a une arme principale récemment utilisée
	if (PlayerInfo[playerid][pLastPrim] != -1) 
	{
		if (GetWeaponType(Inventory[playerid][PlayerInfo[playerid][pLastPrim]][pInvItem]) == WEAPON_TYPE_PRIMARY) 
		{
			foundid = PlayerInfo[playerid][pLastPrim];
		} 
		else 
		{
			PlayerInfo[playerid][pLastPrim] = -1;
			SQL_SetInteger("users", "LastPrim", -1, PlayerInfo[playerid][pID]);
		}
	}

	// Sinon cherche une arme principale dans l'inventaire
	if (foundid == -1) 
	{
		for(new i = 0; i < MAX_INV_SLOTS; i++) 
		{
			if (Inventory[playerid][i][pInvItem] >= 25 && Inventory[playerid][i][pInvItem] <= 34) 
			{
				foundid = i;
				break;
			}
		}
	}

	if (foundid == -1) 
		return SendErrorMessage(playerid, "Vous n'avez pas d'arme de gros calibre !");

	GivePlayerWeaponEx(playerid, Inventory[playerid][foundid][pInvItem], Inventory[playerid][foundid][pInvQ], Inventory[playerid][foundid][pInvEx], Inventory[playerid][foundid][pInvS]);
	ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);

	SM2(playerid, "Vous avez sorti %s de votre inventaire.", inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);

	format(str, sizeof(str), "* %s a sorti %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);
	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

	RemoveInvItem(playerid, Inventory[playerid][foundid][pInvItem], 0, foundid);

	return 1;
}

ALTX:takesecond("/ts");
CMD:takesecond(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])  
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (PlayerInfo[playerid][pPlayerWeapon]) 
		return SendErrorMessage(playerid, "Vous avez déjà une arme en main !");

	new 
		foundid = -1, 
		str[64]
	;

	// Si une arme secondaire a été utilisée récemment
	if (PlayerInfo[playerid][pLastSec] != -1) 
	{
		if (GetWeaponType(Inventory[playerid][PlayerInfo[playerid][pLastSec]][pInvItem]) == WEAPON_TYPE_SECONDARY) 
		{
			foundid = PlayerInfo[playerid][pLastSec];
		} 
		else 
		{
			PlayerInfo[playerid][pLastSec] = -1;
			SQL_SetInteger("users", "LastSec", -1, PlayerInfo[playerid][pID]);
		}
	}

	// Sinon cherche dans l'inventaire une arme secondaire
	if (foundid == -1)
	{
		for(new i = 0; i < MAX_INV_SLOTS; i++)
		{
			if (Inventory[playerid][i][pInvItem] >= 22 && Inventory[playerid][i][pInvItem] <= 24)
			{
				foundid = i;
				break;
			}
		}
	}

	if (foundid == -1) 
		return SendErrorMessage(playerid, "Vous n'avez pas d'arme de petit calibre !");

	GivePlayerWeaponEx(playerid, Inventory[playerid][foundid][pInvItem], Inventory[playerid][foundid][pInvQ], Inventory[playerid][foundid][pInvEx], Inventory[playerid][foundid][pInvS]);
	ApplyAnimation(playerid, "SILENCED", "Silence_reload", 3.0, 0, 0, 0, 0, 0);
	
	SM2(playerid, "Vous avez sorti %s de votre inventaire.", inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);

	format(str, sizeof(str), "* %s sort %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);
	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

	RemoveInvItem(playerid, Inventory[playerid][foundid][pInvItem], 0, foundid);

	return 1;
}

ALTX:takemelee("/tm");
CMD:takemelee(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])  
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (PlayerInfo[playerid][pPlayerWeapon]) return SendErrorMessage(playerid, "Vous avez déjà une arme en main !");

	new 
		foundid = -1, 
		str[64]
	;

	if (PlayerInfo[playerid][pLastMelee] != -1) 
	{
		if (GetWeaponType(Inventory[playerid][PlayerInfo[playerid][pLastMelee]][pInvItem]) == WEAPON_TYPE_MELEE) 
		{
			foundid = PlayerInfo[playerid][pLastMelee];
		} 
		else 
		{
			PlayerInfo[playerid][pLastMelee] = -1;
			SQL_SetInteger("users", "LastMelee", -1, PlayerInfo[playerid][pID]);
		}
	}
	if (foundid == -1) 
	{
		for(new i = 0; i < MAX_INV_SLOTS; i++) 
		{
			if (Inventory[playerid][i][pInvItem] >= 1 && Inventory[playerid][i][pInvItem] <= 15) 
			{
				foundid = i;
				break;
			}
		}
	}
	if (foundid == -1) return SendErrorMessage(playerid, "Vous n'avez pas d'arme de mêlée.");

	GivePlayerWeaponEx(playerid, Inventory[playerid][foundid][pInvItem], Inventory[playerid][foundid][pInvQ], Inventory[playerid][foundid][pInvEx], Inventory[playerid][foundid][pInvS]);

	SM2(playerid, "Vous avez sorti %s de votre inventaire.", inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);

	format(str, sizeof(str), "* %s sort %s.", GetNameWithMask(playerid), inventory_items[Inventory[playerid][foundid][pInvItem]][itemName]);
	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

	RemoveInvItem(playerid, Inventory[playerid][foundid][pInvItem], 0, foundid);
	return 1;
}

ALTX:putgun("/pg");
CMD:putgun(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (!PlayerInfo[playerid][pPlayerWeapon]) return SendErrorMessage(playerid, "Vous n'avez pas d'arme en main.");
	if (!CheckInv(playerid)) return SendErrorMessage(playerid, "Votre inventaire est plein.");

	new
		str[64],
		wep = CheckPlayerWeaponEx(playerid);

	if (!wep) return SendAdmMessage("Le joueur %s[ID:%i] est suspecté d'utiliser des cheats d'arme !", GetNameEx(playerid), playerid);

	if (wep >= 22 && wep <= 34) {
		new am = CheckAmmo(PlayerInfo[playerid][pAmmoType]);
		if (PlayerInfo[playerid][pPlayerAmmo] > am) PlayerInfo[playerid][pPlayerAmmo] = am;
	}

	new ammo = PlayerInfo[playerid][pPlayerAmmo],
		ammotype = PlayerInfo[playerid][pAmmoType],
		serial = PlayerInfo[playerid][pWepSerial];

	ResetPlayerWeapons(playerid);
	PlayerInfo[playerid][pPlayerWeapon] =
	PlayerInfo[playerid][pPlayerAmmo] =
	PlayerInfo[playerid][pAmmoType] =
	PlayerInfo[playerid][pWepSerial] = 0;

	new slot = GiveInvItem(playerid, wep, ammo, ammotype, serial);
	if (slot == -1) return SendErrorMessage(playerid, "Faites une capture d'écran avec /timestamp et publiez-la sur le forum dans la section Bug Tracker !");

	switch(GetWeaponType(wep))
	{
		case WEAPON_TYPE_PRIMARY:
		{
			PlayerInfo[playerid][pLastPrim] = slot;
			SQL_SetInteger("users", "LastPrim", slot, PlayerInfo[playerid][pID]);
		}
		case WEAPON_TYPE_SECONDARY:
		{
			PlayerInfo[playerid][pLastSec] = slot;
			SQL_SetInteger("users", "LastSec", slot, PlayerInfo[playerid][pID]);
		}
		case WEAPON_TYPE_MELEE:
		{
			PlayerInfo[playerid][pLastMelee] = slot;
			SQL_SetInteger("users", "LastMelee", slot, PlayerInfo[playerid][pID]);
		}
	}

	SM2(playerid, "Vous avez rangé %s dans votre inventaire.", inventory_items[wep][itemName]);

	format(str, sizeof(str), "* %s range son arme.", GetNameWithMask(playerid));
	SetPlayerChatBubble(playerid, str, COLOR_PURPLE, 20.0, 4000);

	RemovePlayerAttachedObject(playerid, 9);
	CallRemoteFunction("LoadHolsters", "i", playerid);

	return 1;
}