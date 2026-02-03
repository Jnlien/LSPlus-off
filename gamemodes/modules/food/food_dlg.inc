Dialog:BuyStandFood(playerid, response, listitem, inputtext[])
{
	if (!response)	return 1;

	new price_food[4] = {PRICE_BURGER, PRICE_PIZZA, PRICE_CHICKEN, PRICE_HOTDOG}, name_food[4][8] = {"burger", "pizza", "poulet", "hot-dog"};

	if (GetCash(playerid) < price_food[listitem])					return SendNotCash(playerid);
	if (GiveInvItem(playerid, INV_BURGER + listitem, 1) == -1)	return SendErrorMessage(playerid, "Il n'y a pas de place dans l'inventaire.");

	SendSyntaxMessage(playerid, "Vous avez acheté %s pour $%i.", name_food[listitem], price_food[listitem]);
	GiveCash(playerid, -price_food[listitem]);
	return 1;
}