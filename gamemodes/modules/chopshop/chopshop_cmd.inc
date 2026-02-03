CMD:chopshop(playerid, params[])
{
    if (!IsAdminDuty(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_3) && !isLeader(playerid))
        return SendErrorMessage(playerid, "Vous n'avez pas la permission d'utiliser cette commande.");
        
    if (!IsAdminDuty(playerid) && !CheckAdmin(playerid, ADMIN_LEVEL_3) && Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_GANG && Factions[PlayerInfo[playerid][pFaction]][fType] != FACTION_MAFIA)
        return SendErrorMessage(playerid, "Vous n'avez pas la permission d'utiliser cette commande.");

    if (GetChopshopID(playerid) != -1)
    {
        Dialog_Show(playerid, DIALOG_CHOPSHOP, DIALOG_STYLE_LIST,   "Atelier",\
                                                                    "Supprimer l'atelier\n\
                                                                    Mettre à jour la liste des véhicules\n\
                                                                    Statistiques de l'atelier\n\
                                                                    Modifier le pourcentage\n\
                                                                    Éditer la position de l'atelier",\
                                                                    "Choisir", "<<");
    }
    else if (IsAdminDuty(playerid) && CheckAdmin(playerid, ADMIN_LEVEL_2) || !IsFactionHasChopShop(PlayerInfo[playerid][pFaction])) 
    {
        Dialog_Show(playerid, DIALOG_CHOPSHOP, DIALOG_STYLE_LIST,   "Atelier", 
                                                                    "Créer un atelier", 
                                                                    "Choisir", "<<");
    }
    return 1;
}