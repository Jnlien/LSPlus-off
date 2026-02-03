forward ShowWeaponLicenseDialog(playerid, list);
forward ShowWeaponLicensePlayer(playerid);

LicenseOffice_Nearest(playerid)
{
	if (!IsPlayerLogged(playerid))	return -1;

	for(new i = 0; i < MAX_BIZ; i++)
	{		
	    if (BizInfo[i][bizID] == 0 || BizInfo[i][bizType] != BIZ_LICENSE)	continue;
	    if (PlayerToPoint(85.0,playerid,BizInfo[i][bizPosX], BizInfo[i][bizPosY], BizInfo[i][bizPosZ]) && GetWorld(playerid) == BizInfo[i][bizPosVW] && GetInterior(playerid) == BizInfo[i][bizPosInt])
		return i;
	}
	return -1;
}

ShowLicenses(playerid, targetid)
{
	SendClientMessage(targetid, COLOR_GREEN, "__________CARTE D'IDENTITÉ__________");
	SendClientMessageEx(targetid, COLOR_WHITE, "Nom : %s, Date de naissance : %s", GetNameEx(playerid), PlayerInfo[playerid][pBirthdate]);
	SendClientMessageEx(targetid, COLOR_WHITE, "Permis de conduire : {AFAFAF}%s{FFFFFF}. {FF8C8C}Avertissements : (%i)", (PlayerInfo[playerid][pCarLic]) ? ("Oui") : ("Non"), PlayerInfo[playerid][pDriveWarns]);
	SendClientMessageEx(targetid, COLOR_WHITE, "Permis d'armes : {AFAFAF}%s{FFFFFF}. {FF8C8C}Avertissements : (%i)", (PlayerInfo[playerid][pWepLic]) ? ("Oui") : ("Non"), PlayerInfo[playerid][pWepWarns]);
	SendClientMessageEx(targetid, COLOR_WHITE, "Permis de vol : {AFAFAF}%s{FFFFFF}.", (PlayerInfo[playerid][pFlyLic]) ? ((PlayerInfo[playerid][pFlyLic] == 1) ? ("Oui") : ("Supprimé")) : ("Non"));
	SendClientMessageEx(targetid, COLOR_WHITE, "Licence de taxi : {AFAFAF}%s{FFFFFF}.", (PlayerInfo[playerid][pTaxiLic]) ? ((PlayerInfo[playerid][pTaxiLic] == 1) ? ("Oui") : ("Supprimée")) : ("Non"));
	SendClientMessage(targetid, COLOR_GREEN, "_____________________________________");
	return 1;
}

ShowWeaponLicense(playerid, list)
{
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `id`,`name` FROM `users` WHERE `weplic`='1' LIMIT %i,%i", (30 * list), (30 * (list + 1)));
	mysql_tquery(dbHandle, query, "ShowWeaponLicenseDialog", "ii", playerid, list);
	return 1;
}

public ShowWeaponLicenseDialog(playerid, list)
{
	if (!cache_num_rows())
	{
  		SendErrorMessage(playerid, "Aucune donnée disponible.");
  	 	return cmd::mdc(playerid, "");
	}

	new mes[(MAX_PLAYER_NAME + 1) * 30 + 7] = "<<\n>>", name[MAX_PLAYER_NAME + 1];

	for (new i; i != cache_num_rows(); i++)
	{
	    pTemp[playerid][pWeaponListed][i] = cache_get_field_content_int(i, "id");
	    cache_get_field_content(i, "name", name);
	    strreplace(name, '_', ' ');
		format(mes, sizeof(mes), "%s\n%s", mes, name);
	}

	SetPVarInt(playerid, #weapon_list, list);
	Dialog_Show(playerid, MDCWeapon, DIALOG_STYLE_LIST, "Historique des licences", mes, "Choisir", "Retour");
	return 1;
}

public ShowWeaponLicensePlayer(playerid)
{
	if (!cache_num_rows())
	{
	    SendErrorMessage(playerid, "Aucune donnée disponible.");
	    return cmd::mdc(playerid, "");
	}

	new name[MAX_PLAYER_NAME+1], officer[MAX_PLAYER_NAME+1], data[24], price, warns;

	cache_get_field_content(0, "name", name);
	strreplace(name, '_', ' ');
   	cache_get_field_content(0, "wep_officer", officer);
    cache_get_field_content(0, "wep_data", data);
    price = cache_get_field_content_int(0, "wep_price");
    warns = cache_get_field_content_int(0, "wepwarns");

	static const str_size[] = "{C3C3C3}- Nom complet : %s\n{C3C3C3}- Avertissements : %i\n{C3C3C3}- Officier : %s\n{C3C3C3}- Date d'émission : %s\n{C3C3C3}- Montant : $%i\n- Révoquer la licence\n- Émettre un avertissement";
	new mes[sizeof(str_size) - 10 + MAX_PLAYER_NAME + MAX_PLAYER_NAME + 24 + 12];
	format(mes, sizeof(mes), str_size, name, warns, officer, data, price);
	Dialog_Show(playerid, MDCWeaponPlayer, DIALOG_STYLE_LIST, "Informations", mes, "Choisir", "Retour");

	pTemp[playerid][pMDCWepPlayer] = cache_get_field_content_int(0, "id");
	format(pTemp[playerid][pMDCWepName], MAX_PLAYER_NAME + 1, name);

	return 1;
}