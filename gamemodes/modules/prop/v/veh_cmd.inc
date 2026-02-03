CMD:rentcar(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!IsDriver(playerid))				return 1;
	if (!IsRentCar(vehicle))				return SendErrorMessage(playerid, "Ce véhicule ne peut pas être loué.");
	if (VehicleInfo[vehicle][carRent])	return SendErrorMessage(playerid, "Ce véhicule est déjà loué.");
	if (pTemp[playerid][pRentveh])		return SendErrorMessage(playerid, "Vous louez déjà un véhicule. Tapez /unrentcar pour arrêter la location.");
	if (GetCash(playerid) < 300)			return SendNotCash(playerid);
		
	GiveCash(playerid, -300);
	pTemp[playerid][pRentveh] = vehicle;
	VehicleInfo[vehicle][carRent] = PlayerInfo[playerid][pID];
	SendSyntaxMessage(playerid, "Vous avez loué %s pour $300. Commandes disponibles : /unrentcar, /v lock.", VehicleNames[GetVehicleModel(vehicle)-400]);
	SendSyntaxMessage(playerid, "Attention ! Si vous n'utilisez pas le véhicule pendant 10 minutes, il disparaîtra.");
	return 1;
}

CMD:unrentcar(playerid, params[])
{
    if (!pTemp[playerid][pRentveh]) return 1;
		
	new vehicle = pTemp[playerid][pRentveh];

	SendSyntaxMessage(playerid, "Vous avez arrêté la location de %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
	pTemp[playerid][pRentveh] = 0;
    VehicleInfo[vehicle][carRent] = 0;
    RespawnVehicle(vehicle);
	return 1;
}

CMD:unimpound(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
	if (!vehicle)														return SendNotInVehicle(playerid);
	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
	if (!VehicleInfo[vehicle][Impounded])								return SendErrorMessage(playerid, "Votre véhicule n'est pas confisqué.");
	if (GetCash(playerid) < IMPOUND_FEE) 								return SendErrorMessage(playerid, "Fonds insuffisants pour effectuer cette opération ($%i).", IMPOUND_FEE);
	
	GiveCash(playerid, -IMPOUND_FEE);
	VehicleInfo[vehicle][Impounded] = 0;
	Save_Car(vehicle);

	SendSyntaxMessage(playerid, "Vous avez récupéré votre %s de la fourrière pour $%i.", VehicleNames[GetVehicleModel(vehicle)-400], IMPOUND_FEE);
	SendHelpMessage(playerid, "N'oubliez pas de re-parquer votre véhicule après l'avoir récupéré de la fourrière ! (/park)");
	return 1;
}

ALTX:vehicle("/v");
CMD:vehicle(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new parametrs[13], arg[128], vehicle = GetPlayerVehicleID(playerid), model = GetVehicleModel(vehicle);

	if (sscanf(params, "s[13]S()[128]", parametrs, arg))
	{
		SendHelpMessage(playerid, "/(v)ehicle [paramètre]");
		SendSyntaxMessage(playerid, "list | get | park | find | lock | hidenum | fakenum");
		SendSyntaxMessage(playerid, "corps | tow | stats | fines | sell | sellto");
		SendClientMessageEx(playerid, COLOR_GRAD2, "CMD:{FFFFFF} /vmenu /mesveh /vfa /(en)gine /lights /trunk /hood /setstation(/ss) /(wi)ndow /unimpound");
		return 1;
	}

	else if (!strcmp(parametrs, "hidenum", true))
	{
		vehicle = GetNearVehicle(playerid, 4.0);

		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
		if (!vehicle)														return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'un véhicule.");
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
		if (VehicleInfo[vehicle][carFakePlate])								return SendErrorMessage(playerid, "Vous devez d'abord retirer les plaques falsifiées.");
		if (!IsPlayerHasItem(playerid, INV_TOOLS)) 							return SendErrorMessage(playerid, "Vous n'avez pas d'outils."); 
		if (VehicleInfo[vehicle][carPlate] == '_') 							return SendErrorMessage(playerid, "Votre véhicule n'a pas de plaques.");

		if (VehicleInfo[vehicle][carHidedPlate])
		{
			VehicleInfo[vehicle][carHidedPlate] = 0;
			SQL_SetInteger("cars", "hidednum", VehicleInfo[vehicle][carHidedPlate], VehicleInfo[vehicle][carID]);
			SetVehicleNumberPlate(vehicle, VehicleInfo[vehicle][carPlate]);
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "Vous avez remis les plaques sur le véhicule.");
		}
		else 
		{
			VehicleInfo[vehicle][carHidedPlate] = 1;
			SQL_SetInteger("cars", "hidednum", VehicleInfo[vehicle][carHidedPlate], VehicleInfo[vehicle][carID]);
			SetVehicleNumberPlate(vehicle, "_");
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "Vous avez retiré les plaques du véhicule.");
		}
		return 1;
	}

	else if (!strcmp(parametrs, "fakenum", true))
	{
		vehicle = GetNearVehicle(playerid, 4.0);

		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
		if (!vehicle)														return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'un véhicule.");
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
		if (!IsPlayerHasItem(playerid, INV_FAKENUM)) 						return SendWarningMessage(playerid, "Vous n'avez pas de plaques falsifiées.");
		if (VehicleInfo[vehicle][carHidedPlate])								return SendErrorMessage(playerid, "Vous avez retiré les plaques. (/v hidenum)");								
		
		if (VehicleInfo[vehicle][carFakePlate])
		{
			VehicleInfo[vehicle][carFakePlate] = 0;
			SQL_SetInteger("cars", "fakenum", VehicleInfo[vehicle][carFakePlate], VehicleInfo[vehicle][carID]);
			SetVehicleNumberPlate(vehicle, VehicleInfo[vehicle][carPlate]);
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "Vous avez retiré les plaques falsifiées du véhicule.");

		}
		else 
		{
			VehicleInfo[vehicle][carFakePlate] = 1;
			SQL_SetInteger("cars", "fakenum", VehicleInfo[vehicle][carFakePlate], VehicleInfo[vehicle][carID]);
			
			if (isempty(VehicleInfo[vehicle][carFakePlateS]))
			{
				format(VehicleInfo[vehicle][carFakePlateS], 24, "%s", GetVehicleNumber());
				SQL_SetString("cars", "fakenum_str", VehicleInfo[vehicle][carFakePlateS], VehicleInfo[vehicle][carID]);
			}

			SetVehicleNumberPlate(vehicle, VehicleInfo[vehicle][carFakePlateS]);
			SetVehicleToRespawn(vehicle);
			SendSyntaxMessage(playerid, "Vous avez installé des plaques falsifiées sur le véhicule.");
		}
		return 1;
	}

	else if (!strcmp(parametrs, "menu", true)) return cmd::vmenu(playerid, "");
	    
	else if (!strcmp(parametrs, "fines", true))
	{
		cmd::vtickets(playerid, "");
	}

	else if (!strcmp(parametrs, "get", true))
	{
		if (IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Vous avez déjà un véhicule spawné.");

		if (CountPlayerVehicles(playerid) == 0)
			return SendErrorMessage(playerid, "Vous ne possédez aucun véhicule.");
			
		return ShowVehicleList(playerid); 
	}

	else if (!strcmp(parametrs, "park", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
		if (vehicle == 0)													return SendNotInVehicle(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
		if (VehicleInfo[vehicle][Impounded])									return SendErrorMessage(playerid, "Votre véhicule est à la fourrière, vous ne pouvez pas utiliser cette fonction. (/unimpound)");

		new Float:hp;
		AntiCheatGetVehicleHealth(vehicle, hp);

		if (hp < 250.0)									return SendErrorMessage(playerid, "Vous ne pouvez pas garer ce véhicule car il est gravement endommagé.");
		if (VehicleInfo[vehicle][carAttach3DtextUse])	return SendErrorMessage(playerid, "Veuillez patienter, une opération est déjà en cours.");
		
		GetVehiclePos(vehicle, VehicleInfo[vehicle][carParkX], VehicleInfo[vehicle][carParkY], VehicleInfo[vehicle][carParkZ]);
		GetVehicleZAngle(vehicle, VehicleInfo[vehicle][carParkA]);
		
		SendSyntaxMessage(playerid, "Le véhicule \"%s\" a été garé.", VehicleNames[model-400]);
		ResetVehicleStuff(vehicle, 1);
		DestroyVehicle(vehicle);
		PlayerInfo[playerid][pCarKey] = 0;
		VehicleInfo[vehicle][carOwnerPlayer] = INVALID_PLAYER_ID;
		return 1;
	}

	else if (!strcmp(parametrs, "sellto", true))
	{
		new userid, price;

		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))									return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
		if (!vehicle)																		return SendNotInVehicle(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])					return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
		if (sscanf(arg, "ui", userid, price))												return SendHelpMessage(playerid, "/v sell [ID/nom du joueur] [prix]");
		if (!IsPlayerLogged(userid))															return SendNotConnected(playerid);
		if (IsADonateModel(GetVehicleModel(vehicle)))										return SendErrorMessage(playerid, "Vous ne pouvez pas vendre un véhicule de don à un joueur.");
		if (PlayerInfo[userid][pPremium] < 2 && VehicleInfo[vehicle][carComponent][5] != 0)	return SendErrorMessage(playerid, "Ce joueur n'a pas accès au nitro.");
		if (userid == playerid)																return 1;
		if (!IsABicycle(vehicle) && !VehicleInfo[vehicle][carRegistered])					return SendErrorMessage(playerid, "Ce véhicule n'est pas enregistré.");
		if (!PlayerNearPlayer(3.0, playerid, userid))										return SendNotNearPlayer(playerid);
		if (IsValidVehicle(PlayerInfo[userid][pCarKey]))										return SendErrorMessage(playerid, "Ce joueur a déjà un véhicule spawné.");
		if (price < 0 || price > 10000000)													return SendErrorMessage(playerid, "Le prix doit être compris entre $0 et $10,000,000.");

		SendSyntaxMessage(userid, "%s souhaite vous vendre %s pour $%i. (Appuyez sur {33AA33}Y{FFFFFF} pour acheter.)", GetNameEx(playerid), VehicleNames[GetVehicleModel(vehicle)-400], price);
		if (IsABicycle(vehicle))
		{
			SendSyntaxMessage(userid, "Sécurité : Verrouillage[%i]", VehicleInfo[vehicle][carLock]);
			SendSyntaxMessage(userid, "Divers : Couleur[{%06x}#%i{FFFFFF}], Couleur[{%06x}#%i{FFFFFF}]", VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2]);
		}
		else
		{
			SendSyntaxMessage(userid, "État du véhicule : Moteur[%.2f], Batterie[%.2f], Kilométrage[%.2f]", VehicleInfo[vehicle][carEngine], VehicleInfo[vehicle][carBattery], VehicleInfo[vehicle][carMilliage]);
			SendSyntaxMessage(userid, "Sécurité : Verrouillage[%i], Alarme[%i], Immobilisateur[%i], Assurance[%i]", VehicleInfo[vehicle][carLock], VehicleInfo[vehicle][carAlarm], VehicleInfo[vehicle][carImmob], VehicleInfo[vehicle][carInsurance]);
			
			if (VehicleInfo[vehicle][carArmour] > 0.0)	SendSyntaxMessage(userid, "Divers : Couleur[{%06x}#%i{FFFFFF}], Couleur[{%06x}#%i{FFFFFF}], Plaque[%s], Blindage[%.2f], Radio[%i]", VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2], VehicleInfo[vehicle][carPlate], VehicleInfo[vehicle][carArmour], VehicleInfo[vehicle][carRadio]);
			else										SendSyntaxMessage(userid, "Divers : Couleur[{%06x}#%i{FFFFFF}], Couleur[{%06x}#%i{FFFFFF}], Plaque[%s], Radio[%i]", VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2], VehicleInfo[vehicle][carPlate], VehicleInfo[vehicle][carRadio]);
		}

		SendSyntaxMessage(playerid, "Vous avez proposé de vendre votre \"%s\" à %s pour $%i.", VehicleNames[GetVehicleModel(vehicle)-400], GetNameWithMask(userid), price);

		pTemp[userid][pCarPlayer] = playerid;
		pTemp[userid][pCarPrice] = price;
		SetPVarInt(userid, #accept_time, gettime() + 30);
		return 1;
	}

	else if (!strcmp(parametrs, "lock", true))
	{
	    if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])	return SendErrorMessage(playerid, "Âû íå ìîæåòå èñïîëüçîâàòü äàííóþ ôóíêöèþ ïðÿìî ñåé÷àñ.");
		
	    vehicle = 0;
		if ((vehicle = GetNearVehicle(playerid, 4.0)) != 0)
		{
			if ((VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] == PlayerInfo[playerid][pFaction])
			 || pTemp[playerid][pRentveh] == vehicle
			 || PlayerInfo[playerid][pCarKey] == vehicle
			 || (PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehicle][carID] && PlayerInfo[playerid][pSpareKey] != 0)
			 || IsWindowOpened(vehicle) && !VehicleInfo[vehicle][carFaction])
			{
				PlaySound(playerid, 1145);
				
				new mes[128];

				if (VehicleInfo[vehicle][carLocked])		format(mes, sizeof(mes), "~w~%s ~g~OUVERT", VehicleNames[GetVehicleModel(vehicle)-400]);
				else									format(mes, sizeof(mes), "~w~%s ~r~FERMÉ", VehicleNames[GetVehicleModel(vehicle)-400]);
				GameTextForPlayer(playerid, mes, 2500, 4);

				if (VehicleInfo[vehicle][carLocked])	format(mes, sizeof(mes), "a ouvert le verrou centralisé de %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
				else								format(mes, sizeof(mes), "a fermé le verrou centralisé de %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
				SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

				ToggleVehicleLock(vehicle, (VehicleInfo[vehicle][carLocked]) ? (false): (true));
				return 1;
			}
			else {
				SendErrorMessage(playerid, "Vous n'avez pas la clé de ce véhicule !");
				SendHelpMessage(playerid, "Pour forcer une voiture, utilisez /vbreaklock (/vbl).");
			}
		}
	    return 1;
	}

	else if (!strcmp(parametrs, "find", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");

		vehicle = PlayerInfo[playerid][pCarKey];

		if (!VehicleInfo[vehicle][carGps])			return SendErrorMessage(playerid, "Votre véhicule n'est pas équipé d'un GPS.");
		if (IsPlayerInVehicle(playerid, vehicle))	return SendErrorMessage(playerid, "Vous utilisez actuellement votre véhicule.");
		if (!VehicleEmpty(vehicle))					return SendErrorMessage(playerid, "Votre véhicule est actuellement utilisé par quelqu'un.");
		if (GetVehicleInteriorEx(vehicle))			return SendErrorMessage(playerid, "Impossible de localiser votre véhicule. (Dans un intérieur)");

		new Float:x, Float:y, Float:z;
		GetVehiclePos(vehicle, x, y, z);

		SendSyntaxMessage(playerid, "Votre véhicule \"%s\" se trouve dans la zone \"%s\".", VehicleNames[GetVehicleModel(vehicle)-400], GetVehicleZone(vehicle));
		SendSyntaxMessage(playerid, "L'emplacement de votre véhicule a été marqué sur la carte. (/removecp)");
		Waypoint_Set(playerid, x, y, z);
		return 1;
	}

	else if (!strcmp(parametrs, "tow", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");

		vehicle = PlayerInfo[playerid][pCarKey];

		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
		if (VehicleInfo[vehicle][carAttach3DtextUse])						return SendErrorMessage(playerid, "Veuillez patienter, une opération est déjà en cours.");
		if (IsPlayerInVehicle(playerid, vehicle))							return SendErrorMessage(playerid, "Vous utilisez actuellement votre véhicule.");
		if (!VehicleEmpty(vehicle))											return SendErrorMessage(playerid, "Votre véhicule est actuellement utilisé par quelqu'un.");
		if (!IsABicycle(vehicle) && !VehicleInfo[vehicle][carRegistered])	return SendErrorMessage(playerid, "Ce véhicule n'est pas enregistré.");
		if (VehicleInfo[vehicle][Impounded])								return SendErrorMessage(playerid, "Tant que votre véhicule est à la fourrière, vous ne pouvez pas utiliser cette fonction. (/unimpound)");
		if (GetCash(playerid) < 200)										return SendNotCash(playerid);

		SendSyntaxMessage(playerid, "Votre véhicule sera remorqué à l'emplacement de stationnement dans 10 secondes.");
		StartVehicleUpgrade(playerid, vehicle, 1, 200);
		UpdateDynamic3DTextLabelText(status_update[vehicle], 0x33AA33FF, "(( ---------- ))\nREMORQUAGE DU VÉHICULE");
		return 1;
	}

	else if (!strcmp(parametrs, "stats", true))
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");

		vehicle = PlayerInfo[playerid][pCarKey];
		
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");

		new title[24], diatxt[512];
		format(title, sizeof(title), "%s", VehicleNames[GetVehicleModel(vehicle)-400]);

		if (IsABicycle(vehicle))
		{
			format(diatxt, sizeof(diatxt), "Sécurité:\n- Verrou\t[%i]\n", VehicleInfo[vehicle][carLock]);
			format(diatxt, sizeof(diatxt), "%sDivers:\n- Couleur\t[{%06x}#%i{FFFFFF}]\n- Couleur\t[{%06x}#%i{FFFFFF}]", diatxt, VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2]);
			Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, title, diatxt, ">>", "");
			return 1;
		}

		format(diatxt, sizeof(diatxt), "{ecc317}Enregistrement:{FFFFFF}\n");

		if (VehicleInfo[vehicle][carRegistered]) format(diatxt, sizeof(diatxt), "%s{FFFFFF}Plaque - %s\n", diatxt, VehicleInfo[vehicle][carPlate]);
		else 									format(diatxt, sizeof(diatxt), "%s{FF6347}SANS PLAQUES{FFFFFF}\n", diatxt, VehicleInfo[vehicle][carPlate]);
		
		if (VehicleInfo[vehicle][carHidedPlate]) format(diatxt, sizeof(diatxt), "%s{FF6347}PLAQUES RETIRÉES{FFFFFF}\n", diatxt, VehicleInfo[vehicle][carPlate]);
		if (VehicleInfo[vehicle][carFakePlate])	format(diatxt, sizeof(diatxt), "%s{FF6347}PLAQUES FALSIFIÉES INSTALLÉES\n\t\t[%s]\n", diatxt, VehicleInfo[vehicle][carFakePlateS]);
																							
		format(diatxt, sizeof(diatxt), "%s\n{ecc317}Principal:{FFFFFF}\n- Moteur\t\t[%.2f]\n- Batterie\t\t[%.2f]\n- Carburant\t\t[%.1f / %.0f]\n- Kilométrage\t\t[%.2f]\n\n", diatxt, VehicleInfo[vehicle][carEngine], VehicleInfo[vehicle][carBattery], VehicleInfo[vehicle][carFuel], GetVehicleFuelCapacity(GetVehicleModel(vehicle)), VehicleInfo[vehicle][carMilliage]);
		format(diatxt, sizeof(diatxt), "%s{ecc317}Sécurité:{FFFFFF}\n- Verrou\t\t\t[%i]\n- Alarme\t[%i]\n- Immobilisateur\t[%i]\n- Assurance\t\t[%i]\n", diatxt, VehicleInfo[vehicle][carLock], VehicleInfo[vehicle][carAlarm], VehicleInfo[vehicle][carImmob], VehicleInfo[vehicle][carInsurance]);
		
		if (VehicleInfo[vehicle][carArmour] > 0.0) format(diatxt, sizeof(diatxt), "%sBlindage\t[%.2f]\n", diatxt, VehicleInfo[vehicle][carArmour]);
		
		format(diatxt, sizeof(diatxt), "%s\n{ecc317}Divers:{FFFFFF}\n- Radio\t\t%s\n- Couleur1\t\t\t[{%06x}#%i{FFFFFF}]\n- Couleur2\t\t\t[{%06x}#%i{FFFFFF}]", diatxt, (VehicleInfo[vehicle][carRadio])?("{33AA33}Présente{FFFFFF}"):("{FF6347}Absente{FFFFFF}"), VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor1]] >>> 8, VehicleInfo[vehicle][carColor1], VehicleColoursTableRGBA[VehicleInfo[vehicle][carColor2]] >>> 8, VehicleInfo[vehicle][carColor2]);
		
		Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, title, diatxt, ">>", "");
		return 1;
	}

	else if (!strcmp(parametrs, "list", true))
	{
	    new query[128];
	    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `owner` = %i", PlayerInfo[playerid][pID]);
		mysql_tquery(dbHandle, query, "ShowCarsForPlayer", "ii", playerid, playerid);
	    return 1;
	}
	else if (!strcmp(parametrs, "corps", true))
	{	
		vehicle = GetNearVehicle(playerid, 4.0);
		if (vehicle == -1)							return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'un véhicule.");
		if (!IsNearTrunk(vehicle, playerid, 2.0)) 	return SendErrorMessage(playerid, "Vous n'êtes pas près du coffre !");
		if (!VehicleInfo[vehicle][carTrunkLock])	return SendErrorMessage(playerid, "Le coffre doit être ouvert.");
		if (!VehicleInfo[vehicle][vCorp] && !CorpInfo[VehicleInfo[vehicle][vCorp]][cUsed]) return SendErrorMessage(playerid, "Il n'y a pas de corps dans le coffre.");
	
		new Float:pos[3];
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

		DestroyDynamic3DTextLabel(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cText]);
		
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cText]=CreateDynamic3DTextLabel("(( CADAVRE ))\nappuyez sur '~k~~GROUP_CONTROL_BWD~'", COLOR_LIGHTRED, pos[0]+0.75, pos[1], pos[2]-0.4, 50.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), -1, 50.0);
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cX] = pos[0]+0.75;
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cY] = pos[1];
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cZ] = pos[2];
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cVeh] = 0;
		CorpInfo[VehicleInfo[vehicle][vCorp]-1][cTime] = gettime();
		if (CorpInfo[VehicleInfo[vehicle][vCorp]-1][cType] == 0) {
			CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody]=CreateActor(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cSkin], pos[0]+0.75, pos[1], pos[2], 0.0);
			SetActorInvulnerable(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody], true);
			ApplyActorAnimation(CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody], "PED", "KO_shot_stom", 4.0, 0, 0, 0, 1, 0);
		} else {
			CorpInfo[VehicleInfo[vehicle][vCorp]-1][cBody]=CreateDynamicObject(19944, pos[0]+0.75, pos[1], pos[2]-0.5, 0.0, 0.0, 0.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
		}
		
		VehicleInfo[vehicle][vCorp]=0;    
		SM2(playerid, "Vous avez sorti le corps du coffre.");
		return 1;
	}
	else if (!strcmp(parametrs, "sell", true))
	{
		new veh = PlayerInfo[playerid][pCarKey], lockuse[250], alarmuse[250], immobuse[260], insuranceuse[260], xmradiouse[50], gpsuse[50], string1[1024];

		if (!IsValidVehicle(veh))											return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
		if (!IsPlayerInAnyVehicle(playerid))								return SendNotInVehicle(playerid);
		if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");

		if (VehicleInfo[veh][carLock] == 1) lockuse = "{A9C4E4}Verrou : \t{bab9ba}Niveau 1\t{e2bd63}$3.500\n{4af085}+{e2bd63}120 secondes pour forcer le véhicule.\n\n";
		else if (VehicleInfo[veh][carLock] == 2) lockuse = "{A9C4E4}Verrou : \t{bab9ba}Niveau 2\t{e2bd63}$5.500\n{4af085}+{e2bd63}180 secondes pour forcer le véhicule.\n\n";
		else if (VehicleInfo[veh][carLock] == 3) lockuse = "{A9C4E4}Verrou : \t{bab9ba}Niveau 3\t{e2bd63}$7.500\n{4af085}+{e2bd63}240 secondes pour forcer le véhicule.\n\n";
		else if (VehicleInfo[veh][carLock] == 4) lockuse = "{A9C4E4}Verrou : \t{bab9ba}Niveau 4\t{e2bd63}$10.500\n{4af085}+{e2bd63}300 secondes pour forcer le véhicule.\n\n";
		else if (VehicleInfo[veh][carLock] == 5) lockuse = "{A9C4E4}Verrou : \t{bab9ba}Niveau 5\t{e2bd63}$13.500\n{4af085}+{e2bd63}360 secondes pour forcer le véhicule.\n\n";

		if (VehicleInfo[veh][carAlarm] == 1) 		alarmuse = "{A9C4E4}Alarme : \t{bab9ba}Niveau 1\t{e2bd63}$2.600\n{f0634a}+{e2bd63}Alarme sonore forte.\n";
		else if (VehicleInfo[veh][carAlarm] == 2) 	alarmuse = "{A9C4E4}Alarme : \t{bab9ba}Niveau 2\t{e2bd63}$5.500\n{f0634a}+{e2bd63}Alarme sonore forte.\n{f0634a}+{e2bd63}Notification de vol à la police.\n\n";
		else if (VehicleInfo[veh][carAlarm] == 3) 	alarmuse = "{A9C4E4}Alarme : \t{bab9ba}Niveau 3\t{e2bd63}$8.000\n{f0634a}+{e2bd63}Alarme sonore forte.\n{f0634a}+{e2bd63}Notification de vol à la police.\n{f0634a}+{e2bd63}Notification de vol au propriétaire par SMS.\n\n";
		else if (VehicleInfo[veh][carAlarm] == 4) 	alarmuse = "{A9C4E4}Alarme : \t{bab9ba}Niveau 4\t{e2bd63}$10.500\n{f0634a}+{e2bd63}Alarme sonore forte.\n{f0634a}+{e2bd63}Notification de vol à la police.\n{f0634a}+{e2bd63}Notification de vol au propriétaire par SMS + géolocalisation.\n\n";

		if (VehicleInfo[veh][carImmob] == 1) 		immobuse = "{A9C4E4}Antidémarrage : \t{bab9ba}Niveau 1\t{e2bd63}$5.600\n{feff00}+{e2bd63}Aide à prévenir le vol du véhicule.\n\n";
		else if (VehicleInfo[veh][carImmob] == 2) 	immobuse = "{A9C4E4}Antidémarrage : \t{bab9ba}Niveau 2\t{e2bd63}$11.300\n{feff00}+{e2bd63}Aide à prévenir le vol du véhicule.\n\n";
		else if (VehicleInfo[veh][carImmob] == 3) 	immobuse = "{A9C4E4}Antidémarrage : \t{bab9ba}Niveau 3\t{e2bd63}$17.000\n{feff00}+{e2bd63}Aide à prévenir le vol du véhicule.\n\n";
		else if (VehicleInfo[veh][carImmob] == 4) 	immobuse = "{A9C4E4}Antidémarrage : \t{bab9ba}Niveau 4\t{e2bd63}$22.600\n{feff00}+{e2bd63}Aide à prévenir le vol du véhicule.\n\n";

		if (VehicleInfo[veh][carInsurance] == 1) 		insuranceuse = "{A9C4E4}Assurance : \t{bab9ba}Niveau 1\t{e2bd63}$5.600\n{4a90f0}+{e2bd63}Le véhicule sera toujours restauré avec sa santé maximale.\n\n";
		else if (VehicleInfo[veh][carInsurance] == 2) 	insuranceuse = "{A9C4E4}Assurance : \t{bab9ba}Niveau 2\t{e2bd63}$11.300\n{4a90f0}+{e2bd63}Le véhicule sera toujours restauré avec sa santé maximale.\n{4a90f0}+{e2bd63}Le véhicule sera restauré sans dommages.\n\n";
		else if (VehicleInfo[veh][carInsurance] == 3)	insuranceuse = "{A9C4E4}Assurance : \t{bab9ba}Niveau 3\t{e2bd63}$17.000\n{4a90f0}+{e2bd63}Le véhicule sera toujours restauré avec sa santé maximale.\n{4a90f0}+{e2bd63}Le véhicule sera restauré sans dommages.\n{4a90f0}+{e2bd63}Conservation du tuning et de la radio.\n\n";

		if (VehicleInfo[veh][carRadio] == 1) xmradiouse = "{A9C4E4}Radio :\t{e2bd63}$7.500\n";
		if (VehicleInfo[veh][carGps] == 1) gpsuse = "{A9C4E4}GPS :\t{e2bd63}$3.500\n";

		new mes[56];

		if (IsABicycle(veh))	format(mes, sizeof(mes), "{33AA33}%s", VehicleNames[VehicleInfo[veh][carModel]-400]);
		else
		{
			if (VehicleInfo[veh][carRegistered])	format(mes, sizeof(mes), "{FFFFFF}%s [%s]", VehicleNames[VehicleInfo[veh][carModel]-400], VehicleInfo[veh][carPlate]);
			else								format(mes, sizeof(mes), "{FFFFFF}%s", VehicleNames[VehicleInfo[veh][carModel]-400]);
		}

		format(string1, sizeof(string1), "{ffffff}Prix de vente :\t\t{e2bd63}$%i\n{ffffff}Santé maximale :\t{e2bd63}%.2f\n\n%s%s%s%s%s%s", GetVehiclePrice(VehicleInfo[veh][carModel])/2, GetVehicleMaxHealth(GetVehicleModel(veh)), lockuse, alarmuse, immobuse, insuranceuse, xmradiouse, gpsuse);
		Dialog_Show(playerid, ScrapSell, DIALOG_STYLE_MSGBOX, mes, string1, "Accepter", "Annuler");
		return 1;
	}
	else
	{
		SendHelpMessage(playerid, "/(v)ehicle [paramètres]");
		SendSyntaxMessage(playerid, "list | get | park | find | lock | hidenum | fakenum | corpse");
		SendSyntaxMessage(playerid, "tow | stats | fines | sell | sellto | menu");
		SendClientMessageEx(playerid, COLOR_GRAD2, "CMD:{FFFFFF} /(en)gine /lights /trunk /hood /ss /(wi)ndow /unimpound");
	}
	return 1;
}

