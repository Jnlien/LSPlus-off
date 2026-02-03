Interior_OnGameModeInit()
{
	buy_interior = CreateMenu("_", 1, 20.0, 160.0, 80.0);
	AddMenuItem(buy_interior, 0, "> SUIVANT");
	AddMenuItem(buy_interior, 0, "> RETOUR");
	AddMenuItem(buy_interior, 0, "> SÉLECTIONNER");
	AddMenuItem(buy_interior, 0, "> QUITTER");
	return 1;
}