this::VehicleUpgrade(playerid, cost, level, color2)
{
	new 
		uptime = 1000,
		vehicleid = PlayerInfo[playerid][pCarKey],
		model = GetVehicleModel(vehicleid);

	if (!IsValidVehicle(vehicleid) || !IsPlayerLogged(playerid) || VehicleInfo[vehicleid][carEngineOn])
	{
		DeletePVar(playerid, #vehicle_upgrade_timer);
		UpdateDynamic3DTextLabelText(status_update[vehicleid], 0x33AA3300, "(( ---------- ))");
		VehicleInfo[vehicleid][carAttach3DtextUse] = 0;
		return 1;
	}

	new upgrade_name[13][32] = {
		"Peinture du véhicule",
		"Mise à jour de la serrure",
		"Mise à jour de l'alarme",
		"Mise à jour de l'immobilisateur",
		"Mise à jour de l'assurance",
		"Mise à jour de l'armure",
		"Enregistrement du véhicule",
		"Suppression de l'enregistrement",
		"Remplacement du moteur",
		"Peinture du véhicule",
		"Remplacement de la batterie",
		"Remplissage de carburant",
		"Réparation du véhicule"
	};

	new string[128] = "(( ", upgrade = 0;

	if (upgrade == 12) uptime = 15 / 11 * 1000;

	for (new i; i != 10; i++)
	{
		if (GetPVarInt(playerid, #vehicle_upgrade_timer) >= i) strcat(string, "I");
		else strcat(string, "-");
	}

	format(string, sizeof(string), "%s ))\n%s", string, upgrade_name[VehicleInfo[vehicleid][carAttach3DtextUse] - 1]);
	UpdateDynamic3DTextLabelText(status_update[vehicleid], 0x33AA33FF, string);
	SetPVarInt(playerid, #vehicle_upgrade_timer, GetPVarInt(playerid, #vehicle_upgrade_timer) + 1);

	if (GetPVarInt(playerid, #vehicle_upgrade_timer) == 11)
	{
		upgrade = VehicleInfo[vehicleid][carAttach3DtextUse];
		DeletePVar(playerid, #vehicle_upgrade_timer);
		UpdateDynamic3DTextLabelText(status_update[vehicleid], 0x33AA3300, "(( ---------- ))");
		VehicleInfo[vehicleid][carAttach3DtextUse] = 0;
	}
	else
	{
		SetTimerEx("VehicleUpgrade", uptime, 0, "iiii", playerid, cost, level, color2);
	}

	if (upgrade == 0) return 1;

	new dealer = -1;

	if (upgrade != 13 && upgrade != 12 && upgrade != 1 && upgrade != 7 && (dealer = ModShop_Nearest(playerid)) == -1) return SendErrorMessage(playerid, "Vous devez être près d'un atelier de tuning.");
	if (upgrade == 12 && AZC_Nearest(playerid) == -1) return SendErrorMessage(playerid, "Vous devez être près d'une station-service.");
	if (upgrade == 13 && PAS_Nearest(playerid) == -1) return SendErrorMessage(playerid, "Vous devez être près d'un atelier de réparation.");

	if (upgrade == 1)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		GiveCash(playerid, -cost);
		RespawnVehicle(vehicleid);
		return SendSyntaxMessage(playerid, "Votre %s a été remorqué avec succès à son emplacement de parking.", VehicleNames[model - 400]);
	}
	else if (upgrade == 2)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		GiveCash(playerid, -cost);
		SendSyntaxMessage(playerid, "Mise à jour de la serrure au niveau %i effectuée pour $%i.", level, cost);
		UnFreezePlayer(playerid);
		VehicleInfo[vehicleid][carLock] = level;
		Save_Car(vehicleid);
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 3)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		GiveCash(playerid, -cost);
		SendSyntaxMessage(playerid, "Mise à jour de l'alarme au niveau %i effectuée pour $%i.", level, cost);
		UnFreezePlayer(playerid);
		VehicleInfo[vehicleid][carAlarm] = level;
		Save_Car(vehicleid);
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 4)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		GiveCash(playerid, -cost);
		SendSyntaxMessage(playerid, "Mise à jour de l'immobilisateur au niveau %i effectuée pour $%i.", level, cost);
		UnFreezePlayer(playerid);
		VehicleInfo[vehicleid][carImmob] = level;
		Save_Car(vehicleid);
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 5)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		GiveCash(playerid, -cost);
		SendSyntaxMessage(playerid, "Mise à jour de l'assurance au niveau %i effectuée pour $%i.", level, cost);
		UnFreezePlayer(playerid);
		VehicleInfo[vehicleid][carInsurance] = level;
		Save_Car(vehicleid);
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 6)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		GiveCash(playerid, -cost);
		SendSyntaxMessage(playerid, "%i d'armure ajoutée, vous avez été débité de $%i.", level, cost);
		UnFreezePlayer(playerid);
		VehicleInfo[vehicleid][carArmour] += float(level);

		if (VehicleInfo[vehicleid][carArmour] > 250.0) VehicleInfo[vehicleid][carArmour] = 250.0;
		
		SetVehicleHealth(vehicleid, GetVehicleMaxHealth(GetVehicleModel(vehicleid)) + VehicleInfo[vehicleid][carArmour]);
		UpdateVehicleDamageStatus(vehicleid, 0, 0, 0, 0);
		Save_Car(vehicleid);
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 7)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		if (!IsVehicleInRangeOfPoint(vehicleid, 15.0, 697.6670, 1624.8490, 785.5494))
		{
			return SendErrorMessage(playerid, "Votre véhicule doit être près du poste de police de Palomino.");
		}
		
		format(VehicleInfo[vehicleid][carPlate], 24, "%s", GetVehicleNumber());
		VehicleInfo[vehicleid][carRegistered] = 1;
		SetVehicleNumberPlate(vehicleid, VehicleInfo[vehicleid][carPlate]);
		RespawnVehicle(vehicleid, 1);
		GiveCash(playerid, -cost);
		SendSyntaxMessage(playerid, "Véhicule enregistré pour $%i. Vos nouvelles plaques : %s", cost, VehicleInfo[vehicleid][carPlate]);
		UnFreezePlayer(playerid);

		new mes[128];
		format(mes, sizeof(mes), "~w~Vous avez installé de nouvelles plaques~n~~y~%s pour $%i.", VehicleInfo[vehicleid][carPlate], cost);
		JobMessage(playerid, mes, 6);

		Save_Car(vehicleid);
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 8)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		if (!IsVehicleInRangeOfPoint(vehicleid, 15.0, 697.6670, 1624.8490, 785.5494))
		{
			return SendErrorMessage(playerid, "Votre véhicule doit être près du poste de police de Palomino.");
		}

		format(VehicleInfo[vehicleid][carPlate], 24, "_");
		VehicleInfo[vehicleid][carRegistered] = 0;
		SetVehicleNumberPlate(vehicleid, "_");
		RespawnVehicle(vehicleid, 1);
		Save_Car(vehicleid);

		SendSyntaxMessage(playerid, "Véhicule désenregistré.");
		JobMessage(playerid, "~w~Votre véhicule a été désenregistré.", 2);
		UnFreezePlayer(playerid);
		return 1;
	}
	else if (upgrade == 9)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);
			
		format(string, sizeof(string), "~w~Moteur remplacé (100.00) pour $%i.", cost);
		JobMessage(playerid, string, 6);
		SendSyntaxMessage(playerid, "Remplacement du moteur effectué pour $%i.", cost);
		GiveCash(playerid, -cost);
		UnFreezePlayer(playerid);
		VehicleInfo[vehicleid][carEngine] = 100.00;
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 10)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		SendSyntaxMessage(playerid, "Vous avez repeint votre véhicule pour $%i.", cost);
		GiveCash(playerid, -cost);
		UnFreezePlayer(playerid);
		ChangeVehicleColor(vehicleid, level, color2);
		VehicleInfo[vehicleid][carColor1] = level;
		VehicleInfo[vehicleid][carColor2] = color2;
		Save_Car(vehicleid);
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 11)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);

		format(string, sizeof(string), "~w~Batterie remplacée (100.00) pour $%i.", cost);
		JobMessage(playerid, string, 6);
		SendSyntaxMessage(playerid, "Remplacement de la batterie effectué. Vous avez été débité de $%i.", cost);
		GiveCash(playerid, -cost);
		UnFreezePlayer(playerid);
		VehicleInfo[vehicleid][carBattery] = 100.00;
		GiveBizCash(dealer, cost / 100 * 80);
		return 1;
	}
	else if (upgrade == 12)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);
		return 1;
	}
	else if (upgrade == 13)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);
		GiveCash(playerid, -cost);

		FixVehicle(vehicleid);

		if (BizInfo[GetPVarInt(playerid, #bizid)][bizOwnerID] != 0)
		{
			if (BizInfo[GetPVarInt(playerid, #bizid)][bizProduct] > 10) BizInfo[GetPVarInt(playerid, #bizid)][bizProduct]--;
			GiveBizCash(GetPVarInt(playerid, #bizid), cost);
		}

		format(string, sizeof(string), "~w~Vous avez réparé\nvotre véhicule pour ~h~~g~$%i.", cost);
		InfoMSG(playerid, string, 5);
		SM2(playerid, "Votre véhicule a été réparé pour $%i.", cost);		
		
		return 1;
	}
	else if (upgrade == 14)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);
		GiveCash(playerid, -cost);

		UpdateVehicleDamageStatus(vehicleid, pTemp[playerid][pListed][0], pTemp[playerid][pListed][1], pTemp[playerid][pListed][2], pTemp[playerid][pListed][3]);
		for (new i; i < 4; i++) pTemp[playerid][pListed][i] = 0;

		if (BizInfo[GetPVarInt(playerid, #bizid)][bizOwnerID] != 0)
		{
			if (BizInfo[GetPVarInt(playerid, #bizid)][bizProduct] > 10) BizInfo[GetPVarInt(playerid, #bizid)][bizProduct]--;
			GiveBizCash(GetPVarInt(playerid, #bizid), cost);
		}

		format(string, sizeof(string), "~w~Vous avez réparé\nvotre véhicule pour ~h~~g~$%i.", cost);
		InfoMSG(playerid, string, 5);
		SM2(playerid, "Votre véhicule a été réparé pour $%i.", cost);		
		
		return 1;
	}
	else if (upgrade == 15)
	{
		if (GetCash(playerid) < cost) return SendNotCash(playerid);
		GiveCash(playerid, -cost);
		
		SetVehicleHealth(vehicleid, GetVehicleMaxHealth(GetVehicleModel(vehicleid)));

		if (BizInfo[GetPVarInt(playerid, #bizid)][bizOwnerID] != 0)
		{
			if (BizInfo[GetPVarInt(playerid, #bizid)][bizProduct] > 10) BizInfo[GetPVarInt(playerid, #bizid)][bizProduct]--;
			GiveBizCash(GetPVarInt(playerid, #bizid), cost);
		}

		format(string, sizeof(string), "~w~Vous avez réparé\nvotre véhicule pour ~h~~g~$%i.", cost);
		InfoMSG(playerid, string, 5);
		SM2(playerid, "Votre véhicule a été réparé pour $%i.", cost);		
		
		return 1;
	}
	return 1;
}

SendMechanicHelp(playerid, location[])
{
	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pJob] != JOB_MECHANIC) continue;

		SendClientMessage(i, COLOR_GREEN, "|_________Appel au service des mécaniciens_________|");
		if (pTemp[playerid][pPayphone] == -1) 
			SendClientMessageEx(i, COLOR_WHITE, "Appel de : %s Tel : %i", GetNameEx(playerid), PlayerInfo[playerid][pNumber]);
		else 
			SendClientMessageEx(i, COLOR_WHITE, "Appel de : Cabine téléphonique, Tel : %i", PayphoneInfo[pTemp[playerid][pPayphone]][phNumber]);
		SendClientMessageEx(i, COLOR_WHITE, "Situation : %s", location);
	}
	return 1;
}

Mechanic_OnPlayerExitVehicle(playerid)
{
	if (RepairService[playerid] != 0)
	{
		ShowJobMessage(playerid, "~h~~p~COMMENCEZ À PEINDRE LE VÉHICULE");
		GivePlayerWeaponEx(playerid, 41, 999);
	}
	return 1;
}

StartVehicleUpgrade(playerid, vehicleid, type, cost, level = 0, color2 = 0)
{
	if (type != 12 && VehicleInfo[vehicleid][carOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce véhicule doit vous appartenir.");
		
	if (VehicleInfo[vehicleid][carAttach3DtextUse] != 0) 
		return SendErrorMessage(playerid, "Veuillez patienter, une opération est déjà en cours.");
		
	VehicleInfo[vehicleid][carAttach3DtextUse] = type;
	SetTimerEx("VehicleUpgrade", 1, 0, "iiii", playerid, cost, level, color2);
	SetPVarInt(playerid, #vehicle_upgrade_timer, 0);
	return 1;
}