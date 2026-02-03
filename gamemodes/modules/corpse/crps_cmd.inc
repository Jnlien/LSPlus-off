ALTX:cadavre("/corps");
CMD:cadavre(playerid, params[]) {
    new i;

    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
        return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");
    
    if (!IsPlayerLogged(playerid)) 
        return SendNotConnected(playerid);
    
    if ((i = GetNearestCorpse(playerid)) == -1) 
        return SendErrorMessage(playerid, "Aucun corps à proximité.");

    Dialog_Show(playerid, CorpseInfo, DIALOG_STYLE_MSGBOX, "Informations sur le corps", CorpInfo[i][cNote], "Options", "Fermer");
    return 1;
}