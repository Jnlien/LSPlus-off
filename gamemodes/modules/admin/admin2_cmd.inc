CMD:up(playerid, params[])
{
    new amount, Float:x, Float:y, Float:z;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 	return SendNotAdmin(playerid);
    if (sscanf(params,"d", amount)) 				return SendHelpMessage(playerid, "/up [blocs]");
    
    GetPlayerPos(playerid, x, y ,z);
    SetPlayerPosEx(playerid, x, y, z+amount);
    return 1;
}

CMD:down(playerid, params[])
{
    new amount, Float:x, Float:y, Float:z;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 	return SendNotAdmin(playerid);
    if (sscanf(params,"d", amount)) 				return SendHelpMessage(playerid, "/down [blocs]");

    GetPlayerPos(playerid, x, y ,z);
    SetPlayerPosEx(playerid, x, y, z-amount);
    return 1;
}

CMD:policer(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2))
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    AMSG(COLOR_LIGHTRED, "(Admin) %s a effectué un respawn des véhicules de police.", GetAccName(playerid));

    new vehicle,
        occupied[MAX_VEHICLES char];
        
    foreach(new i : Player)
    {
        if ((vehicle = GetPlayerVehicleID(i)) == 0 || !IsFactionPolice(VehicleInfo[vehicle][carFaction]))
            continue;
            
        occupied{vehicle} = 1;
    }
    
    foreach(new i:Vehicle)
    {
        if (!IsFactionPolice(VehicleInfo[vehicle][carFaction]) || occupied{i} == 1)
            continue;

        RespawnVehicle(i);
    }
    return 1;
}

CMD:getincar(playerid, params[])
{
    new vehicle;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2))	return SendNotAdmin(playerid);
    if (sscanf(params,"d",vehicle))				return SendHelpMessage(playerid, "/getincar [ID véhicule]");
    if (!IsValidVehicle(vehicle))				return SendErrorMessage(playerid, "Aucun véhicule trouvé avec cet ID.");

    PutPlayerInVehicle(playerid, vehicle, 0);
    SendAdmMessage("%s s'est téléporté(e) dans le véhicule [ID: %i].", GetAccName(playerid), vehicle);
    return 1;
}

