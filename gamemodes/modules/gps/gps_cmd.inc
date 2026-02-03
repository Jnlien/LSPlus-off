CMD:gps(playerid, params[])
{
	Dialog_Show(playerid, DIALOG_GPS, DIALOG_STYLE_LIST, "GPS > Menu principal", "1. Lieux principaux\n2. Entreprises\n3. Métiers", "Choisir", "Annuler");
	return 1;
}


ALTX:removecp("/rcp");
CMD:removecp(playerid, params[])
{
	SendSyntaxMessage(playerid, "Tous les checkpoints ont été retirés de la carte.");
	DisableWaypoint(playerid);
	return 1;
}