CMD:register(playerid, params[])
{
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");

	new vehicle = PlayerInfo[playerid][pCarKey];

	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
	if (IsABicycle(vehicle))												return SendErrorMessage(playerid, "Impossible d'enregistrer un vélo.");
	if (VehicleInfo[vehicle][carEngineOn])								return SendErrorMessage(playerid, "Coupez le moteur avant de commencer à modifier votre véhicule.");
	if (VehicleInfo[vehicle][carAttach3DtextUse] != 0)					return SendErrorMessage(playerid, "Veuillez patienter, une opération est déjà en cours.");
	if (VehicleInfo[vehicle][carRegistered])								return SendErrorMessage(playerid, "Ce véhicule est déjà enregistré.");
	if (GetCash(playerid) < 300)											return SendNotCash(playerid);

	if (!IsPlayerInRangeOfPoint(playerid, 5.0,2228.1577,-110.8875,26.6406))	return SendErrorMessage(playerid, "Vous devez être à l'intérieur du poste de police de Palomino.");
	if (!IsVehicleInRangeOfPoint(vehicle, 15.0,697.6670,1624.8490,785.5494))	return SendErrorMessage(playerid, "Votre véhicule doit être près du poste de police de Palomino.");
		
	StartVehicleUpgrade(playerid, vehicle, 7, 300);
	SendSyntaxMessage(playerid, "L'enregistrement du véhicule a commencé, cela prendra un certain temps.");
	return 1;
}

