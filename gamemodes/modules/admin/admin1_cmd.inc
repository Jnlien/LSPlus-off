ALTX:ah("/aideadmins", "/aa");
CMD:ah(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] == 0) 
		return SendNotAdmin(playerid);
	
	if (!IsAdminDuty(playerid)) 
		return SendNotDuty(playerid);

	if (CheckAdmin(playerid, ADMIN_LEVEL_1))
	{
		SendClientMessage(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/aduty /adminchat /aooc /annonce /setinterior(/setint) /setworld(/vw) /anomoff");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/kick /jail /unjail /warn /offban /ban /unban /banip /unbanip /mute /unmute /checkcars");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/clearchat /atp /tp /gethere /goto /freeze /unfreeze /slap /getip /getstats(/astats) /getproperty ");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/dv /gotocar /getcar /spec /specoff /disarm /togservice");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/aplant /dmlog /aproximite /removebox /check /areparerveh /supprimerdesc(/suppdesc)");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 1]: {C3C3C3}/supprimerame(/suppame) /healplayer(/aheal) /interdirearme /agraffiti(/agr) /gotopc(-ls/-fc/-d/-m)");
	}
	if (CheckAdmin(playerid, ADMIN_LEVEL_2))
	{
		SendClientMessage(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/up /policer /down /getincar /afire /amoteur /fillveh /fillallveh");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/setskin /sethp /setarmure /showpm /showpmoff /showpms /nopms /telecmds /aveh /mark");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/arendreveh /aretireramende /gotomark /settime /weather /weatherall");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 2]: {C3C3C3}/go /vw /goint /x /y /z /noooc ");
	}
	if (CheckAdmin(playerid, ADMIN_LEVEL_3))
	{
		SendClientMessage(playerid, COLOR_WHITE, "[Level 3]: {C3C3C3}/alisteitem /alock /adonneritem /adonnerarme /aprendrearme /arename /chopshop /cabine");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 3]: {C3C3C3}/afaction /avehicule /agarage /ahouse /abiz /aapart /aparkmeter(/aparkm) /adoor /abillboard(/abb)");
	}
	if (CheckAdmin(playerid, HIGH_ADMIN_LEVEL))
	{
		SendClientMessage(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/admin /makepremium /tester /modecombat /copstocrime /firetocrime");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/adonnerargent /ainvisible /setstat /astreet");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/aobjet /afood(stand) /alabel /as(peed)c(am) /resetcode");
		SendClientMessage(playerid, COLOR_WHITE, "[Level 4]: {C3C3C3}/aatm /atower /acctv /agate /ajob /gmx /reloadmap /streamer");
	}
	if (CheckAdmin(playerid, FOUNDER_LEVEL))
	{
		SendClientMessage(playerid, COLOR_WHITE, "[DEVELOPPEUR]: {C3C3C3}/payday /paydayall /aindustry /anticheat_settings");
	}
	return 1;
}

// File: gamemodes/commands/admin/aduty.pwn
// Fix: exige un code admin configuré avant toute activation et réinitialise la PVar à la sortie.
// Pourquoi: éviter le contournement quand "Admin:CodeInputted" est à 1 et qu'aucun code n'est configuré.

CMD:aduty(playerid, params[])
{
    if (PlayerInfo[playerid][pAdmin] < 1)
        return SendNotAdmin(playerid);

    if (IsAdminDuty(playerid))
    {
        for (new i = 0; i < 52; i++) EnableAntiCheatForPlayer(playerid, i, 1);

        SendAdmMessage("{C4ABA3}%s{B8828C} n'est plus en service admin.", GetAccName(playerid));
        SendClientMessage(playerid, COLOR_YELLOW, "Vous n'êtes plus en service admin.");

        SetPlayerHealth(playerid, pTemp[playerid][pInvulnerable]);
        pTemp[playerid][pInvulnerable] = 0.0;
        pTemp[playerid][pAdminDuty] = 0;
        SetPlayerColor(playerid, TEAM_HIT_COLOR);

        if (pTemp[playerid][pDMLog] == 1)
        {
            ClearKillFeed(playerid);
            pTemp[playerid][pDMLog] = 0;
        }

        DeletePVar(playerid, "Admin:CodeInputted");
        server_admins--;
        return 1;
    }
    else
    {
		if (!GetPVarInt(playerid, "Admin:CodeInputted"))
		{

			new acode[32];

			if (sscanf(params, "s[24]", acode))
				return SendHelpMessage(playerid, "/aduty [mot de passe]");

			if (PlayerInfo[playerid][pAdminCode][0] == '\0')
			return SendErrorMessage(playerid, "Vous devez définir un mot de passe administrateur avec /setcode.");

			new len = strlen(acode);
			if (len < 4 || len > 16)
				return SendErrorMessage(playerid, "Le mot de passe doit contenir entre 4 et 16 caractères.");

			if (!VerifyAdminCode(playerid, acode))
				return SendErrorMessage(playerid, "Mot de passe incorrect.");

			SetPVarInt(playerid, "Admin:CodeInputted", 1);
    	}

		pTemp[playerid][pAdminDuty] = 1;
		pTemp[playerid][pDMLog] = 1;
		SendAdmMessage("{C4ABA3}%s{B8828C} est maintenant en service admin.", GetAccName(playerid));
		SendClientMessage(playerid, COLOR_YELLOW, "Vous êtes maintenant en service admin.");

		pTemp[playerid][pInvulnerable] = GetPlayerHealthEx(playerid);
		SetPlayerHealth(playerid, Float:0x7F800000); // infini

		SetPlayerColor(playerid, TEAM_ADMIN_COLOR);
		server_admins++;

		for (new i = 0; i < 52; i++) EnableAntiCheatForPlayer(playerid, i, 1);
	}
    return 1;
}

CMD:setcode(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] < 1)
		return SendNotAdmin(playerid);

	new new_acode[32], old_acode[32];

	if (isempty(PlayerInfo[playerid][pAdminCode]))
	{
		if (sscanf(params, "s[24]", new_acode))
			return SendHelpMessage(playerid, "/setcode [mot de passe]");

		if (strlen(new_acode) < 4 || strlen(new_acode) > 16)
			return SendErrorMessage(playerid, "Le mot de passe doit contenir entre 4 et 16 caractères.");

		SetAdminCode(playerid, new_acode);
		return 1;
	}
	else
	{
		if (sscanf(params, "s[24]s[24]", old_acode, new_acode))
			return SendSyntaxMessage(playerid, "/setcode [ancien_mot_de_passe] [nouveau_mot_de_passe]");

		if (strlen(old_acode) < 4 || strlen(old_acode) > 16 || strlen(new_acode) < 4 || strlen(new_acode) > 16)
			return SendErrorMessage(playerid, "Chaque mot de passe doit contenir entre 4 et 16 caractères.");

		if (!VerifyAdminCode(playerid, old_acode))
			return SendErrorMessage(playerid, "L'ancien mot de passe est incorrect. Contactez un développeur.");

		SetAdminCode(playerid, new_acode);
	}
	return 1;
}


ALTX:adminchat("/a");
CMD:adminchat(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (isnull(params))
        return SendSyntaxMessage(playerid, "/(a)dminchat [texte]");

    new mes[2][256];

    if (strlen(params) > MAXLEN)
    {
		new pos = MAXLEN;
		if (pos < MAXLEN - 1) pos = MAXLEN;

        format(mes[0], sizeof(mes[]), "[Chat Admin] %s: {FFF6D1}%.*s ...", GetAccName(playerid), pos, params);
        format(mes[1], sizeof(mes[]), "{FFF6D1}... %s", params[pos]);
    }
    else
    {
        format(mes[0], sizeof(mes[]), "[Chat Admin] %s: {FFF6D1}%s", GetAccName(playerid), params);
        mes[1][0] = '\0';
    }

    foreach (new i : Player)
    {
        if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1)) continue;
        SendClientMessageEx(i, 0xFFEC9FFF, mes[0]);
        if (mes[1][0]) SendClientMessageEx(i, 0xFFEC9FFF, mes[1]);
    }

    return 1;
}

