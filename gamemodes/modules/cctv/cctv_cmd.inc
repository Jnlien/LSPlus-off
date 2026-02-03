CMD:cctv(playerid, params[])
{
	new id;

	if (!IsPlayerFactionPolice(playerid))
		return SendErrorMessage(playerid, "Vous devez être un policier.");

	new option[16], secoption[128];

	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendSyntaxMessage(playerid, "/cctv [commande]");
		SendHelpSyntaxMessage(playerid, "liste, voir, quitter");
		return 1;
	}

	if (CompareStrings(option, "voir"))
	{
		if (sscanf(secoption,"d", id))
			return SendSyntaxMessage(playerid, "/cctv voir [ID de la caméra]");
		if (id < 0 || id > MAX_CCTV-1 || !CCTVInfo[id][tvObjectOn])
			return SendErrorMessage(playerid, "Caméra introuvable.");
		if (CCTVInfo[id][tvBreak])
			return SendErrorMessage(playerid, "Caméra hors service.");

    	if (!pTemp[playerid][pCCTV])
		{
			SavePlayerPos(playerid);
			pTemp[playerid][pCCTV] = 1;
			SetPVarInt(playerid, "CCTV:ID", id + 1);

			if (IsPlayerInAnyVehicle(playerid)) {
				SetPVarInt(playerid, #CCTV_VEH, GetPlayerVehicleID(playerid));
				SetPVarInt(playerid, #CCTV_VEH_SEAT, GetPlayerVehicleSeat(playerid));
				RemovePlayerFromVehicle(playerid);
			}

			SetInterior(playerid, CCTVInfo[id][tvInt]);
			SetWorld(playerid, CCTVInfo[id][tvVW]);

			if (MdcInfo[playerid][mdcCreated]) {
				MDC_Hide(playerid);
				SetPVarInt(playerid, #CCTV_MDC_USE, 1);
			}

			for(new i; i < 11; i++) TextDrawShowForPlayer(playerid, zond_cam[i]);

			SetTimerEx("CCTV_PLA_TIMER", 1000, false, "ii", playerid, id);
		}
	}
	else if (CompareStrings(option, "liste"))
	{
		new dialog_header[96] = "{FFFFFF}Caméra\t{FFFFFF}ID\t{FFFFFF}Statut\n", dialog_body[3072];
		format(dialog_body, sizeof(dialog_body), "%s", dialog_header);

		new count;
		for(new i = 0; i < MAX_CCTV; i++)
		{
			if (CCTVInfo[i][tvObjectOn])
			{
				new status_str[24];
				count++;

				if (CCTVInfo[i][tvBreak])	format(status_str, sizeof(status_str), "{CC0000}Hors service");
				else						format(status_str, sizeof(status_str), "{00B200}Fonctionnelle");

				format(dialog_body, sizeof(dialog_body), "%s%s\t%i\t%s\n", dialog_body, CCTVInfo[i][tvName], i, status_str);
			}
		}

		if (!count)	dialog_body = "Aucune caméra disponible.";
		Dialog_Show(playerid, None, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Liste des caméras", dialog_body, "Fermer", "");
		return 1;
	}
	else if (CompareStrings(option, "quitter"))
	{
		if (!pTemp[playerid][pCCTV])
			return SendErrorMessage(playerid, "Vous n'êtes pas en mode caméra.");

		TogglePlayerSpectating(playerid, 0);		

		SetInterior(playerid, PlayerInfo[playerid][pInt]);
		SetWorld(playerid, PlayerInfo[playerid][pWorld]);

		for(new i; i < 11; i++) TextDrawHideForPlayer(playerid, zond_cam[i]);

		if (GetPVarInt(playerid, #CCTV_MDC_USE) && !MdcInfo[playerid][mdcCreated]) {
			MDC_Show(playerid, PAGE_CCTV);
			DeletePVar(playerid, #CCTV_MDC_USE);
		}

		if (GetPVarInt(playerid, #CCTV_VEH))
			SetTimerEx("CCTV_VEH_TIMER", 1000, false, "i", playerid); 
	}
	else 
	{
		SendSyntaxMessage(playerid, "/cctv [commande]");
		SendHelpSyntaxMessage(playerid, "liste, voir, quitter");
	}
	return 1;
}


this::CCTV_VEH_TIMER(playerid) 
{
	PutPlayerInVehicle(playerid, GetPVarInt(playerid, #CCTV_VEH), GetPVarInt(playerid, #CCTV_VEH_SEAT));
	DeletePVar(playerid, #CCTV_VEH);
	DeletePVar(playerid, #CCTV_VEH_SEAT);
	return 1;
}

this::CCTV_PLA_TIMER(playerid, id)
{
	TogglePlayerSpectating(playerid, 1);
	AttachCameraToDynamicObject(playerid, CCTVInfo[id][tvObject]);
	SendHelpMessage(playerid, "Utilisez la souris pour contrôler la caméra, {DABB3E}/cctv quitter{FFFFFF} pour quitter le mode de surveillance.");
	return 1;
}