CMD:plant(playerid, params[])
{
	new option[16], secoption[6];
	
	if (sscanf(params, "s[16]S()[6]", option, secoption))
	{
		SendHelpMessage(playerid, "/plant [paramètre]");
		SendSyntaxMessage(playerid, "add / take");
		return 1;
	}

	if (CompareStrings(option, "add"))
	{
		new drugname[16];

		if (sscanf(secoption, "s[6]", drugname))
		{
			SendHelpMessage(playerid, "/plant add [paramètre]");
			SendSyntaxMessage(playerid, "ganja - graines de marijuana / coca - graines de coca / poppy - graines de pavot.");
			return 1;
		}

		new type;
		if (CompareStrings(drugname, "ganja"))
		{
			if (!IsPlayerHasItem(playerid, INV_SEED_GANJA)) return SendErrorMessage(playerid, "Vous n'avez pas de graines de marijuana.");
			type = 1;
		}
		else if (CompareStrings(drugname, "coca"))
		{
			if (!IsPlayerHasItem(playerid, INV_SEED_COCA)) return SendErrorMessage(playerid, "Vous n'avez pas de graines de coca.");
			type = 2;
		}
		else if (CompareStrings(drugname, "poppy"))
		{
			if (!IsPlayerHasItem(playerid, INV_SEED_POPPY)) return SendErrorMessage(playerid, "Vous n'avez pas de graines de pavot.");
			type = 3;
		}
		else 
		{
			SendHelpMessage(playerid, "/plant add [paramètre]");
			SendSyntaxMessage(playerid, "ganja - graines de marijuana / coca - graines de coca / poppy - graines de pavot.");
			return 1;
		}		
		
		if (Plant_Nearest(playerid, 2.0) != -1)	return SendErrorMessage(playerid, "Une plante est déjà plantée à proximité.");
		if (pTemp[playerid][pPlantTime] > 0)		return SendErrorMessage(playerid, "Vous êtes déjà en train de planter.");

		pTemp[playerid][pPlantTime] = 15;

		switch(type)
		{
			case 1: format(drugname, sizeof(drugname), "marijuana");
			case 2: format(drugname, sizeof(drugname), "coca");
			case 3: format(drugname, sizeof(drugname), "pavot");
		}

		pTemp[playerid][pPlantType] = type;

		SendSyntaxMessage(playerid, "Vous commencez à planter un buisson de %s. Utilisez {FF6347}/stop{FFFFFF} pour annuler.", drugname);

		FreezePlayer(playerid);
		OnPlayAnim(playerid, "BOMBER", "BOM_Plant_Loop", 4.0, 1, 0, 0, 0, 0);
		return 1;
	}
	else if (CompareStrings(option, "take"))
	{
		new plant = -1;
		if ((plant = Plant_Nearest(playerid)) == -1)		
			return SendErrorMessage(playerid, "Aucune plante proche à récolter.");
		if (CropsInfo[plant][pGrowTime] > gettime())		
			return SendErrorMessage(playerid, "La plante n'est pas encore mûre.");

		new drugname[16],
			type,
			Float: gPlant;

		switch(CropsInfo[plant][pType])
		{
			case 1:
			{
				gPlant = float(randomEx(10, 30)) / 10.0;
				type = INV_GANJA;
				format(drugname, sizeof(drugname), "marijuana");
			}
			case 2:
			{
				gPlant = float(randomEx(70, 300)) / 10.0;
				type = INV_COCA;
				format(drugname, sizeof(drugname), "feuilles de coca");
			}
			case 3:
			{
				gPlant = float(randomEx(50, 90)) / 10.0;
				type = INV_POPPY;
				format(drugname, sizeof(drugname), "pavot");
			}
		}

		if (GiveFloatInvItem(playerid, type, gPlant) == -1)
			return SendErrorMessage(playerid, "Pas assez de place dans l'inventaire.");

		DestroyCrop(plant);
		OnPlayAnim(playerid, "BOMBER", "BOM_Plant_Loop", 4.0, 0, 0, 0, 0, 0);

		SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "* %s a récolté les fruits d'une plante.", GetNameWithMask(playerid));
		SendSyntaxMessage(playerid, "Vous avez récolté %.1f grammes de %s.", gPlant, drugname);
	}
	else 
	{
		SendHelpMessage(playerid, "/plant [paramètre]");
		SendSyntaxMessage(playerid, "add / take");
	}
	return 1;
}


