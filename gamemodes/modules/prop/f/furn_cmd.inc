CMD:furniture(playerid, params[])
{
	if (GetPVarInt(playerid, "buy_furniture")) return SendErrorMessage(playerid, "L'action est déjà en cours.");
		
	new type = GetNearBuilding(playerid);
	if (type == -1) return SendErrorMessage(playerid, "Vous devez être à l'intérieur/près de votre propriété.");

	switch (type)
	{
		case 0: HouseFurn(playerid);
		case 1: BizFurn(playerid);
		case 2: GarageFurn(playerid);
	}

	Check3DMenu(playerid);
	return 1;
}