CMD:taxi(playerid, params[])
{
	new option[11], secoption, vehicle = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][pJob] != JOB_TAXI)	return SendErrorMessage(playerid, "Vous n'êtes pas un chauffeur de taxi.");
	if (sscanf(params,"s[11]D(-1)", option, secoption))
	{
		SendHelpMessage(playerid, "/taxi [action]");
		SendSyntaxMessage(playerid, "duty / fare / accept");
		return 1;
	}

	if (!IsDriver(playerid))	return SendErrorMessage(playerid, "Vous devez être au volant d'un Taxi ou Cabbie.");
	if (!IsATaxi(vehicle))		return SendErrorMessage(playerid, "Vous devez être dans un Taxi ou un Cabbie.");

	if (CompareStrings(option, "duty"))
	{
		if (TaxiDuty[playerid])
		{
			SendClientMessage(playerid, COLOR_YELLOWG, "[TAXI] Vous êtes hors service, vous ne recevrez plus d'appels.");
			SendClientMessageEx(playerid, COLOR_YELLOWG, "[TAXI] Vous avez reçu $%i.", TaxiMade[playerid]);
			GiveCash(playerid, TaxiMade[playerid]);
			SetPlayerColor(playerid, TEAM_HIT_COLOR);
			TaxiDuty[playerid] = false;
			TaxiMade[playerid] = 0;
			TaxiMoney[playerid] = 0;
			TaxiPassenger[playerid] = INVALID_PLAYER_ID;
			return 1;
		}

		SendClientMessageEx(playerid, COLOR_YELLOWG, "[TAXI] Vous êtes en service. Prix: $%i/5 secondes.", TaxiFare[playerid]);
		SetPlayerColor(playerid, TAXI_DUTY);
		TaxiDuty[playerid] = true;
		TaxiMoney[playerid] = 0;
		TaxiMade[playerid] = 0;
		TaxiPassenger[playerid] = INVALID_PLAYER_ID;

		foreach(new i : Player)
		{
		    if (!IsPlayerLogged(i) || !IsPlayerInVehicle(i, vehicle) || GetPlayerState(i) != PLAYER_STATE_PASSENGER) continue;

			TaxiPassenger[playerid] = i;
			SendClientMessageEx(i, COLOR_YELLOWG, "[TAXI] Vous êtes monté avec %s. Tarif: $%i/5 secondes.", GetNameEx(playerid), TaxiFare[playerid]);
			break;
		}

		return 1;
	}
	else if (CompareStrings(option, "fare"))
	{
		new fare;

		if (sscanf(params,"{s[11]}d",fare))	return SendHelpMessage(playerid, "/taxi fare [prix]");
		if (fare < 0 || fare > 25)				return SendErrorMessage(playerid, "Le tarif doit être entre $0 et $25.");
		if (IsPlayerLogged(TaxiPassenger[playerid]))	return SendErrorMessage(playerid, "Finissez la course avant de changer le tarif.");

		SendClientMessageEx(playerid, COLOR_YELLOWG, "[TAXI] Tarif modifié à $%i/5 secondes.", fare);
		TaxiFare[playerid] = fare;
	}
	else if (CompareStrings(option, "accept"))
	{
		new id, Float:x, Float:y, Float:z;

		if (sscanf(params,"{s[11]}u",id))	return SendHelpMessage(playerid, "/taxi accept [ID/nom]");
		if (!TaxiDuty[playerid])				return SendErrorMessage(playerid, "Vous devez être en service pour accepter des appels (/taxi duty).");
		if (!IsPlayerLogged(id))				return SendNotConnected(playerid);
		if (GetPVarInt(id, "NeedTaxi") == 0)	return SendErrorMessage(playerid, "Ce joueur n'a pas demandé de taxi ou l'appel a déjà été accepté.");

		SendSyntaxMessage(id, "%s a accepté votre appel. Tarif: $%i/5 secondes.", GetNameEx(playerid), TaxiFare[playerid]);
		SendSyntaxMessage(playerid, "Vous avez accepté l'appel de %s, tél.%i", GetNameEx(id), PlayerInfo[id][pNumber]);
		GetPlayerPos(id, x, y, z);
		Waypoint_Set(playerid, x, y, z, 3.0);
		DeletePVar(id, "NeedTaxi");
	}
	return 1;
}