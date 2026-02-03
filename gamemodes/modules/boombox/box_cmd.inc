ALTX:setstation("/ss");
CMD:setstation(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid), id;

	if (IsDriver(playerid))
	{
		if (VehicleInfo[vehicle][carRadio] == 0)
			return SendErrorMessage(playerid, "Ce véhicule n'a pas de radio.");

		ShowBoomBoxStations(playerid);
		return 1;
	}

	if ((id = GetNearInsideHouse(playerid)) != -1 && HouseInfo[id][hOwnerID] == PlayerInfo[playerid][pID])
	{
		if (HouseInfo[id][hRadio] == 0)
			return SendErrorMessage(playerid, "Votre maison n'a pas de radio.");

		ShowBoomBoxStations(playerid);
		return 1;
	}
	else if ((id = GetNearInsideBiz(playerid)) != -1 && BizInfo[id][bizOwnerID] == PlayerInfo[playerid][pID])
	{
		if (BizInfo[id][bizRadio] == 0)
			return SendErrorMessage(playerid, "Votre entreprise n'a pas de radio.");

		ShowBoomBoxStations(playerid);
		return 1;
	}
	return 1;
}