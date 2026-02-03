CMD:repair(playerid, params[])
{
	new userid, type, vehid = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Vous n'êtes pas mécanicien.");
	if (!IsDriver(playerid) || !IsATowTruck(vehid)) return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse en tant que conducteur.");

	if (sscanf(params, "ui", userid, type)) {
		SendHelpMessage(playerid, "/repair [ID/nom du joueur] [type]");
		SendSyntaxMessage(playerid, "Type 1 : Réparation du moteur (( Restaurer HP ))");
		SendSyntaxMessage(playerid, "Type 2 : Réparation de la carrosserie (( Travail visuel ))");
		SendSyntaxMessage(playerid, "Type 3 : Restauration du véhicule (( Moteur défaillant ))");
		return 1;
	}

	if (type < 1 || type > 3) {
		SendSyntaxMessage(playerid, "Type 1 : Réparation du moteur (( Restaurer HP ))");
		SendSyntaxMessage(playerid, "Type 2 : Réparation de la carrosserie (( Travail visuel ))");
		SendSyntaxMessage(playerid, "Type 3 : Restauration du véhicule (( Moteur défaillant ))");
		return 1;
	}

	if (userid == playerid) return SendErrorMessage(playerid, "Vous ne pouvez pas effectuer cette action sur vous-même.");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (!PlayerNearPlayer(6.0, playerid, userid)) return SendNotNearPlayer(playerid);
	if (!IsDriver(userid)) return SendErrorMessage(playerid, "Le joueur doit être dans un véhicule.");

	new vehicle = GetPlayerVehicleID(userid);

	if (VehicleInfo[vehicle][carFaction] != 0 && type != 1 && type != 2) return SendErrorMessage(playerid, "Ce véhicule appartient à une structure gouvernementale.");
	if (pTemp[userid][pServePlayer] == playerid && (pTemp[userid][pServeType] > 0 && pTemp[userid][pServeType] < 4)) return SendErrorMessage(playerid, "Vous avez déjà proposé vos services à %s.", GetNameEx(userid));

	if (type == 1) {
		if (VehicleInfo[vehid][carComps] < 75) return SendErrorMessage(playerid, "Vous avez besoin d'au moins 75 composants pour réparer le moteur.");

		new mes[128];
		format(mes, sizeof(mes), "propose à %s une réparation du moteur de %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle) - 400]);
		cmd::me(playerid, mes);
		SendSyntaxMessage(userid, "%s vous propose une réparation du moteur. (Appuyez sur Y pour accepter l'offre)", GetNameWithMask(playerid));
	} else if (type == 2) {
		if (VehicleInfo[vehid][carComps] < 75) return SendErrorMessage(playerid, "Vous avez besoin d'au moins 75 composants pour effectuer des travaux de carrosserie.");

		new mes[128];
		format(mes, sizeof(mes), "propose à %s des travaux de carrosserie sur %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle) - 400]);
		cmd::me(playerid, mes);
		SendSyntaxMessage(userid, "%s vous propose des travaux de carrosserie. (Appuyez sur Y pour accepter l'offre)", GetNameWithMask(playerid));
	} else if (type == 3) {
		if (VehicleInfo[vehid][carComps] < 75) return SendErrorMessage(playerid, "Vous avez besoin d'au moins 75 composants pour réparer un moteur défaillant.");
		if (VehicleInfo[vehicle][carEngine] >= 20.0) return SendErrorMessage(playerid, "Le moteur de ce véhicule n'a pas besoin de réparation ( > 20%%).");

		new mes[128];
		format(mes, sizeof(mes), "propose à %s une réparation du moteur défaillant de %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle) - 400]);
		cmd::me(playerid, mes);
		SendSyntaxMessage(userid, "%s vous propose une réparation du moteur défaillant. (Appuyez sur Y pour accepter l'offre)", GetNameWithMask(playerid));
	}

	pTemp[userid][pServePlayer] = playerid;
	pTemp[userid][pServeType] = type;
	SetPVarInt(userid, #accept_time, gettime() + 30);
	return 1;
}

CMD:buycomp(playerid, params[])
{
	new quantite;

	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC)		return SendErrorMessage(playerid, "Vous n'êtes pas mécanicien.");
	if (!IsDriver(playerid))								return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse en tant que conducteur.");
	if (!IsATowTruck(GetPlayerVehicleID(playerid)))	return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse.");
	if (!PlayerToPoint(6.0, playerid, 2430.4412, -2454.9304, 13.6250))
	{
		Waypoint_Set(playerid, 2430.4412, -2454.9304, 13.6250, 8.0);
		return SendSyntaxMessage(playerid, "L'emplacement pour acheter des composants a été marqué sur la carte. (/removecp)");
	}

	if (sscanf(params, "d", quantite))
	{
		SendHelpMessage(playerid, "/buycomp [quantité]");
		return 1;
	}

	if (quantite < 1 || quantite > 300)											return SendErrorMessage(playerid, "La quantité de composants doit être comprise entre 1 et 300.");
	if (VehicleInfo[GetPlayerVehicleID(playerid)][carComps] >= 300)			return SendErrorMessage(playerid, "Le véhicule contient déjà le maximum de composants.");
	if (VehicleInfo[GetPlayerVehicleID(playerid)][carComps] + quantite > 300)	return SendErrorMessage(playerid, "Vous ne pouvez acheter que %i composants.", 300 - VehicleInfo[GetPlayerVehicleID(playerid)][carComps]);
	if (GetCash(playerid) < quantite)											return SendErrorMessage(playerid, "Vous avez besoin de $%i pour cet achat.", quantite);

	GiveCash(playerid, -quantite);
	SendSyntaxMessage(playerid, "Vous avez acheté %i composants pour $%i.", quantite, quantite);
	JobMessage(playerid, "~w~VOUS AVEZ ACHETÉ DES PRODUITS POUR VOTRE CAMION.", 6);
	VehicleInfo[GetPlayerVehicleID(playerid)][carComps] += quantite;
	return 1;
}

CMD:mycomp(playerid, params[])
{
	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Vous n'êtes pas mécanicien.");
	if (!IsDriver(playerid)) return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse en tant que conducteur.");
	if (!IsATowTruck(GetPlayerVehicleID(playerid))) return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse.");

	SendSyntaxMessage(playerid, "Il y a %i composants dans la dépanneuse.", VehicleInfo[GetPlayerVehicleID(playerid)][carComps]);
	return 1;
}

CMD:refill(playerid, params[])
{
	new userid, prix;

	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC)		return SendErrorMessage(playerid, "Vous n'êtes pas mécanicien.");
	if (!IsDriver(playerid))								return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse en tant que conducteur.");
	if (!IsATowTruck(GetPlayerVehicleID(playerid)))		return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse.");
	if (sscanf(params, "ud", userid, prix))				return SendHelpMessage(playerid, "/refill [ID/nom du joueur] [prix]");
	if (userid == playerid)								return SendErrorMessage(playerid, "Vous ne pouvez pas effectuer cette action sur vous-même.");
	if (!IsPlayerLogged(userid))							return SendNotConnected(playerid);
	if (!PlayerNearPlayer(6.0, playerid, userid))		return SendNotNearPlayer(playerid);
	if (!IsDriver(userid))								return SendErrorMessage(playerid, "Le joueur doit être dans un véhicule.");

	new vehicle = GetPlayerVehicleID(userid);

	if (VehicleInfo[vehicle][carFuel] >= GetVehicleFuelCapacity(GetVehicleModel(vehicle)))	
		return SendErrorMessage(playerid, "Le réservoir est déjà plein.");
	if (prix < 1 || prix > 500)															
		return SendErrorMessage(playerid, "Le prix du ravitaillement doit être compris entre $1 et $500.");

	new mes[128];
	format(mes, sizeof(mes), "propose de ravitailler le véhicule de %s pour $%i.", GetNameWithMask(userid), prix);
	cmd::me(playerid, mes);

	SendClientMessageEx(userid, COLOR_INFO, "%s vous propose de ravitailler votre véhicule pour $%i. (Appuyez sur Y pour accepter l'offre)", GetNameWithMask(playerid), prix);
	RefillPrice[userid] = prix;
	RefillOffer[userid] = playerid;
	return 1;
}

CMD:paintcar(playerid, params[])
{
	new userid, couleur1, couleur2;

	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Vous n'êtes pas mécanicien.");
	if (!IsDriver(playerid)) return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse en tant que conducteur.");
	if (!IsATowTruck(GetPlayerVehicleID(playerid))) return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse.");
	if (sscanf(params, "udd", userid, couleur1, couleur2)) return SendHelpMessage(playerid, "/paintcar [ID/nom du joueur] [couleur 1] [couleur 2] (/colorlist)");
	if (couleur1 > 255 || couleur2 > 255 || couleur1 < 0 || couleur2 < 0) return SendErrorMessage(playerid, "L'ID de couleur doit être compris entre 0 et 255.");
	if (userid == playerid) return SendErrorMessage(playerid, "Vous ne pouvez pas effectuer cette action sur vous-même.");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (!PlayerNearPlayer(6.0, playerid, userid)) return SendNotNearPlayer(playerid);
	if (!IsDriver(userid)) return SendErrorMessage(playerid, "Le joueur doit être dans un véhicule.");

	new vehicle = GetPlayerVehicleID(userid);

	if (VehicleInfo[vehicle][carFaction] != 0) return SendErrorMessage(playerid, "Ce véhicule appartient à une structure gouvernementale.");
	if (VehicleInfo[GetPlayerVehicleID(playerid)][carComps] < 45) return SendErrorMessage(playerid, "Vous avez besoin d'au moins 45 composants pour effectuer une peinture.");
	if (pTemp[userid][pServePlayer] == playerid && pTemp[userid][pServeType] == 4) return SendErrorMessage(playerid, "Vous avez déjà proposé vos services à %s.", GetNameEx(userid));

	new message[128];
	format(message, sizeof(message), "propose à %s une peinture pour %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehicle) - 400]);
	cmd::me(playerid, message);

	SendClientMessageEx(userid, COLOR_INFO, "%s vous propose une peinture de carrosserie. (Appuyez sur Y pour accepter l'offre)", GetNameWithMask(playerid));

	SetPVarInt(playerid, "couleur1", couleur1);
	SetPVarInt(playerid, "couleur2", couleur2);
	pTemp[userid][pServePlayer] = playerid;
	pTemp[userid][pServeType] = 4;
	SetPVarInt(userid, #accept_time, gettime() + 30);
	return 1;
}

CMD:attach(playerid, params[])
{
	new userid, vehicle = GetPlayerVehicleID(playerid);

	if (PlayerInfo[playerid][pJob] != JOB_MECHANIC) return SendErrorMessage(playerid, "Vous n'êtes pas mécanicien.");
	if (!IsDriver(playerid) || !IsATowTruck(vehicle)) return SendErrorMessage(playerid, "Vous devez être dans une dépanneuse en tant que conducteur.");
	if (IsTrailerAttachedToVehicle(vehicle)) {
		DetachTrailerFromVehicle(vehicle);
		return SendErrorMessage(playerid, "Le véhicule précédemment attaché a été détaché.");
	}

	if (sscanf(params, "u", userid)) return SendHelpMessage(playerid, "/attach [ID/nom du joueur]");
	if (userid == playerid) return SendErrorMessage(playerid, "Vous ne pouvez pas effectuer cette action sur vous-même.");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (!PlayerNearPlayer(8.0, playerid, userid)) return SendNotNearPlayer(playerid);
	if (!IsPlayerInAnyVehicle(userid)) return SendErrorMessage(playerid, "Le joueur doit être dans un véhicule.");

	if (pTemp[userid][pServePlayer] == playerid && pTemp[userid][pServeType] == 5) return SendErrorMessage(playerid, "Vous avez déjà proposé vos services à %s.", GetNameEx(userid));

	new vehid = GetPlayerVehicleID(userid);

	if (IsABoat(vehid) || IsAPlane(vehid) || IsAHelicopter(vehid) || IsABiker(vehid)) return SendClientMessage(playerid, COLOR_GREY, "Ce type de véhicule ne peut pas être attaché.");

	new mes[128];
	format(mes, sizeof(mes), "propose à %s de remorquer %s.", GetNameWithMask(userid), VehicleNames[GetVehicleModel(vehid) - 400]);
	cmd::me(playerid, mes);

	SendClientMessageEx(userid, COLOR_INFO, "* %s vous propose de remorquer votre véhicule. (Sortez du véhicule et appuyez sur Y pour accepter l'offre)", GetNameWithMask(playerid));
	SetPVarInt(userid, #tow_car, vehid);
	pTemp[userid][pServePlayer] = playerid;
	pTemp[userid][pServeType] = 5;
	SetPVarInt(userid, #accept_time, gettime() + 30);
	return 1;
}
