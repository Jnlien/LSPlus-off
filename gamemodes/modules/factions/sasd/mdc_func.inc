MDC_Hide(playerid)
{
    MdcInfo[playerid][mdcCreated] = 0;
    MdcInfo[playerid][mdcPage] = PAGE_NONE;

    SetPVarInt(playerid, #pDelay, 1);

    for(new i = 0; i < 32; i++)
    {
        PlayerTextDrawDestroy(playerid, MDC_LIC[playerid][i]);
        MDC_LIC[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        if (i < 20) {
            PlayerTextDrawDestroy(playerid, MDC_APB_TD[playerid][i]);
            MDC_APB_TD[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
            PlayerTextDrawDestroy(playerid, MDC_CALLS_TD[playerid][i]);
            MDC_CALLS_TD[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
            if (i < 18) {
                PlayerTextDrawDestroy(playerid, MDC_TD_ROASTER[playerid][i]);
                MDC_TD_ROASTER[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                if (i < 17) {
                    PlayerTextDrawDestroy(playerid, MDC_UI[playerid][i]);
                    MDC_UI[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                    if (i < 16) {
                        PlayerTextDrawDestroy(playerid, MDC_LIST[playerid][i]);
                        MDC_LIST[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                        if (i < 12) {
                            PlayerTextDrawDestroy(playerid, MDC_LOOKUP_PERSON[playerid][i]);
                            MDC_LOOKUP_PERSON[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                            PlayerTextDrawDestroy(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][i]);
                            MDC_LOOKUP_VEHICLES_LIST[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                            if (i < 8) {
                                PlayerTextDrawDestroy(playerid, MDC_LOOKUP_VEHICLE[playerid][i]);
                                MDC_LOOKUP_VEHICLE[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                if (i < 7) {
                                    PlayerTextDrawDestroy(playerid, MDC_MAINSCREEN[playerid][i]);
                                    MDC_MAINSCREEN[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                    if (i < 6) {
                                        PlayerTextDrawDestroy(playerid, MDC_OTHER_ADDRESS[playerid][i]);
                                        MDC_OTHER_ADDRESS[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                        if (i < 5) {
                                            PlayerTextDrawDestroy(playerid, MDC_ADDRESS[playerid][i]);
                                            MDC_ADDRESS[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                            if (i < 4) {
                                                PlayerTextDrawDestroy(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
                                                MDC_LOOKUP_SEARCH[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                                if (i < 3) {
                                                    PlayerTextDrawDestroy(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][i]);
                                                    MDC_LOOKUP_VEHICLES_ARROWS[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                                    PlayerTextDrawDestroy(playerid, MDC_VEH_LIST_CLICK[playerid][i]);
                                                    MDC_VEH_LIST_CLICK[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                                    PlayerTextDrawDestroy(playerid, MDC_LIST_INFO[playerid][i]);
                                                    MDC_LIST_INFO[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                                    PlayerTextDrawDestroy(playerid, MDC_TD_ROASTER_OTHER[playerid][i]);
                                                    MDC_TD_ROASTER_OTHER[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                                    if (i < 2) {
                                                        PlayerTextDrawDestroy(playerid, MDC_LOOKUP_AVATAR[playerid][i]);
                                                        MDC_LOOKUP_AVATAR[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    PlayerTextDrawDestroy(playerid, MDC_LOOKUP_ERROR[playerid]);
    MDC_LOOKUP_ERROR[playerid] = PlayerText:INVALID_TEXT_DRAW;

    PlayerTextDrawDestroy(playerid, MDC_ERROR_CALLS[playerid]);
    MDC_ERROR_CALLS[playerid] = PlayerText:INVALID_TEXT_DRAW;

    PlayerTextDrawDestroy(playerid, MDC_BACK[playerid]);
    MDC_BACK[playerid] = PlayerText:INVALID_TEXT_DRAW;

    CancelSelectTextDraw(playerid);
    SetPVarInt(playerid, #pDelay, 0);

    // PVARS:
    // MDC_OWNER_ID
    // MDC_ID
    // MDC_VEHICLE_NUM
    // MDC_VEH_BACK
    // MDC_MAX_PAGE
    // MDC_FINE
    // MDC_APB_S
    // MDC_APB_TEXT

    return 1;
}

MDC_Show(playerid, page = PAGE_MAIN)
{
    if (GetPVarInt(playerid, #pDelay)) return MDC_Show(playerid, page);
    
    #include "../include/mdc_txd.inc"

    new string[124];
    format(string, sizeof(string), "-_MOBILE DATA COMPUTER_-~n~AUTORISÉ EN TANT QUE~n~%s", GetNameEx(playerid));
    PlayerTextDrawSetString(playerid, MDC_UI[playerid][11], string);

    MdcInfo[playerid][mdcCreated] = 1;
    MdcInfo[playerid][mdcSearch] = 0;

    return MDC_PAGE(playerid, page);
}


MDC_PAGE(playerid, page)
{
    Update_MDC(playerid, page);
    
    switch(page)
    {
        case PAGE_MAIN:
        {
            PlayerTextDrawSetPreviewModel(playerid, MDC_MAINSCREEN[playerid][0], GetPlayerSkin(playerid));
            PlayerTextDrawShow(playerid, MDC_MAINSCREEN[playerid][0]);

            new string[124];
            format(string, sizeof(string), "%s %s", GetRankName(playerid), GetNameEx(playerid));
            PlayerTextDrawSetString(playerid, MDC_MAINSCREEN[playerid][2], string);

            //                             "ÑÎÒÐÓÄÍÈÊÎÂ ÍÀ ÑËÓÆÁÅ:~n~ÎÐÄÅÐÛ:~n~Â ÐÎÇÛÑÊÅ:");
            format(string, sizeof(string), "%i~n~TBA~n~TBA", MembersOnline(playerid));
            PlayerTextDrawSetString(playerid, MDC_MAINSCREEN[playerid][4], string);

            //                            "ÏÐÈÍßÒÎ ÂÛÇÎÂÎÂ:~n~ÀÐÅÑÒÎÂÀÍÎ:~n~ÂÛÏÈÑÀÍÎ ØÒÐÀÔÎÂ:");
            format(string, sizeof(string), "%i~n~%i~n~%i", pTemp[playerid][pPoliceCalls], pTemp[playerid][pPoliceJails], pTemp[playerid][pPoliceFines]);
            PlayerTextDrawSetString(playerid, MDC_MAINSCREEN[playerid][6], string);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Accueil");


            // PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0xa7a7a7FF);
	        // PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0x494949FF); // ÈÌß
        }
        case PAGE_APB:
        {
            new count,
                string[256]
            ;

            SetPVarInt(playerid, #MDC_APB_S, 0);

            for(new i = MAX_APBS-1; i >= 0; i--)
            {
                if (ApbInfo[i][apbOn] == 0) continue;

                if (count < 5)
                {
                    format(string, sizeof(string), "%s, #%i~n~%.60s%s~n~%s, %s", ApbInfo[i][apbSuspect], ApbInfo[i][apbID]+90214, ApbInfo[i][apbCharge], (strlen(ApbInfo[i][apbCharge]) > 60)?("_.."):("_"), ApbInfo[i][apbOfficer], ApbInfo[i][apbDate]);
                    PlayerTextDrawSetString(playerid, MDC_APB_TD[playerid][count*4+1], string);

                    MdcInfo[count][mdcList] = i;

                    for(new j = count*4; j < count*4+4; j++) PlayerTextDrawShow(playerid, MDC_APB_TD[playerid][j]);
                }
                count++;
            }

            PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][0], "CRÉER");
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][1], "PAR NOM");
                                                                            // ÏÎ ÎÏÈÑ.
            MdcInfo[playerid][mdcSearch] = 0;

            if (!count) PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
            else
            {
                if (count > 5)
                {
                    for(new b = 0; b < 3; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);

                    count = floatround(float(count)/5.0, floatround_ceil);
                    SetPVarInt(playerid, #MAX_MDC_PAGE, count);

                    MdcInfo[playerid][mdcListPage] = 0;

                    format(string, sizeof(string), "1_/_%i", count);
                    PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], string);
                }
            }
        }
        case PAGE_CALLS:
        {
            PlayerTextDrawHide(playerid, MDC_ERROR_CALLS[playerid]);

            new
                string[512],
                count,
                type = (IsPlayerFactionMedic(playerid) || IsPlayerFactionFire(playerid))?(2):(1)
            ;

            for(new i = MAX_CALLS_AND_ARREST-1; i >= 0; i--)
            {
                if (CallInfo[i][callExists] == 0) continue;

                if (CallInfo[i][callType] != 3)
                    if (CallInfo[i][callType] != type) continue;

                switch(CallInfo[i][callType])
                {
                    case 1: format(string, sizeof(string), "RCSO");
                    case 2: format(string, sizeof(string), "PARAMEDICS");
                    case 3: format(string, sizeof(string), "RSCO, PARAMEDICS");
                }

                if (count < 5)
                {
                    new statusmes[65];
                    if (CallInfo[i][callAccept]) format(statusmes, sizeof(statusmes), "%s", CallInfo[i][callAcceptBy]);
                    else format(statusmes, sizeof(statusmes), "~r~APPEL NON ACCEPTÉ");

                    format(string, sizeof(string), "%s (Tél. %i)~n~%s~n~%s~n~%.60s%s~n~%s~n~%s",
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
                count++;
            }

            if (!count) 
            {
                PlayerTextDrawShow(playerid, MDC_ERROR_CALLS[playerid]);
                PlayerTextDrawSetString(playerid, MDC_ERROR_CALLS[playerid], "]__ERREUR: AUCUN APPEL POUR L'INSTANT !");
            }

            else
            {
                if (count > 5)
                {
                    for(new b = 0; b < 3; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);

                    count = floatround(float(count)/5.0, floatround_ceil);
                    SetPVarInt(playerid, #MAX_MDC_PAGE, count);

                    MdcInfo[playerid][mdcListPage] = 0;

                    format(string, sizeof(string), "1_/_%i", count);
                    PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], string);
                }
            }
        }
        case PAGE_CCTV:
        {
            new query[255];
            mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `cctvs` ORDER BY `id` DESC LIMIT 16");
            mysql_tquery(dbHandle, query, "MDC_ShowCCTVMenu", "ii", playerid, 0);

            MdcInfo[playerid][mdcListPage] = 0;
            //MdcInfo[playerid][mdcMenu] = PAGE_CCTV;

            new Float:countcctvs = MDC_CountCCTVS();
            countcctvs = countcctvs/16;
            SetPVarInt(playerid, #MAX_MDC_PAGE, floatround(countcctvs, floatround_ceil));
        }
        case PAGE_ROASTER:
        {
            new string[1024], count;

            for(new j = 0; j < 18; j++) PlayerTextDrawHide(playerid, MDC_TD_ROASTER[playerid][j]);
            for(new j = 0; j < MAX_PATROL; j++) MdcInfo[playerid][mdcList][j] = 0;

		    for(new i = 0; i != MAX_PATROL; i++)
		    {
				if (!PatrolInfo[GetPatrolID(playerid)][i][patrulExists]) continue;

				format(string, sizeof(string), "__%s:__", PatrolInfo[GetPatrolID(playerid)][i][patrulName]);

				if (IsPlayerLogged(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0]))
                {
                    new officername[24];
                    format(officername, sizeof(officername), "%s", GetNameEx(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0]));

                    strdel(officername, 1, strfind(officername, " "));
                    strins(officername, ".", 1);

                    format(string, sizeof(string), "%s%s", string, officername);
                }
                if (IsPlayerLogged(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1]))
                {
                    new officername[24];
                    format(officername, sizeof(officername), "%s", GetNameEx(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][1]));

                    strdel(officername, 1, strfind(officername, " "));
                    strins(officername, ".", 1);

                    if (IsPlayerLogged(PatrolInfo[GetPatrolID(playerid)][i][patrulOfficer][0]))
                        format(string, sizeof(string), "%s__,__%s", string, officername);
                    else format(string, sizeof(string), "%s%s", string, officername);
                }
                
                PlayerTextDrawSetString(playerid, MDC_TD_ROASTER[playerid][count], string);
                PlayerTextDrawShow(playerid, MDC_TD_ROASTER[playerid][count]);
                
                MdcInfo[playerid][mdcList][count] = i;
				count++;
			}

			if (!count)
            {
                PlayerTextDrawSetString(playerid, MDC_TD_ROASTER[playerid][0], "_Aucune patrouille active pour le moment..");
                PlayerTextDrawShow(playerid, MDC_TD_ROASTER[playerid][0]);
            }

            if (pTemp[playerid][pPatrol])
            {
                new callsignstr[32];
                format(callsignstr, sizeof(callsignstr), "_%s", PatrolInfo[GetPatrolID(playerid)][pTemp[playerid][pPatrol]-1][patrulName]);
                PlayerTextDrawSetString(playerid, MDC_TD_ROASTER_OTHER[playerid][2], callsignstr);
            }
            else PlayerTextDrawSetString(playerid, MDC_TD_ROASTER_OTHER[playerid][2], "_Non attribué !");
        }
    }
    return 1;
}

Update_MDC(playerid, page)
{
    new oldpage = MdcInfo[playerid][mdcPage];

    if (oldpage == page) return 1;

    if ((oldpage == PAGE_LOOKUP_PERSON || oldpage == PAGE_LOOKUP_VEHICLE || oldpage == PAGE_LOOKUP || oldpage == PAGE_LOOKUP_VEHICLES_LIST) && page == PAGE_LOOKUP)
        return 1;

    MdcInfo[playerid][mdcPage] = page;

    for(new z = 2; z < 13; z++)
    {
        PlayerTextDrawColor(playerid, MDC_UI[playerid][z], 0x494949FF);
        PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][z], 0xa7a7a7FF);
        PlayerTextDrawShow(playerid, MDC_UI[playerid][z]);
    }

    switch(oldpage)
    {
        case PAGE_MAIN:
        {
            for(new y = 0; y < 7; y++)  PlayerTextDrawHide(playerid, MDC_MAINSCREEN[playerid][y]);
        }
        case PAGE_LOOKUP:
        {
            if (page != PAGE_LOOKUP && page != PAGE_LOOKUP_PERSON && page != PAGE_LOOKUP_VEHICLE && page != PAGE_LOOKUP_VEHICLES_LIST)
            {
                for(new i = 0; i < 4; i++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
                PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);
            }
        }
        case PAGE_LOOKUP_PERSON:
        {
            if (page != PAGE_LOOKUP && page != PAGE_LOOKUP_PERSON && page != PAGE_LOOKUP_VEHICLE && page != PAGE_LOOKUP_VEHICLES_LIST)
            {
                for(new i = 0; i < 4; i++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
            }

            for(new a = 0; a < 2; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
            for(new b = 0; b < 12; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][b]);

            PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);
        }
        case PAGE_LOOKUP_VEHICLE:
        {
            if (page != PAGE_LOOKUP && page != PAGE_LOOKUP_PERSON && page != PAGE_LOOKUP_VEHICLE && page != PAGE_LOOKUP_VEHICLES_LIST)
            {
                for(new i = 0; i < 4; i++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
            }
            else
            {
                for(new i = 0; i < 4; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
            }

            for(new a = 0; a < 8; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLE[playerid][a]);

            PlayerTextDrawHide(playerid, MDC_BACK[playerid]);
            PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);
        }
        case PAGE_LOOKUP_VEHICLES_LIST:
        {
            if (page != PAGE_LOOKUP && page != PAGE_LOOKUP_PERSON && page != PAGE_LOOKUP_VEHICLES_LIST && page != PAGE_LOOKUP_VEHICLE)
            {
                for(new i = 0; i < 4; i++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
            }
            //if (page == PAGE_LOOKUP_VEHICLE) PlayerTextDrawShow(playerid, MDC_BACK[playerid]);

            for(new a = 0; a < 12; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][a]);
            for(new b = 0; b < 3; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
            for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);

            PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);
        }
        case PAGE_MENU:
        {
            PlayerTextDrawHide(playerid, MDC_BACK[playerid]);
            for(new a = 0; a < 16; a++)  PlayerTextDrawHide(playerid, MDC_LIST[playerid][a]);

            if (GetPVarInt(playerid, #MAX_MDC_PAGE) > 1)
                for(new b = 0; b < 3; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
        }
        case PAGE_INFO:
        {
            PlayerTextDrawHide(playerid, MDC_BACK[playerid]);
            for(new a = 0; a < 3; a++)  PlayerTextDrawHide(playerid, MDC_LIST_INFO[playerid][a]);
        }
        case PAGE_MAP:
        {
            PlayerTextDrawHide(playerid, MDC_BACK[playerid]);

            for(new j = 0; j < 6; j++) PlayerTextDrawHide(playerid, MDC_OTHER_ADDRESS[playerid][j]);
            for(new i = 0; i < 5; i++) PlayerTextDrawHide(playerid, MDC_ADDRESS[playerid][i]);

            PlayerTextDrawDestroy(playerid, MDC_ADDRESS[playerid][2]);
            MDC_ADDRESS[playerid][2] = PlayerText:INVALID_TEXT_DRAW;
        }
        case PAGE_LIC:
        {
            PlayerTextDrawHide(playerid, MDC_BACK[playerid]);

            for(new i = 0; i < 32; i++) PlayerTextDrawHide(playerid, MDC_LIC[playerid][i]);
        }
        case PAGE_APB:
        {
            for(new a = 0; a < 4; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_SEARCH[playerid][a]);
            for(new b = 0; b < 3; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
            for(new i = 0; i < 20; i++) PlayerTextDrawHide(playerid, MDC_APB_TD[playerid][i]);
            PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);
        }
        case PAGE_CALLS:
        {
            for(new b = 0; b < 3; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
            for(new i = 0; i < 20; i++) PlayerTextDrawHide(playerid, MDC_CALLS_TD[playerid][i]);
            PlayerTextDrawHide(playerid, MDC_ERROR_CALLS[playerid]);
        }

        case PAGE_CCTV:
        {
            PlayerTextDrawHide(playerid, MDC_ERROR_CALLS[playerid]);

            for(new a = 0; a < 16; a++)  PlayerTextDrawHide(playerid, MDC_LIST[playerid][a]);

            if (GetPVarInt(playerid, #MAX_MDC_PAGE) > 1)
                for(new b = 0; b < 3; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
        }

        case PAGE_ROASTER:
        {
            for(new j = 0; j < 3; j++)  PlayerTextDrawHide(playerid, MDC_TD_ROASTER_OTHER[playerid][j]);
            for(new i = 0; i < 18; i++)  PlayerTextDrawHide(playerid, MDC_TD_ROASTER[playerid][i]);
        }
    }

    switch(page)
    {
        case PAGE_MAIN:
        {
            for(new y = 0; y < 7; y++)  PlayerTextDrawShow(playerid, MDC_MAINSCREEN[playerid][y]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][2], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][2], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][2]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Accueil");
        }
        case PAGE_LOOKUP:
        {
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________");
            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Recherche");

            PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][0], "Nom");
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][1], "Véhicule");

            PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0xa7a7a7FF);
	        PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][0], 0x494949FF); // ÈÌß

            PlayerTextDrawColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0x494949FF);
	        PlayerTextDrawBoxColor(playerid, MDC_LOOKUP_SEARCH[playerid][1], 0xa7a7a7FF); // ÒÑ

            for(new i = 0; i < 4; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][i]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            MdcInfo[playerid][mdcSearch] = 0;
            //PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
        }
        case PAGE_LOOKUP_PERSON:
        {
            for(new i = 0; i < 4; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
            //for(new a = 0; a < 2; a++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
            for(new b = 0; b < 12; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_PERSON[playerid][b]);

            PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][10]);
            PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            //PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Recherche");
        }
        case PAGE_LOOKUP_VEHICLE:
        {
            //PlayerTextDrawSetString(playerid, MDC_BACK[playerid], "~<~ ÂÅÐÍÓÒÜÑß ÍÀÇÀÄ");

            if (GetPVarInt(playerid, #MDC_VEH_BACK))
            {
                for(new i = 0; i < 4; i++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
                PlayerTextDrawShow(playerid, MDC_BACK[playerid]);
            }
            else
            {
                for(new i = 0; i < 4; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
                PlayerTextDrawHide(playerid, MDC_BACK[playerid]);
            }

            //for(new a = 0; a < 12; a++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][a]);
            //for(new b = 0; b < 3; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);

            for(new i = 0; i < 8; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLE[playerid][i]);
            PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Recherche");
        }
        case PAGE_LOOKUP_VEHICLES_LIST:
        {
            for(new i = 0; i < 4; i++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][i]);
            //for(new a = 0; a < 12; a++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][a]);
            //for(new b = 0; b < 3; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);

            PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Recherche");
        }
        case PAGE_MENU:
        {
            PlayerTextDrawShow(playerid, MDC_BACK[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            if (GetPVarInt(playerid, #MAX_MDC_PAGE) > 1)
                for(new b = 0; b < 3; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);

            //for(new a = 0; a < 16; a++)  PlayerTextDrawHide(playerid, MDC_LIST[playerid][a]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Liste");
        }
        case PAGE_INFO:
        {
            PlayerTextDrawShow(playerid, MDC_BACK[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            for(new a = 0; a < 3; a++)  PlayerTextDrawShow(playerid, MDC_LIST_INFO[playerid][a]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Recherche");
        }

        case PAGE_MAP:
        {
            PlayerTextDrawShow(playerid, MDC_BACK[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            for(new i = 0; i < 5; i++) if (i != 2) PlayerTextDrawShow(playerid, MDC_ADDRESS[playerid][i]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Carte");
        }

        case PAGE_LIC:
        {
            PlayerTextDrawShow(playerid, MDC_BACK[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][3], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][3], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][3]);

            for(new i = 0; i < 32; i++) PlayerTextDrawShow(playerid, MDC_LIC[playerid][i]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Licences");
        }

        case PAGE_CALLS:
        {
            PlayerTextDrawColor(playerid, MDC_UI[playerid][4], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][4], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][4]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Appels 911");
        }

        case PAGE_APB:
        {
            for(new a = 0; a < 4; a++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_SEARCH[playerid][a]);
            //for(new b = 0; b < 3; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][6], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][6], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][6]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ APB");
            PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________");
        }
        case PAGE_ROASTER:
        {
            PlayerTextDrawColor(playerid, MDC_UI[playerid][7], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][7], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][7]);

            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ Unités actives");

            for(new j = 0; j < 3; j++)  PlayerTextDrawShow(playerid, MDC_TD_ROASTER_OTHER[playerid][j]);
        }
        case PAGE_CCTV:
        {
            PlayerTextDrawSetString(playerid, MDC_ERROR_CALLS[playerid], "LISTE DE TOUS LES CCTV DISPONIBLES :");
            PlayerTextDrawShow(playerid, MDC_ERROR_CALLS[playerid]);

            PlayerTextDrawColor(playerid, MDC_UI[playerid][8], 0xa7a7a7FF);
            PlayerTextDrawBoxColor(playerid, MDC_UI[playerid][8], 0x494949FF);
            PlayerTextDrawShow(playerid, MDC_UI[playerid][8]);
            
            PlayerTextDrawSetString(playerid, MDC_UI[playerid][13], "Police Department MDC ~>~ CCTV");

            if (GetPVarInt(playerid, #MAX_MDC_PAGE) > 1)
                for(new b = 0; b < 3; b++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][b]);
        }
    }

    return 1;
}

MembersOnline(playerid)
{
    new count = 0;
    foreach(new i : Player)
    {
        if (IsPlayerLogged(i) == 0 || PlayerInfo[i][pFaction] != PlayerInfo[playerid][pFaction])	continue;
        count++;
    }
    return count;
}

this::MDCSearchPersonInfo(playerid)
{
    if (!cache_num_rows())
    {
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________");

		for(new a = 0; a < 2; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
		for(new b = 0; b < 12; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][b]);

		for(new c = 0; c < 3; c++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][c]);
        for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);
		for(new d = 0; d < 12; d++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][d]);

		MdcInfo[playerid][mdcPage] = PAGE_LOOKUP;
		return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
    }

    //MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_PERSON;
    MdcInfo[playerid][mdcMenu] = PAGE_LOOKUP_PERSON;

    Update_MDC(playerid, PAGE_LOOKUP_PERSON);
    PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);

    new
        ownername[MAX_PLAYER_NAME],
        strhouse[128],
        strlic[64],
        birthdate[24],
        str[10 + 24 + 128 + 64 + 24],
        count = 0
    ;

    cache_get_field_content(0, "name", ownername, dbHandle, 24);
	PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], ownername);

	if (cache_get_field_content_int(0, "carlic") == 1)
	{
        count++;
	    format(strlic, sizeof(strlic), "Conduite");
	}
	if (cache_get_field_content_int(0, "weplic") == 1)
	{
        if (count > 0) format(strlic, sizeof(strlic), "%s, ", strlic);
	    format(strlic, sizeof(strlic), "%sArmes", strlic);
        count++;
	}
	if (cache_get_field_content_int(0, "flylic") == 1)
	{
	    if (count > 0) format(strlic, sizeof(strlic), "%s, ", strlic);
	    format(strlic, sizeof(strlic), "%sVols", strlic);
        count++;
	}
	if (cache_get_field_content_int(0, "taxilic") == 1)
	{
	    if (count > 0) format(strlic, sizeof(strlic), "%s, ", strlic);
	    format(strlic, sizeof(strlic), "%sTaxi", strlic);
        count++;
	}
    if (!count) format(strlic, sizeof(strlic), "Non trouvé");

	if (cache_get_field_content_int(0, "arrested") || count > 0)
    {
        PlayerTextDrawSetPreviewModel(playerid, MDC_LOOKUP_AVATAR[playerid][1], cache_get_field_content_int(0, "skin"));
        PlayerTextDrawShow(playerid, MDC_LOOKUP_AVATAR[playerid][1]);

        PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][0]);
    }
    else
    {
        PlayerTextDrawShow(playerid, MDC_LOOKUP_AVATAR[playerid][0]);
        PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][1]);
    }

    count = 0;

    for(new i; i < MAX_HOUSES; i++)
	{
	    if (!HouseInfo[i][hID] || HouseInfo[i][hOwnerID] != cache_get_field_content_int(0, "id")) continue;

        count++;
		format(strhouse, sizeof(strhouse), "%s", (!HouseInfo[i][hApart])?(GetHouseZone(i)):(GetApartmentZone(HouseInfo[i][hApart])));
        break;
	}

	if (!count)
    {
        format(strhouse, sizeof(strhouse), "Non trouvé");
        PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][10]);
    }
    else
    {
        if (count > 1) PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][10], "]_PLUSIEURS ADRESSES DISPONIBLES,~n~CLIQUEZ POUR VOIR LES DÉTAILS.");
        else PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][10], "]_CLIQUEZ POUR VOIR~n~LA LOCALISATION DE LA MAISON SUR LA CARTE.");
        PlayerTextDrawShow(playerid, MDC_LOOKUP_PERSON[playerid][10]);
    }

    cache_get_field_content(0, "birthdate", birthdate, dbHandle, 24);

    format(str, sizeof(str), "%s, %s~n~%i~n~%i~n~%s~n~%s", ownername, birthdate, cache_get_field_content_int(0, "number"), cache_get_field_content_int(0, "arrested"), strlic, strhouse);
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][2], str);

    count = cache_get_field_content_int(0, "id");

    new tickets = CountPlayerTicketsID(count);
    if (tickets) {
        format(str, sizeof(str), "~y~%i~w~~n~Amendes impayées !", tickets);
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][11], str);
    } else {
        PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][11]);
    }

    SetPVarInt(playerid, #MDC_ID, count);

    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `criminalrecords` WHERE `userid` = %i", count);
	mysql_tquery(dbHandle, query, "MDCPersonCriminalHistory", "i", playerid);

    return 1;
}

this::MDCPersonCriminalHistory(playerid)
{
	if (!cache_num_rows())
    {
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][8], "- Casier judiciaire vierge.");
        PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][9]);
        return 1;
    }

    new
        count = 0,
        newcount = 0,
        str[5*128]
    ;

    if (cache_num_rows() > 5) count = 5;
    else count = cache_num_rows();

	for(new i; i < count; i++)
	{
        if (cache_get_field_content_int(i, "arrest") == 1)
        {
            new strtemp[124];
            cache_get_field_content(i, "charge", strtemp, dbHandle, 128);

            if (strlen(strtemp) > 36)
                format(str, sizeof(str), "%s-_%s~n~", str, TextTab(strtemp, 30, 36, 0));
            else
                format(str, sizeof(str), "%s-_%s~n~", str, strtemp);

            newcount++;
        }
	}

    if (cache_get_field_content_int(cache_num_rows()-1, "arrest") != 1) PlayerTextDrawShow(playerid, MDC_LOOKUP_PERSON[playerid][9]);
    else                                                               PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][9]);

    if (count == 0) PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][8], "- Casier judiciaire vierge.");
    else if (newcount == 0) PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][8], "- Recherché (lire ~up~).");

    else PlayerTextDrawSetString(playerid, MDC_LOOKUP_PERSON[playerid][8], str);

	return 1;
}

IsPlayerWanted(playerid) 
{
    new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT `arrest` FROM `criminalrecords` WHERE `userid` = %i ORDER BY `id` DESC LIMIT 1", playerid);
	new Cache:cache = mysql_query(dbHandle, query);

    if (!cache_num_rows()) return 0;

	new	arrest = cache_get_field_content_int(0, "arrest", dbHandle);
	
    cache_delete(cache);

    if (arrest != 1) return 1;
    else            return 0;
}

this::MDCSearchVehiclesList_GetID(playerid)
{
    if (!cache_num_rows())
    {
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________");

		for(new a = 0; a < 2; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
		for(new b = 0; b < 12; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][b]);

		for(new c = 0; c < 3; c++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][c]);
        for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);
		for(new d = 0; d < 12; d++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][d]);

		MdcInfo[playerid][mdcPage] = PAGE_LOOKUP;
		return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
    }

    new query[164];
	mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `owner` = %i AND `registered`=1 LIMIT 3", cache_get_field_content_int(0, "id"));
	mysql_tquery(dbHandle, query, "MDCSearchVehiclesList", "i", playerid);

    return 1;
}


