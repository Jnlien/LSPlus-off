House_OnPlayerSelectedMenuRow(playerid, row)
{
    if (GetPlayerMenu(playerid) != buy_interior) return 1;
	
	new houseid = pTemp[playerid][pInteriorHouse]-1;

	switch(row)
	{
	    case 0:
    	{
	        for(new i = pTemp[playerid][pInteriorID]+1; i < MAX_INTERIORS; i++)
	        {
	            if (!AdminTeles[i][telesOn] || HouseInfo[houseid][hClass] != AdminTeles[i][telesType]) continue;
					
                pTemp[playerid][pInteriorID] = i;
				DestroyPlayerObject(playerid, pTemp[playerid][pInteriorObj]);
	            pTemp[playerid][pInteriorObj] = CreatePlayerObject(playerid, 19300, AdminTeles[i][telesPosX], AdminTeles[i][telesPosY], AdminTeles[i][telesPosZ], 0.0, 0.0, 0.0);
				AttachCameraToPlayerObject(playerid, pTemp[playerid][pInteriorObj]);
			    break;
	        }

	        ShowMenuForPlayer(buy_interior, playerid);
		}
		
	    case 1:
		{
		    for(new i = pTemp[playerid][pInteriorID]-1; i != 0; i--)
	        {
	            if (!AdminTeles[i][telesOn] || HouseInfo[houseid][hClass] != AdminTeles[i][telesType]) continue;

                pTemp[playerid][pInteriorID] = i;
				DestroyPlayerObject(playerid, pTemp[playerid][pInteriorObj]);
	            pTemp[playerid][pInteriorObj] = CreatePlayerObject(playerid, 19300, AdminTeles[i][telesPosX], AdminTeles[i][telesPosY], AdminTeles[i][telesPosZ], 0.0, 0.0, 0.0);
				AttachCameraToPlayerObject(playerid, pTemp[playerid][pInteriorObj]);
			    break;
	        }

	        ShowMenuForPlayer(buy_interior, playerid);
		}

		case 2:
		{
			new id = pTemp[playerid][pInteriorID];
			if (!AdminTeles[id][telesOn] || HouseInfo[houseid][hClass] != AdminTeles[id][telesType])	return SendErrorMessage(playerid, "Erreur d'intérieur.");
				
			Dialog_Show(playerid, BuyInteriorAccept, DIALOG_STYLE_MSGBOX, "{FFFFFF}Intérieur", "{ffffff}Voulez-vous acheter un intérieur pour la maison pour $750.\nTous les meubles achetés seront vendus !", "Acheter", "Annuler");
		}
	    case 3:	SetPlayerBuyInterior(playerid, 0);
	    
	}
    return 1;
}

House_OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		for (new i = 0; i < MAX_HOUSES; i++)
		{
			if (!HouseInfo[i][hID] || (checkpointid != HouseInfo[i][hCheckPoint] && checkpointid != HouseInfo[i][hExtCheckPoint])) continue;

			if (pTemp[playerid][pPizzaBoy] && pTemp[playerid][pPizzaBoy] - 1 == i && IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
			{
				pTemp[playerid][pPizzaBoy] = 0;
				#define PIZZABOY_COST 100
				SendSyntaxMessage(playerid, "Pizza livrée, vous avez gagné $%i.", PIZZABOY_COST);
				GiveCash(playerid, PIZZABOY_COST);
				RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
				SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
				return 1;
			}
			new string[128];

			if (!HouseInfo[i][hOwnerID] && checkpointid == HouseInfo[i][hCheckPoint])
			{
				new door[11];
				switch (HouseInfo[i][hDoor])
				{
					case 0: door = "Classique";
					case 1: door = "En bois";
					case 2: door = "En fer";
				}

				format(string, sizeof(string), "Niveau de serrure : %i / Niveau d'alarme : %i / Porte : %s~n~~y~/enter /(ds)hout /ddo /knock~n~~g~~h~/checkint /h(ouse) buy", HouseInfo[i][hLock], HouseInfo[i][hAlarm], door);
				InfoMSG(playerid, string, 5);

				format(string, sizeof(string), "~h~~p~MAISON À VENDRE ! ~w~/ PRIX : ~g~$%i~n~~y~%s", HouseInfo[i][hPrice], (!HouseInfo[i][hApart]) ? (GetHouseZone(i, 2)) : (GetApartmentZone(HouseInfo[i][hApart], 2)));
				CreateLableText(playerid, string, "~h~Appuyez sur ~y~~h~'F' ~w~pour acheter ou~n~entrez ~y~~h~/h buy~h~~w~ pour acheter");
				return 1;
			}
			else if (HouseInfo[i][hOwnerID] != 0)
			{
				if (checkpointid == HouseInfo[i][hCheckPoint]) {
					format(string, sizeof(string), "Vous êtes sur le seuil de %s ~h~~g~propriété~w~.~n~~y~/enter /(ds)hout /ddo /knock.", (HouseInfo[i][hOwnerID] != PlayerInfo[playerid][pID]) ? ("quelqu'un d'autre") : ("votre"));
					InfoMSG(playerid, string, 5);

					format(string, sizeof(string), "~h~~p~Maison de %s~n~%s", HouseInfo[i][hOwner], (!HouseInfo[i][hApart]) ? (GetHouseZone(i, 2)) : (GetApartmentZone(HouseInfo[i][hApart], 2))); 
					CreateLableText(playerid, string, "~h~Appuyez sur ~y~~h~'F' ~w~pour entrer ou~n~entrez ~y~~h~/enter~h~~w~ pour entrer");
				} else if (checkpointid == HouseInfo[i][hExtCheckPoint]) {
					format(string, sizeof(string), "Vous êtes sur le seuil de %s ~h~~g~propriété~w~.~n~~y~/enter /(ds)hout /ddo /knock.", (HouseInfo[i][hOwnerID] != PlayerInfo[playerid][pID]) ? ("quelqu'un d'autre") : ("votre"));
					InfoMSG(playerid, string, 5);

					format(string, sizeof(string), "~h~~p~Entrée arrière~n~%s", (!HouseInfo[i][hApart]) ? (GetHouseZone(i, 2)) : (GetApartmentZone(HouseInfo[i][hApart], 2))); 
					CreateLableText(playerid, string, "~h~Appuyez sur ~y~~h~'F' ~w~pour entrer ou~n~entrez ~y~~h~/enter~h~~w~ pour entrer");
				}

				return 1;
			}
		}
	}
	return 1;
}

House_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT *, (SELECT name FROM users WHERE id = houses.owner) as ownername, (SELECT last_login FROM users WHERE id = houses.owner) as last_login FROM `houses`", "LoadDynamicHouses");
}

House_PlayerTimer(playerid)
{
	if (GetPVarInt(playerid, #check_int))
	{
	    UpdateIntVar(playerid, #check_int, -1);

	    new hid = -1;

		if (!GetPVarInt(playerid, #check_int) && (hid = GetNearInsideHouse(playerid)) != -1)
		{		        
			SetPlayerPosEx(playerid, HouseInfo[hid][hPosX], HouseInfo[hid][hPosY], HouseInfo[hid][hPosZ]);
			SetInterior(playerid, HouseInfo[hid][hPosInt]);
			SetWorld(playerid, HouseInfo[hid][hPosVW]);
		 	pTemp[playerid][pHouseEntered] = -1;
			SendSyntaxMessage(playerid, "Le temps de visualisation est écoulé.");
		    DeletePVar(playerid, #check_int);
		}
	}
	return 1;
}