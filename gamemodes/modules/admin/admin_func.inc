this::OfflineBlockPlayer(playerid, account[], reason[])
{
    if (cache_num_rows() == 0) 
        return SendErrorMessage(playerid, "Le personnage \"%s\" est introuvable dans la base de données.", account);

    new status = cache_get_field_content_int(0, "status");

    if (status == 3) 
        return SendErrorMessage(playerid, "Le personnage \"%s\" est déjà bloqué.", account);
    if (status != 1) 
        return SendErrorMessage(playerid, "Le personnage \"%s\" doit être activé.", account);

    new query[128];
    mysql_format(dbHandle, query, sizeof(query),  "UPDATE `users` SET `block_name`='%e', `block_reason`='%e', `status`='3' WHERE `name`='%e'",  GetAccName(playerid), reason, account);
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "(Admin) Le personnage \"%s\" (hors ligne) a été bloqué par l'administrateur \"%s\". Raison : \"%s\"", account, GetAccName(playerid), reason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, query);
    return 1;
}


this::UnBlockPlayer(playerid, account[])
{
    if (cache_num_rows() == 0) 
        return SendErrorMessage(playerid, "Le personnage \"%s\" est introuvable dans la base de données.", account);

    if (cache_get_field_content_int(0, "status") != 3) 
        return SendErrorMessage(playerid, "Le personnage \"%s\" n'est pas bloqué.", account);

    SendAdmMessage("Le personnage \"%s\" a été débloqué par l'administrateur \"%s\".", account, GetAccName(playerid));

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `status` = '1' WHERE `name` = '%e'", account);
    mysql_tquery(dbHandle, query);
    return 1;
}


this::OfflineBanPlayer(playerid, account[], time, reason[])
{
    if (cache_num_rows() == 0) 
        return SendErrorMessage(playerid, "Le joueur \"%s\" est introuvable dans la base de données.", account);

    if (cache_get_field_content_int(0, "banid") != 0) 
        return SendErrorMessage(playerid, "Le joueur \"%s\" est déjà banni.", account);

    new admin = cache_get_field_content_int(0, "admin");
    new accountid = cache_get_field_content_int(0, "accountid");

    if (admin > PlayerInfo[playerid][pAdmin]) 
        return SendAdmMessage("%s [ID: %i] a tenté de bannir \"%s\" (hors ligne). Raison : %s", GetAccName(playerid), playerid, account, reason);

    new query[256];
    mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (accountid, name, admin, reason, date) VALUES (%i,'%e','%s','%e','%s')", accountid, account, GetAccName(playerid), reason, GetFullDate());
    mysql_tquery(dbHandle, query);

    format(query, sizeof(query), "[/offban] %s a été banni pour %i jour(s) par l'administrateur %s. Raison : %s", account, time, GetAccName(playerid), reason);
    insert_log(query, ADMIN_LOG, 1, accountid, PlayerInfo[playerid][pAccID]);

    format(query, sizeof(query), "(Admin) %s (hors ligne) a été banni pour %i jour(s) par l'administrateur %s. Raison : %s", account, time, GetAccName(playerid), reason);
    SendClientMessageByAdmin(COLOR_LIGHTRED, query);
    return 1;
}

this::UnBanPlayer(playerid, account[])
{
	if (!cache_num_rows())	
		return SendErrorMessage(playerid, "Le joueur \"%s\" n'est pas banni.", account);

	SendAdmMessage("%s a été débanni(e) par l'administrateur %s.", account, GetAccName(playerid));
	
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `bans` WHERE `name` = '%e'", account);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "SELECT accountid FROM `bans` WHERE `name` = '%e' LIMIT 1", account);
	new Cache:cache = mysql_query(dbHandle, query);
	new accountid = cache_get_field_content_int(0, "accountid", dbHandle);
	cache_delete(cache);

	format(query, sizeof(query), "[/unban] %s a été débanni(e) par l'administrateur %s", account, GetAccName(playerid));
	insert_log(query, ADMIN_LOG, 12, accountid, PlayerInfo[playerid][pAccID]);
	return 1;
}