this::MDCSearchVehiclesList(playerid) ////// ÎØÈÁÊÀ
{
	if (!cache_num_rows())
	{
		PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________");

		for(new a = 0; a < 2; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
		for(new b = 0; b < 12; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][b]);

		for(new c = 0; c < 3; c++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][c]);
        for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);
		for(new d = 0; d < 12; d++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][d]);

		MdcInfo[playerid][mdcPage] = PAGE_LOOKUP;
		return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
	}

    else if (cache_num_rows() == 1 && MdcInfo[playerid][mdcListPage] == 0)
    {
        new query[156];
        mysql_format(dbHandle, query, sizeof(query), "SELECT *, (SELECT `name` FROM `users` WHERE id =cars.owner) as ownername FROM `cars` WHERE `id` = %i AND `registered`=1", cache_get_field_content_int(0, "id"));
        mysql_tquery(dbHandle, query, "MDCSearchVehicle", "i", playerid);
        return 1;
    }

    PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);

    SetPVarInt(playerid, #MDC_OWNER_ID, cache_get_field_content_int(0, "owner"));

    new ownername[MAX_PLAYER_NAME];
    cache_get_field_content(0, "ownername", ownername);
    
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], ownername);

    for(new j = 0; j < 12; j++) PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][j]);
    for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);

    for(new i = 0; i < cache_num_rows(); i++)
    {
        PlayerTextDrawShow(playerid, MDC_VEH_LIST_CLICK[playerid][i]);

        MdcInfo[playerid][mdcList][i] = cache_get_field_content_int(i, "id");

        new model, plate[24], insurances[11], impounded[7];

        cache_get_field_content(i, "plate", plate);
        model = cache_get_field_content_int(i, "model");

        if (cache_get_field_content_int(i, "wanted")) PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][3+i*4]);
        else                                          PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][3+i*4]);

        switch(cache_get_field_content_int(i, "insurances"))
        {
            case 0: format(insurances, sizeof(insurances), "Aucune");
            case 1: format(insurances, sizeof(insurances), "NIVEAU 1");
            case 2: format(insurances, sizeof(insurances), "NIVEAU 2");
            case 3: format(insurances, sizeof(insurances), "NIVEAU 3");
        }

        switch(cache_get_field_content_int(i, "impound"))
        {
            case 0: format(impounded, sizeof(impounded), "~l~Non");
            case 1: format(impounded, sizeof(impounded), "~r~Oui");
        }

        static const str_size[] = "%s~n~%s~n~%s~n~%s~n~%s";
        new mes[sizeof(str_size) + 32 + MAX_PLAYER_NAME + 24 + 24 + 11 + 7];

        format(mes, sizeof(mes), str_size, VehicleNames[model-400], plate, ownername, insurances, impounded);
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][1+i*4], mes);
        PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][1+i*4]);
        PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][2+i*4]);

        PlayerTextDrawSetPreviewModel(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][0+i*4], model);
        PlayerTextDrawSetPreviewVehCol(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][0+i*4], cache_get_field_content_int(i, "color1"), cache_get_field_content_int(i, "color2"));
        PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][0+i*4]);
    }

    new max_page = floatround(MDC_VehicleCount(cache_get_field_content_int(0, "owner")) / 3.0, floatround_ceil);

    if (max_page > 1)
    {
        for(new a = 0; a < 3; a++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][a]);

        new str[8];
        format(str, sizeof(str), "%i_/_%i", MdcInfo[playerid][mdcListPage]+1, max_page);
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], str);
        SetPVarInt(playerid, #MAX_MDC_PAGE, max_page);
    } else {
        for(new a = 0; a < 3; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][a]);
    }

    Update_MDC(playerid, PAGE_LOOKUP_VEHICLES_LIST);
    return 1;
}

