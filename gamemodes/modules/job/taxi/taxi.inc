Taxi_OnPlayerStateChange(playerid, newstate)
{
    if (newstate == PLAYER_STATE_PASSENGER)
	{
	    new vehicle = GetPlayerVehicleID(playerid), driver = VehicleInfo[vehicle][carDriver];
        if (IsPlayerLogged(driver) && TaxiDuty[driver] && TaxiPassenger[driver] == INVALID_PLAYER_ID)
        {
            if (GetCash(playerid) < TaxiFare[driver])
			{
		 		SendErrorMessage(playerid, "Vous n'avez pas assez d'argent.");
		 		SendErrorMessage(driver, "Le passager n'a pas assez d'argent.");
		 		return 1;
			}
			TaxiPassenger[driver] = playerid;
            SendClientMessageEx(playerid, COLOR_YELLOWG, "[TAXI] Vous êtes monté dans le taxi de %s. Tarif : $%i / 5 secondes.", GetNameEx(driver), TaxiFare[driver]);
		}
	}
    return 1;
}