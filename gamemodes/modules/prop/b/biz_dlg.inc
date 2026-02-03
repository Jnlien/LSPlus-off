Dialog:BuyGunMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid = -1;

	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO) return 1;

	if (listitem == 0) { // Armes
		static const str[] = "Armes\tPrix\n\
							M1911\t{33CC33}$2000\n\
							Desert Eagle\t{33CC33}$3900\n\
							Shotgun\t{33CC33}$4700\n\
							Uzi\t{33CC33}$4000\n\
							MP5\t{33CC33}$4000\n\
							Tec-9\t{33CC33}$4000\n\
							Country Rifle\t{33CC33}$5800\n\
							Couteau\t{33CC33}$200\n\
							Katana\t{33CC33}$1000";

		new dlg[sizeof(str) + 12] = "";
		format(dlg, sizeof(dlg), str);

		Dialog_Show(playerid, BuyGun, DIALOG_STYLE_TABLIST_HEADERS, "Choix d'armes", dlg, "Acheter", "Retour");

	} else if (listitem == 1) { // Munitions
		static const str[] = "Armes\tType de chargeur\tQuantité\tPrix\n\
							M1911\t.45 ACP\t7 pts\t{33CC33}$35\n\
							{000000}.	\t.45 ACP .ext\t14 pts\t{33CC33}$80\n\
							Desert Eagle\t.50AE\t7 pts\t{33CC33}$60\n\
							{000000}.	\t.50AE .ext\t14 pts\t{33CC33}$130\n\
							Shotgun\t12-G BIRDSHOT\t6 pts\t{33CC33}$300\n\
							{000000}.	\t12-G BUCKSHOT\t6 pts\t{33CC33}$200\n\
							{000000}.	\t12-G SLUG\t6 pts\t{33CC33}$150\n\
							Tec-9 / MP5 / Uzi\t9x19mm\t30 pts\t{33CC33}$200\n\
							{000000}.	\t9x19mm .ext\t60 pts\t{33CC33}$450\n\
							Country Rifle\t7.62x51mm\t4 pts\t{33CC33}$400";

		new dlg[sizeof(str)];
		format(dlg, sizeof(dlg), str);

		Dialog_Show(playerid, BuyAmmo, DIALOG_STYLE_TABLIST_HEADERS, "Choix de chargeur", dlg, "Acheter", "Retour");

	} else if (listitem == 2) { // Supplémentaire
		static const str[] = "Produit\tPrix\n\
							Lunette de visée\t{33CC33}$2700\n\
							Silencieux\t{33CC33}$1000\n\
							Gilet pare-balles léger {c7c7c7}(60%)\t{33CC33}$1000\n\
							Gilet pare-balles standard {c7c7c7}(80%)\t{33CC33}$1500\n\
							Gilet pare-balles lourd {c7c7c7}(100%)\t{33CC33}$2000";

		new dlg[sizeof(str) + 12];
		format(dlg, sizeof(dlg), str);

		Dialog_Show(playerid, BuyAdditional, DIALOG_STYLE_TABLIST_HEADERS, "Supplémentaire", dlg, "Acheter", "Retour");

	} else {
		cmd::ammohelp(playerid, "");
	}

	return 1;
}

Dialog:BuyGun(playerid, response, listitem, inputtext[])
{
	if (!response)	return BuyGunMenu(playerid);

	new bizid = -1;

	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO)	return 1;
	if (!CheckInv(playerid)) return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire."); 
			
	new gun_list[] = {WEAPON_COLT45, WEAPON_DEAGLE, WEAPON_SHOTGUN, WEAPON_UZI, WEAPON_MP5, WEAPON_TEC9, WEAPON_RIFLE, WEAPON_KNIFE, WEAPON_KATANA},
		prc_list[] = {2000, 3900, 4700, 4000, 4000, 4000, 5800, 200, 1000};

	if (GetCash(playerid) < prc_list[listitem])							return SendErrorMessage(playerid, "Vous avez besoin de $%i pour cet achat.", prc_list[listitem]);
	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	return SendErrorMessage(playerid, "Il n'y a pas assez d'armes en stock pour la vente.");
		
	switch(listitem) {
		case 0..6: {
			if (PlayerInfo[playerid][pWepLic] != 1)	 return SendErrorMessage(playerid, "Vous n'avez pas de licence pour les armes.");
			if (PlayerInfo[playerid][pWepCount] != 0) return SendErrorMessage(playerid, "Vous pourrez acheter une autre arme dans %i pay-checks.", PlayerInfo[playerid][pWepCount]);
			
			PlayerInfo[playerid][pWepCount] = 6;
			SQL_SetInteger("users", "wep_count", 6, PlayerInfo[playerid][pID]);
			SendHelpMessage(playerid, "Vous pourrez acheter une autre arme dans %i pay-checks.", PlayerInfo[playerid][pWepCount]);
		}
	}

	new money = GetCash(playerid),
		weapon = gun_list[listitem],
		ammotype = GetDefaultAmmoType(weapon),
		ammo = CheckAmmo(ammotype);
		
	if (BizInfo[bizid][bizOwnerID] != 0)
	{
		BizInfo[bizid][bizProduct]--;
		GiveBizCash(bizid, prc_list[listitem] / 100 * 80);
	}

	GiveCash(playerid, -prc_list[listitem]);
	GiveInvItem(playerid, weapon, ammo, ammotype, PlayerInfo[playerid][pPlayerSerial]);

	SendSyntaxMessage(playerid, "Vous avez acheté %s pour $%i.", inventory_items[weapon][itemName], prc_list[listitem]);

	new query[128];
	format(query, sizeof(query), "%s [MYSQL: %i | IP: %s | Argent(AVANT): %i | Argent(APRÈS): %i] a acheté (%s) pour -- $%i", 
	GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), inventory_items[weapon][itemName], prc_list[listitem]);
	insert_log(query, WEAPON_LOG, 1, PlayerInfo[playerid][pAccID]);

	BuyGunMenu(playerid);
	return 1;
}