this::MDCSearchVehicle(playerid)
{
	if (!cache_num_rows())
	{
		PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________");

		for(new a = 0; a < 2; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
		for(new b = 0; b < 12; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][b]);

		for(new c = 0; c < 3; c++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][c]);
        for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);
		for(new d = 0; d < 12; d++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][d]);

		MdcInfo[playerid][mdcPage] = PAGE_LOOKUP;
		return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
	}
    PlayerTextDrawHide(playerid, MDC_LOOKUP_ERROR[playerid]);

    SetPVarInt(playerid, #MDC_ID, cache_get_field_content_int(0, "id"));

	new ownername[MAX_PLAYER_NAME], model, plate[24], date[24], insurances[8], impounded[4], count;

	cache_get_field_content(0, "ownername", ownername);
    cache_get_field_content(0, "plate", plate);
    model = cache_get_field_content_int(0, "model");
    cache_get_field_content(0, "date", date);

    if (cache_get_field_content_int(0, "wanted"))
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLE[playerid][7], "~>~ Retirer du véhicule le statut de recherche, STATUT: ~r~ EN RECHERCHE");
    else
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLE[playerid][7], "~>~ Déclarer le véhicule en recherche, STATUT: ~g~ NON RECHERCHÉ");

    switch (cache_get_field_content_int(0, "insurances"))
    {
        case 0: format(insurances, sizeof(insurances), "Aucune");
        case 1: format(insurances, sizeof(insurances), "NIVEAU 1");
        case 2: format(insurances, sizeof(insurances), "NIVEAU 2");
        case 3: format(insurances, sizeof(insurances), "NIVEAU 3");
    }

    switch (cache_get_field_content_int(0, "impound"))
    {
        case 0: format(impounded, sizeof(impounded), "Non");
        case 1: format(impounded, sizeof(impounded), "Oui");
    }

	static const str_size[] = "%s~n~%s~n~%s~n~%s~n~%s~n~%s";
	new mes[sizeof(str_size) + 32 + MAX_PLAYER_NAME + 24 + 24 + 8 + 4];

 	format(mes, sizeof(mes), str_size, VehicleNames[model-400], plate, ownername, insurances, impounded, date);
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLE[playerid][1], mes);

    new notes[5][128], string[640];
    format(string, sizeof(string), "NOTES:");

    for(new i = 0; i < 5; i++)
    {
        new notei[6];
        format(notei, sizeof(notei), "Note%i", i+1);
        cache_get_field_content(0, notei, notes[i]);

        if (!CompareStrings(notes[i], "None"))
        {
            format(string, sizeof(string), "%s~n~-_%s", string, TextTab(notes[i], 60, 65, 0));
            count++;
        }
    }
    if (!count) format(string, sizeof(string), "NOTES:~n~-_Aucune note pour le moment.");
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLE[playerid][3], string);

    PlayerTextDrawSetPreviewModel(playerid, MDC_LOOKUP_VEHICLE[playerid][0], model);
    PlayerTextDrawSetPreviewVehCol(playerid, MDC_LOOKUP_VEHICLE[playerid][0], cache_get_field_content_int(0, "color1"), cache_get_field_content_int(0, "color2"));
    PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLE[playerid][0]);

    format(string, sizeof(string), "]__VOIR LES AMENDES__(AMENDES IMPAYÉES:__%i)", CountVehicleIDTickets(cache_get_field_content_int(0, "id")));
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLE[playerid][4], string);

    SetPVarString(playerid, #MDC_VEHICLE_NUM, plate);
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], plate);

    MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_VEHICLES_LIST;
    MdcInfo[playerid][mdcMenu] = PAGE_LOOKUP_VEHICLE;
    Update_MDC(playerid, PAGE_LOOKUP_VEHICLE);
	return 1;
}