CMD:unregister(playerid, params[])
{
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))					return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");

	new vehicle = PlayerInfo[playerid][pCarKey];

	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID])	return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");
	if (IsABicycle(vehicle))												return SendErrorMessage(playerid, "Impossible de retirer un vélo de l'enregistrement.");
	if (VehicleInfo[vehicle][carEngineOn])								return SendErrorMessage(playerid, "Coupez le moteur avant de commencer à modifier votre véhicule.");
	if (VehicleInfo[vehicle][carAttach3DtextUse] != 0)					return SendErrorMessage(playerid, "Veuillez patienter, une opération est déjà en cours.");
	if (!VehicleInfo[vehicle][carRegistered])							return SendErrorMessage(playerid, "Ce véhicule n'est pas enregistré.");
	if (GetCash(playerid) < 300)											return SendNotCash(playerid);

	if (!IsPlayerInRangeOfPoint(playerid, 5.0,2228.1577,-110.8875,26.6406))	return SendErrorMessage(playerid, "Vous devez être à l'intérieur du poste de police de Palomino.");
	if (!IsVehicleInRangeOfPoint(vehicle, 15.0,697.6670,1624.8490,785.5494))	return SendErrorMessage(playerid, "Votre véhicule doit être près du poste de police de Palomino.");
		
	StartVehicleUpgrade(playerid, vehicle, 8, 300);
	SendSyntaxMessage(playerid, "Le retrait de l'enregistrement a commencé, cela prendra un certain temps.");
	return 1;
}