Dialog:BuyAmmo(playerid, response, listitem, inputtext[])
{
	if (!response) return BuyGunMenu(playerid);

	new bizid = -1;
	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO) return 1;
	
	new liste_armes[][20] = {"M1911", "M1911", "Desert Eagle", "Desert Eagle", "Shotgun", "Shotgun", "Shotgun", "Tec-9 / MP5 / Uzi", "Tec-9 / MP5 / Uzi", "Country Rifle"},
		liste_munitions[] = {82, 83, 84, 85, 86, 87, 88, 90, 91, 96},
		prix_munitions[] = {35, 80, 60, 130, 300, 200, 100, 200, 450, 400};

	if (!CheckInv(playerid)) return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire."); 
	if (GetCash(playerid) < prix_munitions[listitem]) return SendErrorMessage(playerid, "Vous avez besoin de $%i pour cet achat.", prix_munitions[listitem]);
	if (!PlayerInfo[playerid][pWepLic]) return SendErrorMessage(playerid, "Vous n'avez pas de licence pour les armes.");
	
	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	
		return SendErrorMessage(playerid, "Il n'y a pas assez de munitions en stock pour la vente.");	

	new argent = GetCash(playerid);
	if (BizInfo[bizid][bizOwnerID] != 0)
	{
		BizInfo[bizid][bizProduct]--;
		GiveBizCash(bizid, prix_munitions[listitem]);
	}

	GiveCash(playerid, -prix_munitions[listitem]);
	GiveInvItem(playerid, liste_munitions[listitem], CheckAmmo(liste_munitions[listitem])); 

	SendSyntaxMessage(playerid, "Vous avez acheté un chargeur %s [%i pts] pour %s pour $%i.", inventory_items[liste_munitions[listitem]][itemName], CheckAmmo(liste_munitions[listitem]), liste_armes[listitem], prix_munitions[listitem]);

	new requete[128];
	format(requete, sizeof(requete), "%s [MYSQL: %i | IP: %s | Argent(AVANT): %i | Argent(APRÈS): %i] a acheté (Munitions:%s) pour -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), argent, GetCash(playerid), liste_munitions[listitem], prix_munitions[listitem]);
	insert_log(requete, WEAPON_LOG, 2, PlayerInfo[playerid][pAccID]);

	BuyGunMenu(playerid);
	return 1;
}

Dialog:BuyAdditional(playerid, response, listitem, inputtext[])
{
	if (!response) return BuyGunMenu(playerid);

	new bizid = -1;
	if ((bizid = GetNearInsideBiz(playerid)) == -1 || BizInfo[bizid][bizType] != BIZ_AMMO) return 1;

	new add_name[][32] = {"Lunette de visée", "Silencieux", "Gilet pare-balles léger", "Gilet pare-balles standard", "Gilet pare-balles lourd"},
		add_list[] = {INV_OPTICS, INV_SILENCER, 0, 0, 0},
		add_pr[] = {2700, 1000, 1000, 1500, 2000};

	if (GetCash(playerid) < add_pr[listitem]) return SendErrorMessage(playerid, "Vous avez besoin de $%i pour cet achat.", add_pr[listitem]);
	if (add_list[listitem] && !CheckInv(playerid)) return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire."); 

	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	
		return SendErrorMessage(playerid, "Il n'y a pas assez de produits en stock pour la vente.");	
	
	new money = GetCash(playerid);
	if (BizInfo[bizid][bizOwnerID] != 0)
	{
		BizInfo[bizid][bizProduct]--;
		GiveBizCash(bizid, add_pr[listitem] / 100 * 90);
	}

	GiveCash(playerid, -add_pr[listitem]);

	if (add_list[listitem] != 0) 
		GiveInvItem(playerid, add_list[listitem], 1); 
	else 
	{
		new Float:armour;
		if (listitem == 2) armour = 60.0;
		else if (listitem == 3) armour = 80.0;
		else if (listitem == 4) armour = 100.0;
		
		SetPlayerArmour(playerid, armour);
	}

	SendSyntaxMessage(playerid, "Vous avez acheté %s pour $%i.", add_name[listitem], add_pr[listitem]);

	new query[128];
	format(query, sizeof(query), "%s [MYSQL: %i | IP: %s | Argent(AVANT): %i | Argent(APRÈS): %i] a acheté %s pour -- $%i", GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), add_name[listitem], add_pr[listitem]);
	insert_log(query, WEAPON_LOG, 2, PlayerInfo[playerid][pAccID]);

	BuyGunMenu(playerid);
	return 1;
}

