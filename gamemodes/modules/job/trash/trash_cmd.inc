ALTX:pickuptrash("/ptrash");
CMD:pickuptrash(playerid, params[])
{
    if (PlayerInfo[playerid][pJob] != JOB_TRASHMASTER)	return SendErrorMessage(playerid, "Vous n'êtes pas éboueur.");
	if (IsPlayerInAnyVehicle(playerid))                  return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette commande dans un véhicule !");
    if (HasTrash[playerid])                              return SendErrorMessage(playerid, "Vous portez déjà un sac poubelle.");

	if (!pTemp[playerid][pRentveh])  return SendErrorMessage(playerid, "Vous devez d'abord louer un véhicule de travail ! (/rcj)");
    if (!pTemp[playerid][pJob])      return SendErrorMessage(playerid, "Vous devez d'abord louer un véhicule de travail ! (/rcj)");
    
    new vehicleid = pTemp[playerid][pRentveh];
    if (!IsTrashMasterCar(vehicleid) || !IsATrashTruck(vehicleid)) return SendErrorMessage(playerid, "Vous n'êtes pas au volant d'un camion poubelle !");

    new id = Trash_Nearest(playerid);
	if (id == -1)                      return SendErrorMessage(playerid, "Aucune benne à ordures proche !");
	if (TrashData[id][TrashLevel] < 1)  return SendErrorMessage(playerid, "Cette benne est vide !");

    new Float: x, Float: y, Float: z;
    GetVehicleBoot(vehicleid, x, y, z);
    if (GetPlayerDistanceFromPoint(playerid, x, y, z) >= 30.0) return SendErrorMessage(playerid, "Vous êtes trop loin du camion poubelle !");

    TrashData[id][TrashLevel]--;
	KillTimer(TrashData[id][TrashTimer]);
    
    TrashData[id][TrashTimer] = SetTimerEx("FillTrash", REFILL_TIME * 1000, false, "i", id);
	
    TrashCP[playerid] = CreateDynamicCP(x, y, z, 3.0, .playerid = playerid);
	HasTrash[playerid] = true;
	
    ApplyAnimation(playerid, "CARRY", "liftup105", 4.1, 0, 0, 0, 0, 0);
	SetPlayerAttachedObject(playerid, ATTACHMENT_INDEX, 1264, 6, 0.222, 0.024, 0.128, 1.90, -90.0, 0.0, 0.5, 0.5, 0.5);
	
    Streamer_SetIntData(STREAMER_TYPE_3D_TEXT_LABEL, TrashData[id][TrashLabel], E_STREAMER_COLOR, (TrashData[id][TrashLevel] == 0) ? 0xE74C3CFF : 0xF39C12FF);
	
    InfoMSG(playerid, "Apportez ~h~~g~le sac poubelle~w~ au camion~n~Vous pouvez jeter le sac avec ~r~~k~~CONVERSATION_NO~", 4);
	return 1;
}