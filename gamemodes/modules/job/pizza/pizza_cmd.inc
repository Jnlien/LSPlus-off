CMD:takepizza(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (!pTemp[playerid][pPizzaBoy])								return SendErrorMessage(playerid, "Vous n'effectuez pas de commande.");
	if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))	return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");
		
	new vehicle = GetPlayerVehicleID(playerid);

	if (vehicle != 0)																		return SendErrorMessage(playerid, "Vous devez être hors du véhicule.");
	if ((vehicle = GetNearVehicle(playerid, 3.0)) == 0 || GetVehicleModel(vehicle) != 448)	return SendErrorMessage(playerid, "Vous devez être près d'un scooter avec des pizzas.");
	
	SetPlayerAttachedObject(playerid, ATTACH_HANDS, 19571, 6, -0.157999, -0.002000, -0.169000, -21.999990, 87.000022, 0.000000, 1.186000, 1.000000, 1.170000);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	InfoMSG(playerid, "APPUYEZ SUR \" ~r~ENTER ~w~\" POUR DÉPOSER LA PIZZA.", 8);
	return 1;
}

CMD:pizzajob(playerid, params[])
{
	if (pTemp[playerid][pPizzaBoy])
	{
		pTemp[playerid][pPizzaBoy] = 0;
		SendSyntaxMessage(playerid, "Vous avez annulé la livraison. Une nouvelle commande sera disponible dans deux minutes.");
		DisableWaypoint(playerid);
		SetPVarInt(playerid, #pizza_time, gettime() + 120);
		return 1;
	}
	
	if (GetPVarInt(playerid, #pizza_time) > gettime())								return SendErrorMessage(playerid, "[Radio]: Pas de commandes disponibles, réessayez plus tard.");
	if (!IsDriver(playerid) || GetVehicleModel(GetPlayerVehicleID(playerid)) != 448)	return SendErrorMessage(playerid, "Vous devez être au volant d'un scooter.");
		
	new i, houseid = random(MAX_HOUSES);
		
	while (i == 0)
	{
		if (!HouseInfo[houseid][hID] || HouseInfo[houseid][hPosInt] || HouseInfo[houseid][hPosVW])
		{
			houseid = random(MAX_HOUSES);
		}
		else
		{
			i = 1;
			pTemp[playerid][pPizzaBoy] = houseid + 1;
			SendClientMessageEx(playerid, COLOR_GREEN, "[Radio]: Nouvelle commande. Livrez la pizza à l'adresse : %s", (!HouseInfo[houseid][hApart]) ? (GetHouseZone(houseid)) : (GetApartmentZone(HouseInfo[houseid][hApart])));
			SendHelpMessage(playerid, "Utilisez /takepizza pour prendre la pizza en main.");

			if (!HouseInfo[houseid][hApart]) 
				Waypoint_Set(playerid, HouseInfo[houseid][hPosX], HouseInfo[houseid][hPosY], HouseInfo[houseid][hPosZ]);
			else 
				Waypoint_Set(playerid, ApartInfo[houseid][aPosX], ApartInfo[houseid][aPosY], ApartInfo[houseid][aPosZ]);

			return 1;
		}
	}
	return 1;
}
