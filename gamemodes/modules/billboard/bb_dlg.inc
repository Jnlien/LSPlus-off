Dialog:BillboardList(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (BillboardInfo[listitem][bbRentDate]) {
		cmd::rentbillboard(playerid, "");
		return SendErrorMessage(playerid, "Ce panneau publicitaire est déjà loué !");
	}

	static const msg[] = 
		"Choisissez la durée de location :\n\
		{FFFFFF}24 heures\t{FF6347}$%i\n\
		{FFFFFF}48 heures\t{FF6347}$%i\n\
		{FFFFFF}72 heures\t{FF6347}$%i";

	new string[sizeof(msg) + 10];
	format(string, sizeof(string), msg,
		BillboardInfo[listitem][bbRentCost],
		floatround(BillboardInfo[listitem][bbRentCost] * 2.2),
		floatround(BillboardInfo[listitem][bbRentCost] * 3.5)
	);

	SetPVarInt(playerid, #BB_SELECT_ID, listitem);
	Dialog_Show(playerid, BillboardRent, DIALOG_STYLE_TABLIST_HEADERS, "Location de panneau publicitaire", string, "Suivant", "Annuler");
		
	return 1;
}

Dialog:BillboardRent(playerid, response, listitem, inputtext[])
{
	if (!response) return DeletePVar(playerid, #BB_SELECT_ID);

	SetPVarInt(playerid, #BB_SELECT_HH, listitem);

	new id = GetPVarInt(playerid, #BB_SELECT_ID);

	static const msg[] =
		"{ffffff}Entrez maintenant le texte de votre publicité !\n\
		Nombre maximum de caractères autorisé : %i\n\n\n\
		{c3c3c3}[ ! ] Sauvegardez ce que vous avez écrit avec CTRL+C si besoin.";

	new string[sizeof(msg) + 1];
	new max_char;

	switch (BillboardInfo[id][bbModel]) {
		case 7302: max_char = 100;
		case 9314: max_char = 87;
	}

	format(string, sizeof(string), msg, max_char);

	Dialog_Show(playerid, BillboardRent2, DIALOG_STYLE_INPUT, "Location de panneau publicitaire", string, "Suivant", "Annuler");

	return 1;
}


/*
bblen(str[]) {
    new stop = 0, count = 0;

    for(new i; i < strlen(str); i++) {
        if (str[i] = "{") stop = 1;
        if (str[i] = "}") stop = 0;

        if (!stop) count++;
    }
    return count++;
}*/

Dialog:BillboardRent2(playerid, response, listitem, inputtext[])
{
	if (!response) {
		DeletePVar(playerid, #BB_SELECT_ID);
		DeletePVar(playerid, #BB_SELECT_HH);
		return 1;
	}

	new id = GetPVarInt(playerid, #BB_SELECT_ID);

	if (CheckDialogString(inputtext)) {
		cmd::rentbillboard(playerid, "");
		DeletePVar(playerid, #BB_SELECT_ID);
		DeletePVar(playerid, #BB_SELECT_HH);
		return SendErrorMessage(playerid, "Le texte ne peut pas contenir de retour à la ligne '\\n' !");
	}

	if (1 < strlen(inputtext) < 100) {
		SetPVarString(playerid, #BB_SELECT_TEXT, TextTab(inputtext, 22, 35, 1, 0));

		static const msg[] =
			"{ffffff}• Panneau publicitaire {c3c3c3}#%i\n\
			{ffffff}• Coût de location : {c3c3c3}$%i\n\
			{ffffff}• Durée de location : {c3c3c3}%i h\n\
			{ffffff}• Texte de la publicité :\n\n\
			{c3c3c3}%s\n\n\
			{ffffff}Vérifiez si tout est correct. Si oui, cliquez sur 'Acheter'.";

		new string[sizeof(msg) + 100 + 2];
		new hours, cost;

		switch (GetPVarInt(playerid, #BB_SELECT_HH)) {
			case 0: { hours = 24; cost = BillboardInfo[id][bbRentCost]; }
			case 1: { hours = 48; cost = floatround(BillboardInfo[id][bbRentCost] * 2.2); }
			case 2: { hours = 72; cost = floatround(BillboardInfo[id][bbRentCost] * 3.5); }
		}

		format(string, sizeof(string), msg, id + 1, cost, hours, TextTab(inputtext, 22, 35, 1, 0)
		);

		Dialog_Show(playerid, BillboardRentFinal, DIALOG_STYLE_MSGBOX, "Location de panneau publicitaire", string, "Acheter", "Annuler");
	} else {
		cmd::rentbillboard(playerid, "");

		new max_char;
		switch (BillboardInfo[id][bbModel]) {
			case 7302: max_char = 100;
			case 9314: max_char = 87;
		}

		DeletePVar(playerid, #BB_SELECT_ID);
		DeletePVar(playerid, #BB_SELECT_HH);

		return SendErrorMessage(playerid, "Le texte doit contenir entre 1 et %i caractères.", max_char);
	}

	return 1;
}


Dialog:BillboardRentFinal(playerid, response, listitem, inputtext[])
{
	if (!response) {
		DeletePVar(playerid, #BB_SELECT_ID);
		DeletePVar(playerid, #BB_SELECT_HH);
		DeletePVar(playerid, #BB_SELECT_TEXT);
		return 1;
	}

	new message[100], hours, cost, id = GetPVarInt(playerid, #BB_SELECT_ID);
	GetPVarString(playerid, #BB_SELECT_TEXT, message, sizeof(message));

	switch (GetPVarInt(playerid, #BB_SELECT_HH)) {
		case 0: { hours = 24; cost = BillboardInfo[id][bbRentCost]; }
		case 1: { hours = 48; cost = floatround(BillboardInfo[id][bbRentCost] * 2.2); }
		case 2: { hours = 72; cost = floatround(BillboardInfo[id][bbRentCost] * 3.5); }
	}

	GiveCash(playerid, -cost);

	BillboardInfo[id][bbRentDate] = gettime() + hours * 3600;
	BillboardInfo[id][bbRentBy] = PlayerInfo[playerid][pID];
	format(BillboardInfo[id][bbText],  100, "%s\n{595959}Tel. %i", message, PlayerInfo[playerid][pNumber]);

	Save_BB(id);

	SendHelpMessage(playerid, "Vous avez loué le panneau publicitaire #%i pour {FF6347}%i h{ffffff} pour {FF6347}$%i{ffffff}.", id + 1, hours, cost);
	SendHelpMessage(playerid, "Utilisez /mybb pour gérer vos panneaux publicitaires.");

	SetDynamicObjectMaterial(BillboardInfo[id][bbObject], 0, -1, "none", "none", 0xFFFFFFFF);
	SetDynamicObjectMaterialText(
		BillboardInfo[id][bbObject], 0, BillboardInfo[id][bbText],
		OBJECT_MATERIAL_SIZE_512x128, "Arial", 28, 0,
		0xFF000000, 0x0FFFFFFF, OBJECT_MATERIAL_TEXT_ALIGN_CENTER
	);

	return 1;
}

Dialog:MyBillboards(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new id, count;

	for (new i; i < MAX_BILLBOARDS; i++) {
		if (BillboardInfo[i][bbRentBy] == PlayerInfo[playerid][pID]) {
			if (count == listitem) {
				id = i;
				break;
			}
			count++;
		}
	}

	static const msg[] = "{FFFFFF}Souhaitez-vous vraiment annuler la location du panneau publicitaire #%i, situé à %s ?";
	new string[sizeof(msg) + MAX_ZONE_NAME];

	new location[MAX_ZONE_NAME];
	GetZone(BillboardInfo[id][bbPosX], BillboardInfo[id][bbPosY], location, MAX_ZONE_NAME);

	format(string, sizeof(string), msg, id + 1, location);

	SetPVarInt(playerid, #BB_SELECT_ID, id);
	ConfirmDialog(playerid, "Confirmation", string, "MyBillBoard_Unrent");

	return 1;
}