this::UnBanAnIP(playerid, ip[])
{
	if (!cache_num_rows())	
		return SendErrorMessage(playerid, "L'adresse IP \"%s\" n'est pas banni.", ip);

	SendAdmMessage("%s a débanni(e) l'adresse IP %s.", GetAccName(playerid), ip);

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT accountid FROM `bans` WHERE `ip` = '%e' LIMIT 1", ip);
	new Cache:cache = mysql_query(dbHandle, query);
	new accountid = cache_get_field_content_int(0, "accountid", dbHandle);
	cache_delete(cache);

	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `bans` WHERE `ip` = '%e'", ip);
	mysql_tquery(dbHandle, query);

	new mes[128];
	format(mes, sizeof(mes), "[/unbanip] IP : %s a été débanni(e) par l'administrateur %s", ip, GetAccName(playerid));
	insert_log(mes, ADMIN_LOG, 12, accountid, PlayerInfo[playerid][pAccID]);
	return 1;
}


this::OnChangeName(playerid, userid, newname[])
{
	if (cache_num_rows())	
		return SendErrorMessage(playerid, "Le nom de personnage \"%s\" est déjà utilisé.", newname);

	foreach (new i : Player)
	{
	    if (!IsPlayerLogged(i) || !CompareStrings(GetName(i), newname)) continue;
		SendErrorMessage(playerid, "Le nom de personnage \"%s\" est déjà utilisé.", newname);
		return 1;
	}

	if (!PlayerInfo[userid][pNameChanges]) 
		return SendErrorMessage(playerid, "Ce joueur n’a pas de changement de nom disponible.");

	PlayerInfo[userid][pNameChanges]--;
	SQL_SetString("accounts", "name_changes", PlayerInfo[userid][pNameChanges], PlayerInfo[userid][pAccID]);

	new oldname[MAX_PLAYER_NAME];
	GetPlayerName(userid, oldname, MAX_PLAYER_NAME);

	ChangeName(userid, newname);

	SendAdmMessage("%s a validé le changement de nom de %s en %s.", GetAccName(playerid), oldname, newname);
	SendSyntaxMessage(userid, "Votre nom a été changé de %s en %s par l’administrateur %s.", oldname, newname, GetAccName(playerid));
	return 1;
}

this::OnAdminChangeName(playerid, userid, newname[])
{
	if (cache_num_rows())	
		return SendErrorMessage(playerid, "Le nom de personnage \"%s\" est déjà utilisé.", newname);
	    
	foreach (new i : Player)
	{
	    if (!IsPlayerLogged(i) || !CompareStrings(GetName(i), newname)) continue;
		SendErrorMessage(playerid, "Le nom de personnage \"%s\" est déjà utilisé.", newname);
		return 1;
	}
	
	new oldname[MAX_PLAYER_NAME];
	GetPlayerName(userid, oldname, MAX_PLAYER_NAME);
	
	ChangeName(userid, newname);

	SendAdmMessage("%s a changé le nom du joueur %s en %s.", GetAccName(playerid), oldname, newname);
	SendSyntaxMessage(userid, "Votre nom a été changé de %s en %s par l’administrateur %s.", oldname, newname, GetAccName(playerid));
	return 1;
}

ChangeName(userid, newname[])
{
    new oldname[MAX_PLAYER_NAME];
	GetPlayerName(userid, oldname, MAX_PLAYER_NAME);

	format(PlayerInfo[userid][pUsername], MAX_PLAYER_NAME+1, newname);
    format(PlayerInfo[userid][pUsername_], MAX_PLAYER_NAME+1, newname);
    strreplace(PlayerInfo[userid][pUsername_], '_', ' ');

    if (!maskOn{userid}) 
	{
	    SetPlayerName(userid, newname);
    	format(PlayerInfo[userid][pMaskname], 32, PlayerInfo[userid][pUsername_]);
	}
	
	for(new i; i != MAX_HOUSES; i++)
	{
	    if (!HouseInfo[i][hID] || HouseInfo[i][hOwnerID] != PlayerInfo[userid][pID]) continue;

		format(HouseInfo[i][hOwner], MAX_PLAYER_NAME, GetNameEx(userid));
	}

	for(new i; i != MAX_BIZ; i++)
	{
	    if (!BizInfo[i][bizID] || BizInfo[i][bizOwnerID] != PlayerInfo[userid][pID]) continue;

		format(BizInfo[i][bizOwner], MAX_PLAYER_NAME, GetName(userid));
	}
	
	foreach(new i:Vehicle)
	{
		if (VehicleInfo[i][carOwnerID] != PlayerInfo[userid][pID]) continue;

		format(VehicleInfo[i][carOwner], MAX_PLAYER_NAME, GetNameEx(userid));
	}
	
	SQL_SetString("users", "name", newname, PlayerInfo[userid][pID]);
	
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `tickets` WHERE `userid`=%i", PlayerInfo[userid][pID]);
	mysql_tquery(dbHandle, query);
	
	DeletePlayerTickets(userid);
	return 1;
}