this::MDC_Veh_Wanted(playerid) // ÎØÈÁÊÀ!
{
    new plates[24], query[156];
    GetPVarString(playerid, #MDC_VEHICLE_NUM, plates, 24);

    new veh_wanted = cache_get_field_content_int(0, "wanted");

    if (veh_wanted)
    {
        mysql_format(dbHandle, query, sizeof(query), "UPDATE `cars` SET `wanted` = '0' WHERE `plate` = '%e' AND `registered`=1", plates);
        mysql_tquery(dbHandle, query);
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLE[playerid][7], "~>~ DÉCLARER LE VÉHICULE EN RECHERCHE, STATUT: ~g~ NON RECHERCHÉ");

        SendWarningMessage(playerid, "Le véhicule n'est plus recherché.");
        
        for (new i = 1, j = GetVehiclePoolSize(); i <= j; i++)
        {
            if (VehicleInfo[i][carID] == cache_get_field_content_int(0, "id")) VehicleInfo[i][carWanted] = 0;
        }
    }
    else
    {
        mysql_format(dbHandle, query, sizeof(query), "UPDATE `cars` SET `wanted` = '1' WHERE `plate` = '%e' AND `registered`=1", plates);
        mysql_tquery(dbHandle, query);
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLE[playerid][7], "~>~ RETIRER LA RECHERCHE DU VÉHICULE, STATUT: ~r~ RECHERCHÉ");

        SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "[RECHERCHÉ] Véhicule: %s [%s] a été déclaré recherché, Officier: %s.", VehicleNames[cache_get_field_content_int(0, "model")-400], plates, GetNameEx(playerid));
        SendWarningMessage(playerid, "Le véhicule a été déclaré recherché, avec succès.");

        for (new i = 1, j = GetVehiclePoolSize(); i <= j; i++)
        {
            if (VehicleInfo[i][carID] == cache_get_field_content_int(0, "id")) VehicleInfo[i][carWanted] = 1;
        }
    }

    return 1;
}