CMD:mixdrug(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Vous devez être dans un laboratoire!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Vous devez être dans un laboratoire!");

	new f = GetClosestFurniture(playerid, 1.5, CENTRIFUGE_OBJECT, DRUG_LAB);
	if (f == -1) return SendErrorMessage(playerid, "Aucune centrifugeuse à proximité.");

	new i;
	switch (b_type)
	{
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}

	if (i == -1 && ShowMixingDialog(playerid, Part_Mixing))
		return SendHelpMessage(playerid, "Sélectionnez 3 réactifs différents pour créer une drogue. {FF6347}(ils doivent être uniques)");

	else if (Chemistry[i][IsWorking] == true)
		return SendErrorMessage(playerid, "La centrifugeuse est déjà en cours d’utilisation!");

	else if (!Chemistry[i][IsWorking] && Chemistry[i][Amount] > 0.0)
	{
		if (GetPVarInt(playerid, "CarryingDrugCrate"))
			return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
		SetPlayerAttachedObject(playerid, 9, 1575, 1, -0.039999, 0.414759, -0.042998, -31.400018, 92.117507, 112.900047);

		SetPlayerHoldingCrate(playerid,
			Chemistry[i][Type],
			Chemistry[i][Amount],
			Chemistry[i][Quality],
			Chemistry[i][Extra][0],
			Chemistry[i][Extra][1],
			Chemistry[i][Extra][2],
			Chemistry[i][Material]
		);

		SendHelpMessage(playerid, "Transportez le mélange vers le réacteur avec la commande {ffc46a}/reactor{FFFFFF}. Pour le déposer, utilisez {ffc46a}/dropmix{FFFFFF}.");
		ClearFurnitureData(f, i);

		ApplyAnimation(playerid, "carry", "liftup", 1, 0, 1, 1, 0, 1000);
	}
	return 1;
}

CMD:drydrug(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Vous devez être dans un laboratoire!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Vous devez être dans un laboratoire!");

	new f = GetClosestFurniture(playerid, 1.5, DRYER_OBJECT, DRUG_LAB);
	if (f == -1) return SendErrorMessage(playerid, "Aucun séchoir à proximité.");

	new i;
	switch (b_type) {
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}

	if (i == -1)
	{
		for (new g = 0; g < MAX_INV_SLOTS; g++) P_CHEMICAL[playerid][g][pSlot] = 0;

		new pickDialog[MAX_INV_SLOTS*64 + 64] = "Sélectionnez un ingrédient à sécher :",
			count = 0;

		for (new g = 0; g < MAX_INV_SLOTS; g++)
		{
			if (Inventory[playerid][g][pInvItem] != 115 && Inventory[playerid][g][pInvItem] != 116) continue;

			format(gString, sizeof(gString), "\n%s (%.1fg)",
				E_CHEMICAL[Inventory[playerid][g][pInvItem]-103][NAME],
				Inventory[playerid][g][pInvQFloat]
			);
			strcat(pickDialog, gString);

			P_CHEMICAL[playerid][count][pSlot] = g;
			count++;
		}

		if (count == 0)
		{
			Dialog_Show(playerid, None, DIALOG_STYLE_LIST, "Séchoir", "Aucune feuille de coca ou capsule de pavot trouvée dans l'inventaire.", ">>", "");
			return 0;
		}
		else Dialog_Show(playerid, ConfDryer, DIALOG_STYLE_TABLIST_HEADERS, "Séchoir", pickDialog, "Choisir", "<<");
	}
	else if (Chemistry[i][IsWorking])
	{
		return SendErrorMessage(playerid, "Le séchoir est déjà en fonctionnement.");
	}
	else if (Chemistry[i][Amount] > 0.0)
	{
		switch (Chemistry[i][Material])
		{
			case 12:
				if (GiveFloatInvItem(playerid, 117, Chemistry[i][Amount]) == -1)
					return SendErrorMessage(playerid, "Pas assez d'espace dans l'inventaire.");
			case 13:
				if (GiveFloatInvItem(playerid, 118, Chemistry[i][Amount]) == -1)
					return SendErrorMessage(playerid, "Pas assez d'espace dans l'inventaire.");
		}

		SM2(playerid, "Vous avez reçu %s (%.1f) dans votre inventaire.", E_CHEMICAL[Chemistry[i][Material]][NAME], Chemistry[i][Amount]);
		ClearFurnitureData(f, i);
	}

	return 1;
}

