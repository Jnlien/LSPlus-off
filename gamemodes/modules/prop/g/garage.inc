Garage_OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		for (new i = 0; i < MAX_GARAGES; i++)
		{
			if (!GarageInfo[i][garageID] || GarageInfo[i][garageCP] != checkpointid) continue;

			if (GarageInfo[i][garageOwner])
			{
				SendSyntaxMessage(playerid, "Le garage appartient à %s.", GarageInfo[i][garageOwnerName]);
				SendHelpMessage(playerid, "/enter exit (/g)arage info");

				new mes[64];
				format(mes, sizeof(mes), "~h~~p~Garage~n~%s", GarageInfo[i][garageOwnerName]);

				CreateLableText(playerid, mes, "~h~Appuyez sur ~y~~h~'F' ~w~pour entrer ou~n~entrez ~y~~h~/enter~h~~w~ pour entrer");
			}
			else if (!GarageInfo[i][garageOwner] && !GarageInfo[i][garageHouse] && !GarageInfo[i][garageBiz])
			{
				SendSyntaxMessage(playerid, "Le garage est à vendre. Prix : $%i.", GarageInfo[i][garagePrice]);
				SendHelpMessage(playerid, "/enter (/g)arage info (/g)arage buy");

				new mes[64];
				format(mes, sizeof(mes), "~h~~p~GARAGE À VENDRE!~n~~g~PRIX: ~g~$%i", GarageInfo[i][garagePrice]);

				CreateLableText(playerid, mes, "~h~Appuyez sur ~y~~h~'F' ~w~pour acheter ou~n~entrez ~y~~h~/g buy~h~~w~ pour acheter");
			}
			else if (!GarageInfo[i][garageOwner] && GarageInfo[i][garageHouse])
			{
				SendSyntaxMessage(playerid, "Ce garage est vendu avec une maison.");
				SendHelpMessage(playerid, "/enter (/g)arage info");

				new mes[64];
				format(mes, sizeof(mes), "~h~~p~GARAGE À VENDRE!~n~~y~(Avec la maison %i)", GarageInfo[i][garageHouse]);

				CreateLableText(playerid, mes, "~h~Appuyez sur ~y~~h~'F' ~w~pour acheter ou~n~entrez ~y~~h~/g buy~h~~w~ pour acheter");
			}
			else if (!GarageInfo[i][garageOwner] && GarageInfo[i][garageBiz])
			{
				SendSyntaxMessage(playerid, "Ce garage est vendu avec un business.");
				SendHelpMessage(playerid, "/enter (/g)arage info");

				CreateLableText(playerid, "~h~~p~GARAGE À VENDRE!~n~~y~(Avec un business)", "~h~Appuyez sur ~y~~h~'F' ~w~pour acheter ou~n~entrez ~y~~h~/g buy~h~~w~ pour acheter");
			}
			return 1;
		}
	}
	return 1;
}

Garage_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT *, (SELECT name FROM users WHERE id = garage.owner) as ownername FROM `garage`", "LoadDynamicGarage");
}