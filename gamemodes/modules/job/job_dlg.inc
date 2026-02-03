Dialog:BuyDrugs(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new drug_list_name[][] = {"Graines de cannabis", "LSD", "Héroïne", "Cocaïne", "Éphédrine"};
	new drug_list_price[] = {50, 150, 100, 200, 50};
	
	static const str_size[] = "{FFFFFF}Souhaitez-vous acheter %s pour $%i par gramme ?\nQuantité disponible : 1 à 1000.";
	new mes[sizeof(str_size) - 4 + 7 + 3 + 1];

	format(mes, sizeof(mes), str_size, drug_list_name[listitem], drug_list_price[listitem]);
    Dialog_Show(playerid, BuyDrugsAccept, DIALOG_STYLE_INPUT, "{FFFFFF}Achat de drogues", mes, "Valider", "Annuler");
    SetPVarInt(playerid, #select_law, listitem);
	return 1;
}

Dialog:BuyDrugsAccept(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::buydrugs(playerid, "");

	new amount = strval(inputtext);

	listitem = GetPVarInt(playerid, #select_law);

	if (amount < 0 || amount > 1000) return SendErrorMessage(playerid, "Quantité invalide de drogue : entre 1 et 1000.");

	new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Vous n'appartenez pas à une faction officielle.");
	if (PlayerInfo[playerid][pSideJob] != JOB_DRUGDEALER) return SendErrorMessage(playerid, "Vous n'avez pas accès à l'achat de drogues.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;
	
	if (!PlayerToPoint(2.0, playerid, 2705.23, -2550.89, 13.634))
	{
		Waypoint_Set(playerid, 2705.23, -2550.89, 13.634);
		return SendErrorMessage(playerid, "Vous devez être dans les docks.");
	}

	new drug_list[] = {INV_SEED_GANJA, INV_LSD, INV_HEROIN, INV_COCAINE, INV_EPHEDRINE};
	new drug_price[] = {50, 150, 100, 200, 50};

	if (GetCash(playerid) < drug_price[listitem] * amount) return SendNotCash(playerid);
	if (GiveInvItem(playerid, drug_list[listitem], amount) == -1) return SendErrorMessage(playerid, "Vous n'avez plus de place dans l'inventaire.");

	SendSyntaxMessage(playerid, "Vous avez acheté %i grammes de %s pour $%i.", amount, inventory_items[drug_list[listitem]][itemName], drug_price[listitem] * amount);
	GiveCash(playerid, -drug_price[listitem] * amount);
	return 1;
}

Dialog:BuyAmmoLaw(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::buygun(playerid, "");

	new faction = PlayerInfo[playerid][pFaction], ammo = 1;

	new gun_list[][20] = {"M1911","M1911","Desert Eagle","Desert Eagle","Shotgun","Shotgun","Shotgun","Tec-9 / MP5 / Uzi", "Tec-9 / MP5 / Uzi","Country Rifle"},
		ammo_list[] = {82, 83, 84, 85, 86, 87, 88, 90, 91, 96},
		ammo_pr[] = {35, 80, 60, 130, 300, 200, 100, 200, 450, 400};

	if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Vous n'appartenez pas à une faction officielle.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER) return SendErrorMessage(playerid, "Vous n'avez pas accès à l'achat d'armes.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;
	
	if (!PlayerToPoint(2.0, playerid, 2705.23, -2550.89, 13.634))
	{
		Waypoint_Set(playerid, 2705.23, -2550.89, 13.634);
		return SendErrorMessage(playerid, "Vous devez être dans les docks.");
	}

	if (!CheckInv(playerid)) return SendErrorMessage(playerid, "Votre inventaire est plein."); 
	if (GetCash(playerid) < ammo_pr[listitem]) return SendNotCash(playerid);
	if (Factions[faction][fPoint] - ammo < 0) return SendErrorMessage(playerid, "Votre faction n'a pas assez de points.");

	GiveCash(playerid, -ammo_pr[listitem]);
	GiveInvItem(playerid, ammo_list[listitem], CheckAmmo(ammo_list[listitem])); 

	Factions[faction][fPoint] -= (listitem + 1);
	SQL_SetInteger("factions", "point", Factions[faction][fPoint], Factions[faction][fID]);

	SendSyntaxMessage(playerid, "Vous avez acheté un chargeur de %s [%i munitions] pour %s pour $%i.", inventory_items[ammo_list[listitem]][itemName], CheckAmmo(ammo_list[listitem]), gun_list[listitem], ammo_pr[listitem]);
	return 1;
}

Dialog:BuyGunLaw(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::buygun(playerid, "");

    new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Vous n'appartenez pas à une faction officielle.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER) return SendErrorMessage(playerid, "Vous n'avez pas accès à l'achat d'armes.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;

	if (Factions[faction][fType] == FACTION_MAFIA)
	{
		new gun_list[] = {24, 22, 23, 25, 26, 27, 28, 32, 29, 30, 31, 33, 16, 18, INV_OPTICS, INV_SILENCER, INV_TOOLS}, 
			gun_point[] = {3, 3, 3, 4, 4, 4, 5, 5, 5, 7, 7, 10, 10, 4, 2, 2, 1}, 
			gun_price[] = {1800, 1000, 1500, 2400, 2100, 4000, 2000, 2500, 2800, 3500, 4300, 3800, 1000, 300, 1700, 500, 300}, 
			gun_ammo[] = {50, 50, 50, 30, 30, 30, 120, 120, 120, 150, 150, 15, 1, 1, 1, 1, 1};

		if (Factions[faction][fPoint] - gun_point[listitem] < 0) return SendErrorMessage(playerid, "Votre faction n'a pas assez de points pour cet achat.");

		new mes[256];
		if (ValidWeaponForInventory(gun_list[listitem]))
		{
			format(mes, sizeof(mes), "{FFFFFF}Confirmez-vous l'achat de %s avec %i munitions pour %i points et $%i ?\nIl restera %i points à votre faction.",
			inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
		else
		{
			format(mes, sizeof(mes), "{FFFFFF}Confirmez-vous l'achat de %s pour %i points et $%i ?\nIl restera %i points à votre faction.",
			inventory_items[gun_list[listitem]][itemName], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
    	Dialog_Show(playerid, BuyGunAccept, DIALOG_STYLE_MSGBOX, "{FFFFFF}Achat d'une arme", mes, "Acheter", "Retour");
	}
	else if (Factions[faction][fType] == FACTION_GANG)
	{
		new gun_list[] = {24, 22, 25, 26, 28, 32, 18, INV_TOOLS}, 
			gun_point[] = {3, 3, 4, 4, 5, 5, 4, 1}, 
			gun_price[] = {1800, 1000, 2400, 2100, 2000, 2500, 300, 300}, 
			gun_ammo[] = {50, 50, 30, 30, 120, 120, 1, 1};

		if (Factions[faction][fPoint] - gun_point[listitem] < 0) return SendErrorMessage(playerid, "Votre faction n'a pas assez de points pour cet achat.");

		new mes[256];
		if (ValidWeaponForInventory(gun_list[listitem]))
		{
			format(mes, sizeof(mes), "{FFFFFF}Confirmez-vous l'achat de %s avec %i munitions pour %i points et $%i ?\nIl restera %i points à votre faction.",
			inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
		else
		{
			format(mes, sizeof(mes), "{FFFFFF}Confirmez-vous l'achat de %s pour %i points et $%i ?\nIl restera %i points à votre faction.",
			inventory_items[gun_list[listitem]][itemName], gun_point[listitem], gun_price[listitem], Factions[faction][fPoint]-gun_point[listitem]);
		}
    	Dialog_Show(playerid, BuyGunAccept, DIALOG_STYLE_MSGBOX, "{FFFFFF}Achat d'une arme", mes, "Acheter", "Retour");
	}
	SetPVarInt(playerid, #select_law, listitem);
	return 1;
}

Dialog:BuyGunAccept(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::buygun(playerid, "");

	listitem = GetPVarInt(playerid, #select_law);

    new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Vous n'appartenez pas à une faction officielle.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER) return SendErrorMessage(playerid, "Vous n'avez pas accès à l'achat d'armes.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;
    if (!PlayerToPoint(2.0, playerid, 2705.23, -2550.89, 13.634))
	{
	    Waypoint_Set(playerid, 2705.23, -2550.89, 13.634);
	    return SendErrorMessage(playerid, "Vous devez être sur le quai.");
	}

	if (Factions[faction][fType] == FACTION_MAFIA)
	{
		new gun_list[] = {24, 22, 23, 25, 26, 27, 28, 32, 29, 30, 31, 33, 16, 18, INV_OPTICS, INV_SILENCER, INV_TOOLS},
		    gun_point[] = {3, 3, 3, 4, 4, 4, 5, 5, 5, 7, 7, 10, 10, 4, 2, 2, 1},
	    	gun_price[] = {1800, 1000, 1500, 2400, 2100, 4000, 2000, 2500, 2800, 3500, 4300, 3800, 1000, 300, 1700, 500, 300},
	    	gun_ammo[] = {50, 50, 50, 30, 30, 30, 120, 120, 120, 150, 150, 15, 1, 1, 1, 1, 1};

		if (Factions[faction][fPoint] - gun_point[listitem] < 0) return SendErrorMessage(playerid, "Votre faction n'a pas assez de points.");
		if (GetCash(playerid) < gun_price[listitem]) return SendNotCash(playerid);

		if (ValidWeaponForInventory(gun_list[listitem]))
		{
		    GivePlayerWeaponEx(playerid, gun_list[listitem], gun_ammo[listitem]);
		    SendSyntaxMessage(playerid, "Vous avez acheté %s avec %i munitions.", inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem]);
		}
		else
		{
        	if (GiveInvItem(playerid, gun_list[listitem], gun_ammo[listitem]) == -1) return SendErrorMessage(playerid, "Inventaire plein.");
			SendSyntaxMessage(playerid, "Vous avez acheté %s.", inventory_items[gun_list[listitem]][itemName]);
		}
		
        Factions[faction][fPoint] -= gun_point[listitem];
        SQL_SetInteger("factions", "point", Factions[faction][fPoint], Factions[faction][fID]);
		GiveCash(playerid, -gun_price[listitem]);
	}
	else if (Factions[faction][fType] == FACTION_GANG)
	{
  		new gun_list[] = {24, 22, 25, 26, 28, 32, 18, INV_TOOLS},
		    gun_point[] = {3, 3, 4, 4, 5, 5, 4, 1},
		    gun_price[] = {1800, 1000, 2400, 2100, 2000, 2500, 300, 300},
		    gun_ammo[] = {50, 50, 30, 30, 120, 120, 1, 1};

		if (Factions[faction][fPoint] - gun_point[listitem] < 0) return SendErrorMessage(playerid, "Votre faction n'a pas assez de points.");
		if (GetCash(playerid) < gun_price[listitem]) return SendNotCash(playerid);

		if (ValidWeaponForInventory(gun_list[listitem]))
		{
		    GivePlayerWeaponEx(playerid, gun_list[listitem], gun_ammo[listitem]);
		    SendSyntaxMessage(playerid, "Vous avez acheté %s avec %i munitions.", inventory_items[gun_list[listitem]][itemName], gun_ammo[listitem]);
		}
		else
		{
        	if (GiveInvItem(playerid, gun_list[listitem], gun_ammo[listitem]) == -1) return SendErrorMessage(playerid, "Inventaire plein.");
			SendSyntaxMessage(playerid, "Vous avez acheté %s.", inventory_items[gun_list[listitem]][itemName]);
		}

        Factions[faction][fPoint] -= gun_point[listitem];
		SQL_SetInteger("factions", "point", Factions[faction][fPoint], Factions[faction][fID]);
		GiveCash(playerid, -gun_price[listitem]);
	}
	return 1;
}

Dialog:BuyGunMain(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    new faction = PlayerInfo[playerid][pFaction];

	if (!PlayerInfo[playerid][pFaction]) return SendErrorMessage(playerid, "Vous n'appartenez pas à une faction officielle.");
	if (PlayerInfo[playerid][pSideJob] != JOB_GUNDEALER) return SendErrorMessage(playerid, "Vous n'avez pas accès à l'achat d'armes.");
	if (Factions[faction][fType] != FACTION_GANG && Factions[faction][fType] != FACTION_MAFIA) return 1;

    new title[56];
	format(title, sizeof(title), "{FFFFFF}Points disponibles dans votre faction : %i", Factions[faction][fPoint]);

	if (listitem == 0)
	{
	    if (Factions[faction][fType] == FACTION_MAFIA)
		{
			new mes[1024] = "Desert Eagle\t-3 points\t$1800\n\
							COLT45\t-3 points\t$1000\n\
							COLT45 (silencieux)\t-3 points\t$1500\n\
							Shotgun\t-4 points\t$2400\n\
							Sawn-off Shotgun\t-4 points\t$2100\n\
							Combat Shotgun\t-4 points\t$4000\n\
							UZI\t-5 points\t$2000\n\
							Tec-9\t-5 points\t$2500\n\
							MP5\t-5 points\t$2800\n\
							M4\t-7 points\t$4300\n\
							Rifle\t-10 points\t$3800\n\
							Grenade\t-10 points\t$1000\n\
							Cocktail Molotov\t-4 points\t$300\n\
							Viseur optique\t-2 points\t$1700\n\
							Silencieux\t-2 points\t$500\n\
							Crochet de serrure\t-1 point\t$300";
			
			Dialog_Show(playerid, BuyGunLaw, DIALOG_STYLE_TABLIST, title, mes, "Acheter", "Retour");
		}
		else
		{
		    Dialog_Show(playerid, BuyGunLaw, DIALOG_STYLE_TABLIST, title, "Desert Eagle\t-3 points\t$1800\n\
																			COLT45\t-3 points\t$1000\n\
																			Shotgun\t-4 points\t$2400\n\
																			Sawn-off Shotgun\t-4 points\t$2100\n\
																			UZI\t-5 points\t$2000\n\
																			Tec-9\t-5 points\t$2500\n\
																			Cocktail Molotov\t-4 points\t$300\n\
																			Crochet de serrure\t-1 point\t$300", "Acheter", "Retour");
		}
	}
	else if (listitem == 1)
	{
	    Dialog_Show(playerid, BuyAmmoLaw, DIALOG_STYLE_TABLIST, title, "100 munitions\t-1 point\t$400\n200 munitions\t-2 points\t$800\n300 munitions\t-3 points\t$1200\n400 munitions\t-4 points\t$1600\n500 munitions\t-5 points\t$2000\n600 munitions\t-6 points\t$2400", "Acheter", "Retour");
	}
	return 1;
}

Dialog:TakeJobMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	switch(listitem) {
		case 0: {
			new i = Job_Nearest(playerid);
			if (i == -1) return 1;

			if (JobsInfo[i][jobType] == JOB_PIZZA || JobsInfo[i][jobType] == JOB_GUNDEALER || JobsInfo[i][jobType] == JOB_DRUGDEALER)
			{
				if (PlayerInfo[playerid][pSideJob] != JOBLESS) return SendSyntaxMessage(playerid, "Vous avez déjà un second emploi. Utilisez /quitsidejob pour quitter votre travail secondaire.");
				
				PlayerInfo[playerid][pSideJob] = JobsInfo[i][jobType];
				SQL_SetInteger("users", "sidejob", JobsInfo[i][jobType], PlayerInfo[playerid][pID]);
				
				if (JobsInfo[i][jobType] == JOB_PIZZA) SendSyntaxMessage(playerid, "Vous avez rejoint le métier de livreur de pizzas. Livrez les commandes via /pizzajob.");
				if (JobsInfo[i][jobType] == JOB_GUNDEALER) SendSyntaxMessage(playerid, "Vous êtes devenu trafiquant d'armes. Achetez via /buygun.");
				if (JobsInfo[i][jobType] == JOB_DRUGDEALER) SendSyntaxMessage(playerid, "Vous êtes devenu trafiquant de drogue. Achetez via /buydrugs.");
				return 1;
			}
				
			if (PlayerInfo[playerid][pJob] != JOBLESS) return SendErrorMessage(playerid, "Vous avez déjà un emploi. Utilisez /quitjob pour le quitter.");
			if (JobsInfo[i][jobType] == JOB_TAXI && !PlayerInfo[playerid][pTaxiLic]) return SendErrorMessage(playerid, "Vous n'avez pas de licence de taxi. Rendez-vous à l'auto-école pour l'obtenir.");

			PlayerInfo[playerid][pJob] = JobsInfo[i][jobType];
			PlayerInfo[playerid][pCareer] = 0;

			SQL_SetInteger("users", "job", JobsInfo[i][jobType], PlayerInfo[playerid][pID]);

			SendSyntaxMessage(playerid, "Vous avez pris le poste de '%s'. Tapez /jobhelp pour voir les commandes.", jobs_names[JobsInfo[i][jobType]]);
			return 1;
		}
		case 1: {
			new i = Job_Nearest(playerid);
			if (i == -1) return 1;

			new mes[512];
			format(mes, sizeof(mes), "/rentcarjob(/rcj) - Prendre un véhicule de travail\n/unrentcarjob(/unrcj) - Rendre un véhicule de travail\n");

			if (JobsInfo[i][jobType] == JOB_PIZZA) {
				format(mes, sizeof(mes), "%s/pizzajob - Livrer des pizzas\n/takepizza - Prendre une pizza\n/quitsidejob - Quitter l'emploi secondaire", mes);
			} else if (JobsInfo[i][jobType] == JOB_CARJACKER) {
				format(mes, sizeof(mes), "/delivercar - Démontage d'un véhicule volé\n/dropoff - Déposer un véhicule\n/leavemission - Annuler la mission\n\n/vbreaklock (n)oisy - Effraction bruyante\n/vbreaklock (q)uite - Effraction discrète");
			} else if (JobsInfo[i][jobType] == JOB_TAXI) {
				format(mes, sizeof(mes), "%s/taxi duty - Démarrer/Terminer un service\n/taxi fare - Changer le tarif\n/taxi accept - Accepter un appel", mes);
			} else if (JobsInfo[i][jobType] == JOB_MECHANIC) {
				format(mes, sizeof(mes), "%s/refill - Faire le plein d'un véhicule\n/repair - Réparer un véhicule\n/paintcar - Repeindre un véhicule\n\n/attach - Remorquer un véhicule\n/buycomp - Acheter des composants\n/mycomp - Voir les composants du véhicule", mes);
			} else if (JobsInfo[i][jobType] == JOB_TRUCKER) {
				format(mes, sizeof(mes), "%s/tpda - Menu de camionneur\n/industry - Informations sur l'industrie\n/cargo - Gestion de la cargaison\n/trailer - Gestion de la remorque\n", mes);
				switch(PlayerInfo[playerid][pCareer]) {
					case 0..11: strcat(mes, "Véhicules accessibles : Picador, Sadler, Bobcat, Walton, Yosemite.");
					case 12..23: strcat(mes, "Véhicules accessibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito.");
					case 24..31: strcat(mes, "Véhicules accessibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.");
					case 32..47:
					{
						strcat(mes, "Véhicules accessibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\nBenson, Mule, DFT-30, Tanker, Securicar.");
					}
					case 48..58:
					{
						strcat(mes, "Véhicules accessibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\nBenson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed.");
					}
					default:
					{
						strcat(mes, "Véhicules accessibles : Picador, Sadler, Bobcat, Walton, Yosemite, Pony, Berkley's RC Van, Burrito, Rumpo, Boxville.\nBenson, Mule, DFT-30, Tanker, Securicar, Yankee, Flatbed, Remorques.");
					}
				}
			} else if (JobsInfo[i][jobType] == JOB_TRASHMASTER) {
				format(mes, sizeof(mes), "%s\nVous devez collecter les déchets dans les containers et les déposer à la déchetterie.", mes);
			} else return cmd::jobhelp(playerid, "");

			Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{FFFFFF}Commandes du métier", mes, "Retour", "Fermer");
			return 1;
		}
		case 2: cmd::takejob(playerid, "");
		case 3: cmd::quitjob(playerid, "");
	}

	return 1;
}