CMD:afire(playerid,params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2))	return SendNotAdmin(playerid);

    new option[16];

    if (sscanf(params, "s[16]", option))
    {
        SendHelpMessage(playerid, "/afire [create / removeall]");
        return 1;
    }

    if (CompareStrings(option, "create"))
    {
        if (GetPVarInt(playerid, #crt_fire) == 0)
        {
            if (IsPlayerHoldingWeapon(playerid)) return SendErrorMessage(playerid, "Rangez votre arme dans l'inventaire!");

            SetPVarInt(playerid, #crt_fire, 1);

            GivePlayerWeapon(playerid, 24, 500);

            SendSyntaxMessage(playerid, "Vous êtes entré en mode création de feux.");
            SendSyntaxMessage(playerid, "Tirez à l'endroit où vous souhaitez placer le feu.");
            SendAdmMessage("%s est entré en mode création de feux.", GetAccName(playerid));
            return 1;
        }

        SendAdmMessageToPlayer(playerid, "Vous êtes sorti du mode création de feux. Utilisez /afire removeall pour supprimer tous les feux.");
        SendAdmMessage("%s est sorti du mode création de feux.", GetAccName(playerid));
        
        ResetPlayerWeapons(playerid);
        DeletePVar(playerid, #crt_fire);
     	return 1;
    }

    else if (CompareStrings(option, "removeall"))
    {
        for(new i; i != MAX_FIRES; i++)
        {
            if (FiresInfo[i][fireOn] == 0) continue;

            DeleteFire(i);
        }
        SendAdmMessage("%s a supprimé tous les feux du serveur.", GetAccName(playerid));
    }

    else 
    {
        SendHelpMessage(playerid, "/afire [paramètres]");
        SendHelpMessage(playerid, "create removeall");
    }
    return 1;
}

CMD:amoteur(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
    if (!IsPlayerInAnyVehicle(playerid)) return SendNotInVehicle(playerid);

    new vehicleid = GetPlayerVehicleID(playerid);

    ToggleVehicleEngine(vehicleid, playerid);
    return 1;
}

CMD:fillveh(playerid, params[])
{
    new vehicle;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
    if (sscanf(params, "d", vehicle)) return SendHelpMessage(playerid, "/fillveh [ID du véhicule]");
    if (!IsValidVehicle(vehicle)) return SendErrorMessage(playerid, "Aucun véhicule trouvé avec cet ID.");

    VehicleInfo[vehicle][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(vehicle));
    SendAdmMessageToPlayer(playerid, "Vous avez rempli le véhicule ID:%i.", vehicle);
    return 1;
}

CMD:fillallveh(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

    foreach (new i:Vehicle)
    {
        VehicleInfo[i][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(i));
    }

    SendAdmMessageToAll("Tous les véhicules sur le serveur ont été remplis par l'administrateur %s.", GetAccName(playerid));
    return 1;
}

/*CMD:rtc(playerid, params[])
{
    new vehicle;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
    if (sscanf(params, "d", vehicle)) return SendHelpMessage(playerid, "/rtc [ID du véhicule]");
    if (!IsValidVehicle(vehicle)) return SendErrorMessage(playerid, "Aucun véhicule trouvé avec cet ID.");
    if (!VehicleEmpty(vehicle)) return SendErrorMessage(playerid, "Ce véhicule est actuellement utilisé.");

    if (VehicleInfo[vehicle][carOwnerID] != 0)
    {
        new userid = GetVehicleOwner(vehicle);

        if (userid != INVALID_PLAYER_ID)
        {
            SendAdmMessageToPlayer(userid, "L'administrateur %s a déplacé votre %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(vehicle) - 400]);
            PlayerInfo[userid][pCarKey] = 0;
            VehicleInfo[vehicle][carOwnerPlayer] = INVALID_PLAYER_ID;
        }
        ResetVehicleStuff(vehicle, 1);
        DestroyVehicle(vehicle);
    }
    else
    {
        SendAdmMessageToPlayer(playerid, "Vous avez respawn le véhicule %s [ID: %i].", VehicleNames[GetVehicleModel(vehicle) - 400], vehicle);
        RespawnVehicle(vehicle);
    }
    return 1;
}

CMD:rtcall(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);

    SendAdmMessage("%s a respawn tous les véhicules inutilisés sur le serveur.", GetAccName(playerid));

    new vehicle, occupied[MAX_VEHICLES char];

    foreach (new i : Player)
    {
        if ((vehicle = GetPlayerVehicleID(i)) == 0) continue;

        occupied{vehicle} = 1;
    }

    for (new i = 1; i < MAX_VEHICLES; i++)
    {
        if (!IsValidVehicle(i) || occupied{i} == 1) continue;

        if (VehicleInfo[i][carOwnerID] != 0)
        {
            new userid = GetVehicleOwner(i);
            if (userid != INVALID_PLAYER_ID)
            {
                SendSyntaxMessage(userid, "L'administrateur %s a déplacé votre %s.", GetAccName(playerid), VehicleNames[GetVehicleModel(i) - 400]);
                PlayerInfo[userid][pCarKey] = 0;
                VehicleInfo[i][carOwnerPlayer] = INVALID_PLAYER_ID;
            }
            ResetVehicleStuff(i, 1);
            DestroyVehicle(i);
        }
        else RespawnVehicle(i);
    }
    return 1;
}*/

CMD:amark(playerid, params[])
{
    new option[16], string[145];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette commande.");
    if (sscanf(params, "s[16]S()[144]", option, string))
    {
        SendHelpMessage(playerid, "/amark [paramètre]");
        SendSyntaxMessage(playerid, "destroy info goto");
        return 1;
    }

    if (CompareStrings(option, "destroy"))
    {
        new markid;
        if (sscanf(string, "i", markid)) return SendHelpMessage(playerid, "/amark destroy [ID de la marque]");
        if (markid < 0 || markid >= MAX_MARKS || !E_MARKS[markid][mON]) return SendErrorMessage(playerid, "Aucune marque trouvée avec cet ID.");

        DestroyDynamicMark(markid);
        SendSyntaxMessage(playerid, "Vous avez supprimé la marque [ID: %i].", markid);
        return 1;
    }
    else if (CompareStrings(option, "info"))
    {
        new markid = Mark_Nearest(playerid);

        if (markid == -1) return SendErrorMessage(playerid, "Aucune marque trouvée avec cet ID.");

        new mark_info[512];
        mark_info[0] = EOS;

        format(mark_info, sizeof(mark_info), "ID de la marque\t%i\nCréateur\t%s\nDate de création\t%s", markid, E_MARKS[markid][mCreator], E_MARKS[markid][mCreateDate]);
        Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST, "{FFFFFF}Informations sur la marque", mark_info, "Fermer", "");
        return 1;
    }
    else if (CompareStrings(option, "goto"))
    {
        new markid;
        if (sscanf(string, "i", markid)) return SendHelpMessage(playerid, "/amark goto [ID de la marque]");
        if (markid < 0 || markid > MAX_MARKS || !E_MARKS[markid][mON]) return SendErrorMessage(playerid, "Aucune marque trouvée avec cet ID.");

        SetPlayerPosEx(playerid, E_MARKS[markid][mPosX], E_MARKS[markid][mPosY], E_MARKS[markid][mPosZ]);
        SetInterior(playerid, E_MARKS[markid][mInt]);
        SetWorld(playerid, E_MARKS[markid][mVW]);
    }
    else
    {
        SendHelpMessage(playerid, "/amark [paramètre]");
        SendSyntaxMessage(playerid, "destroy info goto");
    }
    return 1;
}

CMD:sethp(playerid, params[])
{
    new userid, hp;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "ud", userid, hp)) 
        return SendHelpMessage(playerid, "/sethp [ID/nom du joueur] [santé (1-100)]");

    if (!IsPlayerLogged(userid)) 
        return SendNotConnected(playerid);

    if (PlayerInfo[playerid][pInjured]) 
        return SendErrorMessage(playerid, "Le joueur est en état de blessure/mort. Utilisez /healplayer.");

    if (hp < 1 || hp > 100) 
        return SendErrorMessage(playerid, "La valeur de santé doit être comprise entre 1 et 100.");

    SendAdmMessageToPlayer(userid, "L'administrateur %s vous a attribué %i HP.", GetAccName(playerid), hp);
    SendAdmMessageToPlayer(playerid, "Vous avez attribué %i HP à %s.", hp, GetNameEx(userid));
    SetPlayerHealth(userid, hp);

    return 1;
}

