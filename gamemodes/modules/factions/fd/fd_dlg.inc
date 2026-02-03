Dialog:FDArmoury(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::duty(playerid, "");

	if (!IsPlayerFactionMedic(playerid) && !IsPlayerFactionFire(playerid))
		return 1;
		
	if (!PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous devez d'abord commencer votre service.");
		
	if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))
		return SendErrorMessage(playerid, "Vous devez être dans les vestiaires ou dans un véhicule de service.");

	switch (listitem)
	{
		case 0: if (GiveInvItem(playerid, INV_RADIO, 1) == -1) 
			return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 1: if (!GivePlayerWeaponEx(playerid, 9, 1, GetDefaultAmmoType(9), PlayerInfo[playerid][pPlayerSerial]))
			return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 2: if (!GivePlayerWeaponEx(playerid, 42, 999, GetDefaultAmmoType(42), PlayerInfo[playerid][pPlayerSerial]))
			return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 3: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~g~Équipement attribué", 1500, 4);

	Dialog_Show(playerid, FDArmoury, DIALOG_STYLE_LIST, "Équipement de service", "• Radio\n• Tronçonneuse\n• Extincteur\n{FF6347}• Rendre tout l'équipement", "Choisir", "Retour");

	return 1;
}