this::AdminSetPlate(playerid, vehicleid, plate[])
{
	if (cache_num_rows())	
		return SendErrorMessage(playerid, "Cette plaque est déjà utilisée par un autre véhicule.");

	format(VehicleInfo[vehicleid][carPlate], 24, "%s", plate);
	SetVehicleNumberPlate(vehicleid, VehicleInfo[vehicleid][carPlate]);
	RespawnVehicle(vehicleid, 1);
	Save_Car(vehicleid);

	SendSyntaxMessage(playerid, "Vous avez modifié la plaque du véhicule %s. Nouvelle plaque : %s.", VehicleNames[GetVehicleModel(vehicleid) - 400], plate);
	
	SAMALL("%s a modifié la plaque du véhicule %s (ID %i) en : %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(vehicleid) - 400], vehicleid, plate);
	
	new mes[128];
	format(mes, sizeof(mes), "[/asetplate] L'administrateur %s a modifié la plaque du véhicule %s en : %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(vehicleid) - 400], plate);
	insert_log(mes, ADMIN_LOG, 6, 0, PlayerInfo[playerid][pAccID]);
	return 1;
}

this::OnPlayerOfflineJail(playerid, name[32], time, reason[32])
{
	new rows, fields;
	cache_get_data(rows, fields, dbHandle);

	if (cache_affected_rows(dbHandle))
	{
		new prefix[24] = "support";
		if (PlayerInfo[playerid][pAdmin] > 0) prefix = "admin";

		new query[128];
		mysql_format(dbHandle, query, sizeof(query), "SELECT accountid FROM `bans` WHERE `name` = '%e' LIMIT 1", name);
		
		new Cache:cache = mysql_query(dbHandle, query);
		new accountid = cache_get_field_content_int(0, "accountid", dbHandle);
		cache_delete(cache);

		new mes[128];
		format(mes, sizeof(mes), "(Admin) %s a été envoyé en prison administrative %s %s pour %i minute(s), raison : %s", name, prefix, GetAccName(playerid), time, reason);
		SendClientMessageByAdmin(COLOR_LIGHTRED, mes);

		format(mes, sizeof(mes), "[/offjail] %s a été envoyé en prison administrative %s %s pour %i minute(s), raison : %s", name, prefix, GetAccName(playerid), time, reason);
		insert_log(mes, ADMIN_LOG, 51, accountid, PlayerInfo[playerid][pAccID]);
	}
	else
	{
		SendErrorMessage(playerid, "Impossible d'envoyer %s en jail.", name);
	}
	return 1;
}

this::BanAnIP(playerid, ip[], reason[])
{
	if (cache_num_rows()) 
		return SendErrorMessage(playerid, "L'adresse IP \"%s\" est déjà banni.", ip);

	SendAdmMessage("%s a banni l'adresse IP : %s.", GetAccName(playerid), ip);

	new pIP[24];
	foreach (new i : Player)
	{
		GetPlayerIp(i, pIP, sizeof(pIP));

		if (!IsPlayerLogged(i) || !CompareStrings(ip, pIP)) continue;
		SendErrorMessage(i, "Votre IP %s a été banni sur ce serveur.", ip);
		KickEx(i);
	}

	new query[256];
	mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `bans` (admin, reason, date, ip) VALUES ('%s', '%e', '%s', '%e')", GetAccName(playerid), reason, GetFullDate(), ip);
	mysql_tquery(dbHandle, query);
	return 1;
}

ClearKillFeed(playerid = INVALID_PLAYER_ID)
{
	if ((playerid != INVALID_PLAYER_ID) && (0 == IsPlayerConnected(playerid))) return 0;

	goto L_start;
	{
		new dummy[16/(cellbits/charbits)];
		#emit const.pri dummy
	}

	#if __Pawn < 0x030A
		SendDeathMessage(0, 0, 0),
		SendDeathMessageToPlayer(0, 0, 0, 0);
	#endif

	L_start:

	const SDM_MAGIC_ID = INVALID_PLAYER_ID-1;
	new i = 5;

	#emit    push.c    SDM_MAGIC_ID
	#emit    push.c    SDM_MAGIC_ID

	if (playerid == INVALID_PLAYER_ID)
	{
		#emit    push.c    12
		do
		{
			#emit    sysreq.c    SendDeathMessage
		}
		while(--i != 0);

		#emit    stack    12
	}
	else
	{
		#emit    push.s    playerid
		#emit    push.c    16
		do
		{	
			#emit    sysreq.c    SendDeathMessageToPlayer
		}
		while(--i != 0);

		#emit    stack    16
	}
	return 1;
}

SendClientMessageByAdmin(color, string[])
{
    foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || (PlayerInfo[i][pSettings] & togAdmCmds)) continue;

		SendClientMessage(i, color, string);
	}
	return 1;
}


