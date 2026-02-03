CMD:stretcher(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionMedic(faction) && !IsFactionFire(faction))
		return SendErrorMessage(playerid, "Vous devez être un ambulancier ou un pompier.");
	if (IsPlayerInAnyVehicle(playerid))
		return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");

	if (pTemp[playerid][UsingBort]) {
		new idx = pTemp[playerid][UsingBort]-1, modelid = GetDynamicObjectModel(BortInfo[playerid][idx][bortObject]);

		DestroyDynamicObject(BortInfo[playerid][idx][bortObject]);

		new Float:x, Float:y, Float:z, Float:R;
		GetPlayerFacingAngle(playerid, R);
	  	GetXYInFrontOfPlayer(playerid, x, y, z, 1.5);
		BortInfo[playerid][idx][bortObject] = CreateDynamicObject(modelid, x, y, z - 0.5, 0.0, 0.0, 0.0, GetWorld(playerid), GetInterior(playerid));
		
		if (pTemp[playerid][UsingCorpse]) {
			new actorid = pTemp[playerid][UsingCorpse];
			SetActorPos(actorid, x, y, z + 0.60);
			SetActorFacingAngle(actorid, R);
			pTemp[playerid][UsingCorpse] = 0;
		}

		BortInfo[playerid][idx][bortUsing] = 0;
		pTemp[playerid][UsingBort] = 0;

		SendMedicMessage(playerid, "Vous avez relâché la civière.");
		return 1;
	}

    if (!PlayerInfo[playerid][pOnDuty])
		return SendErrorMessage(playerid, "Vous devez être en service pour utiliser cette fonction.");

	new idx = -1;
	for(new i = 0; i < MAX_BORTES; i++)
	{
	    if (!BortInfo[playerid][i][bortOn]) continue;
		if (BortInfo[playerid][i][bortUsing] != 0) continue;

		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(BortInfo[playerid][i][bortObject], x, y, z);

		if (GetPlayerDistanceFromPoint(playerid, x, y, z) > 1.5) continue;

		new modelid = GetDynamicObjectModel(BortInfo[playerid][i][bortObject]);
		if (modelid != 1997 && modelid != 2146) continue;

		idx = i;
		break;
	}
	
	if (idx == -1)
		return SendErrorMessage(playerid, "Aucune civière proche de vous ! (/bort)");

	BortInfo[playerid][idx][bortUsing] = playerid+1;
	pTemp[playerid][UsingBort] = idx+1;
	pTemp[playerid][UsingCorpse] = 0;

	AttachDynamicObjectToPlayer(BortInfo[playerid][idx][bortObject], playerid, 0.00, 1.50, -0.50, 0.0, 0.0, 0.0);

    SendMedicMessage(playerid, "Vous avez pris une civière.");
	return 1;
}

CMD:therapy(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], userid;

	if (!IsFactionMedic(faction))					return SendErrorMessage(playerid, "Vous devez être un ambulancier.");
	if (PlayerInfo[playerid][pRank] < 3)				return SendErrorMessage(playerid, "Disponible à partir du 3ème rang.");
	if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (sscanf(params, "u", userid))					return SendHelpMessage(playerid, "/therapy [ID/Nom du joueur]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)							return SendErrorMessage(playerid, "ID incorrect.");
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pDependence] == 0)		return SendErrorMessage(playerid, "Ce joueur n'a pas de dépendance.");

    PlayerInfo[userid][pDependence] -= (PlayerInfo[userid][pDependence] < 20) ? (PlayerInfo[userid][pDependence]) : (20);
    pTemp[userid][pDemolitionTime] = 0;
	SetPlayerCurrentTimeAndWeather(userid);
	
	SendMedicMessage(userid, "%s %s a réduit votre dépendance de 20 points.", GetRankName(playerid), GetNameWithMask(playerid));
    SendMedicMessage(playerid, "Vous avez réduit la dépendance de %s de 20 points.", GetNameWithMask(userid));

	return 1;
}

CMD:prescribe(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], userid, days;

	if (!IsFactionMedic(faction))					return SendErrorMessage(playerid, "Vous devez être ambulancier.");
	if (PlayerInfo[playerid][pRank] < 3)				return SendErrorMessage(playerid, "Disponible à partir du 3? rang.");
	if (!PlayerInfo[playerid][pOnDuty])				return SendErrorMessage(playerid, "Vous devez être en service (/duty).");		
	if (sscanf(params,"uI(0)",userid, days))			return SendHelpMessage(playerid, "/prescribe [ID/Nom du joueur] [durée en jours (1-93)]");
	if (!IsPlayerLogged(userid))						return SendNotConnected(playerid);
	if (userid == playerid)							return SendErrorMessage(playerid, "ID incorrect.");
	if (!PlayerNearPlayer(3.0, playerid, userid))	return SendNotNearPlayer(playerid);	

	if (PlayerInfo[userid][pPrescribe] < gettime())
	{
	    if (days < 1 || days > 93)					return SendErrorMessage(playerid, "Durée du traitement invalide. (1-93 jours)");
	        
	    PlayerInfo[userid][pPrescribe] = gettime() + days * 86400;
	    SendMedicMessage(userid, "%s %s vous a délivré une ordonnance pour de la marijuana médicale.", GetRankName(playerid), GetNameWithMask(playerid));
		SendHelpMessage(userid, "Utilisez /showprescribe pour montrer votre ordonnance.");
        SendMedicMessage(playerid, "Vous avez délivré une ordonnance de marijuana médicale à %s.", GetNameWithMask(userid));
	}
	else
	{
	    PlayerInfo[userid][pPrescribe] = 0;
	    SendMedicMessage(userid, "%s %s vous a retiré votre ordonnance de marijuana médicale.", GetRankName(playerid), GetNameWithMask(playerid));
        SendMedicMessage(playerid, "Vous avez retiré l'ordonnance de marijuana médicale de %s.", GetNameWithMask(userid));
	}
	return 1;
}