CMD:setarmure(playerid, params[])
{
    new userid, armour;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "ud", userid, armour)) 
        return SendHelpMessage(playerid, "/setarmure [ID/nom du joueur] [armure (1-100)]");

    if (armour < 1 || armour > 100) 
        return SendErrorMessage(playerid, "La valeur d'armure doit être comprise entre 1 et 100.");

    if (!IsPlayerLogged(userid)) 
        return SendNotConnected(playerid);

    SendAdmMessageToPlayer(userid, "L'administrateur %s vous a attribué %i points d'armure.", GetAccName(playerid), armour);
    SendAdmMessageToPlayer(playerid, "Vous avez attribué %i points d'armure à %s.", armour, GetNameEx(userid));
    SetPlayerArmour(userid, armour);

    return 1;
}

CMD:setskin(playerid, params[])
{
    new targetid, skinid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "ui", targetid, skinid)) 
        return SendHelpMessage(playerid, "/setskin [ID/nom du joueur] [ID du skin (0-311 ou 20000-30000)]");

    if (!IsPlayerLogged(targetid)) 
        return SendNotConnected(playerid);

    if ((skinid < 1 || skinid == 74 || skinid > 311) && (skinid < 20000 || skinid > 30000))
        return SendErrorMessage(playerid, "L'ID du skin doit être compris entre 1 et 311 (sauf 74) ou entre 20000 et 30000.");

    SetPlayerSkinEx(targetid, skinid);
    SendAdmMessageToPlayer(targetid, "L'administrateur %s a changé votre skin [ID: %i].", GetAccName(playerid), skinid);
    SendAdmMessageToPlayer(playerid, "Vous avez changé le skin de %s [ID: %i].", GetNameEx(targetid), skinid);

    return 1;
}

