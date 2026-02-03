Dialog:DuplicateKey(playerid, response, listitem, inputtext[])
{
	if (!response)																			return 1;
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))										return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");
	if (VehicleInfo[PlayerInfo[playerid][pCarKey]][carOwnerID] != PlayerInfo[playerid][pID])	return 1;

	static const str_size[] = "{ffffff}Création d'un double pour %s. Entrez {C3C3C3}[ID/partie du nom du joueur] {ffffff}à qui vous souhaitez donner un double des clés :";
	new mes[sizeof(str_size) - 2 + 1 + 32];

	format(mes, sizeof(mes), str_size, VehicleNames[GetVehicleModel(PlayerInfo[playerid][pCarKey])-400]);
		
	new userid;

	if (sscanf(inputtext, "u", userid))
	{
		Dialog_Show(playerid, DuplicateKey, DIALOG_STYLE_INPUT, "{FFFFFF}Création d'un double", mes, "Acheter", "Annuler");
		return SendErrorMessage(playerid, "Veuillez entrer l'ID du joueur.");
	}

	if (!IsPlayerLogged(userid) || userid == playerid)
	{
		Dialog_Show(playerid, DuplicateKey, DIALOG_STYLE_INPUT, "{FFFFFF}Création d'un double", mes, "Acheter", "Annuler");
		return SendNotConnected(playerid);
	}

	if (!PlayerNearPlayer(5.0, playerid, userid))
	{
		Dialog_Show(playerid, DuplicateKey, DIALOG_STYLE_INPUT, "{FFFFFF}Création d'un double", mes, "Acheter", "Annuler");
		return SendNotNearPlayer(playerid);
	}
	
	if (GetCash(playerid) < 150)	 return SendNotCash(playerid);
		
	GiveCash(playerid, -150);
	PlayerInfo[userid][pSpareKey] = VehicleInfo[PlayerInfo[playerid][pCarKey]][carID];
	SendSyntaxMessage(playerid, "Un double des clés de %s a été créé et remis au joueur %s.", VehicleNames[GetVehicleModel(PlayerInfo[playerid][pCarKey])-400], GetNameEx(userid));
	SendSyntaxMessage(userid, "%s a acheté pour vous un double des clés de %s. (/dropkey - pour jeter le double).", GetNameEx(playerid), VehicleNames[GetVehicleModel(PlayerInfo[playerid][pCarKey])-400]);
	return 1;
}

Dialog:ScrapSell(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new veh = PlayerInfo[playerid][pCarKey];
	new string1[128], mes[32];

	if (IsABicycle(veh))
	{
		format(mes, sizeof(mes), "%s", VehicleNames[VehicleInfo[veh][carModel] - 400]);
	}
	else
	{
		if (VehicleInfo[veh][carRegistered])
		{
			format(mes, sizeof(mes), "{FFFFFF}%s [%s]", VehicleNames[VehicleInfo[veh][carModel] - 400], VehicleInfo[veh][carPlate]);
		}
		else
		{
			format(mes, sizeof(mes), "{FFFFFF}%s", VehicleNames[VehicleInfo[veh][carModel] - 400]);
		}
	}

	format(string1, sizeof(string1), "{FFFFFF}Êtes-vous sûr de vouloir vendre ce véhicule pour $%i ?", GetVehiclePrice(VehicleInfo[veh][carModel]) / 2);
	Dialog_Show(playerid, ScrapVehicle, DIALOG_STYLE_MSGBOX, mes, string1, "Oui", "Non");
	return 1;
}

Dialog:ScrapVehicle(playerid, response, listitem, inputtext[])
{
	if (response)
	{
		new veh = PlayerInfo[playerid][pCarKey];

		SendSyntaxMessage(playerid, "Vous avez vendu votre véhicule \"%s\" pour $%i.", VehicleNames[VehicleInfo[veh][carModel] - 400], GetVehiclePrice(VehicleInfo[veh][carModel]) / 2);
		GiveCash(playerid, GetVehiclePrice(VehicleInfo[veh][carModel]) / 2);

		new query[256], money = PlayerInfo[playerid][pCash];

		format(query, sizeof(query), "%s [MYSQL: %i | IP: %s | Argent(AVANT): %i | Argent(APRÈS): %i] a vendu le véhicule %s(Modèle: %i) pour -- $%i", 
		GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, 
		GetCash(playerid), VehicleNames[VehicleInfo[veh][carModel] - 400], VehicleInfo[veh][carModel], GetVehiclePrice(VehicleInfo[veh][carModel]) / 2);
		insert_log(query, VEHICLE_LOG, 3, PlayerInfo[playerid][pAccID]);

		DeleteVehicleTickets(veh);
		
		mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `cars` WHERE `id` = %i", VehicleInfo[veh][carID]);
		mysql_tquery(dbHandle, query);

		PlayerInfo[playerid][pCarKey] = 0;
		VehicleInfo[veh][carOwnerPlayer] = INVALID_PLAYER_ID;
		DestroyVehicle(veh);
	}
	return 1;
}

Dialog:BuyingVehicleList(playerid, response, listitem, inputtext[])
{
	if (response) {
		ShowBuyingVehicleDetails(playerid);
	} else {
		new registerveh = price_reg[GetPVarInt(playerid, "BuyingVehicleRegister")], 
			alarmveh = price_alarm[GetPVarInt(playerid, "BuyingVehicleAlarm")],
			lockveh = price_lock[GetPVarInt(playerid, "BuyingVehicleLock")],
			immobveh = price_immob[GetPVarInt(playerid, "BuyingVehicleImmob")], 
			insuranceveh = GetInsurancePrice(GetPVarInt(playerid, "BuyingVehicleModel")) * GetPVarInt(playerid, "BuyingVehicleInsurance"),
			xmradioveh = price_radio[GetPVarInt(playerid, "BuyingVehicleRadio")], 
			gpsveh = price_gps[GetPVarInt(playerid, "BuyingVehicleGps")];

		new mes[56];

		format(mes, sizeof(mes), "{FFFFFF}%s - {35B835}$%i", 
			VehicleNames[GetPVarInt(playerid, "BuyingVehicleModel") - 400], 
			GetVehiclePrice(GetPVarInt(playerid, "BuyingVehicleModel")) + registerveh + alarmveh + lockveh + immobveh + insuranceveh + xmradioveh + gpsveh
		);

		Dialog_Show(playerid, BuyingVehicleDone, DIALOG_STYLE_MSGBOX, mes, "{FFFFFF}Êtes-vous sûr de vouloir acheter ce véhicule ?", "Acheter", "Annuler");
	}
	return 1;
}

Dialog:BuyingVehicleDone(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new query[128];
   	mysql_format(dbHandle, query, sizeof(query), "SELECT `id` FROM `cars` WHERE `owner` = %i", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query, "BuyVehicleForPlayer", "i", playerid);
	return 1;
}