AMSG(color, const str[], {Float,_}:...)
{
    static args, start, end, string[144];
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 8)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 8); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit ADD.C 4
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1)) continue;

			SendClientMessage(i, color, string);
		}
		return 1;
	}

	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || !CheckAdmin(i, ADMIN_LEVEL_1)) continue;

		SendClientMessage(i, color, str);
	}
	return 1;
}

StaffMSG(color, const str[], {Float,_}:...)
{
    static args, start, end, string[144];

	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 8)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 8); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}

		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit ADD.C 4
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i))) continue;
		    SendClientMessage(i, color, string);
		}
		return 1;
	}
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || (!CheckAdmin(i, ADMIN_LEVEL_1) && !IsTester(i))) continue;
	    SendClientMessage(i, color, str);
	}
	return 1;
}

CheckAdmin(playerid, level)
{
	return PlayerInfo[playerid][pAdmin] >= level && IsAdminDuty(playerid) ? 1 : 0;
}

IsTester(playerid)
{
	return PlayerInfo[playerid][pHelper] >= 1 && IsTesterDuty(playerid) ? 1 : 0;
}

IsAdminDuty(playerid)
{
	return pTemp[playerid][pAdminDuty] == 1 ? 1 : 0;
}

IsTesterDuty(playerid)
{
	return pTemp[playerid][pTesterDuty] == 1 ? 1 : 0;
}

SendStaffMessage(color, string[])
{
	foreach(new i : Player)
	{
	    if (!IsPlayerLogged(i) || (!IsTester(i) && !CheckAdmin(i, ADMIN_LEVEL_1)) || (PlayerInfo[i][pSettings] & togTester)) continue;
        SendClientMessage(i, color, string);
	}
	return 1;
}

SetAdminCode(playerid, adm_code[])
{
    strmid(pTemp[playerid][pPasswordAttempt], adm_code, 0, MAX_HASH_LENGTH);
    bcrypt_hash(playerid, "OnAdminCodeHashed", adm_code, 10);
    return 1;
}

this::OnAdminCodeHashed(playerid, hash[])
{
    new pass_hash[MAX_HASH_LENGTH];
    bcrypt_get_hash(pass_hash);

    SQL_SetString("accounts", "admin_code", pass_hash, PlayerInfo[playerid][pAccID]);

    strcopy(PlayerInfo[playerid][pAdminCode], pass_hash);

    SendAdmMessageToPlayer(playerid, "Votre mot de passe pour activer le mode administrateur a été modifié. Nouveau mot de passe : {C4ABA3}%s{B8828C}.", pTemp[playerid][pPasswordAttempt]);
    SendHelpMessage(playerid, "Notez-le ou mémorisez-le bien !");
    return 1;
}

ResetAdminCode(playerid)
{
    DeletePVar(playerid, "Admin:CodeInputted");
    PlayerInfo[playerid][pAdminCode][0] = '\0';

    SQL_SetString("accounts", "admin_code", "", PlayerInfo[playerid][pAccID]);

    SendSyntaxMessage(playerid, "Votre mot de passe pour activer le mode administrateur a été réinitialisé.");
    return 1;
}