/*ALTX:an("/ans", "/answer");
CMD:an(playerid, params[])
{
	new userid, text[128];

	if (sscanf(params, "us[128]", userid, text))
		return SendSyntaxMessage(playerid, "/an [ID/nom_du_joueur] [réponse]");

	if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (userid == playerid)
		return SendErrorMessage(playerid, "Vous ne pouvez pas vous répondre à vous-même.");

	new mes[256];

	if (strlen(text) > MAXLEN)
	{
	    new pos = MAXLEN;
		if (pos < MAXLEN - 1) pos = MAXLEN;

		format(mes, sizeof(mes), "%s vous répond : %s", GetAccName(playerid), text);
		SendClientMessageEx(userid, 0x00CC99AA, "%.*s", pos, mes);
		SendClientMessageEx(userid, 0x00CC99AA, "... %s", mes[pos]);

		format(mes, sizeof(mes), "[RÉPONSE] : %s a répondu à %s [ID: %i] : %s", GetAccName(playerid), GetNameEx(userid), userid, text);
		StaffMSG(0x00CC99AA, "%.*s", pos, mes);
		StaffMSG(0x00CC99AA, "... %s", mes[pos]);
	}
	else
	{
	    format(mes, sizeof(mes), "%s vous répond : %s", GetAccName(playerid), text);
		SendClientMessage(userid, 0x00CC99AA, mes);

		StaffMSG(0x00CC99AA, "[RÉPONSE] : %s a répondu à %s [ID: %i] : %s", GetAccName(playerid), GetNameEx(userid), userid, text);
	}
	return 1;
}*/


ALTX:aooc("/ao");
CMD:aooc(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (isnull(params))
		return SendSyntaxMessage(playerid, "(/ao)oc [texte]");

	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		if (pos < MAXLEN - 1) pos = MAXLEN;

		format(mes, sizeof(mes), "[OOC] %s : %.*s ...", GetAccName(playerid), pos, params);
		SendClientMessageToAll(0xF97804FF, mes);

		format(mes, sizeof(mes), "... %s", params[pos]);
		SendClientMessageToAll(0xF97804FF, mes);
	}
	else
	{
		format(mes, sizeof(mes), "[OOC] %s : %s", GetAccName(playerid), params);
		SendClientMessageToAll(0xF97804FF, mes);
	}

	return 1;
}


CMD:annonce(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (isnull(params))
		return SendSyntaxMessage(playerid, "/annonce [texte]");

			
	new mes[128];

	if (strlen(params) > MAXLEN)
	{
		new pos = MAXLEN;
		if (pos < MAXLEN - 1) pos = MAXLEN;

		format(mes, sizeof(mes), "ADMIN : %.*s ...", pos, params);
		SendClientMessageToAll(COLOR_RED, mes);

		format(mes, sizeof(mes), "... %s", params[pos]);
		SendClientMessageToAll(COLOR_RED, mes);
	}
	else
	{
		format(mes, sizeof(mes), "ADMIN : %s",params);
		SendClientMessageToAll(COLOR_RED, mes);
	}

	return 1;
}


ALTX:setinterior("/setint");
CMD:setinterior(playerid, params[])
{
	new userid, int;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "ud", userid, int))
		return SendSyntaxMessage(playerid, "/setinterior [ID/nom_du_joueur] [ID_intérieur (0-255)]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (int < 0 || int > 255)
		return SendErrorMessage(playerid, "L'ID d'intérieur doit être compris entre 0 et 255.");

	SetInterior(userid, int);

	SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s(%i){B8828C} a modifié votre intérieur {C4ABA3}[ID: %i]{B8828C}.", GetAccName(playerid), playerid, int);
	SendAdmMessageToPlayer(playerid, "Vous avez modifié l'intérieur de {C4ABA3}%s(%i){B8828C} en {C4ABA3}[ID: %i]{B8828C}.", GetNameEx(userid), userid, int);

	return 1;
}


CMD:setworld(playerid, params[])
{
	new userid, wid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "ud", userid, wid))
		return SendSyntaxMessage(playerid, "/setworld [ID/nom_du_joueur] [ID monde virtuel (0-65535)]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (wid < 0 || wid > 65535)
		return SendErrorMessage(playerid, "L'ID du monde virtuel doit être compris entre 0 et 65535.");

	SetWorld(userid, wid);

	SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s(%i){B8828C} a modifié votre monde virtuel {C4ABA3}[ID: %i]{B8828C}.", GetAccName(playerid), playerid, wid);
	SendAdmMessageToPlayer(playerid, "Vous avez modifié le monde virtuel de {C4ABA3}%s(%i){B8828C} en {C4ABA3}[ID: %i]{B8828C}.", GetNameEx(userid), userid, wid);

	return 1;
}


CMD:getworld(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid))
		return SendSyntaxMessage(playerid, "/getworld [ID/nom_du_joueur]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	SendAdmMessageToPlayer(playerid, "Monde virtuel du joueur {C4ABA3}%s(%i){B8828C} : {C4ABA3}%i{B8828C}.", GetNameEx(userid), userid, GetWorld(userid));
	return 1;
}


CMD:anomoff(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	new mask_string[1024];
	mask_string[0] = EOS;

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || !maskOn{i}) continue;
		format(mask_string, sizeof(mask_string), "%s{FFFFFF}%s - %s [ID: %i]\n", mask_string, GetNameWithMask(i), GetNameEx(i), i);
	}

	if (mask_string[0] == EOS)
		format(mask_string, sizeof(mask_string), "{CCCCCC}Aucun joueur n'est actuellement masqué.");

	Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Joueurs masqués", mask_string, "OK", "");
	return 1;
}


CMD:gotols(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 1529.6, -1691.2, 13.3);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotopc(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 2269.4795,-81.2131,26.5414);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotom(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 1358.6038,251.7677,19.5669);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotod(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, 207.7609,-64.7452,1.5781);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:gotofc(playerid)
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))	return SendNotAdmin(playerid);
	
	SetPlayerPosEx(playerid, -204.1443,1119.1970,19.7422);
	SetInterior(playerid, 0);
	SetWorld(playerid, 0);

	return 1;
}

CMD:kick(playerid, params[])
{
	new userid, reason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "us[64]", userid, reason))
		return SendSyntaxMessage(playerid, "/kick [ID/nom_du_joueur] [raison]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	/*if (PlayerInfo[userid][pAdmin] < PlayerInfo[playerid][pAdmin])
	{*/
	new mes[128], query[512];

	format(mes, sizeof(mes), "(Admin) %s a été kické par %s. Raison : %s", GetNameEx(userid), GetAccName(playerid), reason);
	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	format(mes, sizeof(mes), "[/kick] %s a été kické par l'administrateur %s. Raison : %s", GetNameEx(userid), GetAccName(playerid), reason);
	insert_log(mes, ADMIN_LOG, 2, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

	mysql_format(dbHandle, query, sizeof query, "INSERT INTO `sanctions` (`accountid`,`sanction`,`admin`,`reason`) VALUES (%d,'Kick','%e','%e')", PlayerInfo[userid][pAccID], GetAccName(playerid), reason);
	mysql_tquery(dbHandle, query);

	KickEx(userid);
	//}

	return 1;
}


/*CMD:skick(playerid, params[])
{
	new userid, reason[128];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "us[128]", userid, reason))
		return SendSyntaxMessage(playerid, "/skick [ID/nom_du_joueur] [raison]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (PlayerInfo[userid][pAdmin] < PlayerInfo[playerid][pAdmin])
	{
		new mes[128];

		format(mes, sizeof(mes), "(Admin) %s a kické silencieusement %s.", GetAccName(playerid), GetNameEx(userid));
		SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

		format(mes, sizeof(mes), "[/skick] %s a été kické par l'administrateur %s. Raison : %s", GetNameEx(userid), GetAccName(playerid), reason);
		insert_log(mes, ADMIN_LOG, 2, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

		KickEx(userid);
	}

	return 1;
}*/


CMD:offjail(playerid, params[])
{
	new username[MAX_PLAYER_NAME + 1], reason[32], time;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[32]ds[32]", username, time, reason))
		return SendSyntaxMessage(playerid, "/offjail [nom_du_personnage] [durée_en_minutes (1-2880)] [raison]");

	if (!IsValidPlayerName(username))
		return SendErrorMessage(playerid, "Nom du personnage invalide.");

	if (time <= 0 || time > 2880)
		return SendErrorMessage(playerid, "La durée doit être comprise entre 1 et 2880 minutes.");

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || !CompareStrings(username, GetNameEx(i))) continue;
		SendErrorMessage(playerid, "Ce joueur est actuellement en ligne : %s [ID : %i]", GetNameEx(i), i);
		return 1;
	}

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `ajailed` = '1', `ajailtime` = '%i' WHERE `name` = '%e'", time * 60, username);
	mysql_tquery(dbHandle, query, "OnPlayerOfflineJail", "dsds", playerid, username, time, reason);

	return 1;
}