CMD:lights(playerid, params[])
{
	if (!IsDriver(playerid))	return SendNotInVehicle(playerid);

	new vehicleid = GetPlayerVehicleID(playerid), mes[128];
	new engine,lights,alarm,doors,bonnet,boot,objective;

	GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);

	if (lights)
	{
		format(mes, sizeof(mes), "a éteint les phares de %s.", VehicleNames[GetVehicleModel(vehicleid)-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		format(mes, sizeof(mes), "~w~PHARES DE %s ~r~ÉTEINTS", VehicleNames[GetVehicleModel(vehicleid)-400]);
		GameTextForPlayer(playerid, mes, 2500, 4);

		VehicleLight(vehicleid, false);
		return 1;
	}

	format(mes, sizeof(mes), "a allumé les phares de %s.", VehicleNames[GetVehicleModel(vehicleid)-400]);
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

	format(mes, sizeof(mes), "~w~PHARES DE %s ~g~ALLUMÉS", VehicleNames[GetVehicleModel(vehicleid)-400]);
	GameTextForPlayer(playerid, mes, 2500, 4);

	VehicleLight(vehicleid, true);
	return 1;
}

CMD:alarmoff(playerid, params[])
{
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))						return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
	if (!GetNearVehicle(playerid, 10.0, PlayerInfo[playerid][pCarKey]))		return SendErrorMessage(playerid, "Vous devez être à proximité de votre véhicule.");

	new engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx(PlayerInfo[playerid][pCarKey],engine,lights,alarm,doors,bonnet,boot,objective);

	if (alarm == VEHICLE_PARAMS_OFF)	return SendErrorMessage(playerid, "L'alarme ne fonctionne pas.");

	AlarmOff(PlayerInfo[playerid][pCarKey]);
	SendWarningMessage(playerid, "Vous avez désactivé l'alarme.");
	return 1;
}