Dialog:BusinessMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	switch (listitem)
	{
		case 0:
		{
			new diatxt[300];
			format(diatxt, sizeof(diatxt), "{FFFFFF}ID:\t\t\t%i\nNom:\t\t %s\nType:\t\t %s\nÉtat:\t\t %s\n", BizInfo[bizid][bizID], BizInfo[bizid][bizName], biz_types[BizInfo[bizid][bizType]], (BizInfo[bizid][bizLocked]) ? ("Fermé") : ("Ouvert"));
			format(diatxt, sizeof(diatxt), "%sArgent en caisse:\t\t $%i\nPrix d'entrée:\t $%i\n\nStock: %i / 400\n", diatxt, BizInfo[bizid][bizCash], BizInfo[bizid][bizEntrance], BizInfo[bizid][bizProduct]);

			new id = biz_prod_types[BizInfo[bizid][bizType]];

			format(diatxt, sizeof(diatxt), "%sCommande: %s à $%i / unité", diatxt, truckcargo_product[id], BizInfo[bizid][bizBuyPrice]);

			Dialog_Show(playerid, BusinessMenuInfo, DIALOG_STYLE_MSGBOX, "Informations sur le business", diatxt, "Retour", "");
			return 1;
		}
		case 1:
		{
			static dlgtxt[] = "Nom\tPoste\n\
					{FF6347}>{FFFFFF} RECRUTER UN NOUVEAU EMPLOYÉ\n";

			new diatxt[sizeof(dlgtxt) + 10 * (24 + 32)], count;

			format(diatxt, sizeof(diatxt), "%s", dlgtxt);
			for (new i = 0; i < MAX_WORK_SPACE; i++)
			{
				if (!BizWorkers[bizid][i][bizJobOn]) continue;

				count++;
				format(diatxt, sizeof(diatxt), "%s%i. %s\t%s\n", diatxt, i + 1, BizWorkers[bizid][i][bizWorker], BizWorkers[bizid][i][bizPosition]);
			}

			if (!count) format(diatxt, sizeof(diatxt), "%s- Vous n'avez encore recruté personne.", dlgtxt);

			Dialog_Show(playerid, WorkerInfo, DIALOG_STYLE_TABLIST_HEADERS, "Informations sur les employés", diatxt, "Choisir", "Retour");
			return 1;
		}
		case 2:
		{
			static dlgtxt[] = "En caisse: $%i\n\
								{FF6347}>{FFFFFF} Déposer\n\
								{FF6347}>{FFFFFF} Retirer";

			new diatxt[sizeof(dlgtxt) + 11];
			format(diatxt, sizeof(diatxt), dlgtxt, BizInfo[bizid][bizCash]);

			Dialog_Show(playerid, TillOperations, DIALOG_STYLE_TABLIST, "Caisse", diatxt, "Choisir", "Retour");
			return 1;
		}
		case 3:
		{
			new diatxt[128];

			if (BizInfo[bizid][bizType] == BIZ_GAS_STATION) format(diatxt, sizeof(diatxt), "{FFFFFF}Prix par litre: $%i", BizInfo[bizid][bizEntrance]);
			else format(diatxt, sizeof(diatxt), "{FFFFFF}Prix d'entrée: $%i", BizInfo[bizid][bizEntrance]);

			new biz = BizInfo[bizid][bizType];
			if (biz == BIZ_STORE || biz == BIZ_BAR || biz == BIZ_PHARMACY || biz == BIZ_PIZZA || biz == BIZ_CLUCKIN || biz == BIZ_BURGER || biz == BIZ_DONUT)
				format(diatxt, sizeof(diatxt), "%s\n> Modifier les prix des produits", diatxt);

			Dialog_Show(playerid, ChooseConfigPrices, DIALOG_STYLE_TABLIST, "Configuration des prix", diatxt, "Choisir", "Retour");
			return 1;
		}
		case 4:
		{
			Dialog_Show(playerid, BizUpgrade, DIALOG_STYLE_TABLIST, "Améliorations du business", "Acheter une radio {33CC33}[$100]{FFFFFF}\nRéparer la porte {33CC33}[$1500]{FFFFFF}", "Choisir", "Retour");
			return 1;
		}
		case 5: cmd::furniture(playerid, "");
		case 6:
		{
			static dlgtxt[] = "Vous achetez des produits (%s) à $%i par unité.\nEntrez le nouveau prix d'achat des produits:";

			new diatxt[sizeof(dlgtxt) + 24 + 3], id = biz_prod_types[BizInfo[bizid][bizType]];

			format(diatxt, sizeof(diatxt), dlgtxt, truckcargo_product[id], BizInfo[bizid][bizBuyPrice]);
			Dialog_Show(playerid, ProductBuying, DIALOG_STYLE_INPUT, "Achat de produits", diatxt, "Choisir", "Retour");
			return 1;
		}
		case 7:
		{
			static dlgtxt[] = "Nom de votre business: %s\nEntrez un nouveau nom:";

			new diatxt[sizeof(dlgtxt) + 24];

			format(diatxt, sizeof(diatxt), dlgtxt, BizInfo[bizid][bizName]);
			Dialog_Show(playerid, ChangeBizName, DIALOG_STYLE_INPUT, "Modification du nom", diatxt, "Choisir", "Retour");
			return 1;
		}
		case 8:
		{
			Dialog_Show(playerid, DoorName, DIALOG_STYLE_LIST, "Modification du texte d'entrée", "Modifier\nSupprimer", "Choisir", "Retour");
			return 1;
		}
		case 9:
		{
			Dialog_Show(playerid, SellBiz, DIALOG_STYLE_LIST, "Vente du business", "Vendre à un joueur\nVendre à l'État", "Choisir", "Retour");
			return 1;
		}
	}

	return 1;
}

Dialog:SellBiz(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);

	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");

	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	switch (listitem)
	{
		case 0:
		{
			Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente du business", "{ffffff}Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		}
		case 1:
		{
			static const str_size[] = "{FFFFFF}Voulez-vous vendre le business pour $%i ?";
			new mes[sizeof(str_size) - 2 + 12];

			format(mes, sizeof(mes), str_size, BizInfo[bizid][bizPrice] / 2);
			Dialog_Show(playerid, SellBizState, DIALOG_STYLE_MSGBOX, "{FFFFFF}Vente du business", mes, "Vendre", "Annuler");
		}
	}
	return 1;
}

