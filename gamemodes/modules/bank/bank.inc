ATM_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rz)
{
    new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;

    GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
    GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

    if (GetPVarInt(playerid, #edit_atm) != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
    {
        new id = GetPVarInt(playerid, #edit_atm) - 1;

        DeletePVar(playerid, #edit_atm);
        if (id < 0 || id >= MAX_ATMS || !ATMInfo[id][atmID]) return SendErrorMessage(playerid, "Le distributeur n'a pas été trouvé.");
        if (objectid != ATMInfo[id][atmObject]) return SendErrorMessage(playerid, "Erreur lors de l’édition du distributeur.");

        SetDynamicObjectPos(objectid, x, y, z);
        SetDynamicObjectRot(objectid, 0.0, 0.0, rz);

        GetDynamicObjectPos(objectid, ATMInfo[id][atmPosX], ATMInfo[id][atmPosY], ATMInfo[id][atmPosZ]);
        ATMInfo[id][atmPosRZ] = rz;

        new query[128];
        mysql_format(dbHandle, query, sizeof(query), "UPDATE `atms` SET `posx`=%f,`posy`=%f,`posz`=%f,`posrz`=%f WHERE `id`=%i", x, y, z, rz, ATMInfo[id][atmID]);
        mysql_tquery(dbHandle, query);

        DestroyDynamic3DTextLabel(ATMInfo[id][atmLabel]);
        ATMInfo[id][atmLabel] =CreateDynamic3DTextLabel("[ATM]\n{FFFFFF}Appuyez sur H pour utiliser", 0xA9C4E4FF, ATMInfo[id][atmPosX], ATMInfo[id][atmPosY], ATMInfo[id][atmPosZ] + 0.5, 3.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, ATMInfo[id][atmVW], ATMInfo[id][atmInt]);

        SendAdmMessageToPlayer(playerid, "La position du distributeur [ID : %i] a été modifiée.", id);
    }
    return 1;
}


ATM_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT * FROM `atms`", "LoadDynamicATMS");
}