CMD:operation(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], userid;

	if (!IsFactionMedic(faction) && !IsFactionFire(faction)) return SendErrorMessage(playerid, "Vous devez être ambulancier ou pompier.");
	if (!PlayerInfo[playerid][pOnDuty])						return SendErrorMessage(playerid, "Vous devez être en service (/duty).");
	if (sscanf(params, "u", userid))						return SendHelpMessage(playerid, "/operation [ID/Nom du joueur]");
	if (!IsPlayerLogged(userid))							return SendNotConnected(playerid);
	if (userid == playerid)									return SendErrorMessage(playerid, "Vous ne pouvez pas vous soigner vous-même.");
	if (!PlayerNearPlayer(3.0, playerid, userid))			return SendNotNearPlayer(playerid);
	if (PlayerInfo[userid][pInjured] != 1)					return SendErrorMessage(playerid, "Le joueur n'est pas blessé actuellement.");
	if (pTemp[userid][pInvulnerable] != 0.0)				return SendErrorMessage(playerid, "Essayez de nouveau dans quelques instants.");

	ResetDamages(userid);
	SetPlayerHealth(userid, 100.0);
	StopPlayerAnims(userid);
	UnFreezePlayer(userid);

	SendMedicMessage(playerid, "Vous avez stabilisé %s.", GetNameWithMask(userid));
	SendMedicMessage(userid, "%s %s vous a stabilisé.", GetRankName(playerid), GetNameWithMask(playerid));
	return 1;
}

CMD:hpc(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction], id, vehicle = GetPlayerVehicleID(playerid);

	if (!IsFactionMedic(faction) && !IsFactionFire(faction))	return SendErrorMessage(playerid, "Vous devez être ambulancier ou pompier.");
	if (sscanf(params, "u", id))								return SendHelpMessage(playerid, "/hpc [ID/Nom du joueur]");
	if (!IsPlayerLogged(id))									return SendNotConnected(playerid);
	if (VehicleInfo[vehicle][carFaction] != faction)			return SendErrorMessage(playerid, "Vous devez être dans un véhicule de votre organisation.");

	SendMedicMessage(playerid, "Informations - %s", GetFullDate());
	SendMedicMessage(playerid, "Nom complet: %s, Date de naissance: %s, Numéro de téléphone: %i.", GetNameEx(id), PlayerInfo[id][pBirthdate], PlayerInfo[id][pNumber]);

	new count;
	for (new i = 0; i < MAX_HOUSES; i++)
	{
	    if (HouseInfo[i][hID] == 0 || HouseInfo[i][hOwnerID] != PlayerInfo[id][pID]) continue;

		SendMedicMessage(playerid, "Adresse enregistrée: %s.", (!HouseInfo[i][hApart]) ? (GetHouseZone(i)) : (GetApartmentZone(HouseInfo[i][hApart])));
		count++;
		if (count > 4) break;
	}

	if (!count) SendMedicMessage(playerid, "Aucune adresse enregistrée.");

	return 1;
}

CMD:choke(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionFire(faction))			return SendErrorMessage(playerid, "Vous devez être pompier.");
	if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Vous devez d'abord commencer votre service (/duty).");

	if (DeleteNearFire(playerid))
		return SM1(playerid, "Vous avez éteint le feu.");

	SM2(playerid, "Il n'y a aucun feu à proximité !");
	return 1;
}

CMD:fireshape(playerid, params[])
{
	new faction = PlayerInfo[playerid][pFaction];
	
	if (!IsFactionFire(faction)) 		return SendErrorMessage(playerid, "Vous devez être pompier.");
	if (!PlayerInfo[playerid][pOnDuty])	return SendErrorMessage(playerid, "Vous devez d'abord commencer votre service (/duty).");
	if (!PlayerInfo[playerid][pChar])	return SendErrorMessage(playerid, "Vous devez être en uniforme de service.");

	if (GetPlayerSkin(playerid) == PlayerInfo[playerid][pChar])
	{
		if (PlayerInfo[playerid][pChar] == 277 || PlayerInfo[playerid][pChar] == 278 
		|| PlayerInfo[playerid][pChar] == 279 || PlayerInfo[playerid][pChar] == 191)
			return SendErrorMessage(playerid, "Vous portez déjà un uniforme de pompier!");

		Dialog_Show(playerid, MODEL_SELECTION_SHAPE, DIALOG_STYLE_PREVIEW_MODEL, "Choix de l'uniforme", "277\tID: 277\n278\tID: 278\n279\tID: 279\n191\tID: 191", "Choisir", "Fermer");
	}
	else
	{
		SetPlayerSkin(playerid, PlayerInfo[playerid][pChar]);
	}
	return 1;
}

