Dialog:CITYGUN(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::duty(playerid, "");

	if (!IsPlayerFactionCity(playerid))
		return 1;

	if (!PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous devez d'abord commencer votre service.");
		
	if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))
		return SendErrorMessage(playerid, "Vous devez être dans les vestiaires ou dans un véhicule de service.");

	GameTextForPlayer(playerid, "~r~-", 1500, 4);

	switch (listitem)
	{
		case 0: if (GiveInvItem(playerid, INV_RADIO, 1) == -1) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 1: if (!GivePlayerWeaponEx(playerid, 24, 85, GetDefaultAmmoType(24), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 2: if (GiveInvItem(playerid, 85, CheckAmmo(85)) == -1) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 3: if (!GivePlayerWeaponEx(playerid, 29, 91, GetDefaultAmmoType(29), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 4: if (GiveInvItem(playerid, 91, CheckAmmo(91)) == -1) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 5: if (!GivePlayerWeaponEx(playerid, 31, 98, GetDefaultAmmoType(31), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 6: if (GiveInvItem(playerid, 98, CheckAmmo(98)) == -1) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 7: if (GiveInvItem(playerid, 99, CheckAmmo(99)) == -1) return SendErrorMessage(playerid, "Votre inventaire est plein !");
		case 8: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~g~Équipement attribué", 1500, 4);

	Dialog_Show(playerid, CITYGUN, DIALOG_STYLE_LIST, "Équipement de Service", \
		"• Radio\n" \
		"• Desert Eagle (.50AE)\n\t{ffc46a}- Prendre munitions supplémentaires (.50AE)\n" \
		"{FFFFFF}• MP5 (9x19mm)\n\t{ffc46a}- Prendre munitions supplémentaires (9x19mm)\n" \
		"{FFFFFF}• M4 (5.56x45mm drum)\n\t{ffc46a}- Prendre munitions supplémentaires (5.56x45mm drum)\n" \
		"\t{ffc46a}- Prendre munitions non-létales (5.56x45mm rubber)\n" \
		"{FF6347}• Rendre tout l'armement", \
		"Choisir", "Retour");

	return 1;
}