CMD:reactor(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction maintenant.");

	new b_type = GetNearBuilding(playerid);
	if (b_type == -1) return SendErrorMessage(playerid, "Vous devez être dans un laboratoire!");

	new building = GetNearInBuilding(playerid, b_type);
	if (building == -1) return SendErrorMessage(playerid, "Vous devez être dans un laboratoire!");

	new f = GetClosestFurniture(playerid, 1.5, REACTOR_OBJECT, DRUG_LAB);
	if (f == -1) return SendErrorMessage(playerid, "Aucun réacteur à proximité.");

	if (GetClosestFurniture(playerid, 2.5, PUMP_OBJECT, DRUG_LAB) == -1)
		return SendErrorMessage(playerid, "Une station de pompage doit être connectée au réacteur.");

	new i;
	switch (b_type) {
		case 0: i = HFurn[building][f][ChemIdx];
		case 1: i = BFurn[building][f][ChemIdx];
		case 2: i = GFurn[building][f][ChemIdx];
	}

	if (i == -1)
	{
		if (!GetPVarInt(playerid, "CarryingDrugCrate"))
			return SendErrorMessage(playerid, "Vous n'avez aucun mélange de réactifs en main.");

		if ((i = GetFreeChemistyIndex()) == -1)
			return SendErrorMessage(playerid, "Limite atteinte ! Contactez un administrateur technique.");

		SendHelpMessage(playerid, "Pour commencer la synthèse, utilisez {ffc46a}/reactor{FFFFFF}.");

		ReactDrug(i, GetPVarInt(playerid, "nativeCAS"), GetPVarFloat(playerid, "tdrug_Amount"), GetPVarFloat(playerid, "tdrug_Quality"), GetPVarInt(playerid, "BreakingBad_1"),
			GetPVarInt(playerid, "BreakingBad_2"), GetPVarInt(playerid, "BreakingBad_3"), GetPVarInt(playerid, "tdrug_Type"), b_type, building, f);

		ResetDrugCrate(playerid);
		ApplyAnimation(playerid, "carry", "putdwn", 1, 0, 1, 1, 0, 1000);
		return 1;
	}

	if (Chemistry[i][IsWorking])
	{
		static const str_template[] = "> Température actuelle: %s%.0f °C\n\n{FFFFFF}Modes :\n\n1.\t{8fbfd0} Refroidissement{FFFFFF}\t( x < 0 ) \n2.\t{FF6347} Chauffage{FFFFFF}\t( x > 0 )\n\n{FFFFFF}Entrez une température (x) pour ajuster le capteur :";

		new mes[sizeof(str_template) + 6];
		format(mes, sizeof(mes), str_template, (Chemistry[i][Temperature] > 0) ? "+" : "", Chemistry[i][Temperature]);

		Dialog_Show(playerid, ControlSelect, DIALOG_STYLE_INPUT, "Capteur de température", mes, "Confirmer", "Annuler");
	}
	else
	{
		if (Chemistry[i][Amount] <= 0.0)
			return SendErrorMessage(playerid, "Le réacteur est vide.");

		new title[32];
		format(title, sizeof(title), "%s", E_CHEMICAL[Chemistry[i][Material]][NAME]);

		format(gString, sizeof(gString), "{FFFFFF}Structure : %s\nPureté : {FFFF00}%.1f%%{FFFFFF}\n\n\tAnalyse : %.1f g", 
			E_CHEMICAL[Chemistry[i][Material]][CODE],
			Chemistry[i][Quality],
			Chemistry[i][Amount]
		);

		Dialog_Show(playerid, Purification, DIALOG_STYLE_MSGBOX, title, gString, ">>", "Annuler");
	}

	return 1;
}

