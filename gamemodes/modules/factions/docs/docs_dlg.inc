Dialog:DOCSGUN(playerid, response, listitem, inputtext[])
{
	if (!response)															return cmd::duty(playerid, "");
    if (!IsPlayerFactionDocs(playerid))										return 1;
	if (!PlayerInfo[playerid][pOnDuty])										return SendErrorMessage(playerid, "Vous devez être en service pour accéder à l'armurerie.");
    if (!IsPlayerAtUniform(playerid) && !IsPlayerAtSecondUniform(playerid))	return SendErrorMessage(playerid, "Vous devez être dans le vestiaire.");
	    	
	GameTextForPlayer(playerid, "~r~-", 1500, 4);

	switch(listitem)
	{
		case 0: if (GiveInvItem(playerid, INV_RADIO, 1) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 1: if (!GivePlayerWeaponEx(playerid, 3, 1, GetDefaultAmmoType(3), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventaire plein !");
		case 2: if (!GivePlayerWeaponEx(playerid, 41, 999, GetDefaultAmmoType(41), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventaire plein !");
		case 3: if (!GivePlayerWeaponEx(playerid, 24, 85, GetDefaultAmmoType(24), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventaire plein !");
		case 4: if (GiveInvItem(playerid, 85, CheckAmmo(85)) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 5: if (!GivePlayerWeaponEx(playerid, 29, 91, GetDefaultAmmoType(29), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventaire plein !");
		case 6: if (GiveInvItem(playerid, 91, CheckAmmo(91)) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 7: if (!GivePlayerWeaponEx(playerid, 25, 89, GetDefaultAmmoType(25), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventaire plein !");
		case 8: if (GiveInvItem(playerid, 86, CheckAmmo(86)) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 9: if (GiveInvItem(playerid, 89, CheckAmmo(89)) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 10: if (!GivePlayerWeaponEx(playerid, 31, 98, GetDefaultAmmoType(31), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventaire plein !");
		case 11: if (GiveInvItem(playerid, 98, CheckAmmo(98)) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 12: if (GiveInvItem(playerid, 99, CheckAmmo(99)) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 13: if (!GivePlayerWeaponEx(playerid, 34, 96, GetDefaultAmmoType(34), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventaire plein !");
		case 14: if (GiveInvItem(playerid, 96, CheckAmmo(96)) == -1) return SendErrorMessage(playerid, "Inventaire plein !");
		case 15: if (!GivePlayerWeaponEx(playerid, 17, 1, GetDefaultAmmoType(17), PlayerInfo[playerid][pPlayerSerial])) 	return SendErrorMessage(playerid, "Inventaire plein !");
		case 16: if (!GivePlayerWeaponEx(playerid, 43, 50, GetDefaultAmmoType(43), PlayerInfo[playerid][pPlayerSerial])) return SendErrorMessage(playerid, "Inventaire plein !");
		case 17: ResetPlayerWeaponsEx(playerid);
	}

	GameTextForPlayer(playerid, "~g~+", 1500, 4);

	Dialog_Show(playerid, DOCSGUN, DIALOG_STYLE_LIST, "Armurerie",
	"{FFFFFF}• Matraque\n\
	• Spray au poivre\n\
	• Desert Eagle (.50AE)\n\
	\t{ffc46a}- Prendre des munitions supplémentaires (.50AE)\n\
	{FFFFFF}• MP5 (9x19mm)\n\
	\t{ffc46a}- Prendre des munitions supplémentaires (9x19mm)\n\
	{FFFFFF}• Fusil à pompe (12-G BIRDSHOT)\n\
	\t{ffc46a}- Prendre des munitions supplémentaires (12-G BIRDSHOT)\n\
	\t{ffc46a}- Prendre des munitions non létales (12-G BEANBAG)\n\
	{FFFFFF}• M4 (5.56x45mm)\n\
	\t{ffc46a}- Prendre des munitions supplémentaires (5.56x45mm)\n\
	\t{ffc46a}- Prendre des munitions non létales (5.56x45mm en caoutchouc)\n\
	{FFFFFF}• Fusil sniper (7.62x51mm)\n\
	\t{ffc46a}- Prendre des munitions supplémentaires (7.62x51mm)\n\
	{FFFFFF}• Grenade lacrymogène\n\
	• Appareil photo\n\
	{FF6347}• Déposer toutes les armes", 
	"Prendre", "Fermer");

	return 1;
}