CMD:jail(playerid, params[])
{
	new userid, time, reason[128];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "uds[128]", userid, time, reason))
		return SendSyntaxMessage(playerid, "/jail [ID/nom_du_joueur] [durée_en_minutes (1-2880)] [raison]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (time <= 0 || time > 2880)
		return SendErrorMessage(playerid, "La durée doit être comprise entre 1 et 2880 minutes.");

	PlayerInfo[userid][pAJailed] = 1;
	PlayerInfo[userid][pAJailTime] = time * 60;

	SetInterior(userid, 18);
	SetWorld(userid, userid + 100);
	SetPlayerPosEx(userid, -219.4580, 1410.6747, 27.7734);
	SetSpawnInfoEx(userid, -219.4580, 1410.6747, 27.7734);
	ResetPlayerWeaponsEx(userid);

	new mes[256]/*, ames[256]*/;
	format(mes, sizeof(mes), "(Admin) {C4ABA3}%s{B8828C} vient d'être jail {C4ABA3}%d minute(s){B8828C} par {C4ABA3}%s{B8828C}. Raison: {C4ABA3}%s{B8828C}.", GetNameEx(userid), time, GetAccName(playerid), reason);
	SendClientMessageByAdmin(COLOR_REDONE, mes);

	new query[512];
	mysql_format(dbHandle, query, sizeof query, "INSERT INTO `sanctions` (`accountid`,`sanction`,`time`,`admin`,`reason`) VALUES (%d,'Jail',%d,'%e','%e')", PlayerInfo[userid][pAccID], time, GetAccName(playerid), reason);
	mysql_tquery(dbHandle, query);

	format(mes, sizeof(mes), "(/jail) %s vient d'être jail %d minute(s) par %s. Raison: %s.", GetNameEx(userid), time, GetAccName(playerid), reason);
	insert_log(mes, ADMIN_LOG, 5, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

	return 1;
}


CMD:unjail(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid))
		return SendSyntaxMessage(playerid, "/unjail [ID/nom_du_joueur]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (PlayerInfo[userid][pJailed] == 0 && PlayerInfo[userid][pAJailed] == 0)
		return SendErrorMessage(playerid, "Le joueur n'est pas en prison.");

	PlayerInfo[userid][pJailed] =
	PlayerInfo[userid][pIsolateTime] =
	PlayerInfo[userid][pJailTime] =
	PlayerInfo[userid][pAJailed] =
	PlayerInfo[userid][pAJailTime] = 0;

	if (IsValidDynamic3DTextLabel(PrisonLabel[userid]))
	{
		DestroyDynamic3DTextLabel(PrisonLabel[userid]);
		PrisonLabel[userid] = Text3D:INVALID_3DTEXT_ID;
	}

	SetInterior(userid, 0);
	SetWorld(userid, 0);
	SetPlayerPosEx(userid, 2230.5518, -109.0134, 26.6406);
	SetSpawnInfoEx(userid, 2230.5518, -109.0134, 26.6406, 3.9650);

	SendAdmMessageToPlayer(userid, "Vous avez été libéré de prison par {C4ABA3}%s{B8828C}.", GetAccName(playerid));
	SendAdmMessageToPlayer(playerid, "Vous avez libéré {C4ABA3}%s(%i){B8828C} de prison.", GetNameEx(userid), userid);

	return 1;
}


CMD:supprimerame(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid))
		return SendSyntaxMessage(playerid, "/supprimerame [playerid]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (!GetPVarInt(userid, "3DLabel"))
		return SendErrorMessage(playerid, "Ce joueur n'a pas de texte /action actif.");

	DestroyDynamic3DTextLabel(Text3D:GetPVarInt(userid, "3DLabel"));
	DeletePVar(userid, "3DLabel");

	SendAdmMessageToPlayer(userid, "Votre description de l’environnement (/action) a été supprimée par l’administrateur {C4ABA3}%s{B8828C}.", GetAccName(playerid));
	AMSG(COLOR_LIGHTRED, "(Admin) {C4ABA3}%s{B8828C} a supprimé la description /action du joueur {C4ABA3}%s(%i){B8828C}.", GetAccName(playerid), GetNameEx(userid), userid);

	return 1;
}


CMD:warn(playerid, params[])
{
	new userid, reason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "us[125]", userid, reason))
		return SendSyntaxMessage(playerid, "/warn [playerid] [raison]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (strlen(reason) > 63)
		return SendErrorMessage(playerid, "La raison ne doit pas dépasser 63 caractères.");

	new mes[256], query[256];

	PlayerInfo[userid][pWarns]++;
	SQL_SetInteger("users", "warn", PlayerInfo[userid][pWarns], PlayerInfo[userid][pID]);

	if (PlayerInfo[userid][pWarns] >= 3)
	{
		format(mes, sizeof(mes), "(Admin) {C4ABA3}%s{B8828C} a été banni par l’administrateur {C4ABA3}%s{B8828C}. Raison : {C4ABA3}%s{B8828C} (3 avertissements)", GetNameEx(userid), GetAccName(playerid), reason);
		SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

		format(query, sizeof(query), "[/ban] %s a été banni par %s. Raison : %s (3 avertissements)", GetAccName(userid), GetAccName(playerid), reason);
		insert_log(query, ADMIN_LOG, 1, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

		new ip[24];
		GetPlayerIp(userid, ip, sizeof(ip));
		mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (accountid, ip, name, admin, reason, date) VALUES (%i, '%e', '%e', '%e', '%e', '%e')", PlayerInfo[userid][pAccID], ip, GetAccName(userid), GetAccName(playerid), reason, GetFullDate());
		mysql_tquery(dbHandle, query);
		KickEx(userid);
		return 1;
	}

	SendAdmMessageToPlayer(userid, "Vous avez reçu un avertissement de {C4ABA3}%s{B8828C}. Raison : {C4ABA3}%s{B8828C}. [{C4ABA3}%i/3{B8828C}]", GetAccName(playerid), reason, PlayerInfo[userid][pWarns]);

	format(query, sizeof(query), "[/warn] %s a reçu un avertissement de %s. Raison : %s", GetAccName(userid), GetAccName(playerid), reason);
	insert_log(query, ADMIN_LOG, 4, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

	format(mes, sizeof(mes), "(Admin) {C4ABA3}%s{B8828C} a reçu un avertissement de {C4ABA3}%s{B8828C}. Raison : {C4ABA3}%s{B8828C} [{C4ABA3}%i/3{B8828C}]", GetNameEx(userid), GetAccName(playerid), reason, PlayerInfo[userid][pWarns]);
	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	return 1;
}


CMD:block(playerid, params[])
{
	new userid, reason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "us[64]", userid, reason))
		return SendSyntaxMessage(playerid, "/block [ID/nom_du_joueur] [raison]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (strlen(reason) > 63)
		return SendErrorMessage(playerid, "La raison ne doit pas dépasser 63 caractères.");

	new query[256];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `block_name`='%e', `block_reason`='%e', `status`='3' WHERE `id`='%i'", GetAccName(playerid), reason, PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query);

	new mes[128];
	format(mes, sizeof(mes), "(Admin) Le personnage %s a été bloqué par l’administrateur %s. Raison : %s.", GetNameEx(userid), GetAccName(playerid), reason);
	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	KickEx(userid);

	return 1;
}


CMD:offblock(playerid, params[])
{
	new name[MAX_PLAYER_NAME], reason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[24]s[64]", name, reason))
		return SendSyntaxMessage(playerid, "/offblock [nom_du_personnage] [raison]");

	if (!IsValidPlayerName(name))
		return SendErrorMessage(playerid, "Nom de personnage invalide.");

	if (strlen(reason) > 63)
		return SendErrorMessage(playerid, "La raison ne doit pas dépasser 63 caractères.");

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || !CompareStrings(GetNameEx(i), name)) continue;
		SendErrorMessage(playerid, "Ce personnage est en ligne : %s [ID : %i]", GetNameEx(i), i);
		return 1;
	}

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", name);
	mysql_tquery(dbHandle, query, "OfflineBlockPlayer", "iss", playerid, name, reason);
	return 1;
}


