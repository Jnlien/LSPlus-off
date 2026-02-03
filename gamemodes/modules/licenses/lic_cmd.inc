CMD:licenseexam(playerid, params[])
{
	new vehicle = GetPlayerVehicleID(playerid);

	if (!IsADMVCar(vehicle) && !IsADMVHeli(vehicle) && !IsADMVTaxi(vehicle))
		return SendErrorMessage(playerid, "Vous devez être dans un véhicule école.");

	if (!IsDriver(playerid))
		return SendErrorMessage(playerid, "Vous devez être au volant du véhicule.");

	if (pTemp[playerid][pLicenseExam] != 0)
		return SendErrorMessage(playerid, "Vous passez déjà un examen.");

	if (IsADMVCar(vehicle))
	{
		if (PlayerInfo[playerid][pCarLic] != 0)
			return SendErrorMessage(playerid, "Vous possédez déjà un permis de conduire.");

		if (GetCash(playerid) < CARLIC_PRICE)
			return SendNotCash(playerid);

		VehicleEngine(vehicle, true);
		SendSyntaxMessage(playerid, "Vous avez commencé l'examen pour obtenir un permis de conduire.");
		SendSyntaxMessage(playerid, "Vous avez %i minute(s) pour le terminer, sinon vous échouerez.", EXAM_TIME / 60);

		pTemp[playerid][pLicenseExam] = 1;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, license_exam_cp[0], true);
		GiveCash(playerid, -CARLIC_PRICE);

		new bizid = LicenseOffice_Nearest(playerid);
		if (bizid != -1) GiveBizCash(bizid, CARLIC_PRICE);

		if (!IsPlayerInAnyVehicle(playerid)) {
			pTemp[playerid][pLicenseExam] = 0;
			DeletePVar(playerid, #exam_second);
			SendSyntaxMessage(playerid, "Vous avez échoué à l'examen car vous avez quitté le véhicule.");
			TogglePlayerAllDynamicRaceCPs(playerid, false);
			RespawnVehicle(vehicle);
			return 1;
		}
	}
	else if (IsADMVHeli(vehicle))
	{
		if (PlayerInfo[playerid][pFlyLic] != 0)
			return SendErrorMessage(playerid, "Vous possédez déjà une licence de pilote.");

		if (PlayerInfo[playerid][pFlyLic] == 2)
			return SendErrorMessage(playerid, "Vous êtes sur liste noire pour la licence de pilote.");

		if (GetCash(playerid) < FLYLIC_PRICE)
			return SendNotCash(playerid);

		VehicleEngine(vehicle, true);
		SendSyntaxMessage(playerid, "Vous avez commencé l'examen pour obtenir une licence de pilote.");
		SendSyntaxMessage(playerid, "[1]: Pendant le vol, évitez les collisions.");
		SendSyntaxMessage(playerid, "[2]: Conduisez avec prudence et ne vous précipitez pas.");

		pTemp[playerid][pLicenseExam] = 2;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, fly_exam_cp[0], true);
		GiveCash(playerid, -FLYLIC_PRICE);
	}
	else if (IsADMVTaxi(vehicle))
	{
		if (PlayerInfo[playerid][pCarLic] == 0)
			return SendErrorMessage(playerid, "Vous devez d'abord avoir un permis de conduire.");

		if (PlayerInfo[playerid][pTaxiLic] != 0)
			return SendErrorMessage(playerid, "Vous possédez déjà une licence de chauffeur de taxi.");

		if (PlayerInfo[playerid][pTaxiLic] == 2)
			return SendErrorMessage(playerid, "Vous êtes sur liste noire pour la licence de taxi.");

		if (GetCash(playerid) < TAXILIC_PRICE)
			return SendNotCash(playerid);

		VehicleEngine(vehicle, true);
		SendSyntaxMessage(playerid, "Vous avez commencé l'examen pour obtenir une licence de chauffeur de taxi.");
		SendSyntaxMessage(playerid, "Vous avez %i minute(s) pour le terminer, sinon vous échouerez.", EXAM_TIME / 60);

		pTemp[playerid][pLicenseExam] = 3;
		SetPVarInt(playerid, #exam_second, EXAM_TIME);
		TogglePlayerDynamicRaceCP(playerid, license_exam_cp[0], true);
		GiveCash(playerid, -TAXILIC_PRICE);

		new bizid = LicenseOffice_Nearest(playerid);
		if (bizid != -1) GiveBizCash(bizid, TAXILIC_PRICE);
	}
	return 1;
}