ALTX:dropmoney("/dmoney");
CMD:dropmoney(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
        return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette commande pour le moment.");

    if (CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendErrorMessage(playerid, "Veuillez d'abord quitter le mode admin.");

    if (IsPlayerInAnyVehicle(playerid))
        return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");

    if (PlayerInfo[playerid][pHours] < 2)
        return SendErrorMessage(playerid, "Vous devez avoir joué au moins 2 heures pour utiliser cette fonction.");

    new amount;
    if (sscanf(params, "i", amount))
        return SendHelpMessage(playerid, "/dropmoney [montant]");

    if (amount < 1 || amount > 3000)
        return SendErrorMessage(playerid, "Montant incorrect. (min: $1, max: $3000)");

    if (amount > GetCash(playerid))
        return SendNotCash(playerid);

    if (ItemDrop_Drop(playerid, 9999, amount) == -1)
        return SendErrorMessage(playerid, "Nombre maximum d'objets jetés atteint.");

    SendAdmMessage("%s [ID: %i] a jeté $%i au sol.", GetNameEx(playerid), playerid, amount);
    GiveCash(playerid, -amount);

    SendSyntaxMessage(playerid, "Vous avez jeté $%i. Utilisez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", amount);
    return 1;
}


CMD:drop(playerid, params[])
{
    if (!GetPVarInt(playerid, #attach_item))  return 1;

    if (ItemDrop_Drop(playerid, pTemp[playerid][tempItem], pTemp[playerid][tempQ], pTemp[playerid][tempEx]) == -1)
        return SendErrorMessage(playerid, "Nombre maximum d'objets jetés atteint.");

    SendSyntaxMessage(playerid, "Vous avez jeté %s. Utilisez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", inventory_items[pTemp[playerid][tempItem]][itemName], pTemp[playerid][tempQ]);

    RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    SetPVarInt(playerid, #attach_item, 0);

    pTemp[playerid][tempItem] = 0;
    pTemp[playerid][tempEx] = 0;
    pTemp[playerid][tempQ] = 0;

    return 1;
}

ALTX:dropgun("/dgun");
CMD:dropgun(playerid, params[])
{
	if (PlayerInfo[playerid][pPlayerWeapon] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas d'arme en main.");

	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PlayerInfo[playerid][pJailed] || PlayerInfo[playerid][pAJailed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (CheckAdmin(playerid, ADMIN_LEVEL_1) || IsTester(playerid))
		return SendErrorMessage(playerid, "Quittez le mode administrateur avant d'utiliser cette commande.");

	if (PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Commande indisponible en service.");

	if (IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");

	new wep = CheckPlayerWeaponEx(playerid);
	if (!wep)
		return SendAdmMessage("Le joueur %s[ID:%i] est suspecté d'utiliser un cheat d'arme !", GetNameEx(playerid), playerid);

	if (wep >= 22 && wep <= 34)
	{
		new am = CheckAmmo(PlayerInfo[playerid][pAmmoType]);
		if (PlayerInfo[playerid][pPlayerAmmo] > am) PlayerInfo[playerid][pPlayerAmmo] = am;
	}

	if (ItemDrop_Drop(playerid, PlayerInfo[playerid][pPlayerWeapon], PlayerInfo[playerid][pPlayerAmmo], PlayerInfo[playerid][pAmmoType], PlayerInfo[playerid][pWepSerial]) == -1)
		return SendErrorMessage(playerid, "Le nombre maximum d'objets au sol a été atteint.");

	if (PlayerInfo[playerid][pPlayerAmmo])
	{
		SM2(playerid, "Vous avez jeté %s [%s / %i munitions]. Utilisez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName], inventory_items[PlayerInfo[playerid][pAmmoType]][itemName], PlayerInfo[playerid][pPlayerAmmo]);
		ResetPlayerWeapons(playerid);
	}
	else
	{
		SM2(playerid, "Vous avez jeté %s. Utilisez {ffc46a}/pitem{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.", inventory_items[PlayerInfo[playerid][pPlayerWeapon]][itemName]);
		RemovePlayerAttachedObject(playerid, 9);
	}

	PlayerInfo[playerid][pPlayerWeapon] = 0;
	PlayerInfo[playerid][pPlayerAmmo] = 0;
	PlayerInfo[playerid][pAmmoType] = 0;
	PlayerInfo[playerid][pWepSerial] = 0;

	return 1;
}


ALTX:pickupitem("/pitem");
CMD:pickupitem(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new dropid = -1;
	if ((dropid = ItemDrop_Nearest(playerid)) == -1)
		return 1;

	if (IsPlayerCrouching(playerid))
		ItemDrop_Pickup(playerid, dropid);
	else
		InfoMSG(playerid, "Vous devez ~y~vous accroupir~w~ pour ~y~ramasser~w~ un objet.", 3);

	return 1;
}