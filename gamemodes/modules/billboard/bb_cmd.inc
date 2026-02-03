ALTX:rentbillboard("/rentbb");
CMD:rentbillboard(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
        return 1;

    if (!PlayerInfo[playerid][pNumber])
        return SendErrorMessage(playerid, "Vous avez besoin d'un téléphone pour poster une annonce.");

    new bizid = -1;
    if ((bizid = Advertisement_Nearest(playerid)) == -1)
        return SendErrorMessage(playerid, "Vous n'êtes pas près d'une agence publicitaire.");

    if (BizInfo[bizid][bizLocked] == 1)
        return GameTextForPlayer(playerid, "~r~FERMÉ!", 2500, 1);

    new str[90], mes[(13 + MAX_ZONE_NAME + 60) * MAX_BILLBOARDS + 27] = "Localisation\tNom\tStatut\n";

    new count;
    for (new i; i < MAX_BILLBOARDS; i++)
        if (BillboardInfo[i][bbRentBy] == PlayerInfo[playerid][pID])
            count++;

    if (count >= 3)
        return SendErrorMessage(playerid, "Vous avez déjà loué le nombre maximum de panneaux publicitaires ! Utilisez /mybb pour les gérer.");

    new location[MAX_ZONE_NAME];
    count = 0;

    for (new i; i < MAX_BILLBOARDS; i++) {
        if (!BillboardInfo[i][bbActive]) continue;

        GetZone(BillboardInfo[i][bbPosX], BillboardInfo[i][bbPosY], location, MAX_ZONE_NAME);

        if (!BillboardInfo[i][bbRentDate]) {
            format(str, sizeof(str), "%s\tPanneau publicitaire #%i\t{16b819}Disponible{FFFFFF}", location, i + 1);
        } else {
            format(str, sizeof(str), "%s\tPanneau publicitaire #%i\t{FF6347}Loué par %s jusqu'au %s{FFFFFF}",
                location, i + 1,
                GetNameFromBD(BillboardInfo[i][bbRentBy]),
                get_date("%dd.%mm.%yy", BillboardInfo[i][bbRentDate]));
        }

        format(mes, sizeof(mes), "%s\n%s", mes, str);
        count++;
    }

    if (!count)
        return SendErrorMessage(playerid, "Aucun panneau publicitaire disponible sur le serveur !");

    Dialog_Show(playerid, BillboardList, DIALOG_STYLE_TABLIST_HEADERS, "Location de panneau publicitaire", mes, "Louer", "Annuler");
    return 1;
}


ALTX:mybillboards("/mybb");
CMD:mybillboards(playerid, params[])
{
    if (PlayerInfo[playerid][pInjured] != 0 ||
        GetPVarInt(playerid, "Freeze") ||
        pTemp[playerid][pTaserTime] ||
        pTemp[playerid][pCuffed]) return 1;

    if (!PlayerInfo[playerid][pNumber])
         return SendErrorMessage(playerid, "Vous avez besoin d'un téléphone pour poster une annonce.");

    new bizid = -1;
    if ((bizid = Advertisement_Nearest(playerid)) == -1)
        return SendErrorMessage(playerid, "Vous n'êtes pas près d'une agence publicitaire.");

    if (BizInfo[bizid][bizLocked] == 1)
        return GameTextForPlayer(playerid, "~r~FERMÉ!", 2500, 1);

    new count, str[90], mes[(60 + MAX_ZONE_NAME + 13) * 3 + 27] = "Localisation\tNom\tStatut\n";
    new location[MAX_ZONE_NAME];

    for (new i; i < MAX_BILLBOARDS; i++)
    {
        if (BillboardInfo[i][bbRentBy] == PlayerInfo[playerid][pID])
        {
            GetZone(BillboardInfo[i][bbPosX], BillboardInfo[i][bbPosY], location, MAX_ZONE_NAME);
            format(str, sizeof(str), "%s\tPanneau publicitaire #%i\tLoué jusqu'au %s{FFFFFF}",
                   location, i + 1, get_date("%dd.%mm.%yy", BillboardInfo[i][bbRentDate]));

            format(mes, sizeof(mes), "%s\n%s", mes, str);
            count++;

            if (count == 3) break;
        }
    }

    if (!count)
        return SendErrorMessage(playerid, "Vous n'avez aucun panneau publicitaire loué!");

    Dialog_Show(playerid, MyBillboards, DIALOG_STYLE_TABLIST_HEADERS,"Vos panneaux publicitaires", mes, "Supprimer", "Annuler");

    return 1;
}