CMD:unblock(playerid, params[])
{
	new name[MAX_PLAYER_NAME];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[24]", name))
		return SendSyntaxMessage(playerid, "/unblock [nom_du_personnage]");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `name` = '%e'", name);
	mysql_tquery(dbHandle, query, "UnBlockPlayer", "is", playerid, name);
	return 1;
}


CMD:offban(playerid, params[])
{
	new name[MAX_PLAYER_NAME], reason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[24]s[64]", name, reason))
		return SendSyntaxMessage(playerid, "/offban [nom_utilisateur] [raison]");

	if (!IsValidPlayerName(name))
		return SendErrorMessage(playerid, "Nom d'utilisateur invalide.");

	if (strlen(reason) > 63)
		return SendErrorMessage(playerid, "La raison ne doit pas dépasser 63 caractères.");

	foreach (new i : Player)
	{
	    if (!IsPlayerLogged(i) || !CompareStrings(GetAccName(i), name)) continue;
		SendErrorMessage(playerid, "L'utilisateur \"%s\" est connecté - personnage %s [ID : %i]", name, GetNameEx(i), i);
		return 1;
	}

	new query[156];
	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `id` FROM `bans` WHERE `name` = '%e') AS banid FROM `accounts` WHERE `login` = '%e'", name, name);
	mysql_tquery(dbHandle, query, "OfflineBanPlayer", "is[24]s[64]", playerid, name, reason);
	return 1;
}


CMD:ban(playerid, params[])
{
	new 
		userid, 
		reason[64],
		mes[128],
		ames[256],
		ip[24],
		query[512];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "us[64]", userid, reason))
		return SendSyntaxMessage(playerid, "/ban [ID/Nom_du_joueur] [raison]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (strlen(reason) > 63)
		return SendErrorMessage(playerid, "La raison ne doit pas dépasser 63 caractères.");

	format(mes, sizeof(mes), "(Admin) {C4ABA3}%s (%s){B8828C} vient d'être banni du serveur. Raison: {C4ABA3}%s.", GetNameEx(userid), GetAccName(userid), reason);
	SendClientMessageByAdmin(COLOR_REDONE, mes);

	format(ames, sizeof(ames), "(Logs) {C4ABA3}%s (%s){B8828C} vient d'être banni du serveur par {C4ABA3}%s{B8828C}. Raison: {C4ABA3}%s.", GetNameEx(userid), GetAccName(userid), GetAccName(playerid), reason);
	AMSG(COLOR_REDONE, ames);

	GetPlayerIp(userid, ip, sizeof(ip));

	mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (accountid, ip, name, admin, reason, date) VALUES (%i, '%e', '%e', '%e', '%e', '%e')", PlayerInfo[userid][pAccID], ip, GetAccName(userid), GetAccName(playerid), reason, GetFullDate());
	mysql_tquery(dbHandle, query);
	
	mysql_format(dbHandle, query, sizeof query, "INSERT INTO `sanctions` (`accountid`,`sanction`,`admin`,`reason`) VALUES (%d,'Ban','%e','%e')", PlayerInfo[userid][pAccID], GetAccName(playerid), reason);
	mysql_tquery(dbHandle, query);

	format(query, sizeof(query), "(/ban) %s a ete banni par %s. Raison: %s.", GetAccName(userid), GetAccName(playerid), reason);
	insert_log(query, ADMIN_LOG, 1, PlayerInfo[userid][pAccID], PlayerInfo[playerid][pAccID]);

	KickEx(userid);
	return 1;
}


CMD:unban(playerid, params[])
{
	new name[MAX_PLAYER_NAME];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[24]", name))
		return SendSyntaxMessage(playerid, "/unban [nom_utilisateur]");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `bans` WHERE `name` = '%e'", name);
	mysql_tquery(dbHandle, query, "UnBanPlayer", "is", playerid, name);
	return 1;
}


CMD:banip(playerid, params[])
{
	new ip[24], reason[64], userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[24]s[64]", ip, reason))
		return SendSyntaxMessage(playerid, "/banip [IP] [raison]");

	if (!IsAnIP(ip))
		return SendErrorMessage(playerid, "Format IP invalide.");

	if (strlen(reason) > 63)
		return SendErrorMessage(playerid, "La raison ne peut pas dépasser 63 caractères.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `bans` WHERE `ip` = '%e'", ip);
	mysql_tquery(dbHandle, query, "BanAnIP", "iss", playerid, ip, reason);

	mysql_format(dbHandle, query, sizeof query, "INSERT INTO `sanctions` (`accountid`,`sanction`,`admin`,`reason`) VALUES (%d,'Ban','%e','%e')", PlayerInfo[userid][pAccID], GetAccName(playerid), reason);
	mysql_tquery(dbHandle, query);
	return 1;
}


CMD:unbanip(playerid, params[])
{
	new ip[24];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[24]", ip))
		return SendSyntaxMessage(playerid, "/unbanip [IP]");

	if (!IsAnIP(ip))
		return SendErrorMessage(playerid, "Format IP invalide.");

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `bans` WHERE `ip` = '%e'", ip);
	mysql_tquery(dbHandle, query, "UnBanAnIP", "is", playerid, ip);

	return 1;
}


CMD:mute(playerid, params[])
{
	new userid, time, mreason[64];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params,"uds[128]", userid, time, mreason))
		return SendSyntaxMessage(playerid, "/mute [ID/nom du joueur] [durée en minute (1-1440)] [raison]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (time < 1 || time > 1440)
		return SendErrorMessage(playerid, "La durée doit être comprise entre 1 et 1440 minutes.");

	if (strlen(mreason) > 63)
		return SendErrorMessage(playerid, "La raison ne doit pas dépasser 63 caractères.");

	PlayerInfo[userid][pMutedTime] = time * 60;

	new mes[128];
	format(mes, sizeof(mes), "(Admin) %s a été mute par %s pour %i minute(s). Raison: %s", GetNameEx(userid), GetAccName(playerid), time, mreason);
	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	new query[512];
	mysql_format(dbHandle, query, sizeof query, "INSERT INTO `sanctions` (`accountid`,`sanction`,`time`,`admin`,`reason`) VALUES (%d,'Mute',%d,'%e','%e')", PlayerInfo[userid][pAccID], time, GetAccName(playerid), mreason);
	mysql_tquery(dbHandle, query);

	return 1;
}