CMD:showpm(playerid, params[])
{
    new userid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", userid)) 
        return SendHelpMessage(playerid, "/showpm [ID/nom du joueur]");

    if (!IsPlayerLogged(userid)) 
        return SendNotConnected(playerid);

    pTemp[playerid][pPMPlayer] = userid;
    SendSyntaxMessage(playerid, "Vous surveillez les PM de %s. Tapez /showpmoff pour désactiver.", GetNameEx(userid));
    return 1;
}

CMD:showpmoff(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (pTemp[playerid][pPMPlayer] == INVALID_PLAYER_ID) 
        return SendErrorMessage(playerid, "Vous ne surveillez aucun PM.");

    SendSyntaxMessage(playerid, "Vous avez arrêté de surveiller les PM de %s.", GetNameEx(pTemp[playerid][pPMPlayer]));
    pTemp[playerid][pPMPlayer] = INVALID_PLAYER_ID;
    return 1;
}

CMD:showpms(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    SendClientMessage(playerid, COLOR_WHITE, "Vous surveillez les PM de tout le serveur. Tapez \"/nopms\" pour arrêter.");
    SetPVarInt(playerid, #spypm_all, 1);
    return 1;
}

CMD:nopms(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    if (GetPVarInt(playerid, #spypm_all) == 0) 
        return SendClientMessage(playerid, COLOR_LIGHTRED, "Vous ne surveillez aucun PM.");

    SendClientMessage(playerid, COLOR_WHITE, "Vous avez arrêté de surveiller les PM.");
    DeletePVar(playerid, #spypm_all);
    return 1;
}

CMD:telecmds(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    SendClientMessage(playerid, COLOR_WHITE, "[Level 3]: {C3C3C3}/atp /creertp /makehouseint /supprimertp");
    return 1;
}

ALTX:makehouseint("/mhouseint");
CMD:makehouseint(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    new mapname[32], type;

    if (sscanf(params, "is[32]", type, mapname)) 
        return SendHelpMessage(playerid, "/makehouseint [Type de maison: 1-petite, 2-moyenne, 3-grande] [nom de la maison]");

    if (type < 1 || type > 3) 
        return SendErrorMessage(playerid, "Type de maison: 1 - petite, 2 - moyenne, 3 - grande.");

    if (strlen(mapname) < 1 || strlen(mapname) > 32) 
        return SendErrorMessage(playerid, "Le nom doit contenir entre 1 et 32 caractères.");

    new id = -1;
    for (new i = 0; i < MAX_ADMIN_TELES; i++)
    {
        if (AdminTeles[i][telesOn] != 0) 
            continue;

        id = i;
        break;
    }

    if (id == -1) 
        return SendErrorMessage(playerid, "Limite de téléports atteinte.");

    GetPlayerPos(playerid, AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ]);
    AdminTeles[id][telesInt] = GetInterior(playerid);
    AdminTeles[id][telesType] = type - 1;
    format(AdminTeles[id][telesMapName], 32, "%s", mapname);
    AdminTeles[id][telesOn] = 1;

    AddTeleToFile(playerid, id, mapname, AdminTeles[id][telesInt], AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ], type - 1);
    SendClientMessageEx(playerid, COLOR_GREEN, "Nouvel intérieur (%s) ID: %i créé.", AdminTeles[id][telesMapName], id);
    return 1;
}

CMD:aretireramende(playerid, params[])
{
    new option[16], string[145];

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) return SendNotAdmin(playerid);
    if (sscanf(params, "s[16]S()[144]", option, string))
    {
        SendSyntaxMessage(playerid, "/aretireramende [commande]");
		SendHelpSyntaxMessage(playerid, "joueur, (veh)icule");
        return 1;
    }

    if (CompareStrings(option, "joueur"))
    {
        new userid;

        if (sscanf(string, "u", userid)) 
            return SendSyntaxMessage(playerid, "/aretireramende joueur [ID du joueur]");

        if (!IsPlayerLogged(userid)) 
            return SendNotConnected(playerid);

        DeletePlayerTickets(userid);

        SendAdmMessageToPlayer(playerid, "Vous avez supprimé tous les tickets du joueur - %s.", GetNameEx(userid));
        SendAdmMessageToPlayer(userid, "L'administrateur %s a supprimé tous vos tickets.", GetAccName(playerid));
        return 1;
    }
    else if (CompareStrings(option, "vehicule") || CompareStrings(option, "veh"))
    {
        new vehid;

        if (sscanf(params, "i", vehid)) 
            return SendSyntaxMessage(playerid, "/aretireramende vehicule [ID du véhicule]");

        DeleteVehicleTickets(vehid);

        SendAdmMessageToPlayer(playerid, "Vous avez supprimé tous les tickets du véhicule - %s [ID:%i].", VehicleNames[VehicleInfo[vehid][carModel] - 400], vehid);
        return 1;
    }
    else
    {
        SendSyntaxMessage(playerid, "/aretireramende [commande]");
		SendHelpSyntaxMessage(playerid, "joueur, (veh)icule");
    }
    return 1;
}

/*CMD:delplayertickets(playerid, params[])
{
    new userid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "u", userid)) 
        return SendHelpMessage(playerid, "/delplayertickets [ID/nom du joueur]");

    if (!IsPlayerLogged(userid)) 
        return SendNotConnected(playerid);

    DeletePlayerTickets(userid);

    SendAdmMessageToPlayer(playerid, "Vous avez supprimé tous les tickets du joueur - %s.", GetNameEx(userid));
    SendAdmMessageToPlayer(userid, "L'administrateur %s a supprimé tous vos tickets.", GetAccName(playerid));
    return 1;
}

CMD:delvehtickets(playerid, params[])
{
    new vehid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "i", vehid)) 
        return SendHelpMessage(playerid, "/delvehtickets [ID du véhicule]");

    DeleteVehicleTickets(vehid);

    SendAdmMessageToPlayer(playerid, "Vous avez supprimé tous les tickets du véhicule - %s [ID:%i].", VehicleNames[VehicleInfo[vehid][carModel] - 400], vehid);
    return 1;
}*/

CMD:aarendrevehunimpound(playerid, params[])
{
    new vehicle;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "i", vehicle)) 
        return SendHelpMessage(playerid, "/arendreveh [ID du véhicule]");

    if (!IsValidVehicle(vehicle)) 
        return SendErrorMessage(playerid, "Le véhicule n'est pas spawné.");

    if (!VehicleInfo[vehicle][Impounded]) 
        return SendErrorMessage(playerid, "Le véhicule n'est pas en fourrière.");

    VehicleInfo[vehicle][Impounded] = 0;
    Save_Car(vehicle);

    SendAdmMessageToPlayer(playerid, "Le véhicule [ID %i], appartenant à %s, n'est plus en fourrière.", vehicle, GetNameEx(VehicleInfo[vehicle][carOwnerID]));
    SendAdmMessageToPlayer(VehicleInfo[vehicle][carOwnerID], "L'administrateur %s a sortie votre %s de la fourrière.", GetAccName(playerid), VehicleNames[VehicleInfo[vehicle][carModel] - 400]);
    return 1;
}