Dialog:RansomVehicle(playerid, response, listitem, inputtext[])
{
	if (!response) {
		DeletePVar(playerid, #RansomVehModel);
		DeletePVar(playerid, #RansomVehID);
		return 1;
	}

	Dialog_Show(playerid, RansomVehicle2, DIALOG_STYLE_LIST, "Options de rachat", "Racheter mon véhicule\nSupprimer mon véhicule", "Choisir", "<<");
	return 1;
}

Dialog:RansomVehicle2(playerid, response, listitem, inputtext[])
{
	if (!response) {
		DeletePVar(playerid, #RansomVehModel);
		DeletePVar(playerid, #RansomVehID);
		return 1;
	}

	if (listitem == 0) {
		new model = GetPVarInt(playerid, #RansomVehModel),
			vehid = GetPVarInt(playerid, #RansomVehID);
		
		DeletePVar(playerid, #RansomVehModel);
		DeletePVar(playerid, #RansomVehID);

		new price = floatround(GetVehiclePrice(model) * 0.5);
		if (GetCash(playerid) < price) return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent !");
		
		SendSyntaxMessage(playerid, "Vous avez racheté votre %s pour $%i.", VehicleNames[model - 400], price);
		SQL_SetInteger("cars", "jacking", 0, vehid);
		GiveCash(playerid, -price);
	} else {
		new mes[128];
		format(mes, sizeof(mes), "{FFFFFF}Voulez-vous vraiment {FF6347}supprimer{FFFFFF} %s définitivement ?", GetVehName(pTemp[playerid][MyCars][listitem]));
		ConfirmDialog(playerid, "Confirmation de suppression du véhicule", mes, "DELETE_VEHICLE");
	}
	return 1;
}

Dialog:MyCars(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	
	if (IsValidVehicle(PlayerInfo[playerid][pCarKey]) && pTemp[playerid][MyCarSpawn][listitem])
		return cmd::vmenu(playerid, "");
	else
	{
		if (!IsValidVehicle(PlayerInfo[playerid][pCarKey]))
		{
			pTemp[playerid][MyCarSpawn][0] = listitem;

			new dlgmes[64];
			format(dlgmes, sizeof(dlgmes), "{FFFFFF}Voulez-vous vraiment faire apparaître %s ?", GetVehName(pTemp[playerid][MyCars][listitem]));
			ConfirmDialog(playerid, "Confirmation", dlgmes, "MyCarsSpawn");
		}
		else 
		{
			SendErrorMessage(playerid, "Vous avez déjà fait apparaître un véhicule.");
			cmd::mesveh(playerid, "");
		}
	}

	return 1;
}

Dialog:BuyingVehicleDetails(playerid, response, listitem, inputtext[])
{
	new registerveh = price_reg[GetPVarInt(playerid, "BuyingVehicleRegister")], 
		alarmveh = price_alarm[GetPVarInt(playerid, "BuyingVehicleAlarm")], 
		lockveh = price_lock[GetPVarInt(playerid, "BuyingVehicleLock")], 
		immobveh = price_immob[GetPVarInt(playerid, "BuyingVehicleImmob")], 
		insuranceveh = GetInsurancePrice(GetPVarInt(playerid, "BuyingVehicleModel")) * GetPVarInt(playerid, "BuyingVehicleInsurance"), 
		xmradioveh = price_radio[GetPVarInt(playerid, "BuyingVehicleRadio")], gpsveh = price_gps[GetPVarInt(playerid, "BuyingVehicleGps")];
		
	new registeruse[128], lockuse[128], alarmuse[250], immobuse[260], insuranceuse[260], xmradiouse[128], gpsuse[128];

	if (GetPVarInt(playerid, "BuyingVehicleRegister")) 
		format(registeruse, sizeof(registeruse), "{ffffff}Enregistrement : \t{ecc317}$%i\n{ecc317}+ {ffffff}Plaque d'immatriculation du véhicule.\n\n", registerveh);

	if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 1) 
		format(alarmuse, sizeof(alarmuse), "{ffffff}Alarme : \t\t{bab9ba}Niveau 1 \t{ecc317}$%i\n{ecc317}+ {ffffff}Alarme sonore puissante.\n\n", alarmveh);
	else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 2) 
		format(alarmuse, sizeof(alarmuse), "{ffffff}Alarme : \t\t{bab9ba}Niveau 2 \t{ecc317}$%i\n{ecc317}+ {ffffff}Alarme sonore puissante.\n{ecc317}+ {ffffff}Notification de vol à la police.\n\n", alarmveh);
	else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 3) 
		format(alarmuse, sizeof(alarmuse), "{ffffff}Alarme : \t\t{bab9ba}Niveau 3 \t{ecc317}$%i\n{ecc317}+ {ffffff}Alarme sonore puissante.\n{ecc317}+ {ffffff}Notification de vol à la police.\n{ecc317}+ {ffffff}Notification de vol au propriétaire par SMS.\n\n", alarmveh);
	else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 4) 
		format(alarmuse, sizeof(alarmuse), "{ffffff}Alarme : \t\t{bab9ba}Niveau 4 \t{ecc317}$%i\n{ecc317}+ {ffffff}Alarme sonore puissante.\n{ecc317}+ {ffffff}Notification de vol à la police.\n{ecc317}+ {ffffff}Notification de vol au propriétaire par SMS et facturation.\n\n", alarmveh);

	if (GetPVarInt(playerid, "BuyingVehicleLock") > 0) 
		format(lockuse, sizeof(lockuse), "{ffffff}Serrure : \t\t{bab9ba}Niveau %i \t{ecc317}$%i\n{ecc317}+ {ffffff}%i secondes pour crocheter la serrure du véhicule.\n\n", GetPVarInt(playerid, "BuyingVehicleLock"), lockveh, 250 + (GetPVarInt(playerid, "BuyingVehicleLock") * 250));

	if (GetPVarInt(playerid, "BuyingVehicleImmob") > 0) 
		format(immobuse, sizeof(immobuse), "{ffffff}Antidémarrage : \t{bab9ba}Niveau %i \t{ecc317}$%i\n{ecc317}+ {ffffff}Aide à prévenir le crochetage de la serrure d'allumage.\n\n", GetPVarInt(playerid, "BuyingVehicleImmob"), immobveh);

	if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 1) 
		format(insuranceuse, sizeof(insuranceuse), "{ffffff}Assurance : \t\t{bab9ba}Niveau 1 \t{ecc317}$%i\n{ecc317}+ {ffffff}Le véhicule sera toujours restauré avec sa santé maximale.\n\n", insuranceveh);
	else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 2) 
		format(insuranceuse, sizeof(insuranceuse), "{ffffff}Assurance : \t\t{bab9ba}Niveau 2 \t{ecc317}$%i\n{ecc317}+ {ffffff}Le véhicule sera toujours restauré avec sa santé maximale.\n{ecc317}+ {ffffff}Le véhicule sera restauré sans dommages.\n\n", insuranceveh);
	else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 3) 
		format(insuranceuse, sizeof(insuranceuse), "{ffffff}Assurance : \t\t{bab9ba}Niveau 3 \t{ecc317}$%i\n{ecc317}+ {ffffff}Le véhicule sera toujours restauré avec sa santé maximale.\n{ecc317}+ {ffffff}Le véhicule sera restauré sans dommages.\n{ecc317}+ {ffffff}Conservation du tuning et de l'autoradio.\n\n", insuranceveh);

	if (GetPVarInt(playerid, "BuyingVehicleRadio") == 1) 
		format(xmradiouse, sizeof(xmradiouse), "{ffffff}Autoradio : \t{ecc317}$%i\n{ecc317}+ {ffffff}Installation de l'autoradio et d'autres composants audio.\n\n", xmradioveh);

	if (GetPVarInt(playerid, "BuyingVehicleGps") == 1) 
		format(gpsuse, sizeof(gpsuse), "{ffffff}GPS : \t\t{ecc317}$%i\n{ecc317}+ {ffffff}Installation du GPS dans le véhicule.\n\n", gpsveh);

	new mes[56];

	format(mes, sizeof(mes), "{ffffff}%s - {35B835}$%i", VehicleNames[GetPVarInt(playerid, "BuyingVehicleModel")-400], GetVehiclePrice(GetPVarInt(playerid, "BuyingVehicleModel"))+registerveh+alarmveh+lockveh+immobveh+insuranceveh+xmradioveh+gpsveh);
	if (!response)
	{
		new vehFuel[32], 
			vehDrive[32],
			modelid = GetPVarInt(playerid, "BuyingVehicleModel"),
			Float:vehMass = GetVehicleModelInfoAsFloat(modelid, "fMass"); 
						
		if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nDriveType") == 'F')
			vehDrive = "Traction avant";
		else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nDriveType") == 'R')
			vehDrive = "Propulsion arrière";
		else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nDriveType") == '4')
			vehDrive = "Transmission intégrale";
							
							
		if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'P')
			vehFuel = "Essence";
		else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'D')
			vehFuel = "Diesel";
		else if (GetVehicleModelInfoAsInt(modelid, "TransmissionData_nEngineType") == 'E')
			vehFuel = "Électrique";
							
		static const str_size[] = "{ecc317}Prix : \t\t\t\t\t\t{ffffff}$%s\n\
					{ecc317}Vitesse max : \t\t\t\t{ffffff}%.1f km/h\n\
					{ecc317}État max : \t\t\t\t\t{ffffff}%.1f\n\
					{ecc317}Poids du véhicule : \t\t{ffffff}%.1f kg\n\n\
					{ecc317}Transmission : \t\t\t\t{ffffff}%s\n\
					{ecc317}Carburant : \t\t\t\t\t{ffffff}%s\n\
					{ecc317}Consommation : \t\t\t\t{ffffff}%.1f L/100 km\n\
					{ecc317}Capacité du réservoir : \t{ffffff}%.1f L\n\n";
		new string[sizeof(str_size) + 1228];

		format(string, sizeof(string), str_size,
            FormatNumber(GetVehiclePrice(modelid)), 
            GetVehicleMaxSpeed[modelid-400], 
            GetVehicleMaxHealth(modelid), 
            vehMass, 
            vehDrive, 
            vehFuel,
            GetVehicleFuelUsage(modelid),
            GetVehicleFuelCapacity(modelid),
			VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color1)] >>> 8, GetPVarInt(playerid, #veh_color1), 
			VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color2)] >>> 8, GetPVarInt(playerid, #veh_color2)
		);
		
		format(string, sizeof(string), "%s{ecc317}Couleur 1{ffffff} - [{%06x}#%i{ffffff}]\n{ecc317}Couleur 2{ffffff} - [{%06x}#%i{ffffff}]\n\n", string, VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color1)] >>> 8, GetPVarInt(playerid, #veh_color1), VehicleColoursTableRGBA[GetPVarInt(playerid, #veh_color2)] >>> 8, GetPVarInt(playerid, #veh_color2));
		format(string, sizeof(string), "%s%s%s%s%s%s%s%s\n{ffffff}Appuyez sur le bouton '{ecc317}Payer{ffffff}' pour acheter le véhicule.", string, lockuse, alarmuse, immobuse, insuranceuse, registeruse, xmradiouse, gpsuse);
		
		Dialog_Show(playerid, BuyingVehicleList, DIALOG_STYLE_MSGBOX, mes, string, "Modifier", "Payer");
	    return 1;
	}

	new string[301];
	switch(listitem)
	{
		case 0:
		{
			if (GetPVarInt(playerid, "BuyingVehicleRegister") == 0) 
				format(string, sizeof(string), "{eaf133}>> {ffffff}Non installé\nEnregistrement - $%i", price_reg[1]);
			else if (GetPVarInt(playerid, "BuyingVehicleRegister") == 1) 
				format(string, sizeof(string), "{ffffff}Non installé\n{eaf133}>> {ffffff}Enregistrement - $%i", price_reg[1]);

			Dialog_Show(playerid, BuyVehDetReg, DIALOG_STYLE_LIST, mes, string, "Installer", "<<");
		}
		case 1:
		{
			if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 0) 
				format(string, sizeof(string), "{ecc317}» {ffffff}Non installé\nNiveau 1 alarme - $%i\nNiveau 2 alarme - $%i\nNiveau 3 alarme - $%i\nNiveau 4 alarme - $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 1) 
				format(string, sizeof(string), "Non installé\n{ecc317}» {ffffff}Niveau 1 alarme - $%i\nNiveau 2 alarme - $%i\nNiveau 3 alarme - $%i\nNiveau 4 alarme - $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 2) 
				format(string, sizeof(string), "Non installé\nNiveau 1 alarme - $%i\n{ecc317}» {ffffff}Niveau 2 alarme - $%i\nNiveau 3 alarme - $%i\nNiveau 4 alarme - $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 3) 
				format(string, sizeof(string), "Non installé\nNiveau 1 alarme - $%i\nNiveau 2 alarme - $%i\n{ecc317}» {ffffff}Niveau 3 alarme - $%i\nNiveau 4 alarme - $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleAlarm") == 4) 
				format(string, sizeof(string), "Non installé\nNiveau 1 alarme - $%i\nNiveau 2 alarme - $%i\nNiveau 3 alarme - $%i\n{ecc317}» {ffffff}Niveau 4 alarme - $%i", price_alarm[1], price_alarm[2], price_alarm[3], price_alarm[4]);

			Dialog_Show(playerid, BuyVehDetAlarm, DIALOG_STYLE_LIST, mes, string, "Installer", "<<");
		}
		case 2:
		{
			if (GetPVarInt(playerid, "BuyingVehicleLock") == 0) 
				format(string, sizeof(string), "{ecc317}» {ffffff}Non installé\nNiveau 1 serrure - $%i\nNiveau 2 serrure - $%i\nNiveau 3 serrure - $%i\nNiveau 4 serrure - $%i\nNiveau 5 serrure - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
			else if (GetPVarInt(playerid, "BuyingVehicleLock") == 1) 
				format(string, sizeof(string), "Non installé\n{ecc317}» {ffffff}Niveau 1 serrure - $%i\nNiveau 2 serrure - $%i\nNiveau 3 serrure - $%i\nNiveau 4 serrure - $%i\nNiveau 5 serrure - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
			else if (GetPVarInt(playerid, "BuyingVehicleLock") == 2) 
				format(string, sizeof(string), "Non installé\nNiveau 1 serrure - $%i\n{ecc317}» {ffffff}Niveau 2 serrure - $%i\nNiveau 3 serrure - $%i\nNiveau 4 serrure - $%i\nNiveau 5 serrure - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
			else if (GetPVarInt(playerid, "BuyingVehicleLock") == 3) 
				format(string, sizeof(string), "Non installé\nNiveau 1 serrure - $%i\nNiveau 2 serrure - $%i\n{ecc317}» {ffffff}Niveau 3 serrure - $%i\nNiveau 4 serrure - $%i\nNiveau 5 serrure - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
			else if (GetPVarInt(playerid, "BuyingVehicleLock") == 4) 
				format(string, sizeof(string), "Non installé\nNiveau 1 serrure - $%i\nNiveau 2 serrure - $%i\nNiveau 3 serrure - $%i\n{ecc317}» {ffffff}Niveau 4 serrure - $%i\nNiveau 5 serrure - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);
			else if (GetPVarInt(playerid, "BuyingVehicleLock") == 5) 
				format(string, sizeof(string), "Non installé\nNiveau 1 serrure - $%i\nNiveau 2 serrure - $%i\nNiveau 3 serrure - $%i\nNiveau 4 serrure - $%i\n{ecc317}» {ffffff}Niveau 5 serrure - $%i", price_lock[1], price_lock[2], price_lock[3], price_lock[4], price_lock[5]);

			Dialog_Show(playerid, BuyVehDetLock, DIALOG_STYLE_LIST, mes, string, "Installer", "<<");
		}
		case 3:
		{
			if (GetPVarInt(playerid, "BuyingVehicleImmob") == 0) 
				format(string, sizeof(string), "{ecc317}» {ffffff}Non installé\nNiveau 1 immobilisateur - $%i\nNiveau 2 immobilisateur - $%i\nNiveau 3 immobilisateur - $%i\nNiveau 4 immobilisateur - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 1) 
				format(string, sizeof(string), "Non installé\n{ecc317}» {ffffff}Niveau 1 immobilisateur - $%i\nNiveau 2 immobilisateur - $%i\nNiveau 3 immobilisateur - $%i\nNiveau 4 immobilisateur - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 2) 
				format(string, sizeof(string), "Non installé\nNiveau 1 immobilisateur - $%i\n{ecc317}» {ffffff}Niveau 2 immobilisateur - $%i\nNiveau 3 immobilisateur - $%i\nNiveau 4 immobilisateur - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 3) 
				format(string, sizeof(string), "Non installé\nNiveau 1 immobilisateur - $%i\nNiveau 2 immobilisateur - $%i\n{ecc317}» {ffffff}Niveau 3 immobilisateur - $%i\nNiveau 4 immobilisateur - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);
			else if (GetPVarInt(playerid, "BuyingVehicleImmob") == 4) 
				format(string, sizeof(string), "Non installé\nNiveau 1 immobilisateur - $%i\nNiveau 2 immobilisateur - $%i\nNiveau 3 immobilisateur - $%i\n{ecc317}» {ffffff}Niveau 4 immobilisateur - $%i", price_immob[1], price_immob[2], price_immob[3], price_immob[4]);

			Dialog_Show(playerid, BuyVehDetImmob, DIALOG_STYLE_LIST, mes, string, "Installer", "<<");
		}
		case 4:
		{
			new amount = GetInsurancePrice(GetPVarInt(playerid, "BuyingVehicleModel"));

			if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 0) 
				format(string, sizeof(string), "{ecc317}» {ffffff}Non installé\nNiveau 1 assurance - $%i\nNiveau 2 assurance - $%i\nNiveau 3 assurance - $%i", amount * 1, amount * 2, amount * 3);
			else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 1) 
				format(string, sizeof(string), "Non installé\n{ecc317}» {ffffff}Niveau 1 assurance - $%i\nNiveau 2 assurance - $%i\nNiveau 3 assurance - $%i", amount * 1, amount * 2, amount * 3);
			else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 2) 
				format(string, sizeof(string), "Non installé\nNiveau 1 assurance - $%i\n{ecc317}» {ffffff}Niveau 2 assurance - $%i\nNiveau 3 assurance - $%i", amount * 1, amount * 2, amount * 3);
			else if (GetPVarInt(playerid, "BuyingVehicleInsurance") == 3) 
				format(string, sizeof(string), "Non installé\nNiveau 1 assurance - $%i\nNiveau 2 assurance - $%i\n{ecc317}» {ffffff}Niveau 3 assurance - $%i", amount * 1, amount * 2, amount * 3);

			Dialog_Show(playerid, BuyVehDetInsur, DIALOG_STYLE_LIST, mes, string, "Installer", "<<");
		}
		case 5:
		{
			if (GetPVarInt(playerid, "BuyingVehicleRadio") == 0) 
				format(string, sizeof(string), "{ecc317}» {ffffff}Non installé\nAutoradio - $%i", price_radio[1]);
			else if (GetPVarInt(playerid, "BuyingVehicleRadio") == 1) 
				format(string, sizeof(string), "Non installé\n{ecc317}» {ffffff}Autoradio - $%i", price_radio[1]);

			Dialog_Show(playerid, BuyVehDetRadio, DIALOG_STYLE_LIST, mes, string, "Installer", "<<");
		}
		case 6:
		{
			if (GetPVarInt(playerid, "BuyingVehicleGps") == 0) 
				format(string, sizeof(string), "{ecc317}» {ffffff}Non installé\nGPS - $%i", price_gps[1]);
			else if (GetPVarInt(playerid, "BuyingVehicleGps") == 1) 
				format(string, sizeof(string), "Non installé\n{ecc317}» {ffffff}GPS - $%i", price_gps[1]);

			Dialog_Show(playerid, BuyVehDetGPS, DIALOG_STYLE_LIST, mes, string, "Installer", "<<");
		}
		case 7:
		{
			SendSyntaxMessage(playerid, "Choisissez la {ffc46a}première{FFFFFF} couleur.");

			new colors[256];

			for (new i; i != 256; i++)
			{
				colors[i] = i;
			}
			SetPVarInt(playerid, #veh_color_type, 1);
			ShowColorSelectionMenu(playerid, MODEL_BUING_COLOR, colors, sizeof(colors));
		}
	}
	return 1;
}

Dialog:BuyVehDetGPS(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleGps", listitem);
		
	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetRadio(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleRadio", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetInsur(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleInsurance", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetImmob(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleImmob", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetLock(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleLock", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetAlarm(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleAlarm", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:BuyVehDetReg(playerid, response, listitem, inputtext[])
{
	if (response) SetPVarInt(playerid, "BuyingVehicleRegister", listitem);

	ShowBuyingVehicleDetails(playerid);
	return 1;
}

Dialog:VehFastAccess(playerid, response, listitem, inputtext[]) 
{
	if (!response) 										return 1;
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "Vous devez être au volant du véhicule !");

	switch(listitem)
	{
		case 0: 
		{ 
			if (CheckAdmin(playerid, ADMIN_LEVEL_2)) return cmd::amoteur(playerid, "");
			else cmd::engine(playerid, "");
			return 1;
		}
		case 1: cmd::vehicle(playerid, "lock");
		case 2: cmd::lights(playerid, "");
		case 3: cmd::trunk(playerid, "");
		case 4: cmd::hood(playerid, "");
		case 5: cmd::window(playerid, "1");
		case 6: cmd::setstation(playerid, "");
		case 7:	cmd::vfa(playerid, "");
		case 8: cmd::vmenu(playerid, "");
	}

	return 1;
}

Dialog:VehMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey])) return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");

	new vehicle = PlayerInfo[playerid][pCarKey];
	if (!vehicle || VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID]) return 1;
	
	switch (listitem)
	{
		case 0: cmd::vehicle(playerid, "stats");
		case 1: cmd::vehicle(playerid, "fines");
		case 2: cmd::vehicle(playerid, "park");
		case 3: cmd::vehicle(playerid, "find");
		case 4: cmd::alarmoff(playerid, "");
		case 5: cmd::vehicle(playerid, "tow");
		case 6: cmd::vehicle(playerid, "sell");
		case 7: Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "Vente de véhicule", "{ffffff}Vente de véhicule. Entrez 2 nombres entiers : {C3C3C3}[ID/partie du nom du joueur] [prix de vente]\nExemple : 1 35000", "Vendre", "Annuler");
	}
	return 1;
}

Dialog:SellVehTo(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::vmenu(playerid, "");
	if (!IsValidVehicle(PlayerInfo[playerid][pCarKey])) return SendErrorMessage(playerid, "Votre véhicule n'est pas spawné.");

	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!vehicle) return 1;
	if (VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Vous n'êtes pas le propriétaire de ce véhicule.");

	new userid, price;

	if (sscanf(inputtext, "ui", userid, price))
	{
		Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de véhicule", "{ffffff}Vente de véhicule. Entrez 2 nombres entiers : {C3C3C3}[ID/partie du nom du joueur] [prix de vente]\nExemple : 1 35000", "Vendre", "Annuler");
		return SendErrorMessage(playerid, "Format incorrect.");
	}

	if (!IsPlayerLogged(userid))
	{
		Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de véhicule", "{ffffff}Vente de véhicule. Entrez 2 nombres entiers : {C3C3C3}[ID/partie du nom du joueur] [prix de vente]\nExemple : 1 35000", "Vendre", "Annuler");
		return SendNotConnected(playerid);
	}

	if (!PlayerNearPlayer(3.0, playerid, userid))
	{
		Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de véhicule", "{ffffff}Vente de véhicule. Entrez 2 nombres entiers : {C3C3C3}[ID/partie du nom du joueur] [prix de vente]\nExemple : 1 35000", "Vendre", "Annuler");
		return SendNotNearPlayer(playerid);
	}

	if (price < 0 || price > 100000000)
	{
		Dialog_Show(playerid, SellVehTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente de véhicule", "{ffffff}Vente de véhicule. Entrez 2 nombres entiers : {C3C3C3}[ID/partie du nom du joueur] [prix de vente]\nExemple : 1 35000", "Vendre", "Annuler");
		return SendErrorMessage(playerid, "Entrez un prix de vente correct.");
	}

	new str[32];
	format(str, sizeof(str), "sellto %i %i", userid, price);
	cmd::vehicle(playerid, str);
	return 1;
}

Dialog:SelectClass(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem)
	{
		case 0: ShowBclass(playerid);
		case 1: ShowLowriders(playerid);
		case 2: ShowAclass(playerid);
		case 3: ShowSport(playerid);
		case 4: ShowPickups(playerid);
		case 5: ShowOffRoad(playerid);
		case 6: ShowIndustrial(playerid);
		case 7: ShowMotoBike(playerid);
		case 8: ShowOther(playerid);
		case 9: ShowDonate(playerid);
	}
	return 1;
}

Dialog:Autoservice(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	if (!IsDriver(playerid))	return SendNotInVehicle(playerid);

	new Float: health;
	AntiCheatGetVehicleHealth(GetPlayerVehicleID(playerid), health);

	new panels, doors, lights, tires;
	GetVehicleStatus(playerid, panels, doors, lights, tires);

	new 
		vehicle = GetPlayerVehicleID(playerid),
		model = GetVehicleModel(vehicle),
		hp_cost = floatround((100-(health/GetVehicleMaxHealth(model))) * (GetVehiclePrice(model)/1000), floatround_ceil)
	;
	if (hp_cost < 0) hp_cost = 0;
	new 
		cost = floatround(GetVehiclePrice(model) / 4, floatround_ceil),
		common_cost = hp_cost +  floatround((cost * 0.5 * panels/100) + (cost * 0.25 * doors/100) + (cost * 0.1 * lights/100) + (cost * 0.15 * tires/100), floatround_ceil)
	;
	
	switch(listitem) {
		case 0: { // Réparer tout
			if (common_cost <= 0)	return cmd::enter(playerid, "");
			static const dlgstr[] = "{FFFFFF}Êtes-vous sûr de vouloir réparer complètement votre véhicule pour {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, common_cost);

			ConfirmDialog(playerid, "Confirmation", mes, "RepairAll");
		}
		case 1: { // Choisir les éléments à réparer
			new panel_cost = floatround((cost * 0.5 * panels/100), floatround_ceil),
				door_cost = floatround((cost * 0.25 * doors/100), floatround_ceil),
				light_cost = floatround((cost * 0.1 * lights/100), floatround_ceil),
				tire_cost = floatround((cost * 0.15 * tires/100), floatround_ceil);

			if (panel_cost+door_cost+light_cost+tire_cost<=0)	return cmd::enter(playerid, "");

			static const dlgstr[] = "Sélectionnez les éléments que vous souhaitez réparer:\n\
									{C5C5C5}[ - ]{FFFFFF} Réparer la carrosserie: {1b8922}$%i {c5c5c5}(Dommages: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5}[ - ]{FFFFFF} Réparer les portes: {1b8922}$%i {c5c5c5}(Dommages: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5}[ - ]{FFFFFF} Réparer les phares: {1b8922}$%i {c5c5c5}(Dommages: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5}[ - ]{FFFFFF} Remplacer les pneus: {1b8922}$%i {c5c5c5}(Dommages: {FF6347}%i%%{c5c5c5})\n\
									{C5C5C5} >> RÉPARER >>";
			new mes[526+8*4];

			format(mes, sizeof(mes), dlgstr, 
				panel_cost, panels,
				door_cost, doors,
				light_cost, lights,
				tire_cost, tires
			);

			for(new i; i < 4; i++) pTemp[playerid][pListed][i] = 0;
			Dialog_Show(playerid, SELECT_Autoservice, DIALOG_STYLE_TABLIST_HEADERS, "Réparation du véhicule - Sélection des éléments", mes, "Choisir", "Annuler");
		}
		case 2: { // Restaurer l'état général
			if (hp_cost == 0)	return cmd::enter(playerid, "");
			static const dlgstr[] = "{FFFFFF}Êtes-vous sûr de vouloir restaurer l'état général du véhicule pour {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, hp_cost);

			ConfirmDialog(playerid, "Confirmation", mes, "RepairHP");
		}
		case 3: { // Remplacer la batterie
			if (VehicleInfo[vehicle][carBattery] >= 20.0)	{
				cmd::enter(playerid, "");
				return SendErrorMessage(playerid, "La batterie de ce véhicule n'a pas besoin d'être remplacée ( > 20%%).");
			}
			static const dlgstr[] = "{FFFFFF}Êtes-vous sûr de vouloir remplacer la batterie du véhicule pour {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, floatround(GetVehiclePrice(model) / 100 * 5, floatround_ceil));

			ConfirmDialog(playerid, "Confirmation", mes, "ChangeBattery");
		}
		case 4: { // Remplacer le moteur
			if (VehicleInfo[vehicle][carEngine] >= 20.0)	{
				cmd::enter(playerid, "");
				return SendErrorMessage(playerid, "Le moteur de ce véhicule n'a pas besoin d'être remplacé ( > 20%%).");
			}
			static const dlgstr[] = "{FFFFFF}Êtes-vous sûr de vouloir remplacer le moteur du véhicule pour {1b8922}$%i{FFFFFF}?";
			new mes[sizeof(dlgstr)+6];

			format(mes, sizeof(mes), dlgstr, floatround(GetVehiclePrice(model) / 100 * 10, floatround_ceil));

			ConfirmDialog(playerid, "Confirmation", mes, "ChangeEngine");
		}
	}
	return 1;
}


Dialog:SELECT_Autoservice(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::enter(playerid, "");

	new panels, doors, lights, tires;
	GetVehicleStatus(playerid, panels, doors, lights, tires);

	new vehicle = GetPlayerVehicleID(playerid),
		model = GetVehicleModel(vehicle),
		cost = floatround(GetVehiclePrice(model) / 4, floatround_ceil), 
		panel_cost = floatround((cost * 0.5 * panels / 100), floatround_ceil), 
		door_cost = floatround((cost * 0.25 * doors / 100), floatround_ceil),
		light_cost = floatround((cost * 0.1 * lights / 100), floatround_ceil),
		tire_cost = floatround((cost * 0.15 * tires / 100), floatround_ceil);

	if (listitem == 4) {
		new count;
		for (new i; i < 4; i++) if (pTemp[playerid][pListed][i]) count++;

		if (count) {
			count = 0;

			if (pTemp[playerid][pListed][0]) count += panel_cost;
			if (pTemp[playerid][pListed][1]) count += door_cost;
			if (pTemp[playerid][pListed][2]) count += light_cost;
			if (pTemp[playerid][pListed][3]) count += tire_cost;

			if (count) {
				static const dlgstr[] = "{FFFFFF}Êtes-vous sûr de vouloir réparer votre véhicule pour {1b8922}$%i{FFFFFF} ?";
				new mes[sizeof(dlgstr) + 6];

				SetPVarInt(playerid, #auto_cost, count);

				format(mes, sizeof(mes), dlgstr, count);
				return ConfirmDialog(playerid, "Confirmation", mes, "RepairList");
			}
		}
	}

	if (!pTemp[playerid][pListed][listitem]) pTemp[playerid][pListed][listitem] = 1;
	else pTemp[playerid][pListed][listitem] = 0;

	if (listitem == 0 && !panel_cost) pTemp[playerid][pListed][listitem] = 0;
	else if (listitem == 1 && !door_cost) pTemp[playerid][pListed][listitem] = 0;
	else if (listitem == 2 && !light_cost) pTemp[playerid][pListed][listitem] = 0;
	else if (listitem == 3 && !tire_cost) pTemp[playerid][pListed][listitem] = 0;

	static const select[] = "{FFFF00}[ X ]{FFFFFF}";
	static const unselect[] = "{C5C5C5}[ - ]{FFFFFF}";

	new mes[526 + 8 * 4] = "Sélectionnez les éléments que vous souhaitez réparer :\n";

	if (pTemp[playerid][pListed][0]) format(mes, sizeof(mes), "%s%s Réparer la carrosserie : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, select, panel_cost, panels);
	else format(mes, sizeof(mes), "%s%s Réparer la carrosserie : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, unselect, panel_cost, panels);

	if (pTemp[playerid][pListed][1]) format(mes, sizeof(mes), "%s%s Réparer les portes : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, select, door_cost, doors);
	else format(mes, sizeof(mes), "%s%s Réparer les portes : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, unselect, door_cost, doors);

	if (pTemp[playerid][pListed][2]) format(mes, sizeof(mes), "%s%s Réparer les phares : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, select, light_cost, lights);
	else format(mes, sizeof(mes), "%s%s Réparer les phares : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, unselect, light_cost, lights);

	if (pTemp[playerid][pListed][3]) format(mes, sizeof(mes), "%s%s Remplacer les pneus : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, select, tire_cost, tires);
	else format(mes, sizeof(mes), "%s%s Remplacer les pneus : {1b8922}$%i {c5c5c5}(Dommages : {FF6347}%i%%{c5c5c5})\n", mes, unselect, tire_cost, tires);

	format(mes, sizeof(mes), "%s{C5C5C5} >> RÉPARER >>", mes);
	Dialog_Show(playerid, SELECT_Autoservice, DIALOG_STYLE_TABLIST_HEADERS, "Réparation du véhicule - Sélection des éléments", mes, "Choisir", "Annuler");
	return 1;
}

Dialog:ModDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new vehicle = GetPlayerVehicleID(playerid);
	
	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if (ModShop_Nearest(playerid) == -1) return 1;

	if (listitem == 0)
	{
		if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle)) return SendErrorMessage(playerid, "Cette fonctionnalité n'est pas disponible pour les vélos, motos et taxis.");
			
		if (IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1008) != 1 && IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1009) != 1 && IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1010) != 1)
		{
			ShowModMenu(playerid);
			return SendErrorMessage(playerid, "Impossible d'installer du nitro sur ce véhicule.");
		}

		Dialog_Show(playerid, NitroDialog, DIALOG_STYLE_TABLIST, "{33AA33}Tuning du véhicule", "Nitro x2\t$4.500\nNitro x5\t$9.000\nNitro x10\t$15.000", "Choisir", "Retour");
		return 1;
	}

	else if (listitem == 1)
	{
		if (!IsVehiclePaintJob(GetVehicleModel(vehicle))) return SendErrorMessage(playerid, "Les travaux de peinture ne sont pas disponibles pour ce véhicule.");

		new mes[128];

		for (new i; i != IsVehiclePaintJob(GetVehicleModel(vehicle)); i++)
		{
			format(mes, sizeof(mes), "%s\nTravail de peinture %i", mes, i + 1);
		}

		Dialog_Show(playerid, PaintjobDialog, DIALOG_STYLE_LIST, "{33AA33}Coût : $3.000", mes, "Choisir", "Retour");
		return 1;
	}

	else if (listitem == 2)
	{
		new colors[256];

		for (new i; i != 256; i++)
		{
			colors[i] = i;
		}

		ShowColorSelectionMenu(playerid, MODEL_SELECTION_COLOR, colors, sizeof(colors));
		SetPVarInt(playerid, #veh_color1, -1);
		SetPVarInt(playerid, #veh_color_type, 2);
		return 1;
	}

	else if (listitem == 3)
	{
		if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle)) return SendErrorMessage(playerid, "Cette fonctionnalité n'est pas disponible pour les vélos, motos et taxis.");
			
		if (VehicleInfo[vehicle][carComponent][CARMODTYPE_HYDRAULICS])
		{
			ShowModMenu(playerid);
			return SendErrorMessage(playerid, "Vous avez déjà installé une hydraulique.");
		}
		Dialog_Show(playerid, HydralicConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Tuning du véhicule", "{FFFFFF}Êtes-vous sûr de vouloir installer une hydraulique pour $8.000 ?", "Oui", "Retour");
		return 1;
	}

	else if (listitem == 4)
	{
		if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle)) return SendErrorMessage(playerid, "Cette fonctionnalité n'est pas disponible pour les vélos, motos et taxis.");
			
		if (VehicleInfo[vehicle][carRadio])
		{
			ShowModMenu(playerid);
			return SendErrorMessage(playerid, "Vous avez déjà une radio.");
		}

		Dialog_Show(playerid, RadioConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Tuning du véhicule", "{FFFFFF}Êtes-vous sûr de vouloir installer une radio pour $400 ?", "Oui", "Retour");
		return 1;
	}
	else if (listitem == 5)
	{
		if (VehicleInfo[vehicle][carGps])
		{
			ShowModMenu(playerid);
			return SendErrorMessage(playerid, "Vous avez déjà un GPS.");
		}

		Dialog_Show(playerid, GPSConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Tuning du véhicule", "{FFFFFF}Êtes-vous sûr de vouloir installer un GPS pour $1.500 ?", "Oui", "Retour");
		return 1;
	}
	else if (listitem == 6)
	{
		new cost = GetVehiclePrice(GetVehicleModel(vehicle)) / 100 * 10;

		if (cost < 1) return 1;
		if (GetCash(playerid) < cost) return SendErrorMessage(playerid, "Le coût du remplacement du moteur est de $%i.", cost);

		StartVehicleUpgrade(playerid, vehicle, 9, cost);
		return 1;
	}
	else if (listitem == 7)
	{
		new cost = GetVehiclePrice(GetVehicleModel(vehicle)) / 100 * 5;

		if (cost < 1) return 1;
		if (GetCash(playerid) < cost) return SendErrorMessage(playerid, "Le coût du remplacement de la batterie est de $%i.", cost);

		StartVehicleUpgrade(playerid, vehicle, 11, cost);
		return 1;
	}
	else if (listitem == 8)
	{
		Dialog_Show(playerid, ChangeLock, DIALOG_STYLE_MSGBOX, "{33AA33}Changement de serrure", "{FFFFFF}Êtes-vous sûr de vouloir changer la serrure pour $300 ?\nTous les doubles des clés de votre véhicule deviendront invalides.", "Oui", "Retour");
		return 1;
	}
	else if (listitem == 9)
	{
		static const str_size[] = "1. Serrure\t[%i niveau]\n2. Alarme\t[%i niveau]\n3. Immobilisateur\t[%i niveau]\n4. Assurance\t[%i niveau]\n5. Armure\t[%.0f HP]";
		new mes[sizeof(str_size) - 18 + 1 + 48];
		
		format(mes, sizeof(mes), str_size, VehicleInfo[vehicle][carLock], VehicleInfo[vehicle][carAlarm], VehicleInfo[vehicle][carImmob], VehicleInfo[vehicle][carInsurance], VehicleInfo[vehicle][carArmour]);
		Dialog_Show(playerid, VehUpgrade, DIALOG_STYLE_TABLIST, "{FFFFFF}Améliorations", mes, "Choisir", "Retour");
		return 1;
	}
	else if (listitem == 10)
	{
		if (IsABicycle(vehicle) || IsABiker(vehicle) || IsATaxi(vehicle)) return SendErrorMessage(playerid, "Cette fonctionnalité n'est pas disponible pour les vélos, motos et taxis.");
			
		new mes[512] = "{AFAFAF}Supprimer{FFFFFF} le nitro\n{AFAFAF}Supprimer{FFFFFF} les travaux de peinture\n{AFAFAF}Supprimer{FFFFFF} l'hydraulique\n{AFAFAF}Supprimer{FFFFFF} la radio";

		Dialog_Show(playerid, DeleteTuning, DIALOG_STYLE_LIST, "{33AA33}Suppression du tuning", mes, "Choisir", "Retour");
		return 1;
	}

	ShowComponentMenu(playerid, vehicle, listitem);
	return 1;
}

Dialog:ModDialogBike(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;
  		
	if (listitem == 0)
	{
	    new colors[256];

     	for(new i; i != 256; i++)
		{
      		colors[i] = i;
		}

		ShowColorSelectionMenu(playerid, MODEL_SELECTION_COLOR, colors, sizeof(colors));
	    SetPVarInt(playerid, #veh_color1, -1);
	    SetPVarInt(playerid, #veh_color_type, 1);
	    return 1;
	}
	return 1;
}

Dialog:BuyVehArmour(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);
		
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
	if (ModShop_Nearest(playerid) == -1)											return 1;

	new amount = strval(inputtext);

	if (amount < 10 || amount > 250)
	{
		Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installation de l'armure", "{ffffff}Entrez la quantité d'armure entre 10 et 250 que vous souhaitez acheter [1 unité coûte $10] :", "Acheter", "Retour");
		return SendErrorMessage(playerid, "{FFFFFF}La quantité d'armure doit être comprise entre 10 et 250.");
	}

	if (VehicleInfo[vehicle][carArmour] + amount > 250.0)
	{
		Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installation de l'armure", "{ffffff}Entrez la quantité d'armure entre 10 et 250 que vous souhaitez acheter [1 unité coûte $10] :", "Acheter", "Retour");
		return SendErrorMessage(playerid, "Vous pouvez acheter seulement %.0f d'armure.", 250.0 - VehicleInfo[vehicle][carArmour]);
	}
	
	if (VehicleInfo[vehicle][carAttach3DtextUse]) return SendErrorMessage(playerid, "Veuillez patienter, une opération est déjà en cours.");

	if (GetCash(playerid) < amount * 10)
	{
		Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installation de l'armure", "{ffffff}Entrez la quantité d'armure entre 10 et 250 que vous souhaitez acheter [1 unité coûte $10] :", "Acheter", "Retour");
		return SendErrorMessage(playerid, "Le prix de l'armure est de $%i.", amount * 10);
	}

	StartVehicleUpgrade(playerid, vehicle, 6, 10 * amount, amount);
	FreezePlayer(playerid);
	return 1;
}

Dialog:BuyVehInsurance(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);
		
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if (ModShop_Nearest(playerid) == -1) return 1;
		
	new cost = GetInsurancePrice(GetVehicleModel(vehicle));

	if (!cost) return SendErrorMessage(playerid, "L'assurance n'est pas disponible pour ce véhicule.");    
	if (!VehicleInfo[vehicle][carRegistered]) return SendErrorMessage(playerid, "Ce véhicule n'est pas enregistré.");
	if (VehicleInfo[vehicle][carAttach3DtextUse]) return SendErrorMessage(playerid, "Veuillez patienter, une opération est déjà en cours.");

	new level = strval(inputtext);

	if (level < 1 || level > 3) return SendErrorMessage(playerid, "Le niveau d'assurance doit être compris entre 1 et 3.");
	if (VehicleInfo[vehicle][carInsurance] >= level) return SendErrorMessage(playerid, "Vous avez déjà un niveau d'assurance de %i.", VehicleInfo[vehicle][carInsurance]);
	if (GetCash(playerid) < cost * level) return SendNotCash(playerid);

	StartVehicleUpgrade(playerid, vehicle, 5, cost * level, level);
	return 1;
}

Dialog:VehUpgrade(playerid, response, listitem, inputtext[])
{
	if (!response)	return ShowModMenu(playerid);
		
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
	if (ModShop_Nearest(playerid) == -1)											return 1;

	if (listitem == 3)
	{
		new cost = GetInsurancePrice(GetVehicleModel(vehicle));

		if (!cost)	return SendErrorMessage(playerid, "L'assurance n'est pas disponible pour ce véhicule.");
		
		new mes[590] = "{ffffff}Niveau 1 \t- $%i\n{ecc317}+ {ffffff}Le véhicule sera toujours restauré avec sa santé maximale.\n\n{ffffff}Niveau 2 \t- $%i\n\
		{ecc317}+ {ffffff}Le véhicule sera toujours restauré avec sa santé maximale.\n{ecc317}+ {ffffff}Le véhicule sera restauré sans dommages.\n\n{ffffff}Niveau 3 \t- $%i\n";
		
		strcat(mes, "{ecc317}+ {ffffff}Le véhicule sera toujours restauré avec sa santé maximale.\n{ecc317}+ {ffffff}Le véhicule sera restauré sans dommages.\n{ecc317}+ {ffffff}Conservation du tuning et de l'autoradio.\n\nEntrez le niveau d'assurance que vous souhaitez acheter dans le champ ci-dessous :");
		format(mes, sizeof(mes), mes, cost * 1, cost * 2, cost * 3);
		Dialog_Show(playerid, BuyVehInsurance, DIALOG_STYLE_INPUT, "{FFFFFF}Achat d'assurance", mes, "Acheter", "Retour");
		return 1;
	}

	else if (listitem == 4)
	{
		return Dialog_Show(playerid, BuyVehArmour, DIALOG_STYLE_INPUT, "{FFFFFF}Installation de l'armure", "{ffffff}Entrez la quantité d'armure entre 10 et 250 que vous souhaitez acheter [1 unité coûte $10] :", "Acheter", "Retour");
	}

	new alarm_mes[][] =
	{
		{"{ecc317}+ {ffffff}Alarme sonore puissante."}, {"{ecc317}+ {ffffff}Alarme sonore puissante.\n{ecc317}+ {ffffff}Notification de vol à la police."}, 
		{"{ecc317}+ {ffffff}Alarme sonore puissante.\n{ecc317}+ {ffffff}Notification de vol à la police.\n{ecc317}+ {ffffff}Notification de vol au propriétaire par SMS."}, 
		{"{ecc317}+ {ffffff}Alarme sonore puissante.\n{ecc317}+ {ffffff}Notification de vol à la police.\n{ecc317}+ {ffffff}Notification de vol au propriétaire par SMS et facturation."}
	};

	new mes[256];

	switch(listitem)
	{
		case 0:
		{
			if (VehicleInfo[vehicle][carLock] >= sizeof(price_mod_lock))		return SendErrorMessage(playerid, "Votre serrure est déjà au niveau maximum.");

			format(mes, sizeof(mes), "{ffffff}Améliorer la serrure au niveau %i pour $%i ?\n{ecc317}+ {ffffff}%i secondes pour crocheter la serrure du véhicule.", VehicleInfo[vehicle][carLock]+1, price_mod_lock[VehicleInfo[vehicle][carLock]], 250 + (GetPVarInt(playerid, "BuyingVehicleLock") * 250));
		}
		case 1:
		{
			if (VehicleInfo[vehicle][carAlarm] >= sizeof(price_mod_alarm))	return SendErrorMessage(playerid, "Votre alarme est déjà au niveau maximum.");

			format(mes, sizeof(mes), "{ffffff}Améliorer l'alarme au niveau %i pour $%i ?\n%s", VehicleInfo[vehicle][carAlarm]+1, price_mod_alarm[VehicleInfo[vehicle][carAlarm]], alarm_mes[VehicleInfo[vehicle][carAlarm]]);
		}
		case 2:
		{
			if (VehicleInfo[vehicle][carImmob] >= sizeof(price_mod_immob))	return SendErrorMessage(playerid, "Votre immobilisateur est déjà au niveau maximum.");

			format(mes, sizeof(mes), "{ffffff}Améliorer l'immobilisateur au niveau %i pour $%i ?\n{ecc317}+ {ffffff}Aide à prévenir le crochetage de la serrure d'allumage.", VehicleInfo[vehicle][carImmob]+1, price_mod_immob[VehicleInfo[vehicle][carImmob]]);
		}
	}

	SetPVarInt(playerid, #upgrade_veh, listitem);
	Dialog_Show(playerid, BuyVehUpgrade2, DIALOG_STYLE_MSGBOX, "{FFFFFF}Achat d'améliorations", mes, "Acheter", "Retour");
	return 1;
}

Dialog:BuyVehUpgrade2(playerid, response, listitem, inputtext[])
{
    if (!response)	return ShowModMenu(playerid);

    new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
   	if (ModShop_Nearest(playerid) == -1)											return 1;
  		
    listitem = GetPVarInt(playerid, #upgrade_veh);

    switch(listitem)
    {
        case 0: StartVehicleUpgrade(playerid, vehicle, 2, price_mod_lock[VehicleInfo[vehicle][carLock]], VehicleInfo[vehicle][carLock]+1);
        case 1: StartVehicleUpgrade(playerid, vehicle, 3, price_mod_alarm[VehicleInfo[vehicle][carAlarm]], VehicleInfo[vehicle][carAlarm]+1);
        case 2: StartVehicleUpgrade(playerid, vehicle, 4, price_mod_immob[VehicleInfo[vehicle][carImmob]], VehicleInfo[vehicle][carImmob]+1);
    }
	return 1;
}

Dialog:SelectModDialog(playerid, response, listitem, inputtext[])
{
    new vehicle = GetPlayerVehicleID(playerid), comptype = GetPVarInt(playerid, #component_type), compid = -1;

    if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID])	return 1;
    if (!response)																return ShowModMenu(playerid);
   	if (ModShop_Nearest(playerid) == -1)											return 1;

    for(new i; i != max_vehicle_components[comptype]; i++)
	{
		if (!vehicle_component[comptype][i][compID] || IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), vehicle_component[comptype][i][compID]) != 1) continue;

		compid++;
		if (listitem == compid)
		{
			compid = i;
			break;
		}
   	}

   	new modelid = vehicle_component[comptype][compid][compID];

	if (GetVehicleComponentType(modelid) == -1 || IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), modelid) != 1)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Cette pièce n'est pas compatible avec votre véhicule.");
	}

	if (comptype == -1 || compid == -1) return SendErrorMessage(playerid, "Erreur d'ID de pièce, contactez l'administration.");

	new mes[128];
	format(mes, sizeof(mes), "{FFFFFF}Êtes-vous sûr de vouloir installer %s pour $%i ?", GetComponentName(modelid), vehicle_component[comptype][compid][compPrice]);

	Dialog_Show(playerid, ModConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Tuning du véhicule", mes, "Oui", "Retour");
    SetPVarInt(playerid, #component_model, modelid);
    SetPVarInt(playerid, #component_type, comptype);
    AddVehicleComponent(vehicle, modelid);
	return 1;
}

Dialog:DeleteTuning(playerid, response, listitem, inputtext)
{
	if (!response) return ShowModMenu(playerid);

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if (ModShop_Nearest(playerid) == -1) return 1;

	if (listitem == 9 && VehicleInfo[vehicle][carPaintJob] == 3)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous n'avez pas de travaux de peinture installés.");
	}

	if (listitem == 11 && VehicleInfo[vehicle][carRadio] == 0)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous n'avez pas de radio installée.");
	}

	new comp_type[11] = {CARMODTYPE_WHEELS, CARMODTYPE_SPOILER, CARMODTYPE_HOOD, CARMODTYPE_ROOF, CARMODTYPE_SIDESKIRT, CARMODTYPE_EXHAUST, CARMODTYPE_FRONT_BUMPER, CARMODTYPE_REAR_BUMPER, CARMODTYPE_NITRO, 0, CARMODTYPE_HYDRAULICS};

	if (listitem != 9 && listitem != 11 && VehicleInfo[vehicle][carComponent][comp_type[listitem]] == 0)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous n'avez pas cette pièce installée.");
	}

	new comp_name[12][32] = {"roues", "spoiler", "prise d'air", "toit", "jupes latérales", "échappement", "pare-chocs avant", "pare-chocs arrière", "nitro", "travaux de peinture", "hydraulique", "radio"};

	new mes[128];
	format(mes, sizeof(mes), "{FFFFFF}Êtes-vous sûr de vouloir supprimer %s ?", comp_name[listitem]);
	Dialog_Show(playerid, DeleteTuningConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Suppression du tuning", mes, "Oui", "Retour");
	SetPVarInt(playerid, #select_deltun, listitem);
	return 1;
}

Dialog:DeleteTuningConfrim(playerid, response, listitem, inputtext)
{
	if (!response) return ShowModMenu(playerid);

	listitem = GetPVarInt(playerid, #select_deltun);

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if (ModShop_Nearest(playerid) == -1) return 1;

	if (listitem == 9 && VehicleInfo[vehicle][carPaintJob] == 3)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous n'avez pas de travaux de peinture installés.");
	}

	if (listitem == 11 && VehicleInfo[vehicle][carRadio] == 0)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous n'avez pas de radio installée.");
	}

	new comp_type[11] = {CARMODTYPE_WHEELS, CARMODTYPE_SPOILER, CARMODTYPE_HOOD, CARMODTYPE_ROOF, CARMODTYPE_SIDESKIRT, CARMODTYPE_EXHAUST, CARMODTYPE_FRONT_BUMPER, CARMODTYPE_REAR_BUMPER, CARMODTYPE_NITRO, 0, CARMODTYPE_HYDRAULICS};

	if (listitem != 9 && listitem != 11 && VehicleInfo[vehicle][carComponent][comp_type[listitem]] == 0)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous n'avez pas cette pièce installée.");
	}

	if (listitem == 9)
	{
		VehicleInfo[vehicle][carPaintJob] = 3;
		ChangeVehiclePaintjob(vehicle, 3);
	}
	else if (listitem == 11)
	{
		VehicleInfo[vehicle][carRadio] = 0;
	}
	else
	{
		RemoveVehicleComponent(vehicle, VehicleInfo[vehicle][carComponent][comp_type[listitem]]);
		VehicleInfo[vehicle][carComponent][comp_type[listitem]] = 0;
	}

	new comp_name[12][32] = {"roues", "spoiler", "prise d'air", "toit", "jupes latérales", "échappement", "pare-chocs avant", "pare-chocs arrière", "nitro", "travaux de peinture", "hydraulique", "radio"};

	SendSyntaxMessage(playerid, "Vous avez supprimé avec succès %s du véhicule.", comp_name[listitem]);
	return 1;
}

Dialog:HydralicConfrim(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if ((modshop = ModShop_Nearest(playerid)) == -1) return 1;

	if (GetCash(playerid) < 8000)
	{
		ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	if (IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), 1087) != 1)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Impossible d'installer une hydraulique sur ce véhicule.");
	}

	if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct]) 
		return SendErrorMessage(playerid, "L'atelier de tuning n'a pas assez de pièces pour effectuer cette opération.");
	
	GiveCash(playerid, -8000);
	SetVehicleComponent(vehicle, 1087, GetVehicleComponentType(1087));
	SendSyntaxMessage(playerid, "Vous avez acheté une hydraulique pour $8.000.");
	
	if (BizInfo[modshop][bizOwnerID])
	{
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, 8000 / 100 * 80);
	}

	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:RadioConfrim(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if ((modshop = ModShop_Nearest(playerid)) == -1) return 1;

	if (VehicleInfo[vehicle][carRadio] == 1)
	{
		ShowModMenu(playerid);
		return SendClientMessage(playerid, COLOR_LIGHTRED, "Vous avez déjà une radio.");
	}
	if (GetCash(playerid) < 400)
	{
		ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	GiveCash(playerid, -400);
	VehicleInfo[vehicle][carRadio] = 1;
	SendSyntaxMessage(playerid, "Vous avez acheté une radio pour $400. Utilisez /setstation pour configurer une station radio.");
	GiveBizCash(modshop, 400);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:GPSConfrim(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if ((modshop = ModShop_Nearest(playerid)) == -1) return 1;

	if (VehicleInfo[vehicle][carGps] == 1)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous avez déjà un GPS.");
	}

	if (GetCash(playerid) < 1500)
	{
		ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	GiveCash(playerid, -1500);
	VehicleInfo[vehicle][carGps] = 1;
	SendSyntaxMessage(playerid, "Vous avez acheté un GPS pour $1.500. Utilisez /v find pour localiser votre véhicule.");
	GiveBizCash(modshop, 1500);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:ChangeLock(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || VehicleInfo[vehicle][carOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Vous devez être dans votre véhicule personnel.");
	
	if ((modshop = ModShop_Nearest(playerid)) == -1) return 1;

	if (VehicleInfo[vehicle][carGps] == 1)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous avez déjà un GPS.");
	}

	if (GetCash(playerid) < 300)
	{
		ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	GiveCash(playerid, -300);
	foreach(new i : Player)
	{
		if (!IsPlayerLogged(i) || PlayerInfo[i][pSpareKey] != VehicleInfo[vehicle][carID]) continue;
			
		PlayerInfo[i][pSpareKey] = 0;
	}
	
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `sparekey`='0' WHERE `sparekey`=%i", VehicleInfo[vehicle][carID]);
	mysql_tquery(dbHandle, query);
	
	SendSyntaxMessage(playerid, "Vous avez remplacé la serrure pour $300.");
	GiveBizCash(modshop, 300);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:ModConfrim(playerid, response, listitem, inputtext[])
{
	new modshop = -1, vehicle = GetPlayerVehicleID(playerid), modelid = GetPVarInt(playerid, #component_model);
	
	RemoveVehicleComponent(vehicle, modelid);
	LoadComp(vehicle);
	
	if (!response) return ShowComponentMenu(playerid, vehicle, GetPVarInt(playerid, #component_type));
	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if ((modshop = ModShop_Nearest(playerid)) == -1) return 1;
	if (GetVehicleComponentType(modelid) == -1 || IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), modelid) != 1)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Cette pièce n'est pas compatible avec votre véhicule.");
	}

	new compid = -1, comptype = -1;

	for (new i; i != sizeof(vehicle_component); i++)
	{
		for (new a; a != max_vehicle_components[i]; a++)
		{
			if (vehicle_component[i][a][compID] != modelid) continue;

			comptype = i;
			compid = a;
			break;
		}

		if (comptype != -1 && compid != -1) break;
	}

	if (comptype == -1 || compid == -1) return SendErrorMessage(playerid, "Erreur d'ID de pièce, contactez l'administration.");

	if (GetCash(playerid) < vehicle_component[comptype][compid][compPrice])
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent. Le coût de cette pièce est de $%i.", vehicle_component[comptype][compid][compPrice]);
	}

	if (HasComponent(vehicle, modelid))
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous avez déjà cette pièce.");
	}

	if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct]) return SendErrorMessage(playerid, "L'atelier de tuning n'a pas assez de pièces pour effectuer cette opération.");
		
	GiveCash(playerid, -vehicle_component[comptype][compid][compPrice]);
	
	if (BizInfo[modshop][bizOwnerID] != 0)
	{
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, vehicle_component[comptype][compid][compPrice] / 100 * 80);
	}
	
	SendSyntaxMessage(playerid, "Vous avez acheté %s pour $%i.", GetComponentName(modelid), vehicle_component[comptype][compid][compPrice]);
	SetVehicleComponent(vehicle, modelid, GetVehicleComponentType(modelid));
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:PaintjobDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if (IsVehiclePaintJob(GetVehicleModel(vehicle)) < listitem + 1) return 1;
	if (ModShop_Nearest(playerid) == -1) return 1;

	if (VehicleInfo[vehicle][carPaintJob] == listitem)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Cette peinture est déjà installée.");
	}

	new mes[128];

	format(mes, sizeof(mes), "{FFFFFF}Êtes-vous sûr de vouloir installer la peinture %i pour $3.000 ?", listitem + 1);
	Dialog_Show(playerid, PaintjobConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Tuning du véhicule", mes, "Oui", "Retour");
	SetPVarInt(playerid, #select_paintjob, listitem);

	ChangeVehiclePaintjob(vehicle, listitem);
	return 1;
}

Dialog:PaintjobConfrim(playerid, response, listitem, inputtext[])
{
	listitem = GetPVarInt(playerid, #select_paintjob);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	ChangeVehiclePaintjob(vehicle, VehicleInfo[vehicle][carPaintJob]);
	
	if (!response) return ShowModMenu(playerid);
	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if ((modshop = ModShop_Nearest(playerid)) == -1) return 1;
	if (!IsVehiclePaintJob(GetVehicleModel(vehicle))) return SendErrorMessage(playerid, "Les travaux de peinture ne sont pas disponibles pour ce véhicule.");
	if (VehicleInfo[vehicle][carPaintJob] == listitem)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Cette peinture est déjà installée.");
	}
	if (GetCash(playerid) < 3000)
	{
		ShowModMenu(playerid);
		return SendNotCash(playerid);
	}
	if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct]) return SendErrorMessage(playerid, "L'atelier de tuning n'a pas assez de pièces pour effectuer cette opération.");
		
	GiveCash(playerid, -3000);
	VehicleInfo[vehicle][carPaintJob] = listitem;
	ChangeVehiclePaintjob(vehicle, listitem);
	Save_Car(vehicle);
	SendSyntaxMessage(playerid, "Vous avez acheté la peinture %i pour $3.000.", listitem + 1);

	if (BizInfo[modshop][bizOwnerID] != 0)
	{
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, 3000 / 100 * 80);
	}
	PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:NitroDialog(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if (ModShop_Nearest(playerid) == -1) return 1;

	new nitro_id[3] = {1009, 1008, 1010}, nitro_price[3] = {4500, 9000, 15000};

	if (HasComponent(vehicle, nitro_id[listitem]))
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous avez déjà installé ce nitro.");
	}

	new mes[128];
	format(mes, sizeof(mes), "{FFFFFF}Êtes-vous sûr de vouloir installer le nitro x%i pour $%i ?", (listitem == 0) ? 2 : ((listitem == 1) ? 5 : 10), nitro_price[listitem]);
	Dialog_Show(playerid, NitroConfrim, DIALOG_STYLE_MSGBOX, "{33AA33}Tuning du véhicule", mes, "Oui", "Retour");
	SetPVarInt(playerid, #select_nitro, listitem);
	return 1;
}