this::MDC_NEW_NOTE(playerid)
{
    new
        plates[24];

    GetPVarString(playerid, #MDC_VEHICLE_NUM, plates, 24);

    SetPVarInt(playerid, #MDC_FREE_NOTE, -1);

    new notesadd[5][128];

    for(new i = 0; i < 5; i++)
    {
        new notei[6];
        format(notei, sizeof(notei), "Note%i", i+1);
        cache_get_field_content(0, notei, notesadd[i]);

        if (CompareStrings(notesadd[i], "None"))
        {
            SetPVarInt(playerid, #MDC_FREE_NOTE, i+1);
            break;
        }
    }

    if (GetPVarInt(playerid, #MDC_FREE_NOTE) == -1)
        Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{8D8DFF}MDC  - Ajouter une note", "{B4B5B7}AJOUT D'UNE NOTE SUR LE VÉHICULE\n\nATTENTION !\nToutes les notes sont déjà utilisées !", "X", "");
    else
        Dialog_Show(playerid, MDC_Add_Note, DIALOG_STYLE_INPUT, "{8D8DFF}MDC  - Ajouter une note sur le véhicule", "{B4B5B7}AJOUT D'UNE NOTE SUR LE VÉHICULE\n\nVeuillez entrer le contenu de la note :", "Valider", "Retour");

    return 1;
}

this::MDC_DELETE_NOTE(playerid)
{
    new eplates[24];

    GetPVarString(playerid, #MDC_VEHICLE_NUM, eplates, 24);

    SetPVarInt(playerid, #MDC_FREE_NOTE, -1);

    new notes[5][128], string[35*5+10];
    format(string, sizeof(string), "#\tNote");

    for(new i = 0; i < 5; i++)
    {
        new notei[6];
        format(notei, sizeof(notei), "Note%i", i+1);
        cache_get_field_content(0, notei, notes[i]);

        if (CompareStrings(notes[i], "None"))
            format(notes[i], 128, "Vide");
        else
            SetPVarInt(playerid, #MDC_FREE_NOTE, 1);

        format(string, sizeof(string), "%s\n%i.\t%0.30s", string, i+1, notes[i]);
    }

    if (GetPVarInt(playerid, #MDC_FREE_NOTE) == -1)
        Dialog_Show(playerid, None, DIALOG_STYLE_MSGBOX, "{8D8DFF}MDC  - Suppression de note véhicule", "{B4B5B7}SUPPRESSION DE NOTE SUR VÉHICULE\n\nATTENTION !\nAucune note existante.", "OK", "");
    else
        Dialog_Show(playerid, MDC_Del_Note, DIALOG_STYLE_TABLIST_HEADERS, "{8D8DFF}MDC  - Suppression de note véhicule", string, "Supprimer", "Annuler");

    return 1;
}

/*SHOW_MDC_LIST(playerid)
{
    for(new i = 0; i < 16; i++)
        PlayerTextDrawSetString(playerid, MDC_LIST[playerid][i], MdcInfo[playerid][mdcList][i]);

    return 1;
}*/

this::MDC_ShowCCTVMenu(playerid, back)
{
	new
		string[256]
	;

    for(new j = 0; j < 16; j++)
    {
        PlayerTextDrawHide(playerid, MDC_LIST[playerid][j]);
        MdcInfo[playerid][mdcList][j] = -1;
    }

    if (cache_num_rows())
    {
		if (!back)
		{
            for(new i = 0; i < cache_num_rows(); i++)
            {
                new 
                    info[32]
                ;

                cache_get_field_content(i, "info", info);
                MdcInfo[playerid][mdcList][i] = cache_get_field_content_int(i, "id");

                if (cache_get_field_content_int(i, "break")) format(string, sizeof(string), "~r~HORS SERVICE");
                else format(string, sizeof(string), "~g~EN SERVICE");

                new index[5];

                if (cache_get_field_content_int(i, "id") > 99) format(index, sizeof(index), "0%i", cache_get_field_content_int(i, "id"));
                else if (cache_get_field_content_int(i, "id") > 9) format(index, sizeof(index), "00%i", cache_get_field_content_int(i, "id"));
                else format(index, sizeof(index), "000%i", cache_get_field_content_int(i, "id"));

                format(string, sizeof(string), "__ID:_%s_____________%s______________%s", index, info, string);

                PlayerTextDrawSetString(playerid, MDC_LIST[playerid][i], string);

                PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], 0x494949FF);
                PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], 0xa7a7a7FF);

                PlayerTextDrawShow(playerid, MDC_LIST[playerid][i]);
            }
        }
		else
		{
            for(new i = cache_num_rows()-1; i >= 0; i--)
            {
                new 
                    info[32]
                ;

                cache_get_field_content(i, "info", info);
                MdcInfo[playerid][mdcList][i] = cache_get_field_content_int(i, "id");

                if (cache_get_field_content_int(i, "break")) format(string, sizeof(string), "~r~HORS SERVICE");
                else format(string, sizeof(string), "~g~EN SERVICE___");

                new index[5];
                if (i > 99) format(index, sizeof(index), "0%i", i);
                else if (i > 9) format(index, sizeof(index), "00%i", i);
                else format(index, sizeof(index), "000%i", i);
                    
                format(string, sizeof(string), "__ID: 00%i_____________%s______________%s", index, info, string);

                PlayerTextDrawSetString(playerid, MDC_LIST[playerid][i], string);
                PlayerTextDrawShow(playerid, MDC_LIST[playerid][i]);
            }
		}
    }
    else 
    {
        PlayerTextDrawShow(playerid, MDC_ERROR_CALLS[playerid]);
        return PlayerTextDrawSetString(playerid, MDC_ERROR_CALLS[playerid], "]__ERREUR : AUCUNE DONNÉE POUR LE MOMENT !");
    }

    Update_MDC(playerid, PAGE_CCTV);

    new strpage[7];
    format(strpage, sizeof(strpage), "%i_/_%i", MdcInfo[playerid][mdcListPage]+1, GetPVarInt(playerid, #MAX_MDC_PAGE));
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], strpage);

    if (GetPVarInt(playerid, #MAX_MDC_PAGE) == 1) for(new k = 0; k < 3; k++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][k]);
    else for(new k = 0; k < 3; k++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][k]);

	return 1;
}

this::MDC_ShowMenu(playerid, back)
{
	new
		string[256]
	;

    for(new j = 0; j < 16; j++)
    {
        PlayerTextDrawHide(playerid, MDC_LIST[playerid][j]);
        MdcInfo[playerid][mdcList][j] = -1;
    }

    if (cache_num_rows())
    {
		if (!back)
		{
                for(new i = 0; i < cache_num_rows(); i++)
                {
                    new
                        date[24],
                        reason[41]
                    ;
                    if (GetPVarInt(playerid, #MDC_FINE))
                        cache_get_field_content(i, "reason", reason);
                    else
                        cache_get_field_content(i, "charge", reason);

                    cache_get_field_content(i, "date", date);

                    MdcInfo[playerid][mdcList][i] = cache_get_field_content_int(i, "id");

                    if (strlen(reason) > 40)
                        format(string, sizeof(string), "%s_-_%.40s..", date, reason);
                    else
                        format(string, sizeof(string), "%s_-_%.40s", date, reason);

                    if (GetPVarInt(playerid, #MDC_FINE))
                    {
                        if (!cache_get_field_content_int(i, "paid"))
                        {
                            PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], -1); // Íå îïëà÷åííûå
                            PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], COLOR_LIGHTRED);
                        }
                        else
                        {
                            PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], 255); // Îïëà÷åííûå
                            PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], -1);
                        }
                    }
                    else
                    {
                        if (cache_get_field_content_int(i, "arrest"))
                        {
                            PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], -1); // Àðåñòîâàí
                            PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], COLOR_LIGHTRED);

                            format(string, sizeof(string), "%s_-_Arrêté.", date);
                        }
                        else
                        {
                            PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], 255); // Îáâèíåíèå
                            PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], -1);
                        }
                    }

                    PlayerTextDrawSetString(playerid, MDC_LIST[playerid][i], string);
                    PlayerTextDrawShow(playerid, MDC_LIST[playerid][i]);

                }
        }
		else
		{
            for(new i = cache_num_rows()-1; i >= 0; i--)
            {
                new
                    date[24],
                    reason[41]
                ;

                if (GetPVarInt(playerid, #MDC_FINE))
                    cache_get_field_content(i, "reason", reason);
                else
                    cache_get_field_content(i, "charge", reason);

                cache_get_field_content(i, "date", date);

                MdcInfo[playerid][mdcList][cache_num_rows()-1-i] = cache_get_field_content_int(i, "id");

                if (strlen(reason) > 40)
                    format(string, sizeof(string), "%s_-_%.40s..", date, reason);
                else
                    format(string, sizeof(string), "%s_-_%.40s", date, reason);

                if (GetPVarInt(playerid, #MDC_FINE))
                {
                    if (!cache_get_field_content_int(i, "paid"))
                    {
                        PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], -1); // Íå îïëà÷åííûå
                        PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], COLOR_LIGHTRED);
                        }
                    else
                    {
                        PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], 255); // Îïëà÷åííûå
                        PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], -1);
                    }
                }
                else
                {
                    if (cache_get_field_content_int(i, "arrest"))
                    {
                        PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], -1); // Àðåñòîâàí
                        PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], COLOR_LIGHTRED);

                        format(string, sizeof(string), "%s - Arrêté.", date);
                    }
                    else
                    {
                        PlayerTextDrawColor(playerid, MDC_LIST[playerid][i], 255); // Îáâèíåíèå
                        PlayerTextDrawBoxColor(playerid, MDC_LIST[playerid][i], -1);
                    }
                }

                PlayerTextDrawSetString(playerid, MDC_LIST[playerid][i], string);
                PlayerTextDrawShow(playerid, MDC_LIST[playerid][i]);
            }
		}
    }
    // DeletePVar(playerid, #MDC_FINE);
    else return SendWarningMessage(playerid, "Vide!");

    if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_PERSON)
    {
        //new Float:countticket = MDC_CountPlayerTickets(GetPVarInt(playerid, #MDC_ID));
        //countticket = countticket/16;

        MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_PERSON;
        //SetPVarInt(playerid, #MAX_MDC_PAGE, floatround(countticket, floatround_ceil));
    }
    else if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_VEHICLE)
    {
        //new Float:countticket = MDC_CountVehicleTickets(GetPVarInt(playerid, #MDC_ID));
        //countticket = countticket/16;

        MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_VEHICLE;
        //SetPVarInt(playerid, #MAX_MDC_PAGE, floatround(countticket, floatround_ceil));
    }

    Update_MDC(playerid, PAGE_MENU);

    new strpage[7];
    format(strpage, sizeof(strpage), "%i_/_%i", MdcInfo[playerid][mdcListPage]+1, GetPVarInt(playerid, #MAX_MDC_PAGE));
    PlayerTextDrawSetString(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][0], strpage);

    if (GetPVarInt(playerid, #MAX_MDC_PAGE) == 1) for(new k = 0; k < 3; k++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][k]);
    else for(new k = 0; k < 3; k++)  PlayerTextDrawShow(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][k]);

	return 1;
}

this::MDC_ShowInfo(playerid)
{
    if (!cache_num_rows())
    {
        PlayerTextDrawSetString(playerid, MDC_LOOKUP_SEARCH[playerid][2], "_________________");

		for(new a = 0; a < 2; a++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_AVATAR[playerid][a]);
		for(new b = 0; b < 12; b++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_PERSON[playerid][b]);

		for(new c = 0; c < 3; c++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_ARROWS[playerid][c]);
        for(new g = 0; g < 3; g++)  PlayerTextDrawHide(playerid, MDC_VEH_LIST_CLICK[playerid][g]);
		for(new d = 0; d < 12; d++)  PlayerTextDrawHide(playerid, MDC_LOOKUP_VEHICLES_LIST[playerid][d]);

		MdcInfo[playerid][mdcPage] = PAGE_LOOKUP;
		return PlayerTextDrawShow(playerid, MDC_LOOKUP_ERROR[playerid]);
    }

    new
        reason[128],
		mes[256],
        date[24],
        status[20],

        ownername[MAX_PLAYER_NAME],
        officername[MAX_PLAYER_NAME]
	;

    if (MdcInfo[playerid][mdcMenu] == PAGE_LOOKUP_PERSON)
	    cache_get_field_content(0, "ownername", ownername, dbHandle, 24);
    else
        format(ownername, sizeof(ownername), "%s", VehicleNames[cache_get_field_content_int(0, "vehiclemodel")-400]);

    cache_get_field_content(0, "officer", officername);
    cache_get_field_content(0, "date", date);

    if (GetPVarInt(playerid, #MDC_FINE))
    {
        cache_get_field_content(0, "reason", reason);

        if (!cache_get_field_content_int(0, "paid")) format(status, sizeof(status), "~r~Non payé");
        else format(status, sizeof(status), "~g~Payé");

        format(mes, sizeof(mes), "ID de l'amende:~n~Contrevenant:~n~Officier:~n~Montant:~n~Date:~n~Statut:~n~~n~Description:");

        PlayerTextDrawSetString(playerid, MDC_LIST_INFO[playerid][0], mes);

        format(mes, sizeof(mes), "#%i~n~%s~n~%s [%s]~n~$%i~n~%s~n~%s",
            1012441+cache_get_field_content_int(0, "id"),
            ownername,
            officername,
            GetInitials(Factions[cache_get_field_content_int(0, "faction")][fName]),
            cache_get_field_content_int(0, "amount"),
            date,
            status
        );
        PlayerTextDrawSetString(playerid, MDC_LIST_INFO[playerid][1], mes);
    }
    else
    {
        cache_get_field_content(0, "charge", reason);

        if (!cache_get_field_content_int(0, "arrest")) format(status, sizeof(status), "Inculpé");
        else format(status, sizeof(status), "~r~Arrêté");

        format(mes, sizeof(mes), "ID:~n~Suspect:~n~Officier:~n~_~n~Date:~n~Statut:~n~~n~Description:");

        PlayerTextDrawSetString(playerid, MDC_LIST_INFO[playerid][0], mes);

        format(mes, sizeof(mes), "#%i~n~%s~n~%s [%s]~n~_~n~%s~n~%s",
            912441+cache_get_field_content_int(0, "id"),
            ownername,
            officername,
            GetInitials(Factions[cache_get_field_content_int(0, "faction")][fName]),
            //cache_get_field_content_int(0, "amount"),
            date,
            status
        );
        PlayerTextDrawSetString(playerid, MDC_LIST_INFO[playerid][1], mes);
    }


    format(reason, sizeof(reason), "- %s", TextTab(reason, 55, 60, 0));
    PlayerTextDrawSetString(playerid, MDC_LIST_INFO[playerid][2], reason);

    MdcInfo[playerid][mdcBack] = PAGE_MENU;
    Update_MDC(playerid, PAGE_INFO);

	return 1;
}

MDC_CountPlayerRecords(playerid)
{
	new
		query[64]
	;

 	mysql_format(dbHandle, query, sizeof(query), "SELECT COUNT(id) AS total FROM `criminalrecords` WHERE `userid` = %i", PlayerInfo[playerid][pID]);
	new Cache:cache = mysql_query(dbHandle, query),
		count = cache_get_field_content_int(0, "total");
	
	cache_delete(cache);
	return count;
}

MDC_VehicleCount(ownerid)
{
	new
		query[100]
	;

 	mysql_format(dbHandle, query, sizeof(query), "SELECT COUNT(id) AS total FROM `cars` WHERE `owner` = %i AND `registered`=1", PlayerInfo[ownerid][pID]);
	new Cache:cache = mysql_query(dbHandle, query),
		count = cache_get_field_content_int(0, "total");
	
	cache_delete(cache);
	return count;
}

this::MDC_MAP(playerid)
{
    if (!cache_num_rows())
        return 1;
    new
        Float:map_kY = -0.04795376,
        Float:map_kX = 0.0469,

        Float:map_sX = 333.999420,
        Float:map_sY = 295.181152,

        Float:corX, Float:corY,

        count = 0,

        strhouse[80]
    ;

    //for(new j = 1; j < 6; j++) PlayerTextDrawHide(playerid, MDC_OTHER_ADDRESS[playerid][j]);

    for(new i; i < MAX_HOUSES; i++)
	{
	    if (!HouseInfo[i][hID] || HouseInfo[i][hOwnerID] != cache_get_field_content_int(0, "id")) continue;

        if (count == 0)
        {
            format(strhouse, sizeof(strhouse), "%s~n~San Andreas", (!HouseInfo[i][hApart])?(GetHouseZone(i, 1)):(GetApartmentZone(HouseInfo[i][hApart], 1)));
            PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][4], strhouse);

            if (HouseInfo[i][hPosX] > 0)
            {
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][0], "samaps:gtasamapbit2");
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][1], "samaps:gtasamapbit4");
                map_kX = 0.0469;
                map_sX = 333.999420;

                corX = HouseInfo[i][hPosX] * map_kX + map_sX;
                corY = HouseInfo[i][hPosY] * map_kY + map_sY;
            }
            else
            {
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][0], "samaps:gtasamapbit1");
                PlayerTextDrawSetString(playerid, MDC_ADDRESS[playerid][1], "samaps:gtasamapbit3");
                map_kX = -0.0469;
                map_sX = 480.066314;

                corX = map_sX - HouseInfo[i][hPosX] * map_kX;
                corY = HouseInfo[i][hPosY] * map_kY + map_sY;
            }

            MDC_ADDRESS[playerid][2] = CreatePlayerTextDraw(playerid, corX, corY, "hud:radar_waypoint");
            PlayerTextDrawTextSize(playerid, MDC_ADDRESS[playerid][2], 5.000000, 6.000000);
            PlayerTextDrawFont(playerid, MDC_ADDRESS[playerid][2], 4);
            PlayerTextDrawShow(playerid, MDC_ADDRESS[playerid][2]);
        }
        else
        {
            format(strhouse, sizeof(strhouse), "- %.35s", (!HouseInfo[i][hApart])?(GetHouseZone(i, 2)):(GetApartmentZone(HouseInfo[i][hApart], 2)));
            PlayerTextDrawSetString(playerid, MDC_OTHER_ADDRESS[playerid][count], strhouse);
            PlayerTextDrawShow(playerid, MDC_OTHER_ADDRESS[playerid][count]);
        }

        MdcInfo[playerid][mdcList][count] = i;
        count++;
	}
    if (count > 1) PlayerTextDrawShow(playerid, MDC_OTHER_ADDRESS[playerid][0]);

    MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_PERSON;
    Update_MDC(playerid, PAGE_MAP);
    return 1;
}