CMD:makebizint(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    new mapname[32];

    if (sscanf(params, "s[32]", mapname)) 
        return SendHelpMessage(playerid, "/makebizint [nom de l'intérieur]");

    if (strlen(mapname) < 1 || strlen(mapname) > 32) 
        return SendErrorMessage(playerid, "Le nom doit contenir entre 1 et 32 caractères.");

    new id = -1;
    for (new i = 0; i < MAX_ADMIN_TELES; i++)
    {
        if (AdminTeles[i][telesOn] != 0) 
            continue;

        id = i;
        break;
    }

    if (id == -1) 
        return SendErrorMessage(playerid, "Limite de téléports atteinte.");

    GetPlayerPos(playerid, AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ]);
    AdminTeles[id][telesInt] = GetInterior(playerid);
    AdminTeles[id][telesType] = BUSSINESS_INT_TYPE;
    format(AdminTeles[id][telesMapName], 32, "%s", mapname);
    AdminTeles[id][telesOn] = 1;

    AddTeleToFile(playerid, id, mapname, AdminTeles[id][telesInt], AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ], 4);
    SendClientMessageEx(playerid, COLOR_GREEN, "Nouvel intérieur (%s) ID: %i créé.", AdminTeles[id][telesMapName], id);
    return 1;
}

CMD:creertp(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    new mapname[32];

    if (sscanf(params, "s[32]", mapname)) 
        return SendHelpMessage(playerid, "/creertp [nom du point de téléportation]");

    if (strlen(mapname) < 1 || strlen(mapname) > 32) 
        return SendErrorMessage(playerid, "Le nom doit contenir entre 1 et 32 caractères.");

    new id = -1;
    for (new i = 0; i < MAX_INTERIORS; i++)
    {
        if (AdminTeles[i][telesOn]) 
            continue;

        id = i;
        break;
    }

    if (id == -1) 
        return SendErrorMessage(playerid, "Limite de téléports atteinte.");

    format(AdminTeles[id][telesMapName], 32, "%s", mapname);
    GetPlayerPos(playerid, AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ]);
    AdminTeles[id][telesInt] = GetInterior(playerid);
    AdminTeles[id][telesType] = 3;
    AdminTeles[id][telesOn] = 1;

    AddTeleToFile(playerid, id, mapname, AdminTeles[id][telesInt], AdminTeles[id][telesPosX], AdminTeles[id][telesPosY], AdminTeles[id][telesPosZ], 3);
    SendAdmMessageToPlayer(playerid, "Nouveau téléport (%s) ID: %i créé.", AdminTeles[id][telesMapName], id);
    return 1;
}

