Apart_OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
    {
        for(new i = 0; i != MAX_APART; i++)
        {
            if (ApartInfo[i][aApartOn] == 0 || checkpointid != ApartInfo[i][aCheckPoint]) continue;

            //SendSyntaxMessage(playerid, "Vous êtes sur le seuil d'un complexe résidentiel. Adresse : %s, %i.", GetApartmentZone(i), i+1);
            //SendHelpMessage(playerid, "/enter, /(ds)hout, /ddo.");

            new string[128];
            format(string, sizeof(string),"~h~~p~Complexe résidentiel~n~~y~%s", GetApartmentZone(i, 2)); 
            CreateLableText(playerid, string, "~h~Appuyez sur ~y~~h~'F' ~w~pour entrer ou~n~entrez ~y~~h~/h buy~h~~w~ pour accéder");
            return 1;
        }
    }
    return 1;
}

Apart_OnGameModeInit()
{
    mysql_tquery(dbHandle, "SELECT * FROM `apartment`", "LoadDynamicApartment");
    return 1;
}