ALTX:window("/wi", "/ww");
CMD:window(playerid, params[])
{
    if (!IsPlayerInAnyVehicle(playerid))				return SendNotInVehicle(playerid);

	new vehicle = GetPlayerVehicleID(playerid), seat = AntiCheatGetVehicleSeat(playerid), model = GetVehicleModel(vehicle), wi;

	if (IsADontWindow(vehicle))						return SendErrorMessage(playerid, "Ce véhicule n'a pas de fenêtres.");

	if (sscanf(params, "i", wi))
	{
		SendHelpMessage(playerid, "/(wi)ndow [fenêtre]");
		SendSyntaxMessage(playerid, "Fenêtre: 1 - Conducteur | 2 - Passager avant | 3 - Arrière gauche | 4 - Arrière droit");
		return 1;
	}
	
	new seat_name[4][32] = {"conducteur", "passager avant", "arrière gauche", "arrière droit"};

	if (wi > 4 || wi < 1)						return SendErrorMessage(playerid, "1 - Conducteur | 2 - Passager avant | 3 - Arrière gauche | 4 - Arrière droit");
	if (!IsDriver(playerid) && seat+1 != wi)	return SendErrorMessage(playerid, "Vous n'êtes pas le conducteur et ne pouvez ouvrir/fermer que la fenêtre %s.", seat_name[seat]);

	if ((model == 401 || model == 402 || model == 403 || model == 406 || model == 408 ||model == 410 || model == 411 || model == 412 || model == 413 || model == 414 ||model == 415 || model == 416 || model == 417 || model == 419 || 
	model == 422 || model == 425 || model == 427 || model == 428 || model == 429 || model == 431 || model == 433 || model == 434 || model == 436 || model == 437 || model == 439 || model == 440 || model == 443 || model == 444 || 
	model == 447 || model == 451 || model == 455 || model == 456 || model == 459 || model == 474 || model == 475 || model == 477 || model == 478 || model == 480 || model == 482 || model == 489 || model == 491 || model == 494 || 
	model == 495 || model == 496 || model == 498 || model == 499 || model == 500 || model == 502 || model == 503 || model == 505 || model == 506 || model == 508 || model == 514 || model == 515 || model == 517 || model == 518 || 
	model == 524 || model == 525 || model == 526 || model == 527 || model == 528 || model == 533 || model == 534 || model == 535 || model == 536 || model == 541 || model == 542 || model == 543 || model == 544 || model == 545 || 
	model == 549 || model == 552 || model == 554 || model == 555 || model == 556 || model == 557 || model == 558 || model == 559 || model == 562 || model == 565 || model == 573 || model == 574 || model == 575 || model == 576 || 
	model == 578 || model == 582 || model == 583 || model == 587 || model == 589 || model == 593 || model == 599 || model == 600 || model == 602) && wi > 2)
		return SendErrorMessage(playerid, "Ce véhicule n'a pas de fenêtres arrière.");

	new wi_state[4], mes[128];

	GetVehicleParamsCarWindows(vehicle, wi_state[0], wi_state[1], wi_state[2], wi_state[3]);
	wi_state[wi-1] = !wi_state[wi-1] ? (1):(0);

	if (wi_state[wi-1] == 0)
	{
		format(mes, sizeof(mes), "a ouvert la fenêtre %s de %s.", seat_name[wi-1], VehicleNames[model-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		GameTextForPlayer(playerid, "~w~fenêtre ~g~ouverte", 2500, 4);
	}
	else
	{
		format(mes, sizeof(mes), "a fermé la fenêtre %s de %s.", seat_name[wi-1], VehicleNames[model-400]);
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 20.0, 4000);

		GameTextForPlayer(playerid, "~w~fenêtre ~r~fermée", 2500, 4);
	}

	SetVehicleParamsCarWindows(vehicle, wi_state[0], wi_state[1], wi_state[2], wi_state[3]);
	return 1;
}

ALTX:mesveh("/mesvehs", "/mesvehicules", "/mesvehicule");
CMD:mesveh(playerid, params[])
{
	new query[64];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `owner`=%i", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query, "ShowListVeh", "i", playerid);

	return 1;
}

ALTX:vfa("/vehiclefastaccess");
CMD:vfa(playerid, params[])
{
	new vehicle;
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "Vous devez être au volant d'un véhicule !");
	if (!(vehicle = GetPlayerVehicleID(playerid))) 		return SendErrorMessage(playerid, "Vous devez être au volant d'un véhicule !");
	
	new engine = 0, lights = 0, alarm = 0, doors = 0, bonnet = 0, boot = 0, objective = 0;
    GetVehicleParamsEx(vehicle,engine,lights,alarm,doors,bonnet,boot,objective);

	static mestxt[] = "{FFFFFF}• %s moteur\t{C7C7C7}/(en)gine\n\
					{FFFFFF}• %s véhicule\t{C7C7C7}/lock\n\
					{FFFFFF}• %s phares\t{C7C7C7}/lights\n\
					{FFFFFF}• %s coffre\t{C7C7C7}/trunk\n\
					{FFFFFF}• %s capot\t{C7C7C7}/hood\n\
					{FFFFFF}• Fenêtre conducteur\t{C7C7C7}/(wi)ndow\n\
					{FFFFFF}• Radio\t{C7C7C7}/setstation(/ss)%s";

	new mes[sizeof(mestxt) + 90];

	format(mes, sizeof(mes), mestxt,
		(!engine)?("Démarrer"):("Couper"),
		(doors)?("Ouvrir"):("Fermer"),
		(!lights)?("Allumer"):("Éteindre"),
		(!boot)?("Ouvrir"):("Fermer"),
		(!bonnet)?("Ouvrir"):("Fermer"),
		(PlayerInfo[playerid][pCarKey] == vehicle)?("\n	\n{FF6347}> Gestion du véhicule <"):("")
	);

	new title[39];
	format(title, sizeof(title), "{FFFFFF}%s", VehicleNames[GetVehicleModel(vehicle)-400]);

	Dialog_Show(playerid, VehFastAccess, DIALOG_STYLE_TABLIST, title, mes, "Choisir", "Annuler");
	return 1;
}

CMD:vmenu(playerid, params[])
{
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))	return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
	
	new vehicle = PlayerInfo[playerid][pCarKey];
	if (!vehicle) return 1;

	static mestxt[] = "{FFFFFF}• Statistiques du véhicule\n\
					• Amendes impayées\n\
					• Re-garer le véhicule\n\
					• Trouver le véhicule via GPS\n\
					• Désactiver l'alarme [ %s{FFFFFF} ]\n\
					• Évacuer le véhicule\n\
					• Vendre le véhicule à l'État\n\
					• Vendre le véhicule à un joueur";
	new mes[sizeof(mestxt)+12],engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx(PlayerInfo[playerid][pCarKey],engine,lights,alarm,doors,bonnet,boot,objective);

	format(mes, sizeof(mes), mestxt, (alarm)?("{00E100}ON"):("{FF6347}OFF"));	

	new title[39];
	format(title, sizeof(title), "{FFFFFF}%s", VehicleNames[GetVehicleModel(vehicle)-400]);
	Dialog_Show(playerid, VehMenu, DIALOG_STYLE_LIST, title, mes, "Choisir", "Annuler");
	return 1;
}	