Dialog:SellBizTo(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (bizid < 0 || bizid >= MAX_BIZ || BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return 1;

	new userid, price;

	if (sscanf(inputtext, "ui", userid, price))
	{
		Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente du business", "{ffffff}Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendErrorMessage(playerid, "Format incorrect.");
	}

	if (!IsPlayerLogged(userid))
	{
		Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente du business", "{ffffff}Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendNotConnected(playerid);
	}

	if (!PlayerNearPlayer(3.0, playerid, userid))
	{
		Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente du business", "{ffffff}Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendNotNearPlayer(playerid);
	}

	if (price < 0 || price > 100000000)
	{
		Dialog_Show(playerid, SellBizTo, DIALOG_STYLE_INPUT, "{FFFFFF}Vente du business", "{ffffff}Entrez {C3C3C3}[ID/nom du joueur] [prix de vente]", "Vendre", "Annuler");
		return SendErrorMessage(playerid, "Le prix de vente doit être compris entre $0 et $100000000.");
	}

	pTemp[userid][pBizPrice] = price;
	pTemp[userid][pBizPlayer] = playerid;
	pTemp[userid][pBizID] = bizid;

	SetPVarInt(userid, #accept_time, gettime() + 30);
	SendSyntaxMessage(playerid, "Vous avez proposé à %s d'acheter votre business pour $%i.", GetNameEx(userid), price);
	SendSyntaxMessage(userid, "%s souhaite vous vendre un business pour $%i. Appuyez sur Y pour accepter.", GetNameEx(playerid), price);
	return 1;
}

Dialog:SellBizState(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	PlaySound(playerid, 1052);

	new biz_cost = BizInfo[bizid][bizPrice] / 2, money = PlayerInfo[playerid][pCash], cash;

	GiveCash(playerid, biz_cost);
	SendSyntaxMessage(playerid, "Vous avez vendu avec succès votre business \"%s\" pour $%i.", BizInfo[bizid][bizName], biz_cost);

	if (BizInfo[bizid][bizCash] > 0)
	{
		SendSyntaxMessage(playerid, "Après la vente du business, $%i restaient dans la caisse, qui ont été retirés avec succès.", BizInfo[bizid][bizCash]);
		GiveCash(playerid, BizInfo[bizid][bizCash]);
		cash = BizInfo[bizid][bizCash];
	}

	new mes[256];
	format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent(AVANT): %i | Argent(APRÈS): %i] a vendu le business [B_MYSQL: %i] pour -- $%i + $%i",
		   GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), money, GetCash(playerid), BizInfo[bizid][bizID], biz_cost, cash);
	insert_log(mes, PROPERTY_LOG, 2, PlayerInfo[playerid][pAccID]);

	RemoveAllWorkers(bizid);
	Sell_Business(bizid);

	return 1;
}

Dialog:BizUpgrade(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (listitem == 0)
	{
		if (BizInfo[bizid][bizRadio] == 1) 
			return SendErrorMessage(playerid, "Ce business dispose déjà d'une radio.");
		if (GetCash(playerid) < UPGRADE_PRICE_RADIO) 
			return SendErrorMessage(playerid, "Le prix de la radio est de $%i.", UPGRADE_PRICE_RADIO);

		BizInfo[bizid][bizRadio] = 1;
		SQL_SetInteger("business", "radio", BizInfo[bizid][bizRadio], BizInfo[bizid][bizID]);
		GiveCash(playerid, -UPGRADE_PRICE_RADIO);
		SendSyntaxMessage(playerid, "Vous avez acheté une radio pour votre business.");
		SendHelpMessage(playerid, "Utilisez /setstation pour configurer la radio.");
	}
	else
	{
		if (BizInfo[bizid][bizDDoor] == 0) 
			return SendErrorMessage(playerid, "La porte n'a pas besoin de réparation.");
		if (GetCash(playerid) < UPGRADE_PRICE_DOOR) 
			return SendErrorMessage(playerid, "Le prix de la réparation de la porte est de $%i.", UPGRADE_PRICE_DOOR);

		BizInfo[bizid][bizDDoor] = 0;
		SQL_SetInteger("business", "ddoor", BizInfo[bizid][bizDDoor], BizInfo[bizid][bizID]);
		UpdateBusinessDoor(bizid);
		GiveCash(playerid, -UPGRADE_PRICE_DOOR);
		SendSyntaxMessage(playerid, "Vous avez réparé la porte pour $%i.", UPGRADE_PRICE_DOOR);
	}
	return 1;
}

