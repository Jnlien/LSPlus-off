Dialog:AccountRegister(playerid, response, listitem, inputtext[])
{
	if(!response)	KickEx(playerid);
	ClearChatbox(playerid, 50);

	if(isnull(inputtext) || strlen(inputtext) > 24 || strlen(inputtext) < 5) {
		Dialog_Show(playerid, AccountRegister, DIALOG_STYLE_INPUT, "Création de compte", "Veuillez entrer le pseudonyme souhaité (24 caractères max):\n{C7C7C7}Exemple: {FFFFFF}Tommy", "Suivant", "");
		return SendErrorMessage(playerid, "La longueur du nom de compte doit être comprise entre 5 et 24 caractères !");
	}
	
	if(strfind(inputtext, "/") != -1 || strfind(inputtext, "\\") != -1 ||
		strfind(inputtext, ":") != -1 || strfind(inputtext, "*") != -1 ||
		strfind(inputtext, "?") != -1 || strfind(inputtext, "\"") != -1 ||
		strfind(inputtext, "<") != -1 || strfind(inputtext, ">") != -1 ||
		strfind(inputtext, "|") != -1 || isnull(inputtext) || strfind(inputtext, " ") != -1)	{
		
		Dialog_Show(playerid, AccountRegister, DIALOG_STYLE_INPUT, "Création de compte", "Veuillez entrer le pseudonyme souhaité (24 caractères max):\n{C7C7C7}Exemple: {FFFFFF}Tommy", "Suivant", "");
		return SendErrorMessage(playerid, "Caractères non autorisés dans le nom du compte !");
	}

	SetPVarString(playerid, #AccountName, inputtext);
	
	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `accounts` WHERE `login` = '%e'", inputtext);
	mysql_tquery(dbHandle, query, "CheckAccName", "i", playerid);
	return 1;
}

this::CheckAccName(playerid)
{
	if(cache_num_rows()) {
		SendErrorMessage(playerid, "Ce pseudonyme est déjà pris !");
		Dialog_Show(playerid, AccountRegister, DIALOG_STYLE_INPUT, "Création de compte", "Veuillez entrer le pseudonyme souhaité (24 caractères max):\n{C7C7C7}Exemple: {FFFFFF}Tommy", "Suivant", "");
		return 1;
	}

	ClearChatbox(playerid, 50);
	SendServerMessage(playerid, "Veuillez maintenant entrer une adresse e-mail valide.");
	Dialog_Show(playerid, EmailRegister, DIALOG_STYLE_INPUT, "Création de compte", "Veuillez entrer une adresse e-mail valide :", "Suivant", "");
	return 1;
}

Dialog:EmailRegister(playerid, response, listitem, inputtext[])
{
	if(!response)	KickEx(playerid);
	ClearChatbox(playerid, 50);
	
	SetPVarString(playerid, #EmailName, inputtext);

	SendServerMessage(playerid, "Créez un mot de passe sécurisé.");
	Dialog_Show(playerid, PassRegister, DIALOG_STYLE_PASSWORD, "Création de compte", "Créez et entrez un mot de passe sécurisé :", "Suivant", "");
	return 1;
}

Dialog:PassRegister(playerid, response, listitem, inputtext[])
{
	if(!response)	KickEx(playerid);

    if(strlen(inputtext) < 6 || strlen(inputtext) > 32)
    {
        Dialog_Show(playerid, PassRegister, DIALOG_STYLE_PASSWORD, "{FFFFFF}Nouveau mot de passe", "{FFFFFF}Veuillez entrer un mot de passe :", "Suivant", "");
		return SendErrorMessage(playerid, "Le mot de passe doit contenir entre 6 et 32 caractères.");
	}

	for(new i = 0; i < strlen(inputtext); i++)
	{
		switch(inputtext[i])
		{
			case '!', '@', '#', '$','%','^','&','*','(',')','_','+','=','|','[',']','{','}','-','.','`','~','<','>','?',',','/':
			{
			    Dialog_Show(playerid, PassRegister, DIALOG_STYLE_PASSWORD, "Nouveau mot de passe", "Veuillez entrer un mot de passe :", "Suivant", "");
				return SendErrorMessage(playerid, "Caractères non autorisés dans le mot de passe.");
			}
			default: continue;
		}
	}

	new query[512], pass[129], ip_reg[8], name[24], email[24];

	GetPlayerIp(playerid, ip_reg, sizeof(ip_reg));
	GetPVarString(playerid, #AccountName, name, 24);
	GetPVarString(playerid, #EmailName, email, 24);
	
	WP_Hash(pass, sizeof (pass), inputtext);
	mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `accounts` (`login`,`password`,`email`,`reg_ip`,`reg_date`) VALUES ('%s','%e','%e','%s','%e')", name, pass, email, ip_reg, get_date("%dd/%mm/%yyyy, %hh:%ii", gettime()));
	mysql_tquery(dbHandle, query);
	
	ClearChatbox(playerid, 50);
	SendServerMessage(playerid, "Félicitations, vous avez enregistré votre compte avec succès !");
	SendServerMessage(playerid, "Veuillez patienter un instant...");

	SetPlayerName(playerid, name);

	SetTimerEx("CheckAccount", 1500, false, "i", playerid);
	return 1;
}

Dialog:CreateCharacter(playerid, response, listitem, inputtext[])
{
	if(!response)	return ShowCharacterMenu(playerid);

	if(strfind(inputtext, "/") != -1 || strfind(inputtext, "\\") != -1 ||
		strfind(inputtext, ":") != -1 || strfind(inputtext, "*") != -1 ||
		strfind(inputtext, "?") != -1 || strfind(inputtext, "\"") != -1 ||
		strfind(inputtext, "<") != -1 || strfind(inputtext, ">") != -1 ||
		strfind(inputtext, "|") != -1 || isnull(inputtext) || strfind(inputtext, " ") != -1)	{
		Dialog_Show(playerid, CreateCharacter, DIALOG_STYLE_INPUT, "Création de personnage", "Entrez le nom de votre personnage :", "Suivant", "Retour");
		return SendErrorMessage(playerid, "Caractères non autorisés dans le nom du personnage !");
	}
	
	if(!RPnamecheck(inputtext)) {
		Dialog_Show(playerid, CreateCharacter, DIALOG_STYLE_INPUT, "Création de personnage", "Entrez le nom de votre personnage :", "Suivant", "Retour");
		return SendErrorMessage(playerid, "Caractères non autorisés dans le nom du personnage !");
	}

	new underline=0, maxname = strlen(inputtext);

    for(new i=0; i<maxname; i++)
    {
       if(inputtext[i] == '_') underline ++;
    }
    if(underline != 1) return 0;
    inputtext[0] = toupper(inputtext[0]);
    for(new x=1; x<maxname; x++)
    {
        if(inputtext[x] == '_') inputtext[x+1] = toupper(inputtext[x+1]);
        else if(inputtext[x] != '_' && inputtext[x-1] != '_') inputtext[x] = tolower(inputtext[x]);
    }

	SetPVarString(playerid, #CharacterName, inputtext);

	const MAX_SKINS = 312;
    new subString[16];
    static string[MAX_SKINS * sizeof(subString)];

    if (string[0] == EOS) {
        for (new i; i < MAX_SKINS; i++) {
            format(subString, sizeof(subString), "%i\tID: %i\n", i, i);
            strcat(string, subString);
        }
    }

    Dialog_Show(playerid, CreationCharSkinSel, DIALOG_STYLE_PREVIEW_MODEL, "Choisissez l'apparence de votre personnage", string, "Choisir", "Annuler");
	return 1;
}

Dialog:CreationCharSkinSel(playerid, response, listitem, inputtext[])
{
	if (!response) ShowCharacterMenu(playerid);

	new name[MAX_PLAYER_NAME], query[78 - 2 - 2 - 2 + 4 + MAX_PLAYER_NAME + 3];
	GetPVarString(playerid, #CharacterName, name, 24);

	mysql_format(dbHandle, query, sizeof(query), "INSERT INTO `users` (`accountid`,`name`,`skin`,`status`) VALUES (%i,'%e',%i,1)", PlayerInfo[playerid][pAccID], name, listitem);
	mysql_tquery(dbHandle, query);

	ShowCharacterMenu(playerid);
}

Dialog:FirstSpawn(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
    	Dialog_Show(playerid, FirstSpawn, DIALOG_STYLE_LIST, "{8D8DFF}Choisissez votre lieu de spawn permanent", "Los Santos Airport\nGare de Los Santos", "Choisir", "");
		return 1;
	}
	
	PlayerInfo[playerid][pSpawnPoint] = listitem+1;
	SetSpawnInfoEx(playerid, first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][0], first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][1], first_spawn[PlayerInfo[playerid][pSpawnPoint]-1][2]);
	pTemp[playerid][pLogged] = 2;
	
	SQL_SetInteger("users", "spawnpoint", PlayerInfo[playerid][pSpawnPoint], PlayerInfo[playerid][pID]);
	
 	SpawnPlayer(playerid);
	return 1;
}

Dialog:SelectChar(playerid, response, listitem, inputtext[])
{
	if (!response) 						return KickEx(playerid);
	if (listitem < 0 || listitem > 2)	return KickEx(playerid);

	if (login_char_id[playerid][listitem] == 0)
	{	    
		#if REGISTRATION == 0
			ShowCharacterMenu(playerid);
			SendErrorMessage(playerid, "Ce slot est vide.");
		#else
			Dialog_Show(playerid, CreateCharacter, DIALOG_STYLE_INPUT, "Création de personnage", "Entrez le nom de votre personnage :\n {C7C7C7}Exemple : {FFFFFF}Tommy_Vercetti", "Suivant", "Retour");
	    #endif

		return 1;
	}

	SetPVarInt(playerid, #select_char, listitem);

	new mes[128];
	format(mes, sizeof(mes), "{FFFFFF}Souhaitez-vous vraiment vous connecter en tant que - {A9C4E4}%s ?", login_char_name[playerid][listitem]);
	ConfirmDialog(playerid, "Confirmation", mes, "SelectChar2");
	return 1;
}

this::SelectChar2(playerid, response)
{
	if (!response)	return ShowCharacterMenu(playerid);

	new listitem = GetPVarInt(playerid, #select_char);
	if (listitem < 0 || listitem > 2)
		return printf("Catch"), KickEx(playerid);

	new query[256];
	format(query, sizeof(query), "SELECT users.*, accounts.admin, accounts.admin_code, accounts.donate, accounts.helper, accounts.name_changes, accounts.number_changes FROM users, accounts WHERE users.id = %i AND users.accountid = accounts.id", login_char_id[playerid][listitem]);
    mysql_tquery(dbHandle, query, "LoadCharacter", "i", playerid);
	return 1;
}

Dialog:Login(playerid, response, listitem, inputtext[])
{
	if (!response)	
	{
		SendSyntaxMessage(playerid, "Vous avez quitté le serveur.");
  		KickEx(playerid);
	    return 1;
	}

	if (isnull(inputtext))
	{	
		static const str_size[] = "{c7c3c3}SERVEUR : {FFFFFF}Vous avez 5 minutes pour vous connecter\n{c7c3c3}ASTUCE : {FFFFFF}Vous pouvez récupérer votre mot de passe\n                       sur le site : {c7c3c3}"WeburlName"\n                       {FFFFFF}et accéder au panneau de votre compte\n\nEntrez votre mot de passe :";
		new mes[sizeof(str_size)+103];
		
		if (pTemp[playerid][pLoggedTry] != 0) format(mes, sizeof(mes), "{FF6347}[ ! ] {FFFFFF}Vous avez entré un mot de passe incorrect !\n{FF6347}[ ! ] {FFFFFF}Tentatives restantes : {FF6347}%d\n\n%s", 3 - pTemp[playerid][pLoggedTry], str_size);	
		else format(mes, sizeof(mes), str_size);
		
	    
		Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "Bienvenue sur "ServerName"", mes, "Se connecter", "Annuler");
		return 1;
	}

	new query[128 + 129], buf[129];

	WP_Hash(buf, sizeof (buf), inputtext);
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `accounts` WHERE `id` = %i AND `password`='%e'", PlayerInfo[playerid][pAccID], buf);
	mysql_tquery(dbHandle, query, "LoginPlayer", "i", playerid);
	return 1;
}