CMD:unmute(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid))
		return SendSyntaxMessage(playerid, "/unmute [ID/nom du joueur]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (PlayerInfo[userid][pMutedTime] == 0)
		return SendErrorMessage(playerid, "Le joueur n'est pas mute.");

	PlayerInfo[userid][pMutedTime] = 0;

	new mes[128];
	format(mes, sizeof(mes), "(Admin) %s a été unmute par l'administrateur %s.", GetNameEx(userid), GetAccName(playerid));
	SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	return 1;
}


ALTX:clearchat("/cc");
CMD:clearchat(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	ClearChatboxForAll(50);

	SendClientMessageToAll(COLOR_REDONE, ">> {C4ABA3}Le chat vient d'être nettoyé.");
	return 1;
}


/*CMD:test(playerid, params[])
{
	ResetAdminCode(playerid);
	return 1;
}*/


CMD:test(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid))
		return SendSyntaxMessage(playerid, "/test [ID/nom du joueur]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	SetPlayerMarkerForPlayer(playerid, userid, 0xA52A2AFF);
	return 1;
}

CMD:test2(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerLogged(i)) continue;
        SetPlayerMarkerForPlayer(playerid, i, 0xA52A2AFF);
    }
    return 1;
}


CMD:tp(playerid, params[])
{
	new telestring[3072], part[40];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid); 

	telestring[0] = '\0'; 

	for (new OrT = 0; OrT < sizeof(OrTeleports); OrT++)
	{
		format(part, sizeof(part), "%s\n", OrTeleports[OrT][tele_name]);
		strcat(telestring, part);
	}

	Dialog_Show(playerid, Teles, DIALOG_STYLE_LIST, "{FFFFFF}Choisissez un lieu de téléportation", telestring, "Suivant", "Quitter");
	return 1;
}


CMD:gethere(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))				
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid))							
		return SendSyntaxMessage(playerid, "/gethere [ID/nom du joueur]");

	if (!IsPlayerLogged(userid))								
		return SendNotConnected(playerid);

	if (GetPlayerState(userid) == PLAYER_STATE_SPECTATING)	
		return SendErrorMessage(playerid, "Le joueur est en mode spectateur.");

	SendPlayerToPlayer(userid, playerid);

	SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s{B8828C} vous a téléporté à lui.", GetAccName(playerid));
	return 1;
}


CMD:goto(playerid, params[])
{
    new targetid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", targetid))
        return SendSyntaxMessage(playerid, "/goto [ID/Nom du joueur]");

    if (!IsPlayerLogged(targetid))
        return SendNotConnected(playerid);

    if (GetPlayerState(targetid) == PLAYER_STATE_SPECTATING)
        return SendErrorMessage(playerid, "Le joueur est en mode spectateur.");

    SendPlayerToPlayer(playerid, targetid);
    return 1;
}


CMD:freeze(playerid, params[])
{
    new targetid;

    if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", targetid))
        return SendSyntaxMessage(playerid, "/freeze [playerid]");

    if (!IsPlayerLogged(targetid))
        return SendNotConnected(playerid);

	SendAdmMessageToPlayer(targetid, "Vous avez été freeze par l'administrateur {C4ABA3}%s(%i){B8828C}.", GetAccName(playerid), playerid);
	SendAdmMessageToPlayer(playerid, "Vous avez freeze {C4ABA3}%s(%i){B8828C}.", GetNameEx(targetid), targetid);
	FreezePlayer(targetid);

    return 1;
}

CMD:unfreeze(playerid, params[])
{
    new targetid;

    if (!IsTester(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", targetid))
        return SendSyntaxMessage(playerid, "/unfreeze [playerid]");

    if (!IsPlayerLogged(targetid))
        return SendNotConnected(playerid);

	SendAdmMessageToPlayer(targetid, "Vous avez été unfreeze par l'administrateur {C4ABA3}%s(%i){B8828C}.", GetAccName(playerid), playerid);
	SendAdmMessageToPlayer(playerid, "Vous avez unfreeze {C4ABA3}%s(%i){B8828C}.", GetNameEx(targetid), targetid);
	UnFreezePlayer(targetid);

    return 1;
}


CMD:slap(playerid, params[])
{
    new targetid, Float:x, Float:y, Float:z;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", targetid))
        return SendSyntaxMessage(playerid, "/slap [ID/Nom du joueur]");

    if (!IsPlayerLogged(targetid))
        return SendNotConnected(playerid);

    AntiCheatGetPos(targetid, x, y, z);
    SetPlayerPosEx(targetid, x, y, z + 10, 0);
    PlayerPlaySound(targetid, 1130, x, y, z + 10);

    SendAdmMessageToPlayer(targetid, "{C4ABA3}%s(%i){B8828C} vous a propulsé en l'air (slap).", GetAccName(playerid), playerid);
    SendAdmMessageToPlayer(playerid, "Vous avez propulsé (slap) {C4ABA3}%s(%i){B8828C}.", GetNameEx(targetid), targetid);
    return 1;
}


CMD:getip(playerid, params[])
{
    new targetid, ip[24];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", targetid))
        return SendSyntaxMessage(playerid, "/getip [ID/Nom du joueur]");

    if (!IsPlayerLogged(targetid))
        return SendNotConnected(playerid);

    GetPlayerIp(targetid, ip, sizeof(ip));
    SendAdmMessageToPlayer(playerid, "Adresse IP de {C4ABA3}%s{B8828C} : {C4ABA3}%s{B8828C}", GetNameEx(targetid), ip);
    return 1;
}

ALTX:getstats("/astats");
CMD:getstats(playerid, params[])
{
    new targetid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", targetid))
        return SendSyntaxMessage(playerid, "/getstats [ID/Nom du joueur]");

    if (!IsPlayerLogged(targetid))
        return SendNotConnected(playerid);

    ShowStats(playerid, targetid);
    return 1;
}


CMD:getproperty(playerid, params[])
{
    new targetid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", targetid))
        return SendSyntaxMessage(playerid, "/getproperty [ID/Nom du joueur]");

    if (!IsPlayerLogged(targetid))
        return SendNotConnected(playerid);

    ShowProperty(playerid, targetid);
    return 1;
}


ALTX:healplayer("/aheal");
CMD:healplayer(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid)) 
		return SendHelpMessage(playerid, "/healplayer [ID/nom du joueur]");

	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);

	if (pTemp[userid][pInvulnerable] != 0.0) 
		return SendErrorMessage(playerid, "Veuillez réessayer plus tard !");

	SetPlayerHealth(userid, 100.0);
	ResetDamages(userid);
	StopPlayerAnims(userid);
	UnFreezePlayer(userid);
	SetPlayerSkill(playerid);

	SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s{B8828C} vous a soigné.", GetAccName(playerid));
	SendAdmMessageToPlayer(playerid, "Vous avez soigné {C4ABA3}%s{B8828C}.", GetNameEx(userid));

	return 1;
}


CMD:gotocar(playerid, params[])
{
	new vehicle, Float:x, Float:y, Float:z;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 
		return SendNotAdmin(playerid);

	if (sscanf(params, "d", vehicle)) 
		return SendSyntaxMessage(playerid, "/gotocar [ID du véhicule]");

	if (!IsValidVehicle(vehicle)) 
		return SendErrorMessage(playerid, "Véhicule introuvable.");

	GetVehiclePos(vehicle, x, y, z);
	SetPlayerPosEx(playerid, x + 1, y + 1, z);
	SetWorld(playerid, GetVehicleVirtualWorldEx(vehicle));
	SetInterior(playerid, GetVehicleInteriorEx(vehicle));

	return 1;
}