CMD:pickupmix(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette commande pour le moment.");

	if (GetPVarInt(playerid, #pDelay)) return 1;
	if (GetPVarInt(playerid, "CarryingDrugCrate"))
		return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

	new dropid = MixDrop_Nearest(playerid);
	if (dropid == -1)
		return SendErrorMessage(playerid, "Il n'y a aucun mélange de réactifs près de vous.");

	SetPlayerChatBubble(playerid, "* ramasse un mélange de réactifs *", COLOR_PURPLE, Range_Normal, 10000);

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	SetPlayerAttachedObject(playerid, 9, 1575, 1, -0.039999, 0.414759, -0.042998, -31.400018, 92.117507, 112.900047);

	SetPlayerHoldingCrate(playerid,
		DropChemistry[dropid][Type],
		DropChemistry[dropid][Amount],
		DropChemistry[dropid][Quality],
		DropChemistry[dropid][Extra][0],
		DropChemistry[dropid][Extra][1],
		DropChemistry[dropid][Extra][2],
		DropChemistry[dropid][Material]
	);

	DropChemistry_Delete(dropid);

	ApplyAnimation(playerid, "carry", "liftup", 1, 0, 1, 1, 0, 1000);
	return 1;
}


CMD:dropmix(playerid, params[])
{
	if (GetPVarInt(playerid, "CarryingDrugCrate") == 0)
		return SendErrorMessage(playerid, "Vous ne portez aucun mélange de réactifs !");

	for(new i; i != MAX_CHEM_DROPS; i++)
	{
	    if (DropChemistry[i][temp_ID] != 0) continue;

		DropChemistry[i][Material]   = GetPVarInt(playerid, "nativeCAS");
		DropChemistry[i][Type]       = GetPVarInt(playerid, "tdrug_Type");
		DropChemistry[i][Amount]     = GetPVarFloat(playerid, "tdrug_Amount");
		DropChemistry[i][Quality]    = GetPVarFloat(playerid, "tdrug_Quality");
		DropChemistry[i][Extra][0]   = GetPVarInt(playerid, "BreakingBad_1");
		DropChemistry[i][Extra][1]   = GetPVarInt(playerid, "BreakingBad_2");
		DropChemistry[i][Extra][2]   = GetPVarInt(playerid, "BreakingBad_3");
		DropChemistry[i][dropInt]    = GetInterior(playerid);
		DropChemistry[i][dropVW]     = GetWorld(playerid);
		DropChemistry[i][dropTime]   = gettime();

		GetXYInFrontOfPlayer(playerid, DropChemistry[i][dropPosX], DropChemistry[i][dropPosY], DropChemistry[i][dropPosZ], 1.5);
		DropChemistry[i][dropPosZ]  -= 1.0;
		DropChemistry[i][dropPosRZ]  = 90.0;

		new query[350];
		mysql_format(dbHandle, query, sizeof(query), 
			"INSERT INTO `drop_chemistry` (`Material`,`Amount`,`Quality`,`Extra0`,`Extra1`,`Extra2`,`dropPosX`,`dropPosY`,`dropPosZ`,`dropPosRX`,`dropPosRY`,`dropPosRZ`,`dropInt`,`dropVW`,`dropTime`) VALUES (%i,%f,%f,%i,%i,%i,%f,%f,%f,%f,%f,%f,%i,%i,%i)",
			DropChemistry[i][Material],
			DropChemistry[i][Amount],
			DropChemistry[i][Quality],
			DropChemistry[i][Extra][0],
			DropChemistry[i][Extra][1],
			DropChemistry[i][Extra][2],
			DropChemistry[i][dropPosX], DropChemistry[i][dropPosY], DropChemistry[i][dropPosZ],
			DropChemistry[i][dropPosRX], DropChemistry[i][dropPosRY], DropChemistry[i][dropPosRZ],
			DropChemistry[i][dropInt],
			DropChemistry[i][dropVW],
			DropChemistry[i][dropTime]
		);
		mysql_tquery(dbHandle, query, "GetIDforChemistryDrop", "d", i);

		SetPlayerChatBubble(playerid, "* jette un mélange de réactifs *", COLOR_PURPLE, 20.0, 4000);
		SendSyntaxMessage(playerid, "Vous avez jeté un mélange de réactifs. Tapez {ffc46a}/pickupmix{FFFFFF} ou accroupissez-vous et appuyez sur {ffc46a}H{FFFFFF} pour le ramasser.");

		DropChemistry[i][dropObject] = CreateDynamicObject(1575, DropChemistry[i][dropPosX], DropChemistry[i][dropPosY], DropChemistry[i][dropPosZ], 93.7, 120.0, 120.0, GetWorld(playerid), GetInterior(playerid));
		EditDynamicObject(playerid, DropChemistry[i][dropObject]);
		SetPVarInt(playerid, #edit_drug, i + 1);

		ResetDrugCrate(playerid);
		break;
	}
	return 1;
}