VerifyAdminCode(playerid, adm_code[])
{
    strmid(pTemp[playerid][pPasswordAttempt], adm_code, 0, MAX_HASH_LENGTH);
    bcrypt_verify(playerid, "OnAdminCodeVerified", adm_code, PlayerInfo[playerid][pAdminCode]);
    return 1;
}

this::OnAdminCodeVerified(playerid, bool:success)
{
    if (success) { SendErrorMessage(playerid, "Mot de passe incorrect."); return 1; }
    return 1;
}

stock GetAdminRank(playerid)
{
    new rank[32];

    switch (PlayerInfo[playerid][pAdmin])
    {
        case SUPPORT_LEVEL:      rank = "Support";
        case MODERATOR_LEVEL:    rank = "Modérateur";
        case ADMIN_LEVEL_1:      rank = "Admin 1";
        case ADMIN_LEVEL_2:      rank = "Admin 2";
        case ADMIN_LEVEL_3:      rank = "Admin 3";
        case HIGH_ADMIN_LEVEL:   rank = "Admin Général";
        case FOUNDER_LEVEL:      rank = "Fondateur";
        
        default:                 rank = "Admin";
    }

    return rank;
}

stock ShortenForColumn(str[], limit)
{
	new len = strlen(str);
	if (len > limit)
	{
		if (limit < 3) { str[limit] = '?'; return 1; }
		str[limit - 3] = '.';
		str[limit - 2] = '.';
		str[limit - 1] = '.';
		str[limit] = '?';
	}
	return 1;
}

stock ShowSanctionsDialog(playerid, accountid)
{
	new query[256];
	mysql_format(dbHandle, query, sizeof query, "SELECT `sanction`, `admin`, `reason`, DATE_FORMAT(`date`, '%%d/%%m %%H:%%i') AS `occurred_at` FROM `sanctions` WHERE `accountid` = %d ORDER BY `date` DESC LIMIT %d", accountid, SANCTIONS_LIMIT);
	mysql_tquery(dbHandle, query, "OnSanctionsLoaded", "ii", playerid, accountid);
	return 1;
}

this::OnSanctionsLoaded(playerid, accountid)
{
    new rows = cache_num_rows();

    new title[64];
    format(title, sizeof title, "Historique des sanctions (%d)", accountid);

    gSanctionDialog[playerid][0] = '\0';
    format(gSanctionDialog[playerid], sizeof gSanctionDialog[], "Type\tAdmin\tRaison\tDate");

    if (!rows)
    {
        format(gSanctionDialog[playerid], sizeof gSanctionDialog[], "%s\n-\t-\tAucune sanction\t-", gSanctionDialog[playerid]);
        ShowPlayerDialog(playerid, DIALOG_SANCTIONS, DIALOG_STYLE_TABLIST_HEADERS, title, gSanctionDialog[playerid], "OK", "Fermer");
        return 1;
    }

    new typ[16], admin[64], reason[128], occurred[20];
    new line[256], color_type[16];

    for (new i = 0; i < rows; i++)
    {
        cache_get_field_content(i, "sanction", typ, sizeof typ);
        cache_get_field_content(i, "admin", admin, sizeof admin);
        cache_get_field_content(i, "reason", reason, sizeof reason);
        cache_get_field_content(i, "occurred_at", occurred, sizeof occurred);

		if (!strcmp(typ, "Jail", true))    		format(color_type, sizeof color_type, "{E3E04B}"); 
		else if (!strcmp(typ, "Kick", true))    format(color_type, sizeof color_type, "{FF9900}");
        else if (!strcmp(typ, "Ban", true))     format(color_type, sizeof color_type, "{F81414}");
        else                                    format(color_type, sizeof color_type, "{FFFFFF}");

        ShortenForColumn(admin,  COLUMN_ADMIN_MAX);
        ShortenForColumn(reason, COLUMN_REASON_MAX);

        format(line, sizeof line, "%s%s{FFFFFF}\t%s\t%s\t%s", color_type, typ, admin, reason, occurred);

        new used = strlen(gSanctionDialog[playerid]);
        if (used + 1 + strlen(line) >= sizeof gSanctionDialog[])
        {
            format(gSanctionDialog[playerid], sizeof gSanctionDialog[], "%s\n…\t…\t…\t…", gSanctionDialog[playerid]);
            break;
        }
        format(gSanctionDialog[playerid], sizeof gSanctionDialog[], "%s\n%s", gSanctionDialog[playerid], line);
    }

    ShowPlayerDialog(playerid, DIALOG_SANCTIONS, DIALOG_STYLE_TABLIST_HEADERS, title, gSanctionDialog[playerid], "Fermer", "");
    return 1;
}

