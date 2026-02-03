MDC_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if (playertextid == MDC_UI[playerid][16]) // X
    {
        MDC_Hide(playerid);
        return 1;
    }
    
    if (playertextid == MDC_BACK[playerid])
    {
        if (MdcInfo[playerid][mdcBack] == PAGE_LOOKUP_VEHICLES_LIST)
        {
            new query[164];
            mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `owner` = %i AND `registered`=1 LIMIT 3", GetPVarInt(playerid, #MDC_OWNER_ID));
            mysql_tquery(dbHandle, query, "MDCSearchVehiclesList", "i", playerid);
            MdcInfo[playerid][mdcListPage] = 0;
        }
        else if (MdcInfo[playerid][mdcBack] == PAGE_LOOKUP_VEHICLE)
        {
            new query[164];
            mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `id` = %i AND `registered`=1 LIMIT 1", GetPVarInt(playerid, #MDC_ID));
            mysql_tquery(dbHandle, query, "MDCSearchVehicle", "i", playerid);
        }
        else if (MdcInfo[playerid][mdcBack] == PAGE_LOOKUP_PERSON)
        {
            new query[64];
            mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `id` = %i", GetPVarInt(playerid, #MDC_ID));
	        mysql_tquery(dbHandle, query, "MDCSearchPersonInfo", "i", playerid);
            MdcInfo[playerid][mdcListPage] = 0;

            DeletePVar(playerid, #MDC_FINE);
        }
        else if (MdcInfo[playerid][mdcBack] == PAGE_MENU)
        {
            Update_MDC(playerid, PAGE_MENU);

            for(new j = 0; j < 16; j++) 
            {
                if (MdcInfo[playerid][mdcList][j] != -1)
                    PlayerTextDrawShow(playerid, MDC_LIST[playerid][j]);
            }
            
            //MdcInfo[playerid][mdcPage] = PAGE_MENU;

            if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_PERSON)
                MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_PERSON;
            else 
                MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_VEHICLE;
        }
        return 1;
    }

	if (playertextid == MDC_UI[playerid][2]) // "√À¿¬Õ¿ﬂ");
	{
		MDC_PAGE(playerid, PAGE_MAIN);
		return 1;
   	}

	if (playertextid == MDC_UI[playerid][3]) // "œŒ»— ");
   	{
		MDC_PAGE(playerid, PAGE_LOOKUP);
		return 1;
   	}

	if (playertextid == MDC_UI[playerid][4]) // "¬€«Œ¬€");
	{
        MDC_PAGE(playerid, PAGE_CALLS);
		return 1;
   	}

    if (playertextid == MDC_UI[playerid][5]) // "–Œ«€— ");
	{
        Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{3D62A8}Avis de recherche", "{ffffff}Le Mobile Data Computer est encore en dÈveloppement.\nNous vous demandons de faire preuve de patience et de comprÈhension.\n\nCordialement,\n{A9C4E4}DÈpartement Technique du SASD", "Fermer", "");
		return 1;
   	}
    
    if (playertextid == MDC_UI[playerid][6]) // "APB");
   	{
        MDC_PAGE(playerid, PAGE_APB);
        return 1;    
   	}
    
    if (playertextid == MDC_UI[playerid][7]) // "ﬁÕ»“€");
   	{
        MDC_PAGE(playerid, PAGE_ROASTER);
		return 1;    
   	}
    
    if (playertextid == MDC_UI[playerid][8]) // "CCTV");
   	{
        MDC_PAGE(playerid, PAGE_CCTV);
		return 1; 
   	}

    if (playertextid == MDC_LOOKUP_SEARCH[playerid][0]) // "»Ãﬂ");
   	{
        if (MdcInfo[playerid][mdcPage] == PAGE_APB)
        {
            new apb = -1;

            for(new i = MAX_APBS-1; i >= 0; i--)
            {
                if (ApbInfo[i][apbOn] != 0) continue;

                apb = i;
                break;
            }

            if (apb == -1) return SendErrorMessage(playerid, "Pas de place disponible pour une nouvelle note APB.");

            SetPVarInt(playerid, #MDC_ID, apb);
            Dialog_Show(playerid, APB_CREATE_FIRST, DIALOG_STYLE_INPUT, "{098d18}CrÈation d'une APB", "{B4B5B7}Entrez le nom du suspect :\n- Nombre maximal de caractËres : 24", "Suivant", "Annuler");
        }
        else
        {
            PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0x494949FF); // »Ãﬂ

            PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0x494949FF);
            PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0xa7a7a7FF); // “—

            for(new i = 0; i < 2; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][i]);

            MdcInfo[playerid][mdcSearch] = 0;
        }
        return 1; 
   	}

    if (playertextid == MDC_LOOKUP_SEARCH[playerid][1]) // "“—");
    {
        if (MdcInfo[playerid][mdcPage] == PAGE_APB)
        {
            if (MdcInfo[playerid][mdcSearch])
            {
                PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][1], "PAR NOM");
                MdcInfo[playerid][mdcSearch] = 0;

                PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0xa7a7a7FF);
                PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0x494949FF); // “—
            }
            else
            {
                PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][1], "DESCRIPTION");
                MdcInfo[playerid][mdcSearch] = 1;

                PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0x494949FF);
                PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0xa7a7a7FF); // “—
            }
        }
        else
        {
            PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0x494949FF);
            PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0xa7a7a7FF); // »Ãﬂ

            PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0x494949FF); // “—

            for(new i = 0; i < 2; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][i]);

            MdcInfo[playerid][mdcSearch] = 1;
        }
        return 1; 
   	}

    if (playertextid == MDC_LOOKUP_SEARCH[playerid][2]) // "ENTRER"
    {
        if (MdcInfo[playerid][mdcPage] == PAGE_APB)
        {
            if (!MdcInfo[playerid][mdcSearch]) 
                Dialog_Show(playerid, MDC_APB_SEARCH_NAME, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Recherche par nom", "Entrez un nom ou une partie du nom ‡ rechercher :", "Entrer", "Retour");
            else 
                Dialog_Show(playerid, MDC_APB_SEARCH_CHARGE, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Recherche par infraction", "Entrez un mot ou une partie du mot ‡ rechercher :", "Entrer", "Retour");
        }
        else
        {
            if (!MdcInfo[playerid][mdcSearch]) 
                Dialog_Show(playerid, MDCSearchPerson, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Recherche par nom", "{B4B5B7}RECHERCHE PAR NOM\n\nEntrez le nom :\n\nFormat : PrÈnom_Nom (( ou ID du joueur ))", "Entrer", "Retour");
            else 
                Dialog_Show(playerid, MDCSearchVehicleType, DIALOG_STYLE_TABLIST, "{8D8DFF}MDC - Recherche de vÈhicule", "Recherche par propriÈtaire\nRecherche par plaque d'immatriculation", "Choisir", "Retour");
        }

        return 1; 
    }

    if (playertextid == MDC_LOOKUP_SEARCH[playerid][3]) // "—¡–Œ—»“‹");
    {
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "______________");
        PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);

        SetPVarInt(playerid, #MDC_APB_S, 0);

        /*
        new page = MdcInfo[playerid][mdcPage];
        
        switch(page)
        {
			case PAGE_LOOKUP_PERSON:
			{

			}
			case PAGE_LOOKUP_VEHICLE:
			{

			}
			case PAGE_LOOKUP_VEHICLES_LIST:
			{

			}
        }*/

        if (MdcInfo[playerid][mdcPage] == PAGE_APB)
        {
            MDC_PAGE(playerid, PAGE_APB);
        }
        else
        {
            for(new h = 0; h < 8; h++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLE[playerid][h]);

            for(new a = 0; a < 12; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][a]);
            for(new b = 0; b < 3; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
            for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);

            for(new j = 0; j < 2; j++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][j]);
            for(new k = 0; k < 12; k++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][k]);

            MdcInfo[playerid][mdcPage] = PAGE_LOOKUP;
        }

        MdcInfo[playerid][mdcListPage] = 0;

        return 1; 
    }

    if (playertextid == MDC_LOOKUP_PERSON[playerid][4])
    {
        if (playertextid == MDC_LOOKUP_PERSON[playerid][4])
        {
            Dialog_Show(playerid, MDCRecord, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - DÈpÙt d'accusation", "{B4B5B7}D…P‘T D'ACCUSATION\n\nEntrez le nom pour confirmer :\n\nFormat : PrÈnom_Nom (( ou ID du joueur ))", "Entrer", "Retour");
            return 1;
        }

        //new mes[256];
        //format(mes, sizeof(mes), "{B4B5B7}MISE EN ACCUSATION\n\nAccusÈ : %s\n\nSaisissez le texte de l'accusation : (( Infraction / 128 caractËres max ))", GetNameEx(pTemp[playerid][pMDCPlayer]));
        //Dialog_Show(playerid, MDCRecord2, DIALOG_STYLE_INPUT, "{8D8DFF}MDC - Mise en accusation", mes, "Valider", "Annuler");
        return 1;
    }

    if (playertextid == MDC_LOOKUP_PERSON[playerid][5])
    {
        new query[255];
        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `id` = %i LIMIT 1", GetPVarInt(playerid, #MDC_ID));
	    mysql_tquery(dbHandle, query, "MDC_LICENSE", "i", playerid);

        return 1;
    }

    if (playertextid == MDC_LOOKUP_PERSON[playerid][6]) // œÓÒÏÓÚ ¯Ú‡ÙÓ‚
    {
        new query[255];
        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `userid`=%i ORDER BY `id` DESC LIMIT 16", GetPVarInt(playerid, #MDC_ID));
        mysql_tquery(dbHandle, query, "MDC_ShowMenu", "ii", playerid, 0);

        MdcInfo[playerid][mdcListPage] = 0;

        new Float:countticket = MDC_CountPlayerTickets(GetPVarInt(playerid, #MDC_ID));
        countticket = countticket/16;
        SetPVarInt(playerid, #MAX_MDC_PAGE, floatround(countticket, floatround_ceil));

        SetPVarInt(playerid, #MDC_FINE, 1);

        return 1;
    }

    if (playertextid == MDC_LOOKUP_PERSON[playerid][7]) // —”ƒ»ÃŒ—“»
    {
        new query[255];
        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `criminalrecords` WHERE `userid`=%i ORDER BY `id` DESC LIMIT 16", GetPVarInt(playerid, #MDC_ID));
        mysql_tquery(dbHandle, query, "MDC_ShowMenu", "ii", playerid, 0);

        MdcInfo[playerid][mdcListPage] = 0;

        new Float:countticket = MDC_CountPlayerRecords(GetPVarInt(playerid, #MDC_ID));
        countticket = countticket/16;
        SetPVarInt(playerid, #MAX_MDC_PAGE, floatround(countticket, floatround_ceil));

        DeletePVar(playerid, #MDC_FINE);
        return 1;
    }

    if (playertextid == MDC_LOOKUP_PERSON[playerid][10]) // Õ≈ƒ¬»∆»ÃŒ—“‹
    {
        if (GetPVarInt(playerid, "pDelay"))  return 1;

        new query[255];
        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `id` = %i", GetPVarInt(playerid, #MDC_ID));
	    mysql_tquery(dbHandle, query, "MDC_MAP", "i", playerid);

        return 1;
    }
    
    for(new k = 1; k < 6; k++)
    {
        if (playertextid == MDC_OTHER_ADDRESS[playerid][k])
        {
            new h_id = MdcInfo[playerid][mdcList][0],
                strhouse[80],

                Float:map_kY = -0.04795376,
                Float:map_kX = 0.0469,

                Float:map_sX = 333.999420,
                Float:map_sY = 295.181152,

                Float:corX, Float:corY
            ;

            MdcInfo[playerid][mdcList][0] = MdcInfo[playerid][mdcList][k];
            MdcInfo[playerid][mdcList][k] = h_id;

            new dot = MdcInfo[playerid][mdcList][0];
            
            if (HouseInfo[dot][hPosX] >= 0.000000)
            {
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][0], "samaps:gtasamapbit2");
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][1], "samaps:gtasamapbit4");
                PlayerTextDrawShow(playerid, MDC_ADDRESS[playerid][0]);
                PlayerTextDrawShow(playerid, MDC_ADDRESS[playerid][1]);

                map_kX = 0.0469;
                map_sX = 333.999420;

                corX = HouseInfo[dot][hPosX] * map_kX + map_sX;
                corY = HouseInfo[dot][hPosY] * map_kY + map_sY;
            }
            else
            {
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][0], "samaps:gtasamapbit1");
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][1], "samaps:gtasamapbit3");
                PlayerTextDrawShow(playerid, MDC_ADDRESS[playerid][0]);
                PlayerTextDrawShow(playerid, MDC_ADDRESS[playerid][1]);

                map_kX = -0.0469;
                map_sX = 480.066314;

                corX = map_sX - HouseInfo[dot][hPosX] * map_kX;
                corY = HouseInfo[dot][hPosY] * map_kY + map_sY;
            }

            format(strhouse, sizeof(strhouse), "%s,~n~San Andreas", (!HouseInfo[MdcInfo[playerid][mdcList][0]][hApart])?(GetHouseZone(MdcInfo[playerid][mdcList][0], 1)):(GetApartmentZone(HouseInfo[MdcInfo[playerid][mdcList][0]][hApart], 1)));
            PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][4], strhouse);
            
            PlayerTextDrawDestroy(playerid, MDC_ADDRESS[playerid][2]);
            MDC_ADDRESS[playerid][2] = PlayerText:INVALID_TEXT_DRAW;
            
            MDC_ADDRESS[playerid][2] = CreatePlayerTextDraw(playerid, corX, corY, "hud:radar_waypoint");
            PlayerTextDrawTextSize(playerid, MDC_ADDRESS[playerid][2], 5.000000, 6.000000);
            PlayerTextDrawFont(playerid, MDC_ADDRESS[playerid][2], 4);
            PlayerTextDrawShow(playerid, MDC_ADDRESS[playerid][2]);

            format(strhouse, sizeof(strhouse), "- %.35s", (!HouseInfo[h_id][hApart])?(GetHouseZone(h_id)):(GetApartmentZone(HouseInfo[h_id][hApart])));
            PlayerTextDrawSetString(playerid, MDC_OTHER_ADDRESS[playerid][k], strhouse);
        }
    }

    for(new i = 0; i < 3; i++)
    {
        if (playertextid == MDC_VEH_LIST_CLICK[playerid][i])
        {
            new query[156];
            mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `id` = %i AND `registered`=1 LIMIT 1", MdcInfo[playerid][mdcList][i]);
            mysql_tquery(dbHandle, query, "MDCSearchVehicle", "i", playerid);

            Update_MDC(playerid, PAGE_LOOKUP_VEHICLE);
            return 1;
        }
    }

    if (playertextid == MDC_LOOKUP_VEHICLES_ARROWS[playerid][1]) //+
    {
        new query[164];

        if (MdcInfo[playerid][mdcPage] == PAGE_MENU)
        {
            if (MdcInfo[playerid][mdcListPage] == GetPVarInt(playerid, #MAX_MDC_PAGE)-1) 
               return 1;

            if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_PERSON)
            {
                mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `userid`=%i AND `id` < %i ORDER BY `id` DESC LIMIT 16", GetPVarInt(playerid, #MDC_ID), MdcInfo[playerid][mdcList][15]);
                mysql_tquery(dbHandle, query, "MDC_ShowMenu", "ii", playerid, 0);
                MdcInfo[playerid][mdcListPage]++;            
            }
            else if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_VEHICLE)
            {
                mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i AND `id` < %i ORDER BY `id` DESC LIMIT 16", GetPVarInt(playerid, #MDC_ID), MdcInfo[playerid][mdcList][15]);
                mysql_tquery(dbHandle, query, "MDC_ShowMenu", "ii", playerid, 0);
                MdcInfo[playerid][mdcListPage]++; 
            }
            return 1;
        }
        else if (MdcInfo[playerid][mdcPage] == PAGE_LOOKUP_VEHICLES_LIST)
        {
            if (MdcInfo[playerid][mdcListPage] == GetPVarInt(playerid, #MAX_MDC_PAGE)-1) return 1;

            mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `owner` = %i AND `registered`=1 AND `id` > %i LIMIT 3", GetPVarInt(playerid, #MDC_OWNER_ID), MdcInfo[playerid][mdcList][2]);
            mysql_tquery(dbHandle, query, "MDCSearchVehiclesList", "i", playerid);
            
            MdcInfo[playerid][mdcListPage]++;
            return 1;
        }
        else if (MdcInfo[playerid][mdcPage] == PAGE_APB)
        {
            if (MdcInfo[playerid][mdcListPage] == GetPVarInt(playerid, #MAX_MDC_PAGE)-1) return 1;
            for(new apb = 0; apb < 20; apb++)  PlayerTextDrawHide(playerid, MDC_APB_TD[playerid][apb]);

            new count,
                string[256],
                text[128]
            ;

            GetPVarString(playerid, #MDC_APB_TEXT, text, 128);           

            for(new i = MdcInfo[4][mdcList]-1; i >= 0; i--)
            {
                if (ApbInfo[i][apbOn] == 0) continue;
                
                if (count < 5)
                {
                    if (GetPVarInt(playerid, #MDC_APB_S))
                        if (strfind(ApbInfo[i][apbCharge], text, true) != -1) continue;

                    format(string, sizeof(string), "%s, #%i~n~%.60s%s~n~%s, %s", ApbInfo[i][apbSuspect], ApbInfo[i][apbID]+90214, ApbInfo[i][apbCharge], (strlen(ApbInfo[i][apbCharge]) > 60)?("_.."):("_"), ApbInfo[i][apbOfficer], ApbInfo[i][apbDate]);
                    PlayerTextDrawSetString(playerid, MDC_APB_TD[playerid][count*4+1], string);

                    MdcInfo[count][mdcList] = i;

                    for(new j = count*4; j < count*4+4; j++) PlayerTextDrawShow(playerid, MDC_APB_TD[playerid][j]);
                }
                else break;
                count++;
            }
            
            MdcInfo[playerid][mdcListPage]++;
            
            new strpage[7];
            format(strpage, sizeof(strpage), "%i_/_%i", MdcInfo[playerid][mdcListPage]+1, GetPVarInt(playerid, #MAX_MDC_PAGE));
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], strpage);
            return 1;
        }
        else if (MdcInfo[playerid][mdcPage] == PAGE_CALLS)
        {
            if (MdcInfo[playerid][mdcListPage] == GetPVarInt(playerid, #MAX_MDC_PAGE)-1) return 1;
            for(new apb = 0; apb < 20; apb++)  PlayerTextDrawHide(playerid, MDC_CALLS_TD[playerid][apb]);

            new 
                count,
                string[256]
            ;           

            for(new i = MdcInfo[4][mdcList]-1; i >= 0; i--)
            {
                if (CallInfo[i][callExists] == 0) continue;
                
                if (count < 5)
                {
                    new statusmes[65];
                    if (CallInfo[i][callAccept]) format(statusmes, sizeof(statusmes), "%s", CallInfo[i][callAcceptBy]);
                    else format(statusmes, sizeof(statusmes), "~r~APPEL NON ACCEPT…");

                    switch(CallInfo[i][callType])
                    {
                        case 1: format(string, sizeof(string), "SAPD");
                        case 2: format(string, sizeof(string), "SAFD");
                        case 3: format(string, sizeof(string), "SAPD, SAFD");
                    }

                    format(string, sizeof(string), "%s (Tel. %i)~n~%s~n~%s~n~%.60s%s~n~%s~n~%s", 
                        CallInfo[i][callName],
                        CallInfo[i][callPhone],
                        string,
                        CallInfo[i][callLocation],
                        CallInfo[i][callText],
                        (strlen(CallInfo[i][callText]) > 60)?(" .."):(""),
                        CallInfo[i][callDate],
                        statusmes
                    );
                    PlayerTextDrawSetString(playerid, MDC_CALLS_TD[playerid][count*4+1], string);
                
                    MdcInfo[count][mdcList] = i;

                    for(new j = count*4; j < count*4+4; j++) PlayerTextDrawShow(playerid, MDC_CALLS_TD[playerid][j]);
                    if (CallInfo[i][callAccept]) PlayerTextDrawHide(playerid, MDC_CALLS_TD[playerid][count*4+2]);
                }
                else break;
                count++;
            }
            
            MdcInfo[playerid][mdcListPage]++;
            
            new strpage[7];
            format(strpage, sizeof(strpage), "%i_/_%i", MdcInfo[playerid][mdcListPage]+1, GetPVarInt(playerid, #MAX_MDC_PAGE));
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], strpage);
            return 1;
        }
        else if (MdcInfo[playerid][mdcPage] == PAGE_CCTV)
        {
            mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cctvs` WHERE `id` < %i ORDER BY `id` DESC LIMIT 16", MdcInfo[playerid][mdcList][15]);
            mysql_tquery(dbHandle, query, "MDC_ShowCCTVMenu", "ii", playerid, 0);
            MdcInfo[playerid][mdcListPage]++; 
            return 1;
        }
    }
    if (playertextid == MDC_LOOKUP_VEHICLES_ARROWS[playerid][2]) //--
    {
        if (!MdcInfo[playerid][mdcListPage]) return 1;

        new query[164];

        if (MdcInfo[playerid][mdcPage] == PAGE_MENU)
        {
            if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_PERSON)
            {
                mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `userid`=%i AND `id` > %i LIMIT 16", GetPVarInt(playerid, #MDC_ID), MdcInfo[playerid][mdcList][0]);
                mysql_tquery(dbHandle, query, "MDC_ShowMenu", "ii", playerid, 1);
                MdcInfo[playerid][mdcListPage]--;
            }
            else if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_VEHICLE)
            {
                mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i AND `id` > %i LIMIT 16", GetPVarInt(playerid, #MDC_ID), MdcInfo[playerid][mdcList][0]);
                mysql_tquery(dbHandle, query, "MDC_ShowMenu", "ii", playerid, 1);
                MdcInfo[playerid][mdcListPage]--;
            }
            return 1;
        }
        else if (MdcInfo[playerid][mdcPage] == PAGE_LOOKUP_VEHICLES_LIST)
        {
            mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `owner` = %i AND `registered`=1 AND `id` < %i LIMIT 3", GetPVarInt(playerid, #MDC_OWNER_ID), MdcInfo[playerid][mdcList][0]);
            mysql_tquery(dbHandle, query, "MDCSearchVehiclesList", "i", playerid);
            MdcInfo[playerid][mdcListPage]--;
            return 1;
        }
        else if (MdcInfo[playerid][mdcPage] == PAGE_APB)
        {
            for(new apb = 0; apb < 20; apb++)  PlayerTextDrawHide(playerid, MDC_APB_TD[playerid][apb]);

            new count,
                string[256],
                text[128]
            ;

            GetPVarString(playerid, #MDC_APB_TEXT, text, 128);

            for(new i = MdcInfo[0][mdcList]+1; i != MAX_APBS; i++)
            {
                if (ApbInfo[i][apbOn] == 0) continue;
                
                if (count < 5)
                {
                    if (GetPVarInt(playerid, #MDC_APB_S))
                        if (strfind(ApbInfo[i][apbCharge], text, true) != -1) continue;
                        
                    format(string, sizeof(string), "%s, #%i~n~%.60s%s~n~%s, %s", ApbInfo[i][apbSuspect], ApbInfo[i][apbID]+90214, ApbInfo[i][apbCharge], (strlen(ApbInfo[i][apbCharge]) > 60)?("_.."):("_"), ApbInfo[i][apbOfficer], ApbInfo[i][apbDate]);
                    PlayerTextDrawSetString(playerid, MDC_APB_TD[playerid][17 - count*4], string);

                    MdcInfo[4-count][mdcList] = i;
                    
                    for(new j = 19 - count*4; j > 19 - (count+1)*4; j--) PlayerTextDrawShow(playerid, MDC_APB_TD[playerid][j]);
                }
                else break;

                count++;
            }
            
            MdcInfo[playerid][mdcListPage]--;
            
            new strpage[7];
            format(strpage, sizeof(strpage), "%i_/_%i", MdcInfo[playerid][mdcListPage]+1, GetPVarInt(playerid, #MAX_MDC_PAGE));
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], strpage);
            return 1;
        }
        
        else if (MdcInfo[playerid][mdcPage] == PAGE_CALLS)
        {
            for(new apb = 0; apb < 20; apb++)  PlayerTextDrawHide(playerid, MDC_CALLS_TD[playerid][apb]);

            new 
                count,
                string[256]
            ;           

            for(new i = MdcInfo[0][mdcList]+5; i != MAX_CALLS_AND_ARREST; i--)
            {
                if (CallInfo[i][callExists] == 0) continue;
                
                new statusmes[65];
                if (CallInfo[i][callAccept]) format(statusmes, sizeof(statusmes), "%s", CallInfo[i][callAcceptBy]);
                else format(statusmes, sizeof(statusmes), "~r~APPEL NON ACCEPT…");

                switch(CallInfo[i][callType])
                {
                    case 1: format(string, sizeof(string), "SAPD");
                    case 2: format(string, sizeof(string), "SAFD");
                    case 3: format(string, sizeof(string), "SAPD, SAFD");
                }

                if (count < 5)
                {
                    format(string, sizeof(string), "%s (Tel. %i)~n~%s~n~%s~n~%.60s%s~n~%s~n~%s", 
                        CallInfo[i][callName],
                        CallInfo[i][callPhone],
                        string,
                        CallInfo[i][callLocation],
                        CallInfo[i][callText],
                        (strlen(CallInfo[i][callText]) > 60)?(" .."):(""),
                        CallInfo[i][callDate],
                        statusmes
                    );
                    PlayerTextDrawSetString(playerid, MDC_CALLS_TD[playerid][count*4+1], string);

                    MdcInfo[count][mdcList] = i;

                    for(new j = count*4; j < count*4+4; j++) PlayerTextDrawShow(playerid, MDC_CALLS_TD[playerid][j]);
                    if (CallInfo[i][callAccept]) PlayerTextDrawHide(playerid, MDC_CALLS_TD[playerid][count*4+2]);
                }
                else break;
                count++;
            }
            
            MdcInfo[playerid][mdcListPage]--;
            
            new strpage[7];
            format(strpage, sizeof(strpage), "%i_/_%i", MdcInfo[playerid][mdcListPage]+1, GetPVarInt(playerid, #MAX_MDC_PAGE));
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], strpage);
            return 1;
        }
        else if (MdcInfo[playerid][mdcPage] == PAGE_CCTV)
        {
            mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cctvs` WHERE `id` > %i LIMIT 16", MdcInfo[playerid][mdcList][0]);
            mysql_tquery(dbHandle, query, "MDC_ShowCCTVMenu", "ii", playerid, 1);
            MdcInfo[playerid][mdcListPage]--;
            return 1;
        }
    }

    if (playertextid == MDC_LOOKUP_VEHICLE[playerid][7]) // Œ¡‹ﬂ¬»“‹ “— ¬ –Œ«€— 
    {
        new plates[24], query[156];
        GetPVarString(playerid, #MDC_VEHICLE_NUM, plates, 24);

        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `plate` = '%e' LIMIT 1", plates);
        mysql_tquery(dbHandle, query, "MDC_Veh_Wanted", "i", playerid);
        return 1;
    }

    if (playertextid == MDC_LOOKUP_VEHICLE[playerid][6]) //”ƒ¿À»“‹ «¿Ã≈“ ”");
    {
        new plates[24], query[156];
        GetPVarString(playerid, #MDC_VEHICLE_NUM, plates, 24);

        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `plate` = '%e' LIMIT 1", plates);
        mysql_tquery(dbHandle, query, "MDC_DELETE_NOTE", "i", playerid);
        return 1;
    }

    if (playertextid == MDC_LOOKUP_VEHICLE[playerid][5]) //ƒŒ¡¿¬»“‹ «¿Ã≈“ ”");
    {
        new plates[24], query[156];
        GetPVarString(playerid, #MDC_VEHICLE_NUM, plates, 24);

        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cars` WHERE `plate` = '%e' LIMIT 1", plates);
        mysql_tquery(dbHandle, query, "MDC_NEW_NOTE", "i", playerid);
        return 1;
    }

    if (playertextid == MDC_LOOKUP_VEHICLE[playerid][4]) // œÓÒÏÓÚ ¯Ú‡ÙÓ‚
    {
        new query[255];
        mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `tickets` WHERE `vehicle`=%i ORDER BY `id` DESC LIMIT 16", GetPVarInt(playerid, #MDC_ID));
        mysql_tquery(dbHandle, query, "MDC_ShowMenu", "ii", playerid, 0);

        MdcInfo[playerid][mdcListPage] = 0;

        new Float:countticket = MDC_CountVehicleTickets(GetPVarInt(playerid, #MDC_ID));
        countticket = countticket/16;
        SetPVarInt(playerid, #MAX_MDC_PAGE, floatround(countticket, floatround_ceil));

        SetPVarInt(playerid, #MDC_FINE, 1);
        
        return 1;
    }    

    for(new j = 0; j < 16; j++)
    {
        if (playertextid == MDC_LIST[playerid][j])
        {
            new query[156];

            if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_PERSON)
            {
                if (GetPVarInt(playerid, #MDC_FINE))
                {
                    mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =tickets.userid) as ownername FROM `tickets` WHERE `userid`=%i AND `id`=%i LIMIT 1", 
                        GetPVarInt(playerid, #MDC_ID), 
                        MdcInfo[playerid][mdcList][j]
                    );
                    return mysql_tquery(dbHandle, query, "MDC_ShowInfo", "i", playerid);
                }
                else 
                {
                    mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =criminalrecords.userid) as ownername FROM `criminalrecords` WHERE `userid`=%i AND `id`=%i LIMIT 1", 
                        GetPVarInt(playerid, #MDC_ID), 
                        MdcInfo[playerid][mdcList][j]
                    );
                    return mysql_tquery(dbHandle, query, "MDC_ShowInfo", "i", playerid);
                }
                
            }
            else if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_VEHICLE)
            {
                mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `model` FROM `cars` WHERE id =tickets.vehicle) as vehiclemodel FROM `tickets` WHERE `vehicle`=%i AND `id`=%i LIMIT 1", 
                    GetPVarInt(playerid, #MDC_ID), 
                    MdcInfo[playerid][mdcList][j]);
                return mysql_tquery(dbHandle, query, "MDC_ShowInfo", "i", playerid);
            } 
            else if (MdcInfo[playerid][mdcPage] == PAGE_CCTV)
            {
                new camid[10];

                for(new cam = 0; cam < MAX_CCTV; cam++)
                {
                    if (CCTVInfo[cam][tvID] == MdcInfo[playerid][mdcList][j])
                    {
                        format(camid, sizeof(camid), "show %i", cam);
                        break;
                    }
                }
                cmd::cctv(playerid, camid);
                //SM1(playerid, "/cctv %s", camid);
            }

            return 1;
        }
    }

    for(new r = 0; r < 18; r++)
    {
        if (playertextid == MDC_TD_ROASTER[playerid][r])
        {
            new count;

		    for(new i = 0; i != MAX_PATROL; i++)
		    {
				if (!PatrolInfo[GetPatrolID(playerid)][i][patrulExists]) continue;
				count++;
			}

			if (!count) return 1;
            
            if (pTemp[playerid][pPatrol])
            {
                cmd::callsign(playerid, "");
                return MDC_PAGE(playerid, PAGE_ROASTER);
            }

            new roaster = MdcInfo[playerid][mdcList][count];
            if(roaster > 19) return SendErrorMessage(playerid, "Rechargez la page !");

            if (PatrolInfo[GetPatrolID(playerid)][roaster][patrulOfficer][0] != INVALID_PLAYER_ID && PatrolInfo[GetPatrolID(playerid)][roaster][patrulOfficer][1] != INVALID_PLAYER_ID)
                return SendErrorMessage(playerid, "Cette patrouille est dÈj‡ complËte.");

            if (PatrolInfo[GetPatrolID(playerid)][roaster][patrulOfficer][0] == INVALID_PLAYER_ID) 
                PatrolInfo[GetPatrolID(playerid)][roaster][patrulOfficer][0] = playerid;
            else
                PatrolInfo[GetPatrolID(playerid)][roaster][patrulOfficer][1] = playerid;

            pTemp[playerid][pPatrol] = roaster + 1;

            SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_POLICECHAT, "[DISP] %s %s a rejoint la patrouille sous l'indicatif %s", GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][roaster][patrulName]);

            MDC_PAGE(playerid, PAGE_ROASTER);
            return 1;
        }
    }

    if (playertextid == MDC_TD_ROASTER_OTHER[playerid][1])
    {
        if (pTemp[playerid][pPatrol])
        {
            cmd::callsign(playerid, "");
            MDC_PAGE(playerid, PAGE_ROASTER);
        }
        else Dialog_Show(playerid, MDC_ROASTER_CHANGE, DIALOG_STYLE_INPUT, "Modifier l'indicatif", "{FFFFFF}Entrez un nouvel indicatif†:", "Valider", "Annuler");
        
        return 1;
    }
    
    for(new apb = 0; apb < 5; apb++)
    {
        if (playertextid == MDC_APB_TD[playerid][2+4*apb]) // ƒÂÚ‡ÎË
        {
            new
                mes[256],
                i_apb = MdcInfo[apb][mdcList]
            ;

            SetPVarInt(playerid, #MDC_ID, apb);

            format(mes, sizeof(mes), 
                "{A9C4E4}#%i\n\n\
                {A9C4E4}Suspect :{ffffff}\t %s\n\
                {A9C4E4}Description :{ffffff}\t %s\n\n\
                {A9C4E4}AjoutÈ par :{ffffff}\t %s\n\
                {A9C4E4}Date :{ffffff}\t\t %s\n",
                ApbInfo[i_apb][apbID]+90214, 
                ApbInfo[i_apb][apbSuspect], 
                ApbInfo[i_apb][apbCharge], 
                ApbInfo[i_apb][apbOfficer], 
                ApbInfo[i_apb][apbDate]);

            Dialog_Show(playerid, MDC_APB_DETAIL, DIALOG_STYLE_MSGBOX, "{098d18}APB", mes, "Modifier", "Annuler");

        }
        else if (playertextid == MDC_APB_TD[playerid][3+4*apb]) // ”‰‡ÎËÚ¸
        {
            SetPVarInt(playerid, #MDC_ID, MdcInfo[apb][mdcList]);
            ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Voulez-vous vraiment supprimer l'APB ?", "MDC_DELETE_APB");
        }
        else if (playertextid == MDC_CALLS_TD[playerid][2+4*apb]) // handle
        {
            new 
                string[512],
                id = MdcInfo[apb][mdcList]
            ;
            
            if (pTemp[playerid][pPatrol])
                SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_LSPD, "* [%s] %s %s [%s] a pris l'appel 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol] - 1][patrulName], id + 1);
            else
                SendFactionMessage(PlayerInfo[playerid][pFaction], COLOR_LSPD, "* [%s] %s %s a pris l'appel 911. #%i.", GetInitials(Factions[PlayerInfo[playerid][pFaction]][fName]), GetRankName(playerid), GetNameEx(playerid),  id + 1);
            
            Waypoint_Set(playerid, CallInfo[id][callPosX],CallInfo[id][callPosY],CallInfo[id][callPosZ], 3.0);

            CallInfo[id][callAccept]++;
            pTemp[playerid][pPoliceCalls]++;

            switch(CallInfo[id][callType])
            {
                case 1: format(string, sizeof(string), "SAPD");
                case 2: format(string, sizeof(string), "SAFD");
                case 3: format(string, sizeof(string), "SAPD, SAFD");
            }

            if (pTemp[playerid][pPatrol])    format(CallInfo[id][callAcceptBy], 64, "Appel pris par %s (%s)", GetNameEx(playerid), PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName]);
            else                            format(CallInfo[id][callAcceptBy], 64, "Appel pris par %s", GetNameEx(playerid));
            
            format(string, sizeof(string), "%s (“ÂÎ. %i)~n~%s~n~%s~n~%.60s%s~n~%s~n~%s", CallInfo[id][callName], CallInfo[id][callPhone], string, CallInfo[id][callLocation], CallInfo[id][callText], (strlen(CallInfo[id][callText]) > 60)?(" .."):(""), CallInfo[id][callDate], CallInfo[id][callAcceptBy]);
            PlayerTextDrawSetString(playerid, MDC_CALLS_TD[playerid][apb*4+1], string);

            PlayerTextDrawHide(playerid, MDC_CALLS_TD[playerid][apb*4 + 2]);
        }
        else if (playertextid == MDC_CALLS_TD[playerid][3+4*apb]) // details
        {
            new 
                mes[512], 
                id = MdcInfo[apb][mdcList]//, type = (PlayerInfo[playerid][pFaction] == LSFD)?(2):(1)
            ;

            new statusmes[65];
			if (CallInfo[id][callAccept]) format(statusmes, sizeof(statusmes), "%s", CallInfo[id][callAcceptBy]);
			else format(statusmes, sizeof(statusmes), "{A9C4E4}Appel non pris");

            format(mes, sizeof(mes), "{B4B5B7}911 - INFORMATIONS SUR L'APPEL - #%i\n\n\
                                    Appel de :\t\t%s\n\
                                    NumÈro de tÈlÈphone :\t%i\n\
                                    Situation :\t\t%s\n\
                                    Localisation :\t%s\n\
                                    Date :\t\t\t%s\n\n\
                                    Statut :\t\t\t%s\n\
                                    Nombre de rÈponses : %i", 
            id+1, CallInfo[id][callName], CallInfo[id][callPhone], CallInfo[id][callText], CallInfo[id][callLocation], CallInfo[id][callDate], statusmes, CallInfo[id][callAccept]);

            Dialog_Show(playerid, MDCCall2, DIALOG_STYLE_MSGBOX, "{8D8DFF}MDC - Informations sur l'appel", mes, "Options", "Retour");

            SetPVarInt(playerid, #last_call, id);
        }
    }
    
    for(new f = 0; f < 4; f++)
    {
        if (playertextid == MDC_LIC[playerid][5+7*f]) // RETIRER LICENCE
        {
            SetPVarInt(playerid, #MDC_LIC, f);
            switch(f)
            {
                case 0: return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment retirer le permis de conduire ?", "MDC_LIC_LOADING");
                case 1: 
                {
                    if (PlayerInfo[playerid][pAuthwep] != 1) 
                        return SendErrorMessage(playerid, "Vous n'avez pas les droits pour annuler une licence d'arme.");
                    else    
                        return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment retirer la licence d'arme ?", "MDC_LIC_LOADING");
                }
                case 2: return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment retirer la licence de pilote ?", "MDC_LIC_LOADING");
                case 3: return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment retirer la licence de taxi ?", "MDC_LIC_LOADING");
            }
        }

        if (playertextid == MDC_LIC[playerid][6+7*f]) // AVERTISSEMENT / LISTE NOIRE
        {
            SetPVarInt(playerid, #MDC_LIC, f+4); 
            switch(f)
            {
                case 0: return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment Èmettre un avertissement\nsur le permis de conduire ?", "MDC_LIC_LOADING");
                case 1: return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment Èmettre un avertissement\nsur la licence d'arme ?", "MDC_LIC_LOADING");
                case 2: return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment ajouter/retirer cette personne\n‡/de la liste noire des pilotes ?", "MDC_LIC_LOADING");
                case 3: return ConfirmDialog(playerid, "Confirmation", "{FFFFFF}Souhaitez-vous vraiment ajouter/retirer cette personne\n‡/de la liste noire des chauffeurs de taxi ?", "MDC_LIC_LOADING");
            }
        }
    }
    return 1;
}