CMD:fill(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!GetPVarInt(playerid, #UsePump)) {
		new price, Float: f_price, bizid;
		if (!IsDriver(playerid))						return SendNotInVehicle(playerid);
		if ((bizid = AZC_Nearest(playerid)) == -1)	return SendErrorMessage(playerid, "Aucune station-service à proximité.");
		if (BizInfo[bizid][bizLocked])				return GameTextForPlayer(playerid, "~r~FERMÉE!", 2500, 1);
		if (VehicleInfo[vehicle][carEngineOn]) return SendErrorMessage(playerid, "Coupez le moteur.");

		new modelid = GetVehicleModel(vehicle);

		if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'P') 	  f_price = float(BizInfo[bizid][bizEntrance]) * 1.2;
		else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'E') f_price = float(BizInfo[bizid][bizEntrance]) * 0.8;
		else f_price = float(BizInfo[bizid][bizEntrance]);

		price = floatround(f_price, floatround_ceil);
		if (GetCash(playerid) < price) return SendNotCash(playerid);

		new str[40];
		format(str, sizeof(str), "Voulez-vous commencer le ravitaillement pour $%i/L ?", price);
		ConfirmDialog(playerid, "Ravitaillement du véhicule", str, "ConfirmFilling");
	} else {
		Fuel_ResetPlayer(playerid);
		ToggleVehicleEngine(vehicle, playerid);
	}
	return 1;
}

ALTX:engine("/en");
CMD:engine(playerid, params[])
{
	if (!IsDriver(playerid)) return 1;

	new vehicle = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][MissionTime]) return 1;
	if (pTemp[playerid][pCuffed]) return SendCuffedMessage(playerid);
	if (IsADMVCar(vehicle) || IsADMVHeli(vehicle) || IsABicycle(vehicle)) return SendErrorMessage(playerid, "Impossible de démarrer le moteur de ce véhicule.");
	if (VehicleInfo[vehicle][carFuel] == 0.0) return SendErrorMessage(playerid, "Le véhicule n'a plus de carburant.");
	if (GetPVarInt(playerid, #UsePump)) return cmd::fill(playerid, "");
	if (VehicleInfo[vehicle][carOwnerID] && VehicleInfo[vehicle][carEngine] <= 0.0) return SendErrorMessage(playerid, "Le moteur du véhicule est hors service.");
	if (VehicleInfo[vehicle][carOwnerID] && VehicleInfo[vehicle][carBattery] <= 0.0) return SendErrorMessage(playerid, "La batterie du véhicule est hors service.");
	if (IsRentCar(vehicle) && pTemp[playerid][pRentveh] != vehicle) return SendErrorMessage(playerid, "Ce véhicule doit être loué.");
	if (VehicleInfo[vehicle][Impounded]) return SendErrorMessage(playerid, "Tant que le véhicule est à la fourrière, vous ne pouvez pas utiliser cette fonction. (/unimpound)");

	if (VehicleInfo[vehicle][carFaction] && VehicleInfo[vehicle][carFaction] != PlayerInfo[playerid][pFaction]) return 1;

	if (VehicleInfo[vehicle][carType] >= RENT_CAR && VehicleInfo[vehicle][carType] <= MINER_CAR && pTemp[playerid][pRentveh] != vehicle)
		return SendErrorMessage(playerid, "Pour commencer, tapez /rentcarjob(/rcj).");

	if (VehicleInfo[vehicle][carOwnerID] && (PlayerInfo[playerid][pCarKey] != vehicle && PlayerInfo[playerid][pSpareKey] != VehicleInfo[vehicle][carID]))
	{
		if (!VehicleInfo[vehicle][carEngineOn])
		{
			if (PlayerInfo[playerid][pJob] != JOB_CARJACKER) return SendErrorMessage(playerid, "Vous n'avez pas les clés de ce véhicule !");
			if (!IsPlayerHasItem(playerid, INV_TOOLS)) return SendErrorMessage(playerid, "Vous n'avez pas les outils nécessaires pour forcer le moteur.");
			if (!PoliceOnline()) return SendErrorMessage(playerid, "Il doit y avoir au moins %i policiers en ligne.", NEED_COPS_TO_CRIME);
			if (VehicleInfo[vehicle][carOwnerID] == 0) return SendErrorMessage(playerid, "Seuls les véhicules personnels peuvent être forcés.");
			if (JackingEngine[playerid] != 0) return SendErrorMessage(playerid, "Vous essayez déjà de démarrer le moteur !");

			static const str_size[] = "{"COL_GREY"}Voulez-vous commencer à forcer le moteur de %s ?";
			new mes[sizeof(str_size) - 4 + 1 + 31 + 7];
			format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(vehicle) - 400]);
			Dialog_Show(playerid, StartJackingEngine, DIALOG_STYLE_MSGBOX, "{FFFFFF}Forçage du moteur", mes, "Commencer", "Annuler");
			return 1;
		}
	}

	ToggleVehicleEngine(vehicle, playerid);
	return 1;
}

CMD:dropkey(playerid, params[])
{
	if (!PlayerInfo[playerid][pSpareKey])	return SendErrorMessage(playerid, "Vous n'avez pas de clés.");

	PlayerInfo[playerid][pSpareKey] = 0;
	SendSyntaxMessage(playerid, "Vous avez jeté les clés au sol.");
	return 1;
}

CMD:carsign(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid), string[128];

	if (!IsDriver(playerid))					return SendNotInVehicle(playerid);
	if (isnull(params))						return SendHelpMessage(playerid, "/carsign [texte]");
	if (IsABicycle(vehicle))					return SendErrorMessage(playerid, "Ce véhicule ne peut pas avoir de panneau automobile !");
	if (strlen(params) > 84)					return SendErrorMessage(playerid, "Vous ne pouvez pas entrer plus de 84 caractères.");

	if (strlen(params) > 28)
	{
		new pos = 28, string1[30], string2[30], string3[30];

		if (pos < 28 - 1) pos = 28;

		if (strlen(params[pos]) > 28)
		{
			format(string1, sizeof(string1), "%.*s", pos, params);
			format(string2, sizeof(string2), "%.*s", pos, params[pos]);
			format(string3, sizeof(string3), "%.*s", pos, params[pos + pos]);
			format(string, sizeof(string), "%s\n%s\n%s", string1, string2, string3);
		}
		else
		{
			format(string1, sizeof(string1), "%.*s", pos, params);
			format(string2, sizeof(string2), "%s", params[pos]);
			format(string, sizeof(string), "%s\n%s", string1, string2);
		}
	}
	else
	{
		format(string, sizeof(string), "%s", params);
	}

	if (PlayerInfo[playerid][pCarKey] == vehicle)
	{
		if (!IsPlayerHasItem(playerid, INV_SIGN))		return SendErrorMessage(playerid, "Vous n'avez pas de panneau automobile.");
		if (PlayerInfo[playerid][pCarKey] != vehicle)	return SendErrorMessage(playerid, "Le panneau automobile ne peut être installé que sur votre véhicule personnel.");

		SendHelpMessage(playerid, "/rcarsign  - retirer le panneau automobile.");
		if (!IsValidDynamic3DTextLabel(veicolo_callsign_testo[vehicle]))
		{
			new Float: x, Float: y, Float: z;
			GetCarsignPosition(vehicle, x, y, z);
			veicolo_callsign_testo[vehicle] = CreateDynamic3DTextLabel(string, 0x33AA33FF, -x, y, z, 20.0, INVALID_PLAYER_ID, vehicle, 1, -1, 0);
		}
		else
			UpdateDynamic3DTextLabelText(veicolo_callsign_testo[vehicle], 0x33AA33FF, string);

		return 1;
	}

	new faction = PlayerInfo[playerid][pFaction];

	if (!IsFactionLegal(faction))
		return SendClientMessage(playerid, COLOR_LIGHTRED, "Vous devez être policier/paramédic ou être dans votre propre véhicule.");

	if (!PlayerInfo[playerid][pOnDuty])
		return SendClientMessage(playerid, COLOR_LIGHTRED, "Vous devez être en service (/duty).");

	if (VehicleInfo[vehicle][carFaction] != faction)
		return SendErrorMessage(playerid, "Ce véhicule n'appartient pas à votre organisation.");

	SendHelpMessage(playerid, "/rcarsign  - retirer le panneau automobile.");
	if (!IsValidDynamic3DTextLabel(veicolo_callsign_testo[vehicle]))
	{
		new Float: x, Float: y, Float: z;
		GetCarsignPosition(vehicle, x, y, z);
		veicolo_callsign_testo[vehicle] = CreateDynamic3DTextLabel(string, 0xFFFFFFFF, -x, y, z, 40.0, INVALID_PLAYER_ID, vehicle, 1, -1, 0);
	}
	else
		UpdateDynamic3DTextLabelText(veicolo_callsign_testo[vehicle], 0xFFFFFFFF, string);
	return 1;
}