Dialog:NitroConfrim(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowModMenu(playerid);

	listitem = GetPVarInt(playerid, #select_nitro);

	new modshop = -1, vehicle = GetPlayerVehicleID(playerid);

	if (!IsPlayerInAnyVehicle(playerid) || !VehicleInfo[vehicle][carOwnerID]) return 1;
	if ((modshop = ModShop_Nearest(playerid)) == -1) return 1;

	new nitro_id[3] = {1009, 1008, 1010}, nitro_price[3] = {15000, 15500, 16000}, nitro_level[3] = {2, 5, 10};

	if (HasComponent(vehicle, nitro_id[listitem]))
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Vous avez déjà installé ce nitro.");
	}

	if (IsVehicleUpgradeCompatible(GetVehicleModel(vehicle), nitro_id[listitem]) != 1)
	{
		ShowModMenu(playerid);
		return SendErrorMessage(playerid, "Impossible d'installer du nitro sur ce véhicule.");
	}

	if (GetCash(playerid) < nitro_price[listitem])
	{
		ShowModMenu(playerid);
		return SendNotCash(playerid);
	}

	if (PlayerInfo[playerid][pPremium] < 2) return SendErrorMessage(playerid, "Seuls les joueurs avec un compte premium 'Argent' ou supérieur peuvent acheter du nitro.");
	if (BizInfo[modshop][bizOwnerID] && !BizInfo[modshop][bizProduct]) return SendErrorMessage(playerid, "L'atelier de tuning n'a pas assez de pièces pour effectuer cette opération.");
		
	GiveCash(playerid, -nitro_price[listitem]);
	SetVehicleComponent(vehicle, nitro_id[listitem], CARMODTYPE_NITRO);
	SendSyntaxMessage(playerid, "Vous avez acheté du nitro x%i pour $%i.", nitro_level[listitem], nitro_price[listitem]);

	if (BizInfo[modshop][bizOwnerID] != 0)
	{
		BizInfo[modshop][bizProduct]--;
		GiveBizCash(modshop, nitro_price[listitem] / 100 * 80);
	}
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:StartJackingLock(playerid, response, listitem, inputtext[])
{	
	if (!response)											return 1;

	new vehicle = GetPVarInt(playerid, #JackingVehicle);
	if (!vehicle || !GetNearVehicle(playerid, 3.0, vehicle))	return SendErrorMessage(playerid, "Vous devez être près du véhicule.");
	if (!IsPlayerInFrontVDoor(playerid, vehicle)) 			return SendErrorMessage(playerid, "Vous devez être près de la porte conducteur.");
	if (!VehicleInfo[vehicle][carLocked])					return SendErrorMessage(playerid, "Les portes du véhicule sont déjà ouvertes.");

	if (pTemp[playerid][JackingType] == 2) {
		if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "Vous devez être debout.");
		switch(VehicleInfo[vehicle][carLock])
		{
			case 0: VehicleInfo[vehicle][eDoorHealth] = 25, VehicleInfo[vehicle][eDoorEffect] = BLOCK_NONE;
			case 1: VehicleInfo[vehicle][eDoorHealth] = 50, VehicleInfo[vehicle][eDoorEffect] = LESS_DAMAGE_FIST;
			case 2: VehicleInfo[vehicle][eDoorHealth] = 75, VehicleInfo[vehicle][eDoorEffect] = BLOCK_FIST;
			case 3: VehicleInfo[vehicle][eDoorHealth] = 150, VehicleInfo[vehicle][eDoorEffect] = LESS_DAMAGE_MELEE;
			case 4: VehicleInfo[vehicle][eDoorHealth] = 200, VehicleInfo[vehicle][eDoorEffect] = BLOCK_PHYSICAL;
			case 5: VehicleInfo[vehicle][eDoorHealth] = 250, VehicleInfo[vehicle][eDoorEffect] = BLOCK_PHYSICAL;
		}
				
		PlaySound(playerid, 1138);
		AlarmOn(vehicle);
		SendHelpMessage(playerid, "Vous devez commencer à briser la porte pour ouvrir le véhicule !");
		VehicleInfo[vehicle][ePhysicalAttack] = true;

		new doorhealth[12];
		format(doorhealth, 12, "%d%%", VehicleInfo[vehicle][eDoorHealth]);
		VehicleInfo[vehicle][eVehicleLabel] = CreateDynamic3DTextLabel(doorhealth, COLOR_WHITE, 0.0, 0.0, 0.0, 15.0, INVALID_PLAYER_ID, vehicle, 1, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), playerid);

	} else if (pTemp[playerid][JackingType] == 1) {
		if (IsPlayerCrouching(playerid) == false) return SendErrorMessage(playerid, "Vous devez vous accroupir.");
		if (!CheckHanditem(playerid,18634)) 		 return SendErrorMessage(playerid, "Vous devez avoir un pied-de-biche en main.");
		
		JackingLock[playerid] = 250 + VehicleInfo[vehicle][carLock] * 250;
		
		new mes[128];
		format(mes, sizeof(mes), "sort quelque chose et commence à s'occuper de la serrure de %s.", VehicleNames[GetVehicleModel(vehicle)-400]);
		cmd::me(playerid, mes);

	} else return 1;
	
	JackingVehicle[playerid] = vehicle;
	return 1;
}

