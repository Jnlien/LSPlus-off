ALTX:speedcam("/sc");
CMD:speedcam(playerid, params[])
{
    new option[16], secoption[128], faction = PlayerInfo[playerid][pFaction];

    if (!IsFactionPolice(faction))       return SendErrorMessage(playerid, "Vous devez être de la police pour effectuer cette action.");
    if (!PlayerInfo[playerid][pOnDuty])  return SendErrorMessage(playerid, "Vous devez être en service pour effectuer cette action.");

    if (sscanf(params, "s[16]S()[128]", option, secoption)) 
    {
        SendSyntaxMessage(playerid, "/sc [commande]");
        SendHelpSyntaxMessage(playerid, "creer, (supp)rimer, (edit)er, liste");
        return 1;
    }

    if (CompareStrings(option, "creer"))
    {
        new id = SC_GetFreeID(), sp_limit, sp_name[32], sp_near = GetNearestSpeed(playerid, 20.0), sp_zone[64];

        if (id == -1)                                        return SendErrorMessage(playerid, "Tous les emplacements de radars sont occupés.");
        if (sp_near != -1)                                   return SendErrorMessage(playerid, "Un radar est déjà actif dans cette zone.");
        if (PlayerInfo[playerid][pRank] < SC_MIN_INSTALL)    return SendErrorMessage(playerid, "Commande disponible à partir du rang %i.", SC_MIN_INSTALL);
        if (sscanf(secoption, "d", sp_limit))                return SendSyntaxMessage(playerid, "/sc creer [limite de vitesse]");
        if (sp_limit <= 0 || sp_limit > SC_MAX_SPEED)        return SendErrorMessage(playerid, "Limite de vitesse de 1 à %d km/h.", SC_MAX_SPEED);

        GetPlayerPos(playerid, speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z]);
        GetPlayer2DZone(playerid, sp_zone, sizeof(sp_zone));

        format(speedcam[id][sc_name], 64, "%s %i", sp_zone, id);
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z); 

        speedcam[id][sc_speed] = sp_limit;
        speedcam[id][sc_x] = x;
        speedcam[id][sc_y] = y+1;
        speedcam[id][sc_z] = z;
        speedcam[id][sc_rx] = 0;
        speedcam[id][sc_ry] = 0;
        speedcam[id][sc_rz] = 0;
        speedcam[id][sc_int] = GetPlayerInterior(playerid);
        speedcam[id][sc_vw] = GetPlayerVirtualWorld(playerid);
        speedcam[id][sc_object] = CreateDynamicObject(18880, speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z], speedcam[id][sc_rx], speedcam[id][sc_ry], speedcam[id][sc_rz], speedcam[id][sc_vw], speedcam[id][sc_int]);
        Iter_Add(speedcamObjects, id);

        speedcam[id][sc_areaid] = CreateDynamicSphere(speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z], SC_DEF_RADIUS, speedcam[id][sc_vw], speedcam[id][sc_int]);

        AddSpeedcamToFile(id, sp_limit, sp_name, speedcam[id][sc_x], speedcam[id][sc_y], speedcam[id][sc_z], 0.0, 0.0, 0.0, speedcam[id][sc_int], speedcam[id][sc_vw]);
        SM2(playerid, "Vous avez installé un radar automatique [ID : %i] %s | Limite : %d km/h.", id, speedcam[id][sc_name], sp_limit);
        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "(( %s a installé un radar %s | Limite : %d km/h ))", GetName(playerid), speedcam[id][sc_name], sp_limit);
        return 1;
    }

    else if (CompareStrings(option, "supprimer") || CompareStrings(option, "supp"))
    {
        new sp_near = GetNearestSpeed(playerid, 20.0);

        if (sp_near == -1)                                   return SendErrorMessage(playerid, "Aucun radar dans ce rayon.");
        if (PlayerInfo[playerid][pRank] < SC_MIN_INSTALL)    return SendErrorMessage(playerid, "Cette commande est disponible à partir du rang %i.", SC_MIN_INSTALL);

        if (IsValidDynamicObject(speedcam[sp_near][sc_object]))
        {
            DestroyDynamicObject(speedcam[sp_near][sc_object]);
            speedcam[sp_near][sc_object] = INVALID_OBJECT_ID;
        }

        new sql_query[128];

        SendWarningMessage(playerid, "Vous avez supprimé une caméra [ID : %i | %s].", sp_near, speedcam[sp_near][sc_name]);
        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "(( %s a supprimé une caméra [%s]. ))", GetName(playerid), speedcam[sp_near][sc_name]);

        speedcam[sp_near][sc_name] = EOS;
        Iter_Remove(speedcamObjects, sp_near);

        mysql_format(dbHandle, sql_query, sizeof(sql_query), "DELETE FROM `speedcams` WHERE `id` = %i", sp_near);
        mysql_tquery(dbHandle, sql_query);

        return 1;
    }

    else if (CompareStrings(option, "editer") || CompareStrings(option, "edit"))
    {
        new sp_near = GetNearestSpeed(playerid, 15.0), thirdoption[64], fouroption[64];

        if (sp_near == -1) return SendErrorMessage(playerid, "Aucune caméra de vitesse dans ce rayon.");
        if (PlayerInfo[playerid][pRank] < SC_MIN_INSTALL) return SendErrorMessage(playerid, "Cette commande est disponible à partir du rang %i.", SC_MIN_INSTALL);

        if (sscanf(secoption, "s[64]S()[64]", thirdoption, fouroption))        
        {
            SendSyntaxMessage(playerid, "/sc editer [commande]");
            SendHelpSyntaxMessage(playerid, "(pos)ition, limit");
            return 1;
        }

        else if (CompareStrings(thirdoption, "position") || CompareStrings(thirdoption, "pos"))
        {
            SetPVarInt(playerid, "SC:ID", sp_near + 1);
            SetPVarInt(playerid, "SC:Edit", 1);
            EditDynamicObject(playerid, speedcam[sp_near][sc_object]);
            return 1;
        }

        else if (CompareStrings(thirdoption, "limit"))
        {
            new sp_limit;

            if (sscanf(fouroption, "d", sp_limit)) return SendSyntaxMessage(playerid, "/sc editer limit [limite de vitesse]");
            if (sp_limit <= 0 || sp_limit > SC_MAX_SPEED) return SendErrorMessage(playerid, "La limite de vitesse doit être comprise entre 1 et %d.", SC_MAX_SPEED);

            speedcam[sp_near][sc_speed] = sp_limit;
            SQL_SetInteger("speedcams", "speed", speedcam[sp_near][sc_speed], speedcam[sp_near][sc_id]);
            SendSyntaxMessage(playerid, "Vous avez modifié la limite de vitesse de la caméra [ID : %i] %s à %d.", sp_near, speedcam[sp_near][sc_name], sp_limit);
            SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "(( %s a modifié la limite de vitesse de la caméra [ID : %i] %s à %d. ))", GetName(playerid), sp_near, speedcam[sp_near][sc_name], sp_limit);
            return 1;
        }
    }

    else if (CompareStrings(option, "liste"))
    {
		new dialog_header[96] = "{FFFFFF}Caméra\t{FFFFFF}ID de la caméra\t{FFFFFF}Limite de vitesse\n", dialog_body[3072];

        format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

        for(new i = 0; i < Iter_Count(speedcamObjects); i++)
        {
            format(dialog_body, sizeof(dialog_body), "%s%s\t%i\t%d\n", dialog_body, speedcam[i][sc_name], i, speedcam[i][sc_speed]);
        }

        Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Liste des caméras", dialog_body, "Fermer", "");

    }  
    return 1;
}