this::MDC_LICENSE(playerid)
{
    if (!cache_num_rows())
        return 1;

    new
        string[128]
    ;

    switch(cache_get_field_content_int(0, "carlic"))
	{
        case 0: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][4], "~r~Aucune~n~-");
        case 1:
        {
            format(string, sizeof(string), "~g~Active~n~~r~%i", cache_get_field_content_int(0, "drivewarns"));
            PlayerTextDrawSetString(playerid, MDC_LIC[playerid][4], string);
        }
	}

	switch(cache_get_field_content_int(0, "weplic"))
	{
        case 0: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][11], "~r~-~n~-~n~-~n~-");
	    case 1:
        {
            new name[MAX_PLAYER_NAME+1], officer[MAX_PLAYER_NAME+1], data[24], price, warns;

            cache_get_field_content(0, "name", name);
            strreplace(name, '_', ' ');
            cache_get_field_content(0, "wep_officer", officer);
            cache_get_field_content(0, "wep_data", data);
            price = cache_get_field_content_int(0, "wep_price");
            warns = cache_get_field_content_int(0, "wepwarns");

            format(string, sizeof(string), "%i~n~%s~n~%s~n~~g~$%i", warns, name, data, price);
            PlayerTextDrawSetString(playerid, MDC_LIC[playerid][11], string);
        }
	}
	switch(cache_get_field_content_int(0, "flylic"))
    {
        case 0: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][18], "~r~Aucune");
        case 1: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][18], "~g~Active");
        case 2: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][18], "~l~Liste noire");
    }
    switch(cache_get_field_content_int(0, "taxilic"))
    {
        case 0: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][25], "~r~Aucune");
        case 1: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][25], "~g~Active");
        case 2: PlayerTextDrawSetString(playerid, MDC_LIC[playerid][25], "~l~Liste noire");
    }

    MdcInfo[playerid][mdcBack] = PAGE_LOOKUP_PERSON;
    Update_MDC(playerid, PAGE_LIC);

    return 1;
}