CMD:getcar(playerid, params[])
{
	new vehicle, Float:x, Float:y, Float:z;
	new interior = GetInterior(playerid);
	new vw = GetWorld(playerid);

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 
		return SendNotAdmin(playerid);

	if (sscanf(params, "d", vehicle)) 
		return SendSyntaxMessage(playerid, "/getcar [ID du véhicule]");

	if (!IsValidVehicle(vehicle)) 
		return SendErrorMessage(playerid, "Véhicule introuvable.");

	if (IsPlayerInVehicle(playerid, vehicle)) 
		return SendErrorMessage(playerid, "Vous êtes déjà dans ce véhicule.");

	GetXYInFrontOfPlayer(playerid, x, y, z, 5.0);

	foreach (new i : Player)
	{
		if (!IsPlayerLogged(i) || !IsPlayerInVehicle(i, vehicle))
			continue;

		SetPlayerPosEx(i, x, y, z);
		SetInterior(i, interior);
		SetWorld(i, vw);
	}

	SetVehiclePos(vehicle, x, y, z);
	SetVehicleVirtualWorld(vehicle, vw);
	LinkVehicleToInterior(vehicle, interior);

	return 1;
}

ALTX:fix("/areparerveh");
CMD:fix(playerid, params[])
{
    new vehicle;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "d", vehicle) == 0)
    {
        if (!IsValidVehicle(vehicle)) 
            return SendErrorMessage(playerid, "Véhicule introuvable.");

        FixVehicle(vehicle);
        return SendServerMessage(playerid, "Le véhicule ID %d a été réparé.", vehicle);
    }
    else
    {
        vehicle = GetPlayerVehicleID(playerid);

        if (vehicle == 0 || !IsValidVehicle(vehicle))
            return SendSyntaxMessage(playerid, "/areparerveh [vehicleid]");

        FixVehicle(vehicle);
        return SendServerMessage(playerid, "Votre véhicule a été réparé.");
    }
}


ALTX:spectate("/spec");
CMD:spectate(playerid, params[])
{
	new userid;

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 
		return SendNotAdmin(playerid);

	if (sscanf(params, "u", userid)) 
		return SendSyntaxMessage(playerid, "(/spec)tate [ID/nom du joueur]");

	if (userid == playerid) 
		return SendErrorMessage(playerid, "Impossible de se spec soi-même.");

	if (!IsPlayerLogged(userid)) 
		return SendNotConnected(playerid);

	if (pTemp[playerid][pSpecPlayer] == INVALID_PLAYER_ID) 
		SavePlayerPos(playerid);

	pTemp[playerid][pSpecPlayer] = userid;
	TogglePlayerSpectating(playerid, 1);

	if (IsPlayerInAnyVehicle(userid)) 
		PlayerSpectateVehicle(playerid, GetPlayerVehicleID(userid));
	else 
		PlayerSpectatePlayer(playerid, userid);

	SetInterior(playerid, GetInterior(userid));
	SetWorld(playerid, GetWorld(userid));

	return 1;
}


CMD:specoff(playerid, params[])
{
	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 
		return SendNotAdmin(playerid);

	if (pTemp[playerid][pSpecPlayer] == INVALID_PLAYER_ID) 
		return SendErrorMessage(playerid, "Vous n'êtes pas en mode observation (/spec).");

	TogglePlayerSpectating(playerid, 0);

	return 1;
}


CMD:aplant(playerid, params[])
{
	new option[16], plant_string[16];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[16]S(-1)[127]", option, plant_string))
	{
		SendSyntaxMessage(playerid, "/aplant [info / remove]");
		return 1;
	}
	
	if (CompareStrings(option, "info"))
	{
		new plant = -1;

		if ((plant = Plant_Nearest(playerid)) == -1) 
			return SendErrorMessage(playerid, "Aucune plante trouvée à proximité.");

		new message[1024];

		format(message, sizeof(message),
			"ID de la plante\t%d\nPlantée par\t%s\nDate de plantation\t%s\n",
			plant, CropsInfo[plant][pPlanter], CropsInfo[plant][pPlantDate]);

		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST, "{FFFFFF}Informations sur la plante", message, "Fermer", "");	
	}
	else if (CompareStrings(option, "remove"))
	{
		new plant_id = -1;

		if (sscanf(plant_string, "d", plant_id)) 
			return SendSyntaxMessage(playerid, "/plant remove [ID de la plante]");

		if (plant_id < 0 || plant_id > MAX_PLANTS-1 || !CropsInfo[plant_id][pPlanted]) 
			return SendErrorMessage(playerid, "Aucune plante trouvée avec cet ID.");

		DestroyCrop(plant_id);
		SendAdmMessageToPlayer(playerid, "Vous avez supprimé la plante [ID : {C4ABA3}%d{B8828C}].", plant_id);
	}
	
	return 1;
}


/*CMD:aprison(playerid, params[])
{
	new userid, time, reason[128];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 			return SendNotAdmin(playerid);
	if (sscanf(params,"uds[128]", userid, time, reason))	return SendSyntaxMessage(playerid, "/aprison [ID/èìÿ èãðîêà] [âðåìÿ â ìèíóòàõ] [ïðè÷èíà]");
	if (!IsPlayerLogged(userid))							return SendNotConnected(playerid);
	if (time < 1 || time > 99999)						return SendErrorMessage(playerid, "Âðåìÿ íå ìîæåò áûòü ìåíåå 1 è íå áîëåå 99999 ìèíóò.");

	new mes[128];
	format(mes, sizeof(mes), "(Admin) %s áûë ïîñàæåí â òþðüìó àäìèíèñòðàòîðîì %s íà %s ìèíóò, ïðè÷èíà: %s", GetNameEx(userid), GetAccName(playerid), FormatNumber(time), reason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

	PlayerInfo[userid][pPrisoned] = random(sizeof(prison_sadoc));
	PlayerInfo[userid][pJailed] = PRISON_JAIL;
	PlayerInfo[userid][pJailTime] = time*60;

	SendSyntaxMessage(userid, "Âû áûëè ïîñàæåíû â òþððüìó àäìèíèñòðàòîðîì %s íà %s ìèíóò. Ïðè÷èíà: %s", GetAccName(playerid), FormatNumber(time), reason);
	SendSyntaxMessage(userid, "Âàø íîìåð çàêëþ÷¸ííîãî - A%i (/myprisonnumber)", PlayerInfo[playerid][pPrisoned]+100);

	SpawnPlayer(userid);
	return 1;
}*/

CMD:check(playerid, params[])
{
	new option[10], secoption[5];

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
		return SendNotAdmin(playerid);

	if (sscanf(params, "s[10]S()[5]", option, secoption))
	{
		SendHelpMessage(playerid, "/check [inv / gb / coffre / prop] [ID]");
		return 1;
	}

	if (CompareStrings(option, "inv"))
	{
		new userid;
		if (sscanf(secoption, "u", userid))
			return SendHelpMessage(playerid, "/check inv [ID/nom du joueur]");
		if (!IsPlayerLogged(userid))
			return SendNotConnected(playerid);

		PrintInvEx(userid, playerid);
	}
	else if (CompareStrings(option, "coffre"))
	{
		new vehid;
		if (sscanf(secoption, "i", vehid))
			return SendHelpMessage(playerid, "/check coffre [ID du véhicule]");
		if (vehid < 1 || vehid >= MAX_VEHICLES)
			return SendErrorMessage(playerid, "ID du véhicule invalide !");

		PrintVehInv(playerid, vehid, 1);
	}
	else if (CompareStrings(option, "gb"))
	{
		new vehid;
		if (sscanf(secoption, "i", vehid))
			return SendHelpMessage(playerid, "/check gb [ID du véhicule]");
		if (vehid < 1 || vehid >= MAX_VEHICLES)
			return SendErrorMessage(playerid, "ID du véhicule invalide !");

		PrintGBInv(playerid, vehid, 1);
	}
	else if (CompareStrings(option, "prop"))
	{
		new houseid;
		if (sscanf(secoption, "i", houseid))
			return SendHelpMessage(playerid, "/check prop [ID de la maison]");
		if (houseid < 1 || houseid >= MAX_HOUSES)
			return SendErrorMessage(playerid, "ID de la maison invalide !");
		if (!HouseInfo[houseid][hOwnerID])
			return SendErrorMessage(playerid, "Cette maison n'a pas de propriétaire !");

		PrintHouseInv(playerid, houseid, 1);
	}
	else
	{
		SendHelpMessage(playerid, "/check [paramètre] [ID]");
		SendSyntaxMessage(playerid, "inv / gb / coffre / prop");
	}

	return 1;
}

