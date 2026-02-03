CMD:buyfood(playerid, params[])
{
	if (!CheckInv(playerid)) 					return SendErrorMessage(playerid, "Votre inventaire est complètement rempli.");

	for(new i; i != MAX_FOOD_STAND; i++)
	{
		if (FoodStand[i][fStandOn] != 1 || !PlayerToPoint(3, playerid,FoodStand[i][fPosX], FoodStand[i][fPosY], FoodStand[i][fPosZ]) || FoodStand[i][fVW] != GetWorld(playerid) || FoodStand[i][fInt] != GetInterior(playerid)) continue;

		new mes[128];
		format(mes, sizeof(mes), "Burger\t$%i\nPizza\t$%i\nPoulet\t$%i\nHot-Dog\t$%i", PRICE_BURGER, PRICE_PIZZA, PRICE_CHICKEN, PRICE_HOTDOG);
		Dialog_Show(playerid, BuyStandFood, DIALOG_STYLE_TABLIST, "{ffffff}Stand de nourriture", mes, "Acheter", "Annuler");
		
		break;
	}
	return 1;
}