Dialog:ProductBuying(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	new price = strval(inputtext),
		id = biz_prod_types[BizInfo[bizid][bizType]];

	if (id == -1) return SendErrorMessage(playerid, "Votre business n'a pas besoin de produits.");
	if (price < 50 || price > 1000) return SendErrorMessage(playerid, "Le prix d'achat doit être compris entre $50 et $1000.");

	BizInfo[bizid][bizBuyPrice] = price;
	SQL_SetInteger("business", "buy_price", price, BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Vous achetez désormais %s à $%i par unité.", truckcargo_product[id], price);
	return 1;
}

Dialog:ChangeBizName(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	new name[32];

	if (sscanf(inputtext, "s[32]", name)) return 1;
	if (strlen(name) > 30) return SendErrorMessage(playerid, "Le nom du business ne peut contenir plus de 30 caractères.");
	if (strfind(name, "~n~", true) != -1) return SendErrorMessage(playerid, "Le nom du business ne peut pas contenir de sauts de ligne.");
	if (strfind(name, "\n", true) != -1) return SendErrorMessage(playerid, "Le nom du business ne peut pas contenir de sauts de ligne.");
	if (strfind(name, "'", true) != -1) return SendErrorMessage(playerid, "Le nom du business ne peut pas contenir le caractère '.");
	if (CheckTextDrawString(name)) return SendErrorMessage(playerid, "Le nom du business ne doit pas contenir le caractère '~'.");

	format(BizInfo[bizid][bizName], 32, "%s", name);
	SQL_SetString("business", "name", BizInfo[bizid][bizName], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Vous avez défini le nom du business : %s", BizInfo[bizid][bizName]);

	return 1;
}

Dialog:TillOperations(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");
	if (!listitem) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (listitem == 1)
	{
		Dialog_Show(playerid, TillDeposit, DIALOG_STYLE_INPUT, "Déposer de l'argent dans la caisse", 
			"Entrez le montant d'argent que vous souhaitez déposer dans la caisse :\n\t\t(jusqu'à $25.000.000)", 
			"Choisir", "Annuler");
	}
	
	if (listitem == 2)
	{
		static dlgtxt[] = "Entrez le montant d'argent que vous souhaitez retirer de la caisse :\n\t\t(jusqu'à $%s)";

		new diatxt[sizeof(dlgtxt) + 10];

		format(diatxt, sizeof(diatxt), dlgtxt, FormatNumber(BizInfo[bizid][bizCash]));
		Dialog_Show(playerid, TillWithDraw, DIALOG_STYLE_INPUT, "Retirer de l'argent de la caisse", diatxt, "Choisir", "Annuler");
	}
	
	return 1;
}

Dialog:TillWithDraw(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	new amount = strval(inputtext);

	if (BizInfo[bizid][bizCash] < amount) 
		return SendErrorMessage(playerid, "Il n'y a pas assez d'argent dans la caisse.");
	if (amount < 1 || amount > 25000000) 
		return SendErrorMessage(playerid, "Vous pouvez retirer entre $1 et $25.000.000.");

	GiveCash(playerid, amount);
	GiveBizCash(playerid, -amount);
	SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Vous avez retiré $%i de la caisse.", amount);

	return 1;
}

Dialog:TillDeposit(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	new montant = strval(inputtext);

	if (GetCash(playerid) < montant || montant < 1) 
		return SendNotCash(playerid);
	if (BizInfo[bizid][bizCash] + montant > 25000000) 
		return SendErrorMessage(playerid, "La caisse ne peut contenir plus de $25.000.000.");

	GiveCash(playerid, -montant);
	GiveBizCash(bizid, montant);
	SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Vous avez déposé $%i dans la caisse.", montant);

	return 1;
}

Dialog:DoorName(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (listitem == 0)
	{
		static dlgtxt[] = "%s\n\
						{FFFFFF}Vous pouvez utiliser des couleurs HTML dans le texte. Exemple de couleur : ~FFFF00~ - jaune.\n\
						Exemple de texte : \"~FFFF00~Bonjour\" affichera ce texte - \"{FFFF00}Bonjour{FFFFFF}\" \n\n\
						Entrez le texte ci-dessous :";

		new diatxt[sizeof(dlgtxt) + 24];

		format(diatxt, sizeof(diatxt), dlgtxt, BizInfo[bizid][bizMes]);
		Dialog_Show(playerid, ChangeDoorName, DIALOG_STYLE_INPUT, "Modification du texte d'entrée", diatxt, "Choisir", "Annuler");
	}
	else 
	{
		format(BizInfo[bizid][bizMes], 128, "!");
		SQL_SetString("business", "entermsg", "!", BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Le texte d'entrée ne sera plus affiché.");
	}

	return 1;
}

Dialog:ChangeDoorName(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (strlen(inputtext) < 2 || strlen(inputtext) > 127) 
		return SendErrorMessage(playerid, "Le texte du business doit contenir entre 2 et 127 caractères.");
	if (strfind(inputtext, "\n", true) != -1) 
		return SendErrorMessage(playerid, "Le texte du business ne peut pas contenir de sauts de ligne.");

	format(BizInfo[bizid][bizMes], 128, "%s", inputtext);
	place_color(BizInfo[bizid][bizMes]);
	SQL_SetString("business", "entermsg", BizInfo[bizid][bizMes], BizInfo[bizid][bizID]);
	SendSyntaxMessage(playerid, "Vous avez défini le texte d'entrée du business : %s", BizInfo[bizid][bizMes]);

	return 1;
}

Dialog:WorkerInfo(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (listitem == 0) 
		return Dialog_Show(playerid, WorkerInvite, DIALOG_STYLE_INPUT, "Recruter un employé", "{FFFFFF}Entrez l'ID/nom du joueur que vous souhaitez recruter :", "Suivant", "Retour");

	listitem -= 1;
	SetPVarInt(playerid, #WorkerID, listitem);

	static dlgtxt[] = "{FFFFFF}NOM:\t\t %s\n\
					POSTE:\t %s\n\
					{FF6347}>{FFFFFF} Modifier le poste\n\
					{FF6347}>{FFFFFF} Licencier";

	new diatxt[sizeof(dlgtxt) + 24 + 32];

	format(diatxt, sizeof(diatxt), dlgtxt, BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);

	Dialog_Show(playerid, WorkerEdit, DIALOG_STYLE_TABLIST_HEADERS, "Informations sur l'employé", diatxt, "Choisir", "Retour");
	return 1;
}

Dialog:WorkerEdit(playerid, response, listitem, inputtext[])
{
	if (!response) return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (listitem == 1)
	{
		return Dialog_Show(playerid, WorkerEditPos, DIALOG_STYLE_INPUT, "Changer le poste", "{FFFFFF}Entrez le nouveau poste de l'employé :", "Choisir", "Annuler");
	}

	if (listitem == 2)
	{
		listitem = GetPVarInt(playerid, #WorkerID);
		static dlgtxt[] = "{FFFFFF}Voulez-vous vraiment licencier %s (%s) ?";
		new diatxt[sizeof(dlgtxt) + 24 + 32];

		format(diatxt, sizeof(diatxt), dlgtxt, BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);
		Dialog_Show(playerid, WorkerFire, DIALOG_STYLE_INPUT, "Licenciement de l'employé", diatxt, "Oui", "Annuler");
	}

	return 1;
}

Dialog:WorkerInvite(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid, userid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (WorkSpace(bizid) == -1) return SendErrorMessage(playerid, "Vous n'avez pas de postes de travail disponibles.");

	if (sscanf(inputtext, "u", userid)) return 1;
	if (userid == playerid) return SendErrorMessage(playerid, "Vous ne pouvez pas vous embaucher vous-même.");
	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (!PlayerNearPlayer(3.0, playerid, userid)) return SendNotNearPlayer(playerid);
	if (pTemp[userid][pHirePlayer] == playerid) return SendErrorMessage(playerid, "Vous avez déjà proposé un emploi à ce joueur.");

	pTemp[userid][pHirePlayer] = playerid;
	SetPVarInt(userid, #accept_time, gettime() + 30);

	SendSyntaxMessage(userid, "%s vous a invité à travailler dans son business.", GetNameWithMask(playerid));
	SendHelpMessage(userid, "Appuyez sur Y pour accepter l'offre.");
	SendSyntaxMessage(playerid, "Vous avez invité %s à travailler dans votre business.", GetNameWithMask(userid));

	return 1;
}

Dialog:WorkerEditPos(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	
	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	listitem = GetPVarInt(playerid, #WorkerID);

	format(BizWorkers[bizid][listitem][bizPosition], 24, "%s", inputtext);

	SQL_SetString("users", "position", BizWorkers[bizid][listitem][bizPosition], BizWorkers[bizid][listitem][bizJobOn]);

	SendSyntaxMessage(playerid, "Vous avez changé le poste de %s en %s.", BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);

	for (new i; i < GetPlayerPoolSize(); i++)
	{
		if (PlayerInfo[i][pID] == BizWorkers[bizid][listitem][bizJobOn])
		{
			SendSyntaxMessage(i, "%s a changé votre poste en %s.", GetNameEx(playerid), BizWorkers[bizid][listitem][bizPosition]);
			break;
		}
	}
	
	return 1;
}

Dialog:WorkerFire(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);

	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	listitem = GetPVarInt(playerid, #WorkerID);

	SQL_SetString("users", "position", "-", BizWorkers[bizid][listitem][bizJobOn]);
	SQL_SetInteger("users", "workon", 0, BizWorkers[bizid][listitem][bizJobOn]);

	for (new i; i < GetPlayerPoolSize(); i++)
	{
		if (PlayerInfo[i][pID] == BizWorkers[bizid][listitem][bizJobOn])
		{
			SendSyntaxMessage(i, "%s vous a licencié de son business.", GetNameEx(playerid));
			PlayerInfo[i][pWorkOn] = 0;
			break;
		}
	}

	SendSyntaxMessage(playerid, "Vous avez licencié %s (%s) de votre business.", BizWorkers[bizid][listitem][bizWorker], BizWorkers[bizid][listitem][bizPosition]);

	BizWorkers[bizid][listitem][bizWorker] = EOS;
	BizWorkers[bizid][listitem][bizPosition] = EOS;
	BizWorkers[bizid][listitem][bizJobOn] = 0;

	return 1;
}

Dialog:BusinessMenuInfo(playerid, response, listitem, inputtext[])
{
    if (!response)	return cmd::bmenu(playerid, "");
	return 	cmd::bmenu(playerid, "");
}

Dialog:ChooseConfigPrices(playerid, response, listitem, inputtext[])
{
	if (!response)	return cmd::bmenu(playerid, "");

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	if (listitem == 0)
	{
		Dialog_Show(playerid, ConfigEntranceFee, DIALOG_STYLE_INPUT, "Configuration des prix", "Entrez le nouveau tarif :", "Choisir", "Annuler");
	}
	else 
	{
		new biz = BizInfo[bizid][bizType];
		if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT) 
			return SendErrorMessage(playerid, "Vous ne pouvez pas modifier les prix dans ce type de business.");
		
		new type;
		switch (biz)
		{
			case BIZ_PIZZA:		type = 0;
			case BIZ_CLUCKIN:	type = 1;
			case BIZ_BURGER:	type = 2;
			case BIZ_DONUT:		type = 3;
			case BIZ_STORE:		type = 4;
			case BIZ_BAR:		type = 5;
			case BIZ_PHARMACY:	type = 6;
		}
		
		new mes[LIMIT_MAX_PRODUCTS * 128];

		for (new i = 0; i < max_product[type]; i++)
		{
			format(mes, sizeof(mes), "%s{FFFFFF}Modifier le prix du produit #%i {AFAFAF}[%s, $%i]\n", mes, i + 1, name_order[type][i], BizInfo[bizid][bizProductPrice][i]);
		}

		Dialog_Show(playerid, ConfigPrices, DIALOG_STYLE_LIST, "Configuration des prix", mes, "Choisir", "Retour");
		return 1;
	}

	return 1;
}
Dialog:ConfigEntranceFee(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucun business à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	new fee = strval(inputtext);

	if (fee < 0 || fee > 100) 
	{
		Dialog_Show(playerid, ConfigEntranceFee, DIALOG_STYLE_INPUT, "Configuration des prix", "Entrez le nouveau tarif :", "Choisir", "Annuler");
		return SendErrorMessage(playerid, "Le tarif doit être compris entre $0 et $100.");
	}

	BizInfo[bizid][bizEntrance] = fee;
	SQL_SetInteger("business", "entrance", BizInfo[bizid][bizEntrance], BizInfo[bizid][bizID]);
	
	if (BizInfo[bizid][bizType] == BIZ_GAS_STATION) 
		SendSyntaxMessage(playerid, "Vous avez défini le prix du carburant pour le business [ID: %d] à $%i/L.", bizid, fee);
	else 
		SendSyntaxMessage(playerid, "Vous avez défini le prix d'entrée pour le business [ID: %d] à $%i.", bizid, fee);
	
	return 1;
}


Dialog:ConfigPrices(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearInsideBiz(playerid)) == -1) 
		return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre business.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	new biz = BizInfo[bizid][bizType];
	if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas modifier les prix dans ce type de business.");

	new type;
	switch (biz)
	{
		case BIZ_PIZZA:     type = 0;
		case BIZ_CLUCKIN:   type = 1;
		case BIZ_BURGER:    type = 2;
		case BIZ_DONUT:     type = 3;
		case BIZ_STORE:     type = 4;
		case BIZ_BAR:       type = 5;
		case BIZ_PHARMACY:  type = 6;
	}

	new mes[128];
	format(mes, sizeof(mes), "{FFFFFF}Modifier le prix du produit #%i {AFAFAF}[%s, $%i]", listitem + 1, name_order[type][listitem], BizInfo[bizid][bizProductPrice][listitem]);
	Dialog_Show(playerid, ConfigPricesEnd, DIALOG_STYLE_INPUT, "Configuration des prix", mes, "OK", "Quitter");
	SetPVarInt(playerid, "ConfigPrices", listitem);
	return 1;
}

Dialog:ConfigPricesEnd(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new bizid;

	if ((bizid = GetNearInsideBiz(playerid)) == -1) 
		return SendErrorMessage(playerid, "Vous devez être à l'intérieur de votre business.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Ce business ne vous appartient pas.");

	new money = strval(inputtext);
	listitem = GetPVarInt(playerid, "ConfigPrices");

	new biz = BizInfo[bizid][bizType];
	if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT) 
		return SendErrorMessage(playerid, "Vous ne pouvez pas modifier les prix dans ce type de business.");

	new type;
	switch (biz)
	{
		case BIZ_PIZZA:     type = 0;
		case BIZ_CLUCKIN:   type = 1;
		case BIZ_BURGER:    type = 2;
		case BIZ_DONUT:     type = 3;
		case BIZ_STORE:     type = 4;
		case BIZ_BAR:       type = 5;
		case BIZ_PHARMACY:  type = 6;
	}

	if (money < biz_prices[type][listitem] / 2 || money > biz_prices[type][listitem] * 2)
	{
		new mes[256];
		format(mes, sizeof(mes), ""EMBED_RED"La somme saisie doit être au moins $%i et au maximum $%i!\n{FFFFFF}Modifier le prix du produit #%i {AFAFAF}[%s, $%i]", 
			   biz_prices[type][listitem] / 2, biz_prices[type][listitem] * 2, listitem + 1, name_order[type][listitem], BizInfo[bizid][bizProductPrice][listitem]);
		Dialog_Show(playerid, ConfigPricesEnd, DIALOG_STYLE_INPUT, "Configuration des prix", mes, "Entrer", "Quitter");
		return 1;
	}

	BizInfo[bizid][bizProductPrice][listitem] = money;
	SendSyntaxMessage(playerid, "Vous avez modifié le prix de {FF6347}%s {FFFFFF}à {FF6347}$%i{FFFFFF}.", name_order[type][listitem], BizInfo[bizid][bizProductPrice][listitem]);

	Save_Business(bizid);
	DeletePVar(playerid, "ConfigPrices");
	return 1;
}

Dialog:StoreDialog(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new bizid = GetNearInsideBiz(playerid);

	if (bizid == -1)														return 1;
	if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct])	return SendErrorMessage(playerid, "Il n'y a pas assez de produits dans le business.");
	if (GetCash(playerid) < BizInfo[bizid][bizProductPrice][listitem])	return SendNotCash(playerid);
	
	if (BizInfo[bizid][bizType] == BIZ_STORE)
	{
		switch(listitem)
		{
			case 0:
			{
				SetPlayerNumber(playerid);
				SendSyntaxMessage(playerid, "Vous avez acheté une carte SIM pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 1:
			{
				if (GiveInvItem(playerid, INV_RADIO, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");

				SendSyntaxMessage(playerid, "Vous avez acheté une radio pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
				SendSyntaxMessage(playerid, "Utilisez /radiohelp pour obtenir de l'aide supplémentaire.");
			}
			case 2:
			{
				if (PlayerInfo[playerid][pLevel] < 2)			return SendErrorMessage(playerid, "Vous devez atteindre le niveau 2 pour acheter un masque.");
				if (GiveInvItem(playerid, INV_MASK, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
				
				SendSyntaxMessage(playerid, "Vous avez acheté un masque pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
				SendHelpMessage(playerid, "/mask pour le porter.");
			}
			case 3:
			{
				if (GiveInvItem(playerid, INV_LIGHTER, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");

				SendSyntaxMessage(playerid, "Vous avez acheté un briquet pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 4:
			{
				if (GiveInvItem(playerid, INV_CIGAR, 20) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");

				SendSyntaxMessage(playerid, "Vous avez acheté des cigarettes pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 5:
			{
				if (GiveInvItem(playerid, INV_DICE, 1) == -1)		return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
				
				SendSyntaxMessage(playerid, "Vous avez acheté des dés pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
				SendHelpMessage(playerid, "/dice pour lancer les dés.");
			}
			case 6:
			{
				if (GiveInvItem(playerid, INV_GASCAN, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");

				SendSyntaxMessage(playerid, "Vous avez acheté un bidon pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 7:
			{
				if (GiveInvItem(playerid, INV_PEN, 1) == -1)		return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");

				SendSyntaxMessage(playerid, "Vous avez acheté un stylo pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
				SendHelpMessage(playerid, "/note pour écrire dans un carnet.");
			}
			case 8:
			{
				if (GiveInvItem(playerid, INV_CARDS, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");

				SendSyntaxMessage(playerid, "Vous avez acheté des cartes à jouer pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
				SendHelpMessage(playerid, "/givecard pour commencer une partie.");
			}
			case 9:
			{
				if (GiveInvItem(playerid, 43, 200) == -1)		return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
				SendClientMessageEx(playerid, COLOR_WHITE, "Vous avez acheté un appareil photo pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 10:
			{
				if (GiveInvItem(playerid, INV_SIGN, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
				SendSyntaxMessage(playerid, "Vous avez acheté un autocollant de transport pour $%i.",  BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 11:
			{
				if (GiveInvItem(playerid, 14, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
				SendSyntaxMessage(playerid, "Vous avez acheté des fleurs pour $%i.",  BizInfo[bizid][bizProductPrice][listitem]);
			}
		}
	}
	else if (BizInfo[bizid][bizType] == BIZ_BAR)
	{
		new name_drink[8][8] = {"bière", "vodka", "cola", "eau", "whisky", "brandy", "soda", "vin"};

		if (GiveInvItem(playerid, INV_BEER + listitem, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
		SendSyntaxMessage(playerid, "Vous avez acheté %s pour $%i.", name_drink[listitem], BizInfo[bizid][bizProductPrice][listitem]);
	}
	else if (BizInfo[bizid][bizType] == BIZ_PHARMACY)
	{
		switch(listitem)
		{
			case 0:
			{
				if (PlayerInfo[playerid][pPrescribe] < gettime())		return SendErrorMessage(playerid, "Vous n'avez pas d'ordonnance.");
				if (PlayerInfo[playerid][pGanjaTime] > gettime())		return SendErrorMessage(playerid, "Vous pourrez acheter de la marijuana médicale dans : %s.", ConvertSecond(PlayerInfo[playerid][pGanjaTime] - gettime()));
				if (GiveInvItem(playerid, INV_MED_GANJA, 2) == -1)		return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");

				PlayerInfo[playerid][pGanjaTime] = gettime() + 86400;
				SendSyntaxMessage(playerid, "Vous avez acheté de la marijuana médicale pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 1:
			{
				if (IsPlayerHasItem(playerid, INV_HEAL))		 return SendErrorMessage(playerid, "Vous avez déjà une trousse de secours.");
				if (GiveInvItem(playerid, INV_HEAL, 1) == -1) return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
					
				SendSyntaxMessage(playerid, "Vous avez acheté une trousse de secours pour $%i.", BizInfo[bizid][bizProductPrice][listitem]);
			}
			case 2..11: 
			{
				if (GiveFloatInvItem(playerid, 101+listitem, 100.0) == -1)	return SendErrorMessage(playerid, "Il n'y a pas assez de place dans l'inventaire.");
				SendSyntaxMessage(playerid, "Vous avez acheté '%s' pour $%i.", name_order[6][listitem], BizInfo[bizid][bizProductPrice][listitem]);
			}
		}
	}
	else return 1;

	if (BizInfo[bizid][bizOwnerID] != 0)
	{
		BizInfo[bizid][bizProduct]--;
		GiveBizCash(bizid, BizInfo[bizid][bizProductPrice][listitem]);
	}

	GiveCash(playerid, -BizInfo[bizid][bizProductPrice][listitem]);
	return 1;
}

Dialog:ComputerStore(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	if (IsPlayerPhoneCreated(playerid)) 
		Phone_Hide(playerid);
	else 
		LoadPhoneTextDraw(playerid);
	
	TextDrawBoxColor(PhDisplay[playerid], 255);
	TextDrawShowForPlayer(playerid, PhDisplay[playerid]);

	TextDrawBoxColor(PhoneTDbox[0][playerid], phone_display_color[listitem]);
	TextDrawShowForPlayer(playerid, PhoneTDbox[0][playerid]);

	for (new i = 1; i < 5; i++)
	{
		if (i < 3) 
			TextDrawColor(PhoneTDbox[i][playerid], phone_model_color[listitem]);
		TextDrawShowForPlayer(playerid, PhoneTDbox[i][playerid]);
		if (i < 4) 
			TextDrawShowForPlayer(playerid, PhArrow[i][playerid]);
	}

	for (new i = 0; i < 2; i++) 
	{
		TextDrawColor(PhBtn[i][playerid], phone_btns_color[listitem]);
		TextDrawBackgroundColor(PhBtn[i][playerid], phone_btns_color[listitem]);
		TextDrawShowForPlayer(playerid, PhBtn[i][playerid]);
	}

	new phone_names[5][10] = {"noir", "bleu", "rouge", "vert", "orange"};
	new diatxt[64];

	format(diatxt, sizeof(diatxt), "{FFFFFF}Voulez-vous vraiment acheter un téléphone %s ?", phone_names[listitem]);
	FreezePlayer(playerid);

	SetPVarInt(playerid, #buing_phone, listitem + 1);
	ConfirmDialog(playerid, "Achat de téléphone", diatxt, "BuyingPhone");
	return 1;
}

this::BuyingPhone(playerid, response)
{
	if (!response)	
	{
		new listitem = PlayerInfo[playerid][pPhoneModel];
		TextDrawBoxColor(PhoneTDbox[0][playerid], phone_display_color[listitem]);
	
		for (new i = 0; i < 5; i++)
		{
			if (i < 3) TextDrawColor(PhoneTDbox[i][playerid], phone_model_color[listitem]);
			TextDrawHideForPlayer(playerid, PhoneTDbox[i][playerid]);
			if (i < 4) TextDrawHideForPlayer(playerid, PhArrow[i][playerid]);
		}

		for (new i = 0; i < 2; i++) 
		{
			TextDrawColor(PhBtn[i][playerid], phone_btns_color[listitem]);
			TextDrawBackgroundColor(PhBtn[i][playerid], phone_btns_color[listitem]);
			TextDrawHideForPlayer(playerid, PhBtn[i][playerid]);
		}

		TextDrawHideForPlayer(playerid, PhDisplay[playerid]);

		SetPVarInt(playerid, #buing_phone, 0);
		cmd::buy(playerid, "");
		return UnFreezePlayer(playerid);
	}

	new listitem = GetPVarInt(playerid, #buing_phone),
		phone_names[5][10] = {"noir", "bleu", "rouge", "vert", "orange"};

	if (PlayerInfo[playerid][pPhoneModel] == listitem - 1) return SendErrorMessage(playerid, "Vous avez déjà ce modèle de téléphone.");
	if (GetCash(playerid) < MOBILEMODELPRICE) return SendNotCash(playerid);

	GiveCash(playerid, -MOBILEMODELPRICE);

	if (!PlayerInfo[playerid][pNumber]) SetPlayerNumber(playerid);

	PlayerInfo[playerid][pPhoneModel] = listitem - 1;

	SendSyntaxMessage(playerid, "Vous avez acheté un téléphone %s.", phone_names[listitem - 1]);
	SendHelpMessage(playerid, "Appuyez sur Y ou entrez (/ph)one pour ouvrir le téléphone.");

	UnFreezePlayer(playerid);
	SetPVarInt(playerid, #buing_phone, 0);
	
	for (new i = 0; i < 5; i++) 
	{
		TextDrawHideForPlayer(playerid, PhoneTDbox[i][playerid]);
		if (i < 4) TextDrawHideForPlayer(playerid, PhArrow[i][playerid]);
	}

	for (new i = 0; i < 2; i++) TextDrawHideForPlayer(playerid, PhBtn[i][playerid]);
	TextDrawHideForPlayer(playerid, PhDisplay[playerid]);
		
	return 1;
}

Dialog:ChangeBizInterior(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new index, bizid = GetPVarInt(playerid, "biz_change");

	if (bizid < 0 || bizid > MAX_BIZ - 1 || !BizInfo[bizid][bizID]) 
		return SendErrorMessage(playerid, "Aucun business avec cet ID n'existe.");

	for (new i = 0; i < MAX_INTERIORS; i++)
	{
		if (!AdminTeles[i][telesOn] || BUSSINESS_INT_TYPE != AdminTeles[i][telesType]) 
			continue;

		if (index == listitem)
		{
			index = i;
			break;
		}
		index++;
	}

	SetBizInterior(playerid, bizid, index);
	return 1;
}