/*
CMD:checkprop(playerid, params[])
{
	new house, mes[MAX_HOUSE_INVENTORY * 32] = "{ffffff}¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?\t{ffffff}¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?";

	if (!CheckAdmin(playerid, ADMIN_LEVEL_1))							return SendNotAdmin(playerid);
	if (sscanf(params,"d",house))										return SendHelpMessage(playerid, "/checkprop [ID ¯õ?¯õ?¯õ?¯õ?]");
    if (house < 0 || house > MAX_HOUSES || 0 == HouseInfo[house][hID])	return SendErrorMessage(playerid, "¯õ?¯õ?¯õ? ¯õ?¯õ? ¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?.");

	for(new i = 0; i < MAX_HOUSE_INVENTORY; i++)
	{
		if (HouseInfo[house][pInvItem][i] == 0)	format(mes, sizeof(mes), "%s\n{C3C3C3}¯õ?¯õ?¯õ?¯õ?¯õ?¯õ? ¯õ?¯õ?¯õ?¯õ?\t", mes);
		else								format(mes, sizeof(mes), "%s\n%s\t%i", mes, inventory_items[HouseInfo[house][pInvItem][i]][itemName], HouseInfo[house][hInvAmount][i]);
	}

	new housename[MAX_PLAYER_NAME + 24];
	format(housename, sizeof(housename), "{F5DEB3}¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ? ¯õ?¯õ?¯õ?¯õ?: %s", HouseInfo[house][hOwner]);
	Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, housename, mes, "¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?", "");
	return 1;
}
	

CMD:checkcar(playerid, params[])
{
	new title[56], vehicle, mes[MAX_TRUNK_INVENTORY * 32] = "{ffffff}¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?\t{ffffff}¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?";

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) 																									return SendNotAdmin(playerid);
	if (sscanf(params,"d",vehicle))																												return SendHelpMessage(playerid, "/checkcar [id ¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?]");
    if (!IsValidVehicle(vehicle))																												return SendErrorMessage(playerid, "¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ? ¯õ?¯õ? ¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?.");
	if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))	return SendErrorMessage(playerid, "¯õ? ¯õ?¯õ?¯õ?¯õ?¯õ? ¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ? ¯õ?¯õ?¯õ? ¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?.");

	for(new i; i != MAX_TRUNK_INVENTORY; i++)
	{
	    if (VehicleInfo[vehicle][i][pInvItem] == 0)	format(mes, sizeof(mes), "%s\n{C3C3C3}¯õ?¯õ?¯õ?¯õ?¯õ?¯õ? ¯õ?¯õ?¯õ?¯õ?\t", mes);
		else										format(mes, sizeof(mes), "%s\n%s\t%i", mes, inventory_items[VehicleInfo[vehicle][i][pInvItem]][itemName], VehicleInfo[vehicle][carInvAmount][i]);
	}

	format(title, sizeof(title), "{F5DEB3}%s | ¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?: %s", VehicleNames[GetVehicleModel(vehicle)-400], VehicleInfo[vehicle][carOwner]);
    Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, title, mes, "¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?¯õ?", "");
	return 1;
}*/


CMD:dmlog(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_1)) return SendNotAdmin(playerid);

    if (pTemp[playerid][pDMLog] == 0)
    {
        SendAdmMessageToPlayer(playerid, "Vous avez {C4ABA3}activé{B8828C} le journal DM.");
        pTemp[playerid][pDMLog] = 1;
    }
    else
    {
        SendAdmMessageToPlayer(playerid, "Vous avez {C4ABA3}désactivé{B8828C} le journal DM.");
        ClearKillFeed(playerid);
        pTemp[playerid][pDMLog] = 0;
    }
    return 1;
}

ALTX:removevehicle("/removecar", "/dv");
CMD:removevehicle(playerid, params[])
{
    new vehid = GetPlayerVehicleID(playerid);

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (!IsValidVehicle(vehid) && sscanf(params, "d", vehid))
        return SendSyntaxMessage(playerid, "/dv [ID du véhicule]");

    if (!IsValidVehicle(vehid))
        return SendErrorMessage(playerid, "Véhicule introuvable.");

    if (VehicleInfo[vehid][carOwnerID] != 0)
        return SendErrorMessage(playerid, "Ce véhicule ne peut pas être supprimé.");

    if (VehicleInfo[vehid][carAdmin] == 0)
    {
        if (!CheckAdmin(playerid, ADMIN_LEVEL_3))
            return SendErrorMessage(playerid, "Ce véhicule ne peut pas être supprimé.");

        new query[128];
        mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `cars` WHERE `id` = %i", VehicleInfo[vehid][carID]);
        mysql_tquery(dbHandle, query);
    }

    DestroyVehicle(vehid);
    return 1;
}

ALTX:removebox("/rbox");
CMD:removebox(playerid, params[])
{
    new id = -1;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    for (new i; i != MAX_CARGO_OBJ; i++)
    {
        if (CargoObject[i][oOn] == 0 ||
            !IsPlayerInRangeOfPoint(playerid, 2.0, CargoObject[i][oX], CargoObject[i][oY], CargoObject[i][oZ]) ||
            GetWorld(playerid) != CargoObject[i][oVW] ||
            CargoObject[i][oInt] != GetInterior(playerid))
            continue;

        id = i;
        break;
    }

    if (id == -1)
        return SendErrorMessage(playerid, "Aucune boîte à proximité pouvant être supprimée.");

    CargoObject[id][oOn] = 0;

    if (IsValidDynamicObject(CargoObject[id][oObj]))
        DestroyDynamicObject(CargoObject[id][oObj]);

    if (IsValidDynamic3DTextLabel(CargoObject[id][oLabel]))
    {
        DestroyDynamic3DTextLabel(CargoObject[id][oLabel]);
        CargoObject[id][oLabel] = Text3D:INVALID_3DTEXT_ID;
    }

    SendAdmMessageToPlayer(playerid, "La boîte [{C4ABA3}ID: %i{B8828C}] a été supprimée.", id);
    return 1;
}