CMD:rcarsign(playerid,params[])
{
    if (!IsDriver(playerid))	return SendNotInVehicle(playerid);

	new vehicle = GetPlayerVehicleID(playerid);

    if (IsValidDynamic3DTextLabel(veicolo_callsign_testo[vehicle]))
	{
		DestroyDynamic3DTextLabel(veicolo_callsign_testo[vehicle]);
        veicolo_callsign_testo[vehicle] = Text3D:INVALID_3DTEXT_ID;
	}
	return 1;
}

CMD:stop(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured]) return 1;

	/*if (JackingEngine[playerid] != 0)
	{
		JackingEngine[playerid] = 0;
		UnFreezePlayer(playerid);
		SendSyntaxMessage(playerid, "Âû îñòàíîâèëè âçëîì äâèãàòåëÿ.");
	}
	
	if (JackingLock[playerid] != 0)
	{
		JackingLock[playerid] = 0;
		JackingVehicle[playerid] = 0;
		UnFreezePlayer(playerid);
		SendSyntaxMessage(playerid, "Âû îñòàíîâèëè âçëîì äâåðåé.");
	}*/
	
	if (pTemp[playerid][pPlantTime] != 0)
	{
		UnFreezePlayer(playerid);
		StopPlayerAnims(playerid);
		pTemp[playerid][pPlantTime] = 0;
		pTemp[playerid][pPlantType] = 0;
		SendSyntaxMessage(playerid, "Vous avez arrêté la plantation.");
	}

	if (pTemp[playerid][pFireTime] != 0)
	{
		StopCreatingPlayersFire(playerid);
		SendSyntaxMessage(playerid, "Vous avez arrêté l'allumage du feu.");
	}
	return 1;
}

CMD:trunk(playerid, params[])
{
	if (IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicle = GetPlayerVehicleID(playerid);

		if (IsADontTrunk(vehicle) || IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))
			return SendErrorMessage(playerid, "Ce véhicule n'a pas de coffre.");
		if (VehicleInfo[vehicle][carOwnerID] && PlayerInfo[playerid][pCarKey] != vehicle)
			return SendErrorMessage(playerid, "Ce véhicule ne vous appartient pas.");
		if (VehicleInfo[vehicle][carFaction] != 0 && PlayerInfo[playerid][pFaction] != VehicleInfo[vehicle][carFaction])
			return SendErrorMessage(playerid, "Ce véhicule appartient à une autre faction.");
		if (VehicleInfo[vehicle][carLocked] || (IsRentCar(vehicle) && !VehicleInfo[vehicle][carRent]))
			return SendErrorMessage(playerid, "Le véhicule est verrouillé.");

		if (VehicleInfo[vehicle][carTrunkLock] == 1)
		{
			new mes[56];
			format(mes, sizeof(mes), "~w~COFFRE %s ~r~FERMÉ", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleTrunk(vehicle, false);
			SendSyntaxMessage(playerid, "Vous avez fermé le coffre du véhicule.");
			return 1;
		}
		else
		{
			new mes[56];
			format(mes, sizeof(mes), "~w~COFFRE %s ~g~OUVERT", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleTrunk(vehicle, true);
			SendSyntaxMessage(playerid, "Vous avez ouvert le coffre du véhicule.");
			SendHelpMessage(playerid, "Pour interagir avec le coffre, appuyez sur N à proximité.");
		}
		return 1;
	}

	new vehicle = GetNearVehicle(playerid, 4.0);
	if (vehicle == -1)
		return SendErrorMessage(playerid, "Vous n'êtes pas à proximité d'un véhicule.");
	if (!IsNearTrunk(vehicle, playerid, 2.0))
		return SendErrorMessage(playerid, "Vous n'êtes pas près du coffre !");

	if (VehicleInfo[vehicle][carOwnerID] && PlayerInfo[playerid][pCarKey] != vehicle)
		return SendErrorMessage(playerid, "Ce véhicule ne vous appartient pas.");
	if (VehicleInfo[vehicle][carFaction] && PlayerInfo[playerid][pFaction] != VehicleInfo[vehicle][carFaction])
		return SendErrorMessage(playerid, "Ce véhicule appartient à une autre faction.");
	if (VehicleInfo[vehicle][carLocked] || (IsRentCar(vehicle) && VehicleInfo[vehicle][carRent]))
		return SendErrorMessage(playerid, "Le véhicule est verrouillé.");

	if (VehicleInfo[vehicle][carTrunkLock] == 1)
	{
		new mes[56];
		format(mes, sizeof(mes), "~w~COFFRE %s ~r~FERMÉ", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleTrunk(vehicle, false);
		SendSyntaxMessage(playerid, "Vous avez fermé le coffre du véhicule.");
		return 1;
	}
	else
	{
		new mes[56];
		format(mes, sizeof(mes), "~w~COFFRE %s ~g~OUVERT", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleTrunk(vehicle, true);
		SendSyntaxMessage(playerid, "Vous avez ouvert le coffre du véhicule.");
	}
	return 1;
}

CMD:hood(playerid, params[])
{
	if (IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new vehicle = GetPlayerVehicleID(playerid);
		
		if (IsAPlane(vehicle) || IsABiker(vehicle) || IsAHelicopter(vehicle) || IsABoat(vehicle) || IsABicycle(vehicle))	
			return SendErrorMessage(playerid, "Ce véhicule n'a pas de capot.");
		if (VehicleInfo[vehicle][carLocked])																				
			return SendErrorMessage(playerid, "Le véhicule est verrouillé.");

		if (VehicleInfo[vehicle][carHoodLock] == 1)
		{
			new mes[56];
			format(mes, sizeof(mes), "~w~%s capot ~r~fermé", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleHood(vehicle, false);
			SendSyntaxMessage(playerid, "Vous avez fermé le capot du véhicule.");
			return 1;
		}
		else
		{
			new mes[56];
			format(mes, sizeof(mes), "~w~%s capot ~g~ouvert", VehicleNames[GetVehicleModel(vehicle)-400]);
			GameTextForPlayer(playerid, mes, 3000, 3);

			VehicleHood(vehicle, true);
			SendSyntaxMessage(playerid, "Vous avez ouvert le capot du véhicule.");
		}
		return 1;
	}

	new vehicle = 0;

	foreach(new i : Vehicle)
	{
		if (IsAPlane(i) || IsABiker(i) || IsAHelicopter(i) || IsABoat(i) || IsABicycle(i) || !IsOnHoodVehicle(playerid, i)) 
			continue;

		vehicle = i;
		break;
	}

	if (!vehicle)						
		return SendErrorMessage(playerid, "Aucun véhicule avec capot à proximité.");
	if (VehicleInfo[vehicle][carLocked])	
		return SendErrorMessage(playerid, "Le véhicule est verrouillé.");

	if (VehicleInfo[vehicle][carHoodLock] == 1)
	{
		new mes[56];
		format(mes, sizeof(mes), "~w~%s capot ~r~fermé", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleHood(vehicle, false);
		SendSyntaxMessage(playerid, "Vous avez fermé le capot du véhicule.");
		return 1;
	}
	else
	{
		new mes[56];
		format(mes, sizeof(mes), "~w~%s capot ~g~ouvert", VehicleNames[GetVehicleModel(vehicle)-400]);
		GameTextForPlayer(playerid, mes, 3000, 3);

		VehicleHood(vehicle, true);
		SendSyntaxMessage(playerid, "Vous avez ouvert le capot du véhicule.");
	}
	return 1;
}

ALTX:vbreaklock("/vbl");
CMD:vbreaklock(playerid, params[])
{
	if (!PoliceOnline())	return SendErrorMessage(playerid, "Il doit y avoir au moins %i policiers en ligne.", NEED_COPS_TO_CRIME);

	new vehid = GetNearVehicle(playerid, 3.0);
	if (!vehid)	return SendErrorMessage(playerid, "Aucun véhicule à proximité.");

	if (VehicleInfo[vehid][carOwnerID] == PlayerInfo[playerid][pID] || PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehid][carOwnerID])
		return SendErrorMessage(playerid, "Vous ne pouvez pas forcer votre propre véhicule.");
	
	if (VehicleInfo[vehid][carType])			   return SendErrorMessage(playerid, "Seuls les véhicules personnels peuvent être forcés.");
	if (VehicleInfo[vehid][carOwnerID] == 0)    return SendErrorMessage(playerid, "Seuls les véhicules personnels peuvent être forcés.");
	if (VehicleInfo[vehid][carLocked] == 0)     return SendErrorMessage(playerid, "Les portes du véhicule sont déjà ouvertes.");
	if (JackingLock[playerid] != 0)             return SendErrorMessage(playerid, "Vous êtes déjà en train de forcer les portes.");
	if (!IsPlayerInFrontVDoor(playerid, vehid)) return SendErrorMessage(playerid, "Vous devez être près de la porte du conducteur.");

	foreach(new i : Player) {
		if (JackingVehicle[i] == vehid) return SendErrorMessage(playerid, "Quelqu'un force déjà les portes de ce véhicule.");
	}

	new param[10];
	if (sscanf(params, "s[10]", param)) {
		SendHelpMessage(playerid, "/vbl [type] ou /vbreaklock [type]");
		SendSyntaxMessage(playerid, "(q)uite - Forçage silencieux des portes avec un pied-de-biche.");
		SendSyntaxMessage(playerid, "(n)oisy - Forçage bruyant, active l'alarme du véhicule.");
		return 1;
	}	

	if (CompareStrings(param, "q") || CompareStrings(param, "quite"))
	{
		if (!CheckHanditem(playerid, 18634))		 return SendErrorMessage(playerid, "Vous devez avoir un pied-de-biche en main. (/clothing)");
		if (IsPlayerCrouching(playerid) == false) return SendErrorMessage(playerid, "Vous devez être accroupi.");

		SetPVarInt(playerid, #JackingVehicle, vehid);
		pTemp[playerid][JackingType] = 1;

		static const str_size[] = "{"COL_GREY"}Voulez-vous commencer à forcer les portes de %s ? Cela prendra %i secondes.";
		new mes[sizeof(str_size) - 4 + 1 + 31 + 7];
		format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(vehid)-400], 250 + VehicleInfo[vehid][carLock] * 250);
		Dialog_Show(playerid, StartJackingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Forçage des portes", mes, "Commencer", "Annuler");
	}
	else if (CompareStrings(param, "n") || CompareStrings(param, "noisy"))
	{
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "Vous devez être debout.");

		SetPVarInt(playerid, #JackingVehicle, vehid);
		pTemp[playerid][JackingType] = 2;

		static const str_size[] = "{"COL_GREY"}Voulez-vous commencer à forcer les portes de %s ?";
		new mes[sizeof(str_size) - 4 + 1 + 31 + 7];
		format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(vehid)-400]);
		Dialog_Show(playerid, StartJackingLock, DIALOG_STYLE_MSGBOX, "{FFFFFF}Forçage des portes", mes, "Commencer", "Annuler");
	}
	else {
		SendHelpMessage(playerid, "/vbl [type] ou /vbreaklock [type]");
		SendSyntaxMessage(playerid, "(q)uite - Forçage silencieux des portes avec un pied-de-biche.");
		SendSyntaxMessage(playerid, "(n)oisy - Forçage bruyant, active l'alarme du véhicule.");
		return 1;
	}

	return 1;
}

ALTX:unscramble("/uns");
CMD:unscramble(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid) || !JackingEngine[playerid]) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette commande pour le moment !");

	new unword[64];
	if (sscanf(params, "s[64]", unword))	
		return SendHelpMessage(playerid, "/(uns)cramble [mot déchiffré]");

	new vehid = GetPlayerVehicleID(playerid);
	if (!isnull(unword) && CompareStrings(unword, UnscrambleInfo[pTemp[playerid][pUnscrambleID]][eUnscrambledWord]))
	{
		pTemp[playerid][pScrambleSuccess]++; 
		PlaySound(playerid, 1052);

		switch(VehicleInfo[vehid][carImmob])
		{
			case 0: if (pTemp[playerid][pScrambleSuccess] < 3) return NewScrambleWord(playerid);
			case 1: if (pTemp[playerid][pScrambleSuccess] < 4) return NewScrambleWord(playerid);
			case 2: if (pTemp[playerid][pScrambleSuccess] < 5) return NewScrambleWord(playerid);
			case 3: if (pTemp[playerid][pScrambleSuccess] < 6) return NewScrambleWord(playerid);
			case 4: if (pTemp[playerid][pScrambleSuccess] < 7) return NewScrambleWord(playerid);
		}

		JackingEngine[playerid] = 0;
		pTemp[playerid][pUnscrambleID] = 0;
		pTemp[playerid][pScrambleFailed] = 0;
		pTemp[playerid][pScrambleSuccess] = 0;

		if (IsValidVehicle(GetPlayerVehicleID(playerid))) {
			ToggleVehicleEngine(GetPlayerVehicleID(playerid), playerid);
			PlaySound(playerid, 1145);
			AlarmOff(vehid);
			InfoMSG(playerid, "MOTEUR ~g~DÉMARRÉ ~w~AVEC SUCCÈS !", 3);
		}
	}
	else {
		pTemp[playerid][pScrambleFailed]++; 
		PlaySound(playerid,1085);
		
		switch(VehicleInfo[vehid][carImmob])
		{
			case 0: if (pTemp[playerid][pScrambleFailed] == 3) return StopJackingEngine(playerid);
			case 1: {
				if (pTemp[playerid][pScrambleFailed] >= 2){
					PlaySound(playerid, 1138);
					AlarmOn(vehid);
				
					if (pTemp[playerid][pScrambleFailed] == 3) return StopJackingEngine(playerid);
				}
			}
			case 2: {
				if (pTemp[playerid][pScrambleFailed] >= 2){
					PlaySound(playerid, 1138);
					AlarmOn(vehid);
					return StopJackingEngine(playerid);
				}
			}
			case 3: {
				PlaySound(playerid, 1138);
				AlarmOn(vehid);

				if (pTemp[playerid][pScrambleFailed] == 2) return StopJackingEngine(playerid);
			}
			case 4: {
				PlaySound(playerid, 1138);
				AlarmOn(vehid);
				return StopJackingEngine(playerid);
			}
		}
		
		NewScrambleWord(playerid);
	}

	return 1;
}

