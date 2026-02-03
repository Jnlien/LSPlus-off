CCTV_OnGameModeInit()
{
	return mysql_tquery(dbHandle, "SELECT * FROM `cctvs`", "LoadDynamicCCTV");
}  

CCTV_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	new Float:oldX, Float:oldY, Float:oldZ,
	    Float:oldRotX, Float:oldRotY, Float:oldRotZ;

    GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
	GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

	if (GetPVarInt(playerid, #edit_cctv) && CCTVInfo[GetPVarInt(playerid, #edit_cctv)-1][tvObject] == objectid && CCTVInfo[GetPVarInt(playerid, #edit_cctv)-1][tvObjectOn])
	{
		if (response == EDIT_RESPONSE_FINAL)
		{
		    new cctvid = GetPVarInt(playerid, #edit_cctv)-1;

	    	DeletePVar(playerid, #edit_cctv);
			SetDynamicObjectPos(objectid, x, y, z);
			SetDynamicObjectRot(objectid, rx, ry, rz);
			
			new query[128 + 56];
			mysql_format(dbHandle, query, sizeof(query), "UPDATE `cctvs` SET `posx` = %f, `posy` = %f, `posz` = %f, `posrx` = %f, `posry` = %f, `posrz` = %f WHERE `id` = %i", x, y, z, rx, ry, rz, CCTVInfo[cctvid][tvID]);
			mysql_tquery(dbHandle, query);
			
			GetDynamicObjectPos(objectid, CCTVInfo[cctvid][tvPosX], CCTVInfo[cctvid][tvPosY], CCTVInfo[cctvid][tvPosZ]);
			GetDynamicObjectRot(objectid, CCTVInfo[cctvid][tvPosRX], CCTVInfo[cctvid][tvPosRY], CCTVInfo[cctvid][tvPosRZ]);
            
            DestroyDynamicObject(CCTVInfo[cctvid][tvCollision]);
            CCTVInfo[cctvid][tvCollision] = CreateDynamicObject(3800, CCTVInfo[cctvid][tvPosX], CCTVInfo[cctvid][tvPosY], CCTVInfo[cctvid][tvPosZ]-1.0, CCTVInfo[cctvid][tvPosRX], CCTVInfo[cctvid][tvPosRY], CCTVInfo[cctvid][tvPosRZ], CCTVInfo[cctvid][tvVW], CCTVInfo[cctvid][tvInt]);
            SetDynamicObjectMaterial(CCTVInfo[cctvid][tvCollision], 0, 16644, "a51_detailstuff", "roucghstonebrtb", 0x00000000);

            if (CCTVInfo[cctvid][tvBreak])
            {
                if (IsValidDynamic3DTextLabel(CCTVInfo[cctvid][tvText]))
                {
                    DestroyDynamic3DTextLabel(CCTVInfo[cctvid][tvText]);
                }
                CCTVInfo[cctvid][tvText] = CreateDynamic3DTextLabel("(( HORS SERVICE ))", COLOR_GREY, CCTVInfo[cctvid][tvPosX], CCTVInfo[cctvid][tvPosY], CCTVInfo[cctvid][tvPosZ]-0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, CCTVInfo[cctvid][tvVW], CCTVInfo[cctvid][tvInt]);
            }

  			SendSyntaxMessage(playerid, "Édition de la caméra [ID: %i] terminée.", cctvid);
		}
		else if (response == EDIT_RESPONSE_CANCEL)
		{
		    DeletePVar(playerid, #edit_cctv);
			SetDynamicObjectPos(objectid, oldX, oldY, oldZ);
			SetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);
		}
	}
	return 1;
}

CCTV_OnPlayerShootDynamicObject(playerid, weaponid, STREAMER_TAG_OBJECT objectid)
{
    if (playerid == INVALID_PLAYER_ID) return 1;
    if (weaponid < 22 || weaponid > 36) return 1;

    for (new i = 0; i < MAX_CCTV; i++)
    {
        if (objectid != CCTVInfo[i][tvCollision]) continue;
        if (!CCTVInfo[i][tvObjectOn]) continue;
        if (!IsValidDynamicObject(CCTVInfo[i][tvCollision])) continue;
        if (CCTVInfo[i][tvBreak]) continue;

        SetCameraBreakState(i, 1);
        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** DISPATCH: La caméra [ID: %i | %s] a été détruite. **", i, CCTVInfo[i][tvName]);

        foreach (new j : Player)
        {
            new cameraid = GetPVarInt(j, "CCTV:ID") - 1;

            if (cameraid == i)
            {
                pTemp[j][pCCTV] = 0;
                DeletePVar(j, "CCTV:ID");
                TogglePlayerSpectating(j, 0);
                SendSyntaxMessage(j, "La caméra que vous étiez en train de surveiller a été détruite.");
            }
        }
    }
    return 1;
}

CCTV_OnPlayerWeaponShot(playerid, weaponid)
{
    if (playerid == INVALID_PLAYER_ID) return 1;

    new camid = GetNearestCamera(playerid, 40.0, 1), cam_zone[64];

    if (camid == -1) return 1;
    if (IsPlayerFactionPolice(playerid) && PlayerInfo[playerid][pOnDuty]) return 1;
    if (weaponid < 22 || weaponid > 36) return 1;

    CCTVInfo[camid][tvSignal] = 1;
    PCamera[playerid][camid][pc_used] = 1;
    CCTVInfo[camid][tvSignalTime] = 300;
    PCamera[playerid][camid][pc_time] = 300;

    GetPlayer2DZone(playerid, cam_zone, sizeof(cam_zone));

    if (maskOn{playerid})    
    {        
        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** DISPATCH: CCTV [ID: %i | %s] a détecté des tirs. **", camid, CCTVInfo[camid][tvName]);
        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** DISPATCH: Tireur inconnu / Localisation: %s **", cam_zone);
    }
    else                    
    {
        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** DISPATCH: CCTV [ID: %i | %s] a détecté des tirs. **", camid, CCTVInfo[camid][tvName]);
        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "** DISPATCH: Tireur - %s / Localisation: %s **", GetName(playerid), cam_zone);

        // Si tu veux ajouter une trace criminelle :
        // new charge_res[64];
        // format(charge_res, sizeof(charge_res), "La caméra [%s] a détecté des coups de feu.", CCTVInfo[camid][tvName]);
        // AddCriminalRecord(playerid, charge_res, 0);
    }
    return 1;
}