ALTX:supprimertp("/supptp");
CMD:supprimertp(playerid, params[])
{
    new teleid;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    if (sscanf(params, "d", teleid)) 
        return SendHelpMessage(playerid, "/supprimertp [ID du téléport]");

    if (AdminTeles[teleid][telesOn] == 0) 
        return SendErrorMessage(playerid, "Aucun téléport trouvé avec cet ID.");

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `ateles` WHERE `id` = %i", AdminTeles[teleid][aID]);
    mysql_tquery(dbHandle, query);

    SendAdmMessageToPlayer(playerid, "Vous avez supprimé le téléport ID %i.", teleid);

    AdminTeles[teleid][telesOn] = 0;
    AdminTeles[teleid][telesType] = 0;
    return 1;
}

CMD:aveh(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    new modelInput[32], model, color1, color2;
    new Float:x, Float:y, Float:z, Float:a, Float:distance = 5.0;

    if (sscanf(params, "s[32]I(0)I(0)", modelInput, color1, color2)) 
        return SendHelpMessage(playerid, "/aveh [modèle|nom] [couleur1] [couleur2]");

    if (IsNumeric(modelInput)) {
        model = strval(modelInput);
    } else {
        model = GetVehicleModelByName(modelInput);
        if (model == 0) 
            return SendErrorMessage(playerid, "Nom de véhicule inconnu.");
    }

    if (model == 520 || model == 425 || model == 432 || model == 447 || model == 476) 
        return SendErrorMessage(playerid, "Ce type de véhicule est interdit sur le serveur.");

    if (model < 400 || model > 611) 
        return SendErrorMessage(playerid, "Les modèles de véhicules doivent être compris entre 400 et 611.");

    if (color1 > 255 || color1 < 0 || color2 > 255 || color2 < 0) 
        return SendErrorMessage(playerid, "Les couleurs doivent être comprises entre 0 et 255.");

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    x += (distance * floatsin(-a, degrees));
    y += (distance * floatcos(-a, degrees));
    a += 90.0;
    if (a > 360.0) a -= 360.0;

    new vehicle = CreateVehicle(model, x, y, z, a, color1, color2, -1);
    if (vehicle == INVALID_VEHICLE_ID) 
        return SendErrorMessage(playerid, "Le véhicule n'a pas pu être créé.");

    SetVehicleHealth(vehicle, 1000.0);
    SetVehicleNumberPlate(vehicle, GetVehicleNumber());
    SetVehicleVirtualWorld(vehicle, GetWorld(playerid));
    LinkVehicleToInterior(vehicle, GetInterior(playerid));

    VehicleInfo[vehicle][carDriver] = INVALID_PLAYER_ID;
    VehicleInfo[vehicle][carFuel] = GetVehicleFuelCapacity(GetVehicleModel(vehicle));
    VehicleInfo[vehicle][carAdmin] = 1;
    VehicleInfo[vehicle][carColor1] = color1;
    VehicleInfo[vehicle][carColor2] = color2;
    VehicleInfo[vehicle][carType] = 0;

    SendAdmMessageToPlayer(playerid, "Vous avez créé %s [ID: %i | MODÈLE: %i].", VehicleNames[model - 400], vehicle, model);
    return 1;
}

