ALTX:biz("/business");
CMD:biz(playerid, params[])
{
	new option[16], secoption[129];
	if (sscanf(params, "s[16]S()[128]", option, secoption))
	{
		SendHelpMessage(playerid, "/biz [buy / text / name / info]");
		SendSyntaxMessage(playerid, "Autres commandes : /bmenu /furniture");
		return 1;
	}

	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucune entreprise à proximité.");

	if (CompareStrings(option, "info"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");
		
		new diatxt[300];
		format(diatxt, sizeof(diatxt), "{FFFFFF}bdID:\t\t\t%i\nNom:\t\t %s\nType d'entreprise:\t %s\nÉtat:\t\t %s\n", 
			BizInfo[bizid][bizID], BizInfo[bizid][bizName], biz_types[BizInfo[bizid][bizType]], 
			(BizInfo[bizid][bizLocked]) ? ("Fermé") : ("Ouvert"));
		format(diatxt, sizeof(diatxt), "%sMontant en caisse:\t $%i\nPrix d'entrée:\t $%i\n\nÉtat du stock: %i / 400\n", 
			diatxt, BizInfo[bizid][bizCash], BizInfo[bizid][bizEntrance], BizInfo[bizid][bizProduct]);

		new id = biz_prod_types[BizInfo[bizid][bizType]];

		format(diatxt, sizeof(diatxt), "%sCommande: %s à $%i / unité", 
			diatxt, truckcargo_product[id], BizInfo[bizid][bizBuyPrice]);

		Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "Informations sur l'entreprise", diatxt, ">>", "");
		return 1;
	}

	else if (CompareStrings(option, "fee"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		new fee;

		if (sscanf(secoption, "i", fee)) 
			return SendErrorMessage(playerid, "/biz fee [prix (0-100)]");
		if (fee < 0 || fee > 100) 
			return SendErrorMessage(playerid, "Le prix d'entrée doit être compris entre $0 et $100.");

		BizInfo[bizid][bizEntrance] = fee;
		SQL_SetInteger("business", "entrance", BizInfo[bizid][bizEntrance], BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Vous avez défini le prix d'entrée de l'entreprise [ID: %d] à $%i.", bizid, fee);
		return 1;
	}

	else if (CompareStrings(option, "text"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		new name[129];
		
		if (sscanf(secoption, "s[128]", name))
		{
			SendHelpMessage(playerid, "/biz text [texte]");
			SendSyntaxMessage(playerid, "Pour supprimer le texte d'entrée, entrez {FFFF00}/biz text delete");
			SendSyntaxMessage(playerid, "Vous pouvez utiliser des couleurs HTML dans le texte. Exemple de couleur : ~FFFF00~ - jaune.");
			SendSyntaxMessage(playerid, "Exemple de texte \"~FFFF00~Bonjour\" affichera ce texte - \"{FFFF00}Bonjour\"");
			return 1;
		}
		if (CompareStrings(name, "delete"))
		{
			return 1;
		}

		if (strlen(name) < 2 || strlen(name) > 127) 
			return SendErrorMessage(playerid, "Le texte de l'entreprise peut contenir jusqu'à 127 caractères.");
		if (strfind(name, "\n", true) != -1) 
			return SendErrorMessage(playerid, "Le texte de l'entreprise ne peut pas contenir de sauts de ligne.");

		format(BizInfo[bizid][bizMes], 128, "%s", name);
		place_color(BizInfo[bizid][bizMes]);
		SQL_SetString("business", "entermsg", BizInfo[bizid][bizMes], BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Vous avez défini le texte d'entrée de l'entreprise : %s", BizInfo[bizid][bizMes]);
		return 1;
	}

	else if (CompareStrings(option, "name"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		new name[32];
	
		if (sscanf(secoption, "s[32]", name)) 
			return SendHelpMessage(playerid, "/biz name [nom]");
		if (strlen(name) > 30) 
			return SendErrorMessage(playerid, "Le nom de l'entreprise ne peut pas dépasser 30 caractères.");
		if (strfind(name, "~n~", true) != -1) 
			return SendErrorMessage(playerid, "Le nom de l'entreprise ne peut pas contenir de saut de ligne.");
		if (CheckTextDrawString(name)) 
			return SendErrorMessage(playerid, "Le nom de l'entreprise ne doit pas contenir le caractère '~'.");

		format(BizInfo[bizid][bizName], 32, "%s", name);
		SQL_SetString("business", "name", BizInfo[bizid][bizName], BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Vous avez défini le nom de l'entreprise : %s", BizInfo[bizid][bizName]);
		return 1;
	}
	
	else if (CompareStrings(option, "deposit"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		new amount;

		if (sscanf(secoption, "d", amount))
		{
			SendHelpMessage(playerid, "/biz deposit [montant]");
			SendSyntaxMessage(playerid, "Actuellement dans la caisse: $%i", BizInfo[bizid][bizCash]);
			return 1;
		}
		if (GetCash(playerid) < amount || amount < 1) 
			return SendNotCash(playerid);
		if (BizInfo[bizid][bizCash] + amount > 25000000) 
			return SendErrorMessage(playerid, "La caisse ne peut contenir plus de $25.000.000.");

		GiveCash(playerid, -amount);
		GiveBizCash(bizid, amount);
		SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Vous avez déposé $%i dans la caisse.", amount);
		return 1;
	}

	else if (CompareStrings(option, "withdraw"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");
		
		new amount;

		if (sscanf(secoption, "d", amount))
		{
			SendHelpMessage(playerid, "/biz withdraw [montant]");
			SendSyntaxMessage(playerid, "Actuellement dans la caisse: $%i.", BizInfo[bizid][bizCash]);
			return 1;
		}
		if (BizInfo[bizid][bizCash] < amount) 
			return SendErrorMessage(playerid, "Fonds insuffisants dans la caisse.");
		if (amount < 1 || amount > 25000000) 
			return SendErrorMessage(playerid, "Vous pouvez retirer un montant entre $1 et $25.000.000.");

		GiveCash(playerid, amount);
		GiveBizCash(playerid, -amount);
		SQL_SetInteger("business", "cash", BizInfo[bizid][bizCash], BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Vous avez retiré $%i de la caisse.", amount);
		return 1;
	}

	else if (CompareStrings(option, "buy"))
	{
		if (GetPlayerBizes(playerid) >= GetMaxPlayerBiz(playerid)) 
			return SendErrorMessage(playerid, "Vous ne pouvez pas acheter plus d'entreprises.");
		if (BizInfo[bizid][bizOwnerID] != 0) 
			return SendErrorMessage(playerid, "Cette entreprise n'est pas à vendre.");
		if (BizInfo[bizid][bizFreeze] != 0) 
			return SendErrorMessage(playerid, "Cette entreprise ne peut pas être achetée.");
		if (PlayerInfo[playerid][pLevel] < BizInfo[bizid][bizLevelbuy]) 
			return SendErrorMessage(playerid, "Vous devez être au niveau %i pour acheter cette entreprise.", BizInfo[bizid][bizLevelbuy]);
		if (GetCash(playerid) < BizInfo[bizid][bizPrice]) 
			return SendNotCash(playerid);

		BizInfo[bizid][bizOwnerID] = PlayerInfo[playerid][pID];
		BizInfo[bizid][bizLocked] = 0;
		BizInfo[bizid][bizProduct] = 150;
		BizInfo[bizid][bizBuyPrice] = 50;
		format(BizInfo[bizid][bizOwner], MAX_PLAYER_NAME, "%s", GetNameEx(playerid));
		GiveCash(playerid, -BizInfo[bizid][bizPrice]);
		Save_Business(bizid);
		SendSyntaxMessage(playerid, "Vous avez acheté l'entreprise %s pour $%i.", BizInfo[bizid][bizName], BizInfo[bizid][bizPrice]);

		for (new i = 0; i < MAX_GARAGES; i++)
		{
			if (!GarageInfo[i][garageID] || GarageInfo[i][garageBiz] != BizInfo[bizid][bizID]) 
				continue;

			GarageInfo[i][garageOwner] = PlayerInfo[playerid][pID];
			SQL_SetInteger("garage", "owner", GarageInfo[i][garageOwner], GarageInfo[i][garageID]);
			SendSyntaxMessage(playerid, "Avec l'entreprise, vous avez également obtenu un garage [ID: %i].", i);
		}

		new mes[256];
		format(mes, sizeof(mes), "%s [MYSQL: %i | IP: %s | Argent (AVANT): %i | Argent (APRÈS): %i] a acheté une entreprise [B_MYSQL: %i] pour -- $%i.", 
		GetName(playerid), PlayerInfo[playerid][pID], q_GetPlayerIP(playerid), PlayerInfo[playerid][pCash] + BizInfo[bizid][bizPrice], GetCash(playerid), BizInfo[bizid][bizID], BizInfo[bizid][bizPrice]);
		insert_log(mes, PROPERTY_LOG, 4, PlayerInfo[playerid][pAccID]);
		return 1;
	}

	else if (CompareStrings(option, "sell"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		static const str_size[] = "{FFFFFF}Voulez-vous vendre l'entreprise pour $%i ?";
		new mes[sizeof(str_size) - 2 + 12];

		format(mes, sizeof(mes), str_size, BizInfo[bizid][bizPrice] / 2);
		Dialog_Show(playerid, SellBiz, DIALOG_STYLE_MSGBOX, "{FFFFFF}Vente de l'entreprise", mes, "Vendre", "Annuler");
		return 1;
	}

	else if (CompareStrings(option, "buyprod"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		new price, id = biz_prod_types[BizInfo[bizid][bizType]];

		if (id == -1) 
			return SendErrorMessage(playerid, "Votre entreprise n'a pas besoin de produits.");
		if (sscanf(secoption, "i", price)) 
			return SendHelpMessage(playerid, "/biz buyprod [prix]");
		if (price > 50 && price < 1000) 
			return SendErrorMessage(playerid, "Le prix d'achat doit être compris entre $50 et $1000.");

		BizInfo[bizid][bizBuyPrice] = price;
		SQL_SetInteger("business", "buy_price", price, BizInfo[bizid][bizID]);
		SendSyntaxMessage(playerid, "Vous achetez désormais %s pour $%i par unité.", truckcargo_product[id], price);
		return 1;
	}

	else if (CompareStrings(option, "upgrade"))
	{
		new opt[16];

		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");
		if (sscanf(secoption, "s[16]", opt)) 
		{
			SendHelpMessage(playerid, "/biz upgrade [amélioration]");
			SendSyntaxMessage(playerid, "radio | repairdoor");
			return 1;
		}

		if (CompareStrings(opt, "radio"))
		{
			if (BizInfo[bizid][bizRadio] == 1) 
				return SendErrorMessage(playerid, "Cette entreprise dispose déjà d'une radio.");
			if (GetCash(playerid) < UPGRADE_PRICE_RADIO) 
				return SendErrorMessage(playerid, "Le prix de la radio est de $%i.", UPGRADE_PRICE_RADIO);

			BizInfo[bizid][bizRadio] = 1;
			SQL_SetInteger("business", "radio", BizInfo[bizid][bizRadio], BizInfo[bizid][bizID]);
			GiveCash(playerid, -UPGRADE_PRICE_RADIO);
			SendSyntaxMessage(playerid, "Vous avez acheté une radio pour votre entreprise.");
			SendSyntaxMessage(playerid, "Entrez /setstation pour utiliser la radio.");
			return 1;
		}

		else if (CompareStrings(opt, "repairdoor"))
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
			return 1;
		}
	}

	else if (CompareStrings(option, "hire"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		new userid;

		if (sscanf(secoption, "u", userid)) 
			return SendHelpMessage(playerid, "/biz hire [ID/nom du joueur]");
		if (userid == playerid) 
			return SendErrorMessage(playerid, "Vous ne pouvez pas vous embaucher vous-même.");
		if (!IsPlayerLogged(userid)) 
			return SendNotConnected(playerid);
		if (!PlayerNearPlayer(3.0, playerid, userid)) 
			return SendNotNearPlayer(playerid);
		if (pTemp[userid][pHirePlayer] == playerid) 
			return SendErrorMessage(playerid, "Vous avez déjà proposé un emploi à ce joueur.");

		pTemp[userid][pHirePlayer] = playerid;
		SetPVarInt(userid, #accept_time, gettime() + 30);
		SendSyntaxMessage(userid, "%s vous a invité à travailler dans son entreprise.", GetNameWithMask(playerid));
		SendHelpMessage(userid, "Appuyez sur Y pour accepter l'offre.");
		SendSyntaxMessage(playerid, "Vous avez invité %s à travailler dans votre entreprise.", GetNameWithMask(userid));
		return 1;
	}

	else if (CompareStrings(option, "fire"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
			return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");
	
		new userid;

		if (sscanf(secoption, "u", userid)) 
			return SendHelpMessage(playerid, "/biz fire [ID/nom du joueur]");
		if (userid == playerid) 
			return SendErrorMessage(playerid, "Vous ne pouvez pas vous licencier vous-même.");
		if (!IsPlayerLogged(userid)) 
			return SendNotConnected(playerid);
		if (PlayerInfo[userid][pWorkOn] != BizInfo[bizid][bizID]) 
			return SendErrorMessage(playerid, "Le joueur ne travaille pas dans votre entreprise.");

		PlayerInfo[userid][pWorkOn] = 0;

		SendSyntaxMessage(userid, "%s vous a licencié de l'entreprise.", GetNameWithMask(playerid));
		SendSyntaxMessage(playerid, "Vous avez licencié %s de votre entreprise.", GetNameWithMask(userid));
		return 1;
	}

	else if (CompareStrings(option, "prices"))
	{
		if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

		new biz = BizInfo[bizid][bizType];
		if (biz != BIZ_STORE && biz != BIZ_BAR && biz != BIZ_PHARMACY && biz != BIZ_PIZZA && biz != BIZ_CLUCKIN && biz != BIZ_BURGER && biz != BIZ_DONUT) 
			return SendErrorMessage(playerid, "Vous ne pouvez pas modifier les prix dans cette entreprise.");
		
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
		
		new mes[LIMIT_MAX_PRODUCTS * 128];

		for (new i = 0; i < max_product[type]; i++)
		{
			format(mes, sizeof(mes), "%s{FFFFFF}Modifier le prix du produit #%i {AFAFAF}[%s, $%i]\n", mes, i + 1, name_order[type][i], BizInfo[bizid][bizProductPrice][i]);
		}

		Dialog_Show(playerid, ConfigPrices, DIALOG_STYLE_LIST, "{9ACD32}Configuration des prix", mes, "Choisir", "Retour");
		return 1;
	}

	else
	{
		SendHelpMessage(playerid, "/biz [paramètre]");
		SendSyntaxMessage(playerid, "info | fee | text | name | hire | fire");
		SendSyntaxMessage(playerid, "deposit | withdraw | upgrade | buyprod");
		SendSyntaxMessage(playerid, "buy | sell | prices");
	}
	return 1;
}

CMD:bmenu(playerid, params[])
{
	new bizid;

	if ((bizid = GetNearOutsideBiz(playerid)) == -1) 
		bizid = GetNearInsideBiz(playerid);
	if (bizid == -1) 
		return SendErrorMessage(playerid, "Aucune entreprise à proximité.");
	if (BizInfo[bizid][bizOwnerID] != PlayerInfo[playerid][pID]) 
		return SendErrorMessage(playerid, "Cette entreprise ne vous appartient pas.");

	static diastr[] = "Informations sur l'entreprise\n\
					Informations sur les employés\n\
					Caisse - déposer / retirer\n\
					Configuration des prix\n\
					Améliorations de l'entreprise\n\
					Édition des meubles\n\
					Achat de produits\n\
					Modifier le nom\n\
					Modifier le texte d'entrée\n\
					Vendre l'entreprise";

	Dialog_Show(playerid, BusinessMenu, DIALOG_STYLE_LIST, "Menu de l'entreprise", diastr, "Choisir", "Retour");
	return 1;
}

CMD:meal(playerid, params[])
{
	if (isnull(params))
	{
		SendHelpMessage(playerid, "/meal [action]");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal order {FFFFFF}- ouvrir le menu avec les plats.");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal place {FFFFFF}- placer un plat sur la table.");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal pickup {FFFFFF}- ramasser un plateau avec un plat de la table.");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal throw {FFFFFF}- jeter un plateau avec un plat.");
		return 1;
	}

	if (CompareStrings(params, "order"))
	{
		new bizid;

		if ((bizid = GetNearInsideBiz(playerid)) == -1) 
			return SendErrorMessage(playerid, "Vous devez être à l'intérieur d'une entreprise.");
		if (BizInfo[bizid][bizType] != BIZ_PIZZA && BizInfo[bizid][bizType] != BIZ_CLUCKIN && BizInfo[bizid][bizType] != BIZ_BURGER && BizInfo[bizid][bizType] != BIZ_DONUT)
			return SendErrorMessage(playerid, "Vous n'êtes pas dans un restaurant.");

		for (new i; i != 4; i++)
			TextDrawShowForPlayer(playerid, TD_restaurant[i]);

		new mes[128], type = BizInfo[bizid][bizType] - BIZ_PIZZA;

		for (new i; i != max_product[type]; i++)
		{
			format(mes, sizeof(mes), "%s~n~~r~Santé: +%i~n~~r~Réduit la faim~n~~p~Prix: $%i~n~", name_order[type][i], 30 + (i * 10), BizInfo[bizid][bizProductPrice][i]);
			PlayerTextDrawSetString(playerid, TD_restaurant_prod[playerid][i], mes);
			PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i]);
			PlayerTextDrawSetPreviewModel(playerid, TD_restaurant_prod[playerid][i + 4], model_order[type][i]);
			PlayerTextDrawShow(playerid, TD_restaurant_prod[playerid][i + 4]);
		}

		SelectTextDraw(playerid, 0x00cb77FF);
		SetPVarInt(playerid, #restaurant, 1);
		return 1;
	}

	else if (CompareStrings(params, "place"))
	{
		if (!pTemp[playerid][pFoodStand]) 
			return SendErrorMessage(playerid, "Vous n'avez pas de plateau en main.");

		new dropid = -1;
		for (new i; i != MAX_FOOD_DROPS; i++)
		{
			if (FoodDrop[i][foodDropped] != 0) 
				continue;

			dropid = i;
			break;
		}

		if (dropid == -1) 
			return SendErrorMessage(playerid, "Nombre maximum de plateaux atteints.");

		RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
		GetXYInFrontOfPlayer(playerid, FoodDrop[dropid][foodPosX], FoodDrop[dropid][foodPosY], FoodDrop[dropid][foodPosZ], 0.5);
		FoodDrop[dropid][foodPosZ] -= 0.1;
		FoodDrop[dropid][foodInt] = GetInterior(playerid);
		FoodDrop[dropid][foodVW] = GetWorld(playerid);
		FoodDrop[dropid][foodModel] = pTemp[playerid][pFoodStand];
		FoodDrop[dropid][foodObject] = CreateDynamicObject(pTemp[playerid][pFoodStand], FoodDrop[dropid][foodPosX], FoodDrop[dropid][foodPosY], FoodDrop[dropid][foodPosZ], -25.300008, 23.200012, -16.699985, GetWorld(playerid), GetInterior(playerid));
		EditDynamicObject(playerid, FoodDrop[dropid][foodObject]);
		SetPVarInt(playerid, #edit_food, dropid + 1);
		FoodDrop[dropid][foodDropped] = 1;
		FoodDrop[dropid][foodTime] = 15;
		OnPlayAnim(playerid, "CASINO", "cards_win", 4.1, 0, 0, 0, 0, 0);
		pTemp[playerid][pFoodStand] = 0;
		SendSyntaxMessage(playerid, "Vous éditez la position du plateau. Touche \"espace\" pour déplacer la caméra.");
		return 1;
	}

	else if (CompareStrings(params, "pickup"))
	{
		if (pTemp[playerid][pFoodStand] != 0 || IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) 
			return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

		new dropid = DropFood_Nearest(playerid);

		if (dropid == -1) 
			return SendErrorMessage(playerid, "Aucun plateau à proximité.");

		OnPlayAnim(playerid, "CARRY", "crry_prtial", 4.1, 1, 1, 1, 1, 1);
		SetPlayerAttachedObject(playerid, ATTACH_HANDS, FoodDrop[dropid][foodModel], 5, 0.091998, 0.021999, 0.409, -104.9, 27.6, 0, 1, 1, 1);
		pTemp[playerid][pFoodStand] = FoodDrop[dropid][foodModel];
		FoodDrop[dropid][foodModel] = 0;
		FoodDrop[dropid][foodDropped] = 0;
		if (IsValidDynamicObject(FoodDrop[dropid][foodObject])) 
			DestroyDynamicObject(FoodDrop[dropid][foodObject]);

		return 1;
	}

	else if (CompareStrings(params, "throw"))
	{
		if (pTemp[playerid][pFoodStand] == 0) 
			return SendErrorMessage(playerid, "Vous n'avez pas de plateau en main.");

		OnPlayAnim(playerid, "CASINO", "cards_win", 4.1, 0, 0, 0, 0, 0);
		RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
		pTemp[playerid][pFoodStand] = 0;
		return 1;
	}

	else
	{
		SendHelpMessage(playerid, "/meal [action]");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal order {FFFFFF}- ouvrir le menu avec les plats.");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal place {FFFFFF}- placer un plat sur la table.");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal pickup {FFFFFF}- ramasser un plateau avec un plat de la table.");
		SendClientMessage(playerid, COLOR_LIGHTRED, "/meal throw {FFFFFF}- jeter un plateau avec un plat.");
	}
	return 1;
}