ALTX:near("/aproximite");
CMD:near(playerid, params[])
{
    new id = -1;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if ((id = Plant_Nearest(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une plante [ID: %i].", id);

    if ((id = Gate_Nearest(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un portail [ID: %i | Modèle: %i].", id, Gates[id][gateModel]);

    if ((id = GetNearHouse(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une maison [ID: %i].", id);

    if ((id = ATM_Nearest(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un distributeur automatique [ID: %i].", id);

    if ((id = GetNearGarage(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un garage [ID: %i].", id);

    if ((id = GetNearApartment(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un complexe résidentiel [ID: %i].", id);

    if ((id = GetNearBiz(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une entreprise [ID: %i].", id);

    if ((id = TruckCargo_Nearest(playerid, 2.0)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une entreprise de transport [ID: %i].", id);

    if ((id = Object_Nearest(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un objet [ID: %i | Modèle: %i].", id, ObjectsInfo[id][oModel]);

    if ((id = Job_Nearest(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un travail [ID: %i].", id);

    if ((id = GetNearestCamera(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une caméra [ID: %i].", id);

    if ((id = Payphone_Nearest(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une cabine téléphonique [ID: %i].", id);

    if ((id = ParkMeter_Nearest(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un parcmètre [ID: %i].", id);

    if ((id = GetStreetID(playerid)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un panneau de rue [ID: %i].", id);

    if ((id = GetClosestBillBoard(playerid, 9.0)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'un panneau publicitaire [ID: %i].", id);

    if ((id = Trash_Nearest(playerid, 9.0)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une poubelle [ID: %i].", id);

    if ((id = Factory_Nearest(playerid, 9.0)) != -1)
        SendSyntaxMessage(playerid, "Vous êtes près d'une usine de recyclage [ID: %i].", id);

    for (new i; i != MAX_ENTRANCE; i++)
    {
        if (EntranceInfo[i][entranceOn] == 0)
            continue;

        if (IsPlayerEntranceOutside(playerid, i))
            SendSyntaxMessage(playerid, "Vous êtes près d'une entrée [ID: %i].", i);

        if (IsPlayerEntranceInside(playerid, i))
            SendSyntaxMessage(playerid, "Vous êtes près d'une sortie [ID: %i].", i);
    }

    for (new i = 0; i != MAX_LABELS; i++)
    {
        if (LabelsInfo[i][tLabelOn] != 1 ||
            !PlayerToPoint(3, playerid, LabelsInfo[i][tPosX], LabelsInfo[i][tPosY], LabelsInfo[i][tPosZ]) ||
            LabelsInfo[i][tVW] != GetWorld(playerid) ||
            LabelsInfo[i][tInt] != GetInterior(playerid))
            continue;

        SendSyntaxMessage(playerid, "Vous êtes près d'une étiquette [ID: %i].", i);
    }

    return 1;
}

CMD:disarm(playerid, params[])
{
    new userid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", userid))
        return SendSyntaxMessage(playerid, "/disarm [ID/Nom du joueur]");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    ResetPlayerWeaponsEx(userid);
    SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s{B8828C} vous a désarmé.", GetAccName(playerid));
    SendAdmMessageToPlayer(playerid, "Vous avez désarmé {C4ABA3}%s{B8828C}.", GetNameEx(userid));

    return 1;
}

ALTX:teles("/atp");
CMD:teles(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    new id;
    if (sscanf(params, "i", id))
        pTemp[playerid][pDialogList] = 0;
    else
        pTemp[playerid][pDialogList] = id;

    new string[570],
        min_tp = pTemp[playerid][pDialogList] * MAX_ADMIN_TELES,
        max_tp = (pTemp[playerid][pDialogList] + 1) * MAX_ADMIN_TELES;

    if (pTemp[playerid][pDialogList])
        string = "{bccd32}Page précédente{FFFFFF}\n";

    for (new i = min_tp; i < max_tp; i++)
    {
        if (!AdminTeles[i][telesOn])
            continue;

        format(string, sizeof(string), "%s<%i>{66FF66}%s\n", string, i, AdminTeles[i][telesMapName]);
    }

    if (AdminTeles[max_tp][telesOn])
        format(string, sizeof(string), "%s{bccd32}Page suivante", string);

    Dialog_Show(playerid, AdminTeles, DIALOG_STYLE_LIST, "{FFFFFF}Téléportation", string, "Suivant", "Annuler");

    return 1;
}

CMD:checkcars(playerid, params[])
{
    new userid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", userid))
        return SendSyntaxMessage(playerid, "/checkcars [ID/Nom du joueur]");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `owner` = '%i'", PlayerInfo[userid][pID]);
    mysql_tquery(dbHandle, query, "ShowCarsForPlayer", "ii", playerid, userid);

    return 1;
}

CMD:interdirearme(playerid, params[])
{
    new userid, hr;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "ui", userid, hr))
        return SendSyntaxMessage(playerid, "/interdirearme [playerid] [Durée en heures (0-10)]");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    if (hr < 0 || hr > 10)
        return SendSyntaxMessage(playerid, "/interdirearme [playerid] [Durée en heures (0-10)]");

    if (hr == 0)
    {
        if (!PlayerInfo[userid][pForbid])
            return SendErrorMessage(playerid, "Le joueur n'a aucune interdiction d'utiliser des armes.");

        PlayerInfo[userid][pForbid] = 0;
        SQL_SetInteger("users", "Forbid", 0, PlayerInfo[userid][pID]);

        SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s{B8828C} a levé votre interdiction d'utiliser des armes.", GetAccName(playerid));
        SendAdmMessageToPlayer(playerid, "Vous avez levé l'interdiction d'utiliser des armes pour {C4ABA3}%s{B8828C}.", GetNameEx(userid));
    }
    else
    {
        PlayerInfo[userid][pForbid] = hr;
        SQL_SetInteger("users", "Forbid", hr, PlayerInfo[userid][pID]);

        SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s{B8828C} vous a interdit d'utiliser des armes pour {C4ABA3}%i minutes (%i heures){B8828C}.", GetAccName(playerid), hr * 60, hr);
        SendAdmMessageToPlayer(playerid, "Vous avez interdit à {C4ABA3}%s{B8828C} d'utiliser des armes pour {C4ABA3}%i minutes (%i heures){B8828C}.", GetNameEx(userid), hr * 60, hr);
    }

    return 1;
}


ALTX:supprimerdesc("/suppdesc");
CMD:supprimerdesc(playerid, params[])
{
    new userid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "i", userid))
        return SendHelpSyntaxMessage(playerid, "/supprimerdesc [ID/Nom du joueur]");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    format(PlayerInfo[userid][pDescribe], PAME_MAX_LEN, "NULL");
    format(PlayerInfo[userid][pDescribe2], PAME_MAX_LEN, "NULL");

    SQL_SetString("users", "Desc1", "NULL", PlayerInfo[userid][pID]);
    SQL_SetString("users", "Desc2", "NULL", PlayerInfo[userid][pID]);

    SendAdmMessageToPlayer(playerid, "Vous avez supprimé la description du personnage {C4ABA3}%s{B8828C}.", GetNameEx(userid));
    SendAdmMessageToPlayer(userid, "L'administrateur {C4ABA3}%s{B8828C} a supprimé la description de votre personnage !", GetAccName(playerid));

    return 1;
}


CMD:voirsanction(playerid, params[])
{
    new userid, accountid = PlayerInfo[userid][pAccID];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", userid))
        return SendSyntaxMessage(playerid, "/voirsanction [ID/Nom du joueur]");

	if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

	if (accountid <= 0)
		return SendErrorMessage(playerid, "Ce joueur n'est pas lié à un compte valide.");

	ShowSanctionsDialog(playerid, accountid);
    return 1;
}

CMD:voirperso(playerid, params[])
{
    new userid, accountid = PlayerInfo[userid][pAccID];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", userid))
        return SendSyntaxMessage(playerid, "/voirperso [ID/Nom du joueur]");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    if (accountid <= 0)
        return SendErrorMessage(playerid, "Ce joueur n'est pas lié à un compte valide.");

    ShowPersonnageDialog(playerid, accountid);
    return 1;
}

/*ALTX:deleteadvertisment("/delad");
CMD:deleteadvertisment(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_1))
        return SendNotAdmin(playerid);

    new ad_id;

    if (sscanf(params, "i", ad_id))
        return SendHelpMessage(playerid, "/deleteadvertisment [ID de la publicité]");

    if (!advert_data[ad_id][advert_exists])
        return SendErrorMessage(playerid, "Aucune publicité trouvée avec cet ID !");

    SendAdmMessageToPlayer(playerid, "Vous avez supprimé la publicité - {C4ABA3}%s{B8828C} [{C4ABA3}ID:%i{B8828C}].", GetNameEx(advert_data[ad_id][advert_placeby_id]), ad_id);
    SendAdmMessageToPlayer(advert_data[ad_id][advert_placeby_id], "L'administrateur {C4ABA3}%s{B8828C} a supprimé votre publicité.", GetAccName(playerid));

    ClearAd(ad_id);
    return 1;
}*/