stock IsNumeric(str[])
{
    for (new i = 0, len = strlen(str); i < len; i++)
    {
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    return 1;
}

stock GetVehicleModelByName(name[])
{
    for (new i = 0; i < sizeof(VehicleNames); i++)
        if (strcmp(VehicleNames[i], name, true) == 0)
            return i + 400;
    return 0;
}

CMD:makemark(playerid, params[])
{
    new Float:x, Float:y, Float:z, interior, world;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    interior = GetInterior(playerid);
    world = GetWorld(playerid);
    GetPlayerPos(playerid, x, y, z);
    SetPVarInt(playerid, "MarkedSpot", 1);
    SetPVarFloat(playerid, "MarkX", x);
    SetPVarFloat(playerid, "MarkY", y);
    SetPVarFloat(playerid, "MarkZ", z);
    SetPVarInt(playerid, "MarkInterior", interior);
    SetPVarInt(playerid, "MarkWorld", world);
    SendAdmMessageToPlayer(playerid, "Point marqué. Utilisez \"/gotomark\" pour vous téléporter.");
    return 1;
}

CMD:gotomark(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendErrorMessage(playerid, "Vous n'avez pas les droits nécessaires pour utiliser cette commande.");

    if (GetPVarInt(playerid, "MarkedSpot") == 0) 
        return SendErrorMessage(playerid, "Aucun point marqué. Utilisez \"/mark\".");

    SetPlayerPosEx(playerid, GetPVarFloat(playerid, "MarkX"), GetPVarFloat(playerid, "MarkY"), GetPVarFloat(playerid, "MarkZ"));
    SetWorld(playerid, GetPVarInt(playerid, "MarkWorld"));
    SetInterior(playerid, GetPVarInt(playerid, "MarkInterior"));
    SendClientMessage(playerid, COLOR_GREY, "* Vous avez été téléporté.");
    return 1;
}

CMD:settime(playerid, params[])
{
    new time;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "d", time)) 
        return SendHelpMessage(playerid, "/settime [heure (0-23)]");

    if (time < 0 || time > 23) 
        return SendErrorMessage(playerid, "L'heure doit être comprise entre 0 et 23.");

    SetWorldTime(time);
    SAMALL("%s a défini l'heure à %i:00.", GetAccName(playerid), time);
    return 1;
}

