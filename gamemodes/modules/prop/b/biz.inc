Biz_OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
	for (new i = 0; i < MAX_BIZ; i++)
	{
		if (pickupid != BizInfo[i][bizPickUp] || BizInfo[i][bizID] == 0) continue;

		new 
			title[128],
			mes[128];

		if (BizInfo[i][bizFreeze] == 1)
		{
			format(title, sizeof(title), "~h~~p~%s~n~~y~L'État", BizInfo[i][bizName]);
			format(mes, sizeof(mes), "~w~Prix d'entrée : ~g~$%i~n~~w~Entrez ~y~/enter~w~ ou appuyez sur ~y~~h~'F'", BizInfo[i][bizEntrance]);

			CreateLableText(playerid, title, mes);
			return 1;
		}
		else
		{
			if (BizInfo[i][bizOwnerID] == 0)
			{
				format(title, sizeof(title), "~h~~p~ENTREPRISE À VENDRE !~n~~w~%s", BizInfo[i][bizName]);
				format(mes, sizeof(mes), "~w~Prix : ~g~$%i~n~~w~Appuyez sur ~y~~h~'F'~w~ ou entrez ~y~/biz buy", BizInfo[i][bizPrice]);

				CreateLableText(playerid, title, mes);
			}
			else
			{
				format(title, sizeof(title), "~h~~p~%s~n~~w~~y~%s", BizInfo[i][bizName], BizInfo[i][bizOwner]);
				format(mes, sizeof(mes), "~w~Prix d'entrée : ~g~$%i~n~~w~Entrez ~y~/enter~w~ ou appuyez sur ~y~~h~'F'", BizInfo[i][bizEntrance]);

				CreateLableText(playerid, title, mes);
			}
			return 1;
		}
	}
	return 1;
}

Biz_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT *, (SELECT name FROM users WHERE id = business.owner) as ownername, (SELECT last_login FROM users WHERE id = business.owner) as last_login FROM `business`", "LoadDynamicBiz");
}

Fo_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid) 
{
	for (new i = 0; i < 4; i++)
	{
		if (playertextid != TD_restaurant_prod[playerid][i + 4]) continue;

		RestaurantMenuHide(playerid);

		if (pTemp[playerid][pFoodStand] != 0 || IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS)) 
			return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

		new bizid;

		if ((bizid = GetNearInsideBiz(playerid)) == -1) 
			return SendErrorMessage(playerid, "Vous devez être à l'intérieur d'une entreprise.");

		if (BizInfo[bizid][bizType] != BIZ_PIZZA && BizInfo[bizid][bizType] != BIZ_CLUCKIN && BizInfo[bizid][bizType] != BIZ_BURGER && BizInfo[bizid][bizType] != BIZ_DONUT)
			return SendErrorMessage(playerid, "Vous n'êtes pas dans un restaurant.");

		new type = BizInfo[bizid][bizType] - BIZ_PIZZA;

		if (GetCash(playerid) < BizInfo[bizid][bizProductPrice][i]) 
			return SendNotCash(playerid);

		if (BizInfo[bizid][bizOwnerID] != 0 && !BizInfo[bizid][bizProduct]) 
			return SendErrorMessage(playerid, "Il n'y a pas assez de produits dans l'entreprise.");

		cmd::server_ame(playerid, "commande de la nourriture.");

		SendSyntaxMessage(playerid, "Vous avez commandé {FF6347}%s{FFFFFF} pour {FF6347}$%s{FFFFFF}.", name_order[type][i], FormatNumber(BizInfo[bizid][bizProductPrice][i]));
		SendSyntaxMessage(playerid, "Vous pouvez placer le plat sur une table {FF6347}/meal place{FFFFFF} ou {FF6347}/meal throw{FFFFFF} pour jeter le plateau.");

		GiveCash(playerid, -BizInfo[bizid][bizProductPrice][i]);

		if (BizInfo[bizid][bizOwnerID] != 0)
		{
			BizInfo[bizid][bizProduct]--;
			GiveBizCash(bizid, BizInfo[bizid][bizProductPrice][i]);
		}

		OnPlayAnim(playerid, "CARRY", "crry_prtial", 4.1, 1, 1, 1, 1, 1);
		SetPlayerAttachedObject(playerid, ATTACH_HANDS, model_order[type][i], 5, 0.091998, 0.021999, 0.409, -104.9, 27.6, 0, 1, 1, 1);
		pTemp[playerid][pFoodStand] = model_order[type][i];

		new Float:health;

		if (AntiCheatGetHealth(playerid, health)) 
			SetPlayerHealth(playerid, health + 30.0 + (i * 10.0));

		if (health + 30.0 + (i * 10.0) > 150.0) 
			SetPlayerHealth(playerid, 150.0);

		return 1;
	}
	return 1;
}

Food_OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
	new Float:oldX, Float:oldY, Float:oldZ, Float:oldRotX, Float:oldRotY, Float:oldRotZ;
	
	GetDynamicObjectPos(objectid, oldX, oldY, oldZ);
	GetDynamicObjectRot(objectid, oldRotX, oldRotY, oldRotZ);

    if (GetPVarInt(playerid, #edit_food) != 0 && (response == EDIT_RESPONSE_FINAL || response == EDIT_RESPONSE_CANCEL))
	{
	    new id = GetPVarInt(playerid, #edit_food)-1;
		
	    DeletePVar(playerid, #edit_food);

	    if (GetDistanceBetweenPoints(x, y, z, FoodDrop[id][foodPosX], FoodDrop[id][foodPosY], FoodDrop[id][foodPosZ]) > 10.0)
	    {
     		SetDynamicObjectPos(objectid, FoodDrop[id][foodPosX], FoodDrop[id][foodPosY], FoodDrop[id][foodPosZ]);
			return SendErrorMessage(playerid, "L'objet est trop éloigné de sa position initiale.");
	    }

		if (objectid != FoodDrop[id][foodObject])	return SendErrorMessage(playerid, "Erreur lors de l'édition de l'objet.");

		SetDynamicObjectPos(objectid, x, y, z);
		SetDynamicObjectRot(objectid, rx, ry, rz);

        GetDynamicObjectPos(objectid, FoodDrop[id][foodPosX], FoodDrop[id][foodPosY], FoodDrop[id][foodPosZ]);
	    return 1;
	}
	return 1;
}