stock ShowPersonnageDialog(playerid, accountid)
{
	new query[512];
    mysql_format(dbHandle, query, sizeof query, "SELECT `name`, `statusCharacter`, DATE_FORMAT(`createdAt`, '%%Y-%%m-%%d %%H:%%i') AS `createdAt`, CASE WHEN `last_login` > 0 THEN DATE_FORMAT(FROM_UNIXTIME(`last_login`), '%%Y-%%m-%%d %%H:%%i') ELSE '' END AS `lastLogin` FROM `users` ");
	format(query, sizeof query, "%s WHERE `accountid` = %d ORDER BY `users`.`createdAt` IS NULL, `users`.`createdAt` DESC, `id` ASC LIMIT 200", query, accountid);
    mysql_tquery(dbHandle, query, "OnPersonnageLoaded", "ii", playerid, accountid);
	return 1;
}

this::OnPersonnageLoaded(playerid, accountid)
{
    new rows = cache_num_rows();

    new title[64];
    format(title, sizeof title, "Liste des personnages (%d)", accountid);

    gPersonnageDialog[playerid][0] = '\0';
    format(gPersonnageDialog[playerid], sizeof gPersonnageDialog[], "Nom\tStatut\tCréé le\tDernière connexion\n");

    if (!rows)
    {
        format(gPersonnageDialog[playerid], sizeof gPersonnageDialog[], "%s\n-\t-\tAucun personnage\t-", gPersonnageDialog[playerid]);
        ShowPlayerDialog(playerid, DIALOG_VOIRPERSOS, DIALOG_STYLE_TABLIST_HEADERS, title, gPersonnageDialog[playerid], "OK", "Fermer");
        return 1;
    }

    new name[32], status[32], color_status[64], created[32], lastLogin[32];
    new line[284];

    for (new i = 0; i < rows; i++)
    {
        cache_get_field_content(i, "name", name, sizeof name);
        cache_get_field_content(i, "statusCharacter", status, sizeof status);
        cache_get_field_content(i, "createdAt", created, sizeof created);
        cache_get_field_content(i, "lastLogin", lastLogin, sizeof lastLogin);
		
		if (!strcmp(status[i], "En attente", true))    			format(color_status, sizeof(color_status), "{DABB3E}%s", status[i]);
		else if (!strcmp(status[i], "En revalidation", true))   format(color_status, sizeof(color_status), "{DABB3E}%s", status[i]);
		else if (!strcmp(status[i], "Seconde chance", true))   	format(color_status, sizeof(color_status), "{DABB3E}%s", status[i]);
		else if (!strcmp(status[i], "Refusé", true))   			format(color_status, sizeof(color_status), "{FF6347}%s", status[i]);
		else if (!strcmp(status[i], "Bloqué", true))   			format(color_status, sizeof(color_status), "{FF6347}%s", status[i]);
		else if (!strcmp(status[i], "Approuvé", true))        	format(color_status, sizeof(color_status), "{33AA33}%s", status[i]);
		else                                          			format(color_status, sizeof(color_status), "{FFFFFF}%s", status[i]);

        format(line, sizeof line, "%s\t%s\t%s\t%s", name, color_status, created, lastLogin);

        new used = strlen(gPersonnageDialog[playerid]);
        if (used + 1 + strlen(line) >= sizeof gPersonnageDialog[])
        {
            format(gPersonnageDialog[playerid], sizeof gPersonnageDialog[], "%s\n…\t…\t…\t…", gPersonnageDialog[playerid]);
            break;
        }
        format(gPersonnageDialog[playerid], sizeof gPersonnageDialog[], "%s\n%s", gPersonnageDialog[playerid], line);
    }

    ShowPlayerDialog(playerid, DIALOG_VOIRPERSOS, DIALOG_STYLE_TABLIST_HEADERS, title, gPersonnageDialog[playerid], "Fermer", "");
    return 1;
}