this::MDC_LIC_LOADING(playerid, response)
{
    if (!response)    return 1;

    new query[255];
    mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `users` WHERE `id` = %i LIMIT 1", GetPVarInt(playerid, #MDC_ID));
    mysql_tquery(dbHandle, query, "MDC_LIC_DO", "ii", playerid, GetPVarInt(playerid, #MDC_LIC));

    DeletePVar(playerid, #MDC_LIC);
    return 1;
}

this::MDC_LIC_DO(playerid, func)
{
    if (!cache_num_rows())
        return 1;

    new
        userid,
        playeronline,
        name[MAX_PLAYER_NAME+1]
    ;

    if ((userid = GetIDByID(GetPVarInt(playerid, #MDC_ID))) != INVALID_PLAYER_ID)
        playeronline = 1;
    else playeronline = 0;

    cache_get_field_content(0, "name", name);

    switch(func)
    {
        case 0: // Retirer le permis de conduire
        {
            if (!cache_get_field_content_int(0, "carlic")) 
                return SendErrorMessage(playerid, "%s n'a pas de permis de conduire.", name);

            if (playeronline)
            {
                SendSpecialMessage(userid, "%s %s a retiré votre permis de conduire.", GetRankName(playerid), GetNameEx(playerid));
                PlayerInfo[userid][pCarLic] = 0;
                PlayerInfo[userid][pDriveWarns] = 0;
            }

            new query[156];
            mysql_format(dbHandle, query, sizeof(query), "UPDATE `users` SET `carlic`='0',`drivewarns`='0' WHERE `id` = %i", GetPVarInt(playerid, #MDC_ID));
            mysql_tquery(dbHandle, query);

            SendSpecialMessage(playerid, "Vous avez retiré le permis de conduire de %s.", name);
            PlayerTextDrawSetString(playerid, MDC_LIC[playerid][4], "~r~Aucun~n~-");

            return 1;
        }

        case 1: // Retirer licence d'arme
        {
            if (!cache_get_field_content_int(0, "weplic"))
                return SendErrorMessage(playerid, "%s n'a pas de licence d'arme.", name);

            if (playeronline)
            {
                SendSpecialMessage(userid, "%s %s a annulé votre licence d'arme.", GetRankName(playerid), GetNameEx(playerid));
                PlayerInfo[userid][pWepLic] = 0;
                PlayerInfo[userid][pWepWarns] = 0;
                PlayerInfo[userid][pWepCount] = 0;
            }

            new query[156];
            mysql_format(dbHandle, query, sizeof(query), 
                "UPDATE `users` SET `weplic`='0',`wepwarns`='0',`wep_officer`='-',`wep_data`='-',`wep_price`='0',`wep_count`='0' WHERE `id` = %i", 
                GetPVarInt(playerid, #MDC_ID));
            mysql_tquery(dbHandle, query);

            SendSpecialMessage(playerid, "Vous avez annulé la licence d'arme de %s.", name);
            PlayerTextDrawSetString(playerid, MDC_LIC[playerid][11], "~r~-~n~-~n~-~n~-~");

            return 1;
        }

        case 2: // Retirer licence de pilote
        {
            if (!cache_get_field_content_int(0, "flylic"))
                return SendErrorMessage(playerid, "%s n'a pas de licence de pilote.", name);
            if (cache_get_field_content_int(0, "flylic") == 2)
                return SendErrorMessage(playerid, "Impossible de retirer la licence de pilote, %s est en liste noire.", name);

            if (playeronline)
            {
                SendSpecialMessage(userid, "%s %s a retiré votre licence de pilote.", GetRankName(playerid), GetNameEx(playerid));
                PlayerInfo[userid][pFlyLic] = 0;
            }

            SQL_SetInteger("users", "flylic", 0, cache_get_field_content_int(0, "id"));

            SendSpecialMessage(playerid, "Vous avez retiré la licence de pilote de %s.", name);
            PlayerTextDrawSetString(playerid, MDC_LIC[playerid][18], "~r~Non");

            return 1;
        }

        case 3: // Retirer licence taxi
        {
            if (!cache_get_field_content_int(0, "taxilic"))
                return SendErrorMessage(playerid, "%s ne possède pas de licence de chauffeur de taxi.", name);

            if (cache_get_field_content_int(0, "taxilic") == 2)
                return SendErrorMessage(playerid, "Impossible de retirer la licence de taxi car %s est dans la liste noire.", name);

            if (playeronline)
            {
                SendSpecialMessage(userid, "%s %s a retiré votre licence de chauffeur de taxi.", GetRankName(playerid), GetNameEx(playerid));
                PlayerInfo[userid][pTaxiLic] = 0;
            }

            SQL_SetInteger("users", "taxilic", 0, cache_get_field_content_int(0, "id"));

            SendSpecialMessage(playerid, "Vous avez retiré la licence de taxi de %s.", name);
            PlayerTextDrawSetString(playerid, MDC_LIC[playerid][25], "~r~Non");

            return 1;
        }

        case 4: // Avertissement permis de conduire
        {
            if (!cache_get_field_content_int(0, "carlic"))
                return SendErrorMessage(playerid, "%s ne possède pas de permis de conduire.", name);

            SendSpecialMessage(playerid, "Vous avez donné un avertissement sur le permis de conduire de %s.", name);

            new mes[128];
            format(mes, sizeof(mes), "a donné un avertissement sur le permis de conduire de %s.", name);
            cmd::me(playerid, mes);

            if (cache_get_field_content_int(0, "drivewarns") + 1 == 3)
            {
                SendWarningMessage(playerid, "Le permis de %s a été automatiquement révoqué après 3 avertissements.", name);

                SQL_SetInteger("users", "drivewarns", 0, cache_get_field_content_int(0, "id"));
                SQL_SetInteger("users", "carlic", 0, cache_get_field_content_int(0, "id"));

                if (playeronline)
                {
                    SendSpecialMessage(userid, "%s %s vous a donné un avertissement sur votre permis de conduire.", GetRankName(playerid), GetNameEx(playerid));
                    PlayerInfo[userid][pDriveWarns] = 0;
                    PlayerInfo[userid][pCarLic] = 0;
                    SendWarningMessage(userid, "Votre permis a été révoqué automatiquement après 3 avertissements.");
                }

                PlayerTextDrawSetString(playerid, MDC_LIC[playerid][4], "~r~Non~n~-");
                return 1;
            }
            else
            {
                if (playeronline)
                {
                    PlayerInfo[userid][pDriveWarns]++;
                }

                new string[32];
                format(string, sizeof(string), "~g~Active~n~~r~%i", cache_get_field_content_int(0, "drivewarns")+1);
                PlayerTextDrawSetString(playerid, MDC_LIC[playerid][4], string);

                SQL_SetInteger("users", "drivewarns", cache_get_field_content_int(0, "drivewarns")+1, cache_get_field_content_int(0, "id"));
                return 1;
            }
        }

        case 5: // AVERTISSEMENT ARME
        {
            if (!cache_get_field_content_int(0, "weplic"))
                return SendErrorMessage(playerid, "%s ne possède pas de licence d'arme.", name);

            if (playeronline)
            {
                SendSpecialMessage(userid, "%s %s a émis un avertissement sur votre licence d'arme.", GetRankName(playerid), GetNameEx(playerid));
                PlayerInfo[userid][pWepWarns]++;
            }

            SendSpecialMessage(playerid, "Vous avez émis un avertissement sur la licence d'arme de %s.", name);

            new officer[MAX_PLAYER_NAME+1], data[24], price, warns;

            strreplace(name, '_', ' ');
            cache_get_field_content(0, "wep_officer", officer);
            cache_get_field_content(0, "wep_data", data);
            price = cache_get_field_content_int(0, "wep_price");
            warns = cache_get_field_content_int(0, "wepwarns")+1;

            new string[128];
            format(string, sizeof(string), "%i~n~%s~n~%s~n~~g~$%i", warns, name, data, price);
            PlayerTextDrawSetString(playerid, MDC_LIC[playerid][11], string);

            SQL_SetInteger("users", "wepwarns", warns, cache_get_field_content_int(0, "id"));
            return 1;
        }

        case 6: // LISTE NOIRE PILOTE
        {
            if (cache_get_field_content_int(0, "flylic") == 2)
            {
                // RETRAIT DE LA LISTE NOIRE
                if (playeronline)
                {
                    SendSpecialMessage(userid, "%s %s vous a retiré de la liste noire des licences de pilote.", GetRankName(playerid), GetNameEx(playerid));
                    PlayerInfo[userid][pFlyLic] = 0;
                }

                SQL_SetInteger("users", "flylic", 0, cache_get_field_content_int(0, "id"));

                SendSpecialMessage(playerid, "Vous avez retiré %s de la liste noire des licences de pilote.", name);
                PlayerTextDrawSetString(playerid, MDC_LIC[playerid][18], "~r~Non");
            }
            else
            {
                // AJOUT À LA LISTE NOIRE
                if (playeronline)
                {
                    SendSpecialMessage(userid, "%s %s vous a inscrit sur la liste noire des licences de pilote.", GetRankName(playerid), GetNameEx(playerid));
                    PlayerInfo[userid][pFlyLic] = 2;
                }

                SQL_SetInteger("users", "flylic", 2, cache_get_field_content_int(0, "id"));

                SendSpecialMessage(playerid, "Vous avez inscrit %s sur la liste noire des licences de pilote.", name);
                PlayerTextDrawSetString(playerid, MDC_LIC[playerid][18], "~l~Liste noire");
            }

            return 1;
        }

        case 7: // LISTE NOIRE TAXI
        {
            if (cache_get_field_content_int(0, "taxilic") == 2)
            {
                // RETRAIT DE LA LISTE NOIRE
                if (playeronline)
                {
                    SendSpecialMessage(userid, "%s %s vous a retiré de la liste noire des conducteurs de taxi.", GetRankName(playerid), GetNameEx(playerid));
                    PlayerInfo[userid][pTaxiLic] = 0;
                }

                SQL_SetInteger("users", "taxilic", 0, cache_get_field_content_int(0, "id"));

                SendSpecialMessage(playerid, "Vous avez retiré %s de la liste noire des conducteurs de taxi.", name);
                PlayerTextDrawSetString(playerid, MDC_LIC[playerid][25], "~r~Non");
            }
            else
            {
                // AJOUT À LA LISTE NOIRE
                if (playeronline)
                {
                    SendSpecialMessage(userid, "%s %s vous a inscrit sur la liste noire des conducteurs de taxi.", GetRankName(playerid), GetNameEx(playerid));
                    PlayerInfo[userid][pTaxiLic] = 2;
                }

                SQL_SetInteger("users", "taxilic", 2, cache_get_field_content_int(0, "id"));

                SendSpecialMessage(playerid, "Vous avez inscrit %s sur la liste noire des conducteurs de taxi.", name);
                PlayerTextDrawSetString(playerid, MDC_LIC[playerid][25], "~l~Liste noire");
            }

            return 1;
        }

    }
    return 1;
}

CheckTextDrawString(string[])
{
    if (strfind(string, "~", true) != -1)  return 1;

    return 0;
}

this::MDC_DELETE_APB(playerid, response)
{
    if (!response) return 1;

    new number = GetPVarInt(playerid, #MDC_ID);

    if (!ApbInfo[number][apbOn])
        return SendErrorMessage(playerid, "La note APB est introuvable.");

    SendSyntaxMessage(playerid, "Vous avez supprimé la note APB numéro %i.", number);
    SendFactionMessage(FACTION_POLICE, COLOR_POLICECHAT, "* HQ: %s %s a supprimé la note APB numéro %i.", GetRankName(playerid), GetNameEx(playerid), number);

    new query[128];
    mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `apbs` WHERE `id` = %i", ApbInfo[number][apbID]);
    mysql_tquery(dbHandle, query);

    ApbInfo[number][apbOn] = 0;
    ApbInfo[number][apbID] = 0;

    return MDC_PAGE(playerid, PAGE_APB);
}

MDC_CountCCTVS()
{
	new 
		query[64]
	;

 	mysql_format(dbHandle, query, sizeof(query), "SELECT COUNT(id) AS total FROM `cctvs`");
	new Cache:cache = mysql_query(dbHandle, query),
		count = cache_get_field_content_int(0, "total");
	
	cache_delete(cache);
	return count;
}
