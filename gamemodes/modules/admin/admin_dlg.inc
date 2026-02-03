Admin_OnPlayerClickPlayer(playerid, clickedplayerid)
{
	if (PlayerInfo[playerid][pAdmin] > 0) 
    {
        new string[896], str[120];
		ClickedPlayerID[playerid] = clickedplayerid;

		if (!IsAdminDuty(playerid)) return SendNotDuty(playerid);
        
		if (CheckAdmin(playerid, ADMIN_LEVEL_1))
		{
			strcat(string,"{7EA0DA}[UTILE]{FFFFFF} Voir les stats\n");
			strcat(string,"{7EA0DA}[UTILE]{FFFFFF} Voir les personnages\n");
			strcat(string,"{7EA0DA}[UTILE]{FFFFFF} Historique des sanctions\n");
			strcat(string,"{7EA0DA}[TP]{FFFFFF} Se téléporter à lui\n");
			strcat(string,"{7EA0DA}[TP]{FFFFFF} Le téléporter à moi\n");
			strcat(string,"{7EA0DA}[UTILE]{FFFFFF} Surveiller le joueur (/spec)\n");
		}

		format(str,sizeof(str),"Panel d'administration (%s [%d])", GetNameEx(clickedplayerid), clickedplayerid);
	    Dialog_Show(playerid, paneladmin, DIALOG_STYLE_LIST, str, string, "Valider", "Fermer");
    }
	return 1;
}

Dialog:paneladmin(playerid, response, listitem, inputtext[])
{
	new string[156];
	if (!response) return 1;
	if (response)
	{
		format(string, sizeof(string), "%d", ClickedPlayerID[playerid]);
		switch(listitem)
		{
			case 0: return cmd_getstats(playerid, string);
			case 1: return cmd_voirperso(playerid, string);
			case 2: return cmd_voirsanction(playerid, string);
			case 3: return cmd_goto(playerid, string); 
			case 4: return cmd_gethere(playerid, string);
			case 5: return cmd_spectate(playerid, string);
		}
	}
	return 1;
}