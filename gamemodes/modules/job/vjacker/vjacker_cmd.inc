CMD:livrerveh(playerid, params[])
{
	if (IsPoliceNear(playerid)) return InfoMSG(playerid, "UN POLICIER EST PROCHE DE VOUS!", 4);
	if (!PoliceOnline()) return SendErrorMessage(playerid, "Il doit y avoir au moins %i policiers en ligne.", NEED_COPS_TO_CRIME);

	new id = GetChopshopID(playerid), vehID = GetPlayerVehicleID(playerid);
	
	if (PlayerInfo[playerid][pJob] != JOB_CARJACKER) return SendErrorMessage(playerid, "Vous n'êtes pas un voleur de voitures.");
	if (id == -1 || !IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Il n'y a pas d'atelier proche de vous, ou vous n'êtes pas au volant d'un véhicule.");
	
	if (VehicleInfo[vehID][carOwnerID] == PlayerInfo[playerid][pID] || PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehID][carOwnerID])
		return SendErrorMessage(playerid, "Vous ne pouvez pas démonter votre propre véhicule.");

	if (VehicleInfo[vehID][carOwnerID] == 0) return SendErrorMessage(playerid, "Seuls les véhicules personnels peuvent être volés.");
	if (PlayerInfo[playerid][InMission]) return SendErrorMessage(playerid, "Vous êtes déjà en mission.");
	if (!CheckWantedModel(id, GetVehicleModel(vehID))) return SendErrorMessage(playerid, "Ce véhicule n'est pas recherché.");
	
	new count, time[64];
	for (new i; i < 14; i++) if (VehicleInfo[vehID][carComponent][i] > 0) count++;

	if (VehicleInfo[vehID][carEngineOn]) ToggleVehicleEngine(vehID, playerid);

	PlayerInfo[playerid][InMission] = CARJACKER_DELIVER;
	PlayerInfo[playerid][MissionTime] = 15 + count;
	PlayerInfo[playerid][MissionTarget][0] = vehID;
	PlayerInfo[playerid][MissionTarget][1] = id;
			
	format(time, sizeof(time), "~r~DÉMONTAGE DU VÉHICULE~n~~w~IL RESTE ~y~%d ~w~SECONDES", PlayerInfo[playerid][MissionTime]);
	InfoMSG(playerid, time, 2);
	
	return 1;
}

CMD:leavemission(playerid, params[])
{
	if (!PlayerInfo[playerid][InMission]) return SendErrorMessage(playerid, "Vous n'êtes pas en mission.");
	LeaveMission(playerid);
	return 1;
}

CMD:dropoff(playerid, params[])
{
	if (IsPoliceNear(playerid)) return InfoMSG(playerid, "UN POLICIER EST PROCHE DE VOUS!", 4);

	new vehID = PlayerInfo[playerid][MissionTarget][0],
		id = PlayerInfo[playerid][MissionTarget][1],
		Float: playerPos[3];

	GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

	if (PlayerInfo[playerid][pJob] != JOB_CARJACKER) return SendErrorMessage(playerid, "Vous n'êtes pas un voleur de voitures.");
	if (!PlayerInfo[playerid][InMission]) return SendErrorMessage(playerid, "Vous n'êtes pas en mission.");
	if (PlayerInfo[playerid][InMission] != CARJACKER_DROPOFF) return SendErrorMessage(playerid, "Vous ne livrez pas de véhicule volé.");
	if (GetPlayerVehicleID(playerid) != PlayerInfo[playerid][MissionTarget][0]) return SendErrorMessage(playerid, "Ce n'est pas le bon véhicule!");

	if (!IsPlayerInRangeOfPoint(playerid, 50.0, chopshop_data[id][chopshop_pos][0], chopshop_data[id][chopshop_pos][1], chopshop_data[id][chopshop_pos][2]))
	{
		new Float: distance_reward = XB_GetDistanceBetweenTPoints(playerPos[0], playerPos[1], playerPos[2], chopshop_data[id][chopshop_pos][0], chopshop_data[id][chopshop_pos][1], chopshop_data[id][chopshop_pos][2]);

		if (distance_reward > 200.0) PlayerInfo[playerid][MissionReward] += 400;
		else PlayerInfo[playerid][MissionReward] += floatround(distance_reward) * 2;

		new money[32];
		format(money, 32, "~g~MISSION ACCOMPLIE~n~~w~VOUS AVEZ GAGNÉ ~y~$%s", FormatNumber(PlayerInfo[playerid][MissionReward]));
		InfoMSG(playerid, money, 5);

		GiveCash(playerid, PlayerInfo[playerid][MissionReward]);

		new userid;
		foreach (new i : Player)
		{
			if (PlayerInfo[i][pID] == VehicleInfo[vehID][carOwnerID])
			{
				userid = i;
				break;
			}
		}

		SendClientMessageEx(userid, COLOR_LIGHTRED, "[ VÉHICULE ] Votre %s a été volé. (/mesveh)", VehicleNames[GetVehicleModel(vehID) - 400]);

		VehicleInfo[vehID][carDestroyed]++;

		SQL_SetInteger("cars", "jacking", 1, VehicleInfo[vehID][carID]);
		SQL_SetInteger("cars", "destroys", VehicleInfo[vehID][carDestroyed], VehicleInfo[vehID][carID]);

		new money_str[70];
		format(money_str, sizeof(money_str), "VOUS AVEZ ~g~RÉUSSI ~w~À VOUS DÉBARRASSER DU VÉHICULE~n~ET GAGNÉ ~g~$%i", PlayerInfo[playerid][MissionReward]);
		InfoMSG(playerid, money_str, 5);
		RemovePlayerFromVehicle(playerid);

		ResetVehicleStuff(vehID);
		DestroyVehicle(vehID);

		PlayerInfo[playerid][MissionReward] = 0;
		PlayerInfo[playerid][InMission] = MISSION_NONE;
		PlayerInfo[playerid][MissionTime] = 0;
		PlayerInfo[playerid][MissionTarget][0] = 0;
		PlayerInfo[playerid][MissionTarget][1] = 0;
	}
	else return SendErrorMessage(playerid, "Vous êtes trop proche de l'atelier!");

	return 1;
}