Dialog:StartJackingEngine(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new vehicle = GetPlayerVehicleID(playerid);

	if (!vehicle || !IsDriver(playerid)) return SendErrorMessage(playerid, "Vous devez être assis à la place du conducteur.");
	if (!VehicleInfo[vehicle][carOwnerID]) return SendErrorMessage(playerid, "Ce véhicule ne peut pas être démarré.");
	if (JackingEngine[playerid]) return SendErrorMessage(playerid, "Vous essayez déjà de démarrer le moteur.");
	if (VehicleInfo[vehicle][carEngineOn]) return SendErrorMessage(playerid, "Le moteur est déjà en marche.");
	if (VehicleInfo[vehicle][carFuel] == 0.0) return SendErrorMessage(playerid, "Le véhicule n'a pas de carburant.");
	if (GetPVarInt(playerid, #vehicle_upgrade_timer) > 0) return SendErrorMessage(playerid, "Impossible de démarrer le moteur pour le moment.");
	if (GetPVarInt(playerid, #UsePump)) return SendErrorMessage(playerid, "Impossible de démarrer le moteur pour le moment.");
	if (!IsPlayerHasItem(playerid, INV_TOOLS)) return SendErrorMessage(playerid, "Vous n'avez pas les outils nécessaires pour forcer le moteur.");

	new mes[128];
	format(mes, sizeof(mes), "essaie de forcer le contact de %s.", VehicleNames[GetVehicleModel(vehicle) - 400]);
	cmd::me(playerid, mes);
		
	pTemp[playerid][pUnscrambleID] = random(sizeof(UnscrambleInfo));
		
	switch (VehicleInfo[vehicle][carImmob])
	{
		case 0: JackingEngine[playerid] = 125;
		case 1: JackingEngine[playerid] = 100;
		case 2: JackingEngine[playerid] = 75;
		case 3: JackingEngine[playerid] = 50;
		case 4: JackingEngine[playerid] = 25;
	}
	
	//InfoMSG(playerid, "~y~/(UNS)CRAMBLE~w~ <MOT DÉCHIFFRÉ>~n~~r~MOT CHIFFRÉ: '~w~%s~r~'.~n~~w~IL VOUS RESTE ~y~%i~w~ SECONDES.", 2);
	return 1;
}