/*
ALTX:jackingvehicle("/jackv");
CMD:jackingvehicle(playerid, params[])
{
	if (!VehicleInfo[vehicle][carOwnerID]) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas voler un véhicule gouvernemental.");
	if (!VehicleInfo[vehicle][carEngineOn]) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas voler un véhicule éteint.");
	if (PlayerInfo[playerid][pCarKey] == vehicle || PlayerInfo[playerid][pSpareKey] == VehicleInfo[vehicle][carID]) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas voler votre propre véhicule ou un véhicule dont vous avez les clés.");
	if (!PoliceOnline()) 
		return SendErrorMessage(playerid, "Il doit y avoir au moins %i policiers en ligne.", NEED_COPS_TO_CRIME);

	new jtime = pTemp[playerid][JackingTime] - gettime();

	if (GetPVarInt(playerid, #JACKING_PROCESS)) 
		return SendErrorMessage(playerid, "Vous êtes déjà en train de voler un véhicule.");
	if (pTemp[playerid][JackingTime] == 0) 
		return SendErrorMessage(playerid, "Vous n'avez volé aucun véhicule.");
	if (jtime > 0) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas voler ce véhicule avant %i secondes.", jtime);
	if (GetPlayerVehicleID(playerid) != pTemp[playerid][JackingVehID]) 
		return SendErrorMessage(playerid, "Vous n'avez pas volé ce véhicule.");
	if (IsPoliceNear(playerid)) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas voler un véhicule car des policiers sont à proximité.");

	pTemp[playerid][JackingTime] = 0;
	SetPVarInt(playerid, #JACKING_PROCESS, 1);

	DisableWaypoint(playerid);
	if (IsValidDynamicCP(pTemp[playerid][JackingCP])) 
		DestroyDynamicCP(pTemp[playerid][JackingCP]);
	pTemp[playerid][JackingCP] = 0;

	new randomCP = random(sizeof(Jacking_CP));
	pTemp[playerid][JackingCP] = CreateDynamicCP(Jacking_CP[randomCP][0], Jacking_CP[randomCP][1], Jacking_CP[randomCP][2], 1.5, 0, 0, playerid, 15000.0);
	TogglePlayerDynamicCP(playerid, pTemp[playerid][JackingCP], true);
	Streamer_Update(playerid, STREAMER_TYPE_CP);

	InfoMSG(playerid, "Livrez le véhicule au ~r~checkpoint~w~ pour recevoir ~g~de l'argent~w~. Si à votre arrivée vous êtes ~y~poursuivi~w~, un ~r~nouveau checkpoint~w~ sera créé !");
	return 1;
}*/

ALTX:deletejackvehicles("/delveh");
CMD:deletejackvehicles(playerid, params[])
{
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM cars WHERE owner = %i AND jacking = 1", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query, "Remove_ShowVehicleList", "i", playerid);

	return 1;
}