CMD:weather(playerid, params[])
{
    new weather;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "d", weather)) 
        return SendHelpMessage(playerid, "/weather [météo]");

    if (weather < 0 || weather > 45) 
        return SendErrorMessage(playerid, "La météo doit être comprise entre 0 et 45.");

    SetPlayerWeather(playerid, weather);
    return 1;
}

CMD:weatherall(playerid, params[])
{
    new weather;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "d", weather)) 
        return SendHelpMessage(playerid, "/weatherall [météo]");

    if (weather < 0 || weather > 45) 
        return SendErrorMessage(playerid, "La météo doit être comprise entre 0 et 45.");

    server_weather = weather;
    SetWeather(server_weather);

    SAMALL("%s a défini la météo pour tous les joueurs [ID %i].", GetAccName(playerid), weather);
    return 1;
}

CMD:vw(playerid, params[])
{
    new vw;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "i", vw)) 
        return SendHelpMessage(playerid, "/vw [monde virtuel (0-65535)]");

    if (vw < 0 || vw > 65535) 
        return SendErrorMessage(playerid, "Le monde virtuel doit être compris entre 0 et 65535.");

    SetWorld(playerid, vw);
    SendSyntaxMessage(playerid, "Vous avez changé votre monde virtuel en %i.", vw);
    return 1;
}

CMD:goint(playerid, params[])
{
    new interior;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "i", interior)) 
        return SendHelpMessage(playerid, "/goint [intérieur (0-255)]");

    if (interior < 0 || interior > 255) 
        return SendErrorMessage(playerid, "L'intérieur doit être compris entre 0 et 255.");

    SetInterior(playerid, interior);
    SendSyntaxMessage(playerid, "Vous avez changé votre intérieur en %i.", interior);
    return 1;
}

CMD:go(playerid, params[])
{
    new Float:x, Float:y, Float:z, interior, vw;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "fffI(0)I(0)", x, y, z, interior, vw)) 
        return SendHelpMessage(playerid, "/go [x] [y] [z] [intérieur (0-255)] [monde virtuel (0-65535)]");

    if (interior < 0 || interior > 255) 
        return SendErrorMessage(playerid, "L'intérieur doit être compris entre 0 et 255.");

    if (vw < 0 || vw > 65535) 
        return SendErrorMessage(playerid, "Le monde virtuel doit être compris entre 0 et 65535.");

    SetPlayerPosEx(playerid, x, y, z);
    SetInterior(playerid, interior);
    SetWorld(playerid, vw);
    return 1;
}

CMD:x(playerid, params[])
{
    new Float:coord;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "f", coord)) 
        return SendHelpMessage(playerid, "/x [+x coordonnée]");

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    SetPlayerPosEx(playerid, x + coord, y, z);
    return 1;
}

CMD:y(playerid, params[])
{
    new Float:coord;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "f", coord)) 
        return SendHelpMessage(playerid, "/y [+y coordonnée]");

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    SetPlayerPosEx(playerid, x, y + coord, z);
    return 1;
}

CMD:z(playerid, params[])
{
    new Float:coord;

    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    if (sscanf(params, "f", coord)) 
        return SendHelpMessage(playerid, "/z [+z coordonnée]");

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    SetPlayerPosEx(playerid, x, y, z + coord);
    return 1;
}

CMD:noooc(playerid, params[])
{
    if (!CheckAdmin(playerid, ADMIN_LEVEL_2)) 
        return SendNotAdmin(playerid);

    noooc = !noooc;

    if (noooc == false) 
        SendClientMessageToAll(COLOR_GRAD2, "Le chat OOC a été activé par un administrateur.");
    else 
        SendClientMessageToAll(COLOR_GRAD2, "Le chat OOC a été désactivé par un administrateur.");

    return 1;
}