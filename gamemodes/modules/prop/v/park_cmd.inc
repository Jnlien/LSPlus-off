CMD:paypark(playerid, params[])
{
    new id = -1, 
        vid = -1, 
        Float:x, Float:y, Float:z;
    
    if (IsPlayerInAnyVehicle(playerid))				 return SendErrorMessage(playerid, "Vous devez être en dehors d'un véhicule.");
    if ((id = ParkMeter_Nearest(playerid)) == -1)     return SendErrorMessage(playerid, "Il n'y a pas de parcmètre à proximité.");
    if (!pMeter[id][parkAvailable])                   return SendErrorMessage(playerid, "Le stationnement est déjà payé !"); 

    if (GetPlayerMoney(playerid) < CASH_PARKING)      return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent. Vous avez besoin de $%d pour payer le stationnement.", CASH_PARKING);
        
    for(new i = 1; i < MAX_VEHICLES; i++)
    {
        if (IsValidVehicle(i) && (!IsAPlane(i) || !IsABiker(i) || !IsAHelicopter(i) || !IsABoat(i) || !IsABicycle(i)))
        {
            GetVehiclePos(i, x, y, z);
            if (IsPlayerInRangeOfPoint(playerid, 6.0, x, y, z))
            {
                vid = i;
                break;
            }
        }
    }

    if (vid == -1) return SendErrorMessage(playerid, "Il n'y a pas de véhicule approprié à proximité.");

    SendClientMessageEx(playerid, 0x33AA33FF, "> Vous avez payé le stationnement de votre %s pour $%d.", VehicleNames[GetVehicleModel(vid)-400], CASH_PARKING);
        
    GivePlayerMoney(playerid, -CASH_PARKING);

    pMeter[id][parkVehicle] = vid;
    pMeter[id][parkAvailable] = false;
        
    return 1;
}