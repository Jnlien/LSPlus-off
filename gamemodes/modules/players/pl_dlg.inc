Dialog:SelectSpawn(playerid, response, listitem, inputtext[])
{
	if (!response)
		return 1;

	SendSyntaxMessage(playerid, "Vous apparaîtrez désormais à l'endroit sélectionné.");
	PlayerInfo[playerid][pSpawnType] = 0;
	PlayerInfo[playerid][pSpawnPoint] = listitem + 1;

	SQL_SetInteger("users", "spawntype", PlayerInfo[playerid][pSpawnType], PlayerInfo[playerid][pID]);
	SQL_SetInteger("users", "spawnpoint", PlayerInfo[playerid][pSpawnPoint], PlayerInfo[playerid][pID]);
	return 1;
}

Dialog:ChangeWalk(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);
	    
	new str[5];
	format(str, sizeof(str), "1 %i", listitem);
	cmd::setstyle(playerid, str);
	ShowSettings(playerid);
	return 1;
}

Dialog:ChangeTalk(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[5];
	format(str, sizeof(str), "2 %i", listitem);
	cmd::setstyle(playerid, str);
	ShowSettings(playerid);
	return 1;
}

Dialog:ChangeFightstyle(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[5];
	format(str, sizeof(str), "3 %i", listitem+1);
	cmd::setstyle(playerid, str);
	ShowSettings(playerid);
	return 1;
}

Dialog:ChangeHud(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[5];
	format(str, sizeof(str), "4 %i", listitem+1);
	cmd::setstyle(playerid, str);
	ShowSettings(playerid);

	// PlayerTextDrawDestroy(playerid, SpeedText[playerid]);
	Speedometr_TDcreate(playerid);
	return 1;
}

Dialog:ChangeSpawn(playerid, response, listitem, inputtext[])
{
	if (!response) return ShowSettings(playerid);

	new str[2];
	format(str, sizeof(str), "%i", listitem);
	cmd::changerspawn(playerid, str);

	if (listitem > 1)
		ShowSettings(playerid);

	return 1;
}

Dialog:MainSettings(playerid, response, listitem, inputtext[])
{
	if (!response)					return cmd::mm(playerid, "");
	if (!IsPlayerLogged(playerid))	return 1;
	    
	switch(listitem)
	{
	    case 0:Dialog_Show(playerid, ChangeWalk, DIALOG_STYLE_LIST, "{FFFFFF}Style de marche (/walk)", "Désactiver\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17", "Choisir", "Retour");
		case 1:Dialog_Show(playerid, ChangeTalk, DIALOG_STYLE_LIST, "{FFFFFF}Style de conversation", "Désactiver\n1\n2\n3\n4\n5\n6\n7\n8\n9", "Choisir", "Retour");
		case 2:Dialog_Show(playerid, ChangeFightstyle, DIALOG_STYLE_LIST, "{FFFFFF}Style de combat", "Normal\nBoxe\nKungfu\nCoup de genou\nCoup de pied sauté", "Choisir", "Retour");
		case 3:Dialog_Show(playerid, ChangeHud, DIALOG_STYLE_LIST, "{FFFFFF}Style de compteur (HUD)", "Police 1, en haut\nPolice 1, en bas\nPolice 2, en haut\nPolice 2, en bas", "Choisir", "Retour");
		case 4:Dialog_Show(playerid, ChangeSpawn, DIALOG_STYLE_LIST, "{FFFFFF}Lieu d'apparition", "Apparition normale\nMaison\nFaction\nDernière déconnexion", "Choisir", "Retour");		
		case 5: cmd::tog(playerid, "ooc");
		case 6: cmd::tog(playerid, "name");
		case 7: cmd::tog(playerid, "ads");
		case 8: cmd::tog(playerid, "join");
		case 9: cmd::tog(playerid, "gov");
		case 10: cmd::tog(playerid, "pm");
		case 11: cmd::tog(playerid, "adm");
		case 12: cmd::tog(playerid, "news");
		case 13: cmd::tog(playerid, "afk");
		case 14: cmd::tog(playerid, "hud");
		case 15: cmd::tog(playerid, "f");
        case 16: cmd::tog(playerid, "enter");
		case 17:
		{
		    if (!CompareStrings(PlayerInfo[playerid][pGoogle], "!"))
		    {
		        format(PlayerInfo[playerid][pGoogle], 17, "!");

		        new query[128];
		
				mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `google_auth`='!' WHERE `id` = %i", PlayerInfo[playerid][pAccID]);
				mysql_tquery(dbHandle, query);
		        SendSyntaxMessage(playerid, "L'authentification à deux facteurs via Google Authenticator a été désactivée.");
		    }
		    else
		    {
		        new __letters[32][] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y","Z","2", "3", "4", "5", "6", "7"};

                pTemp[playerid][pGoogle] = EOS;

                for(new i; i < 17; i++)
				{
					strcat(pTemp[playerid][pGoogle], __letters[random(sizeof(__letters))], 17);
				}
					
				static const str_size[] = "{FFFFFF}Votre clé secrète : {FF6347}%s. {FFFFFF}Entrez-la dans l'application Google Authenticator et sauvegardez-la.\n{FF6347}Attention ! Ne communiquez jamais votre clé secrète à qui que ce soit.\n\n{ffffff}Pour confirmer, entrez le code généré par Google Authenticator ci-dessous :";
				new mes[sizeof(str_size) - 2 + 1 + 17];

				format(mes, sizeof(mes), str_size, pTemp[playerid][pGoogle]);
				Dialog_Show(playerid, AcceptGoogle, DIALOG_STYLE_INPUT, "Confirmation", mes, "Confirmer", "Annuler");

		    }
		}
		case 18: cmd::changermdp(playerid, "");
	}
	
	if (listitem >= 5 && listitem <= 16)  ShowSettings(playerid);
	    
	return 1;
}

CMD:changermdp(playerid, params[])
{
	Dialog_Show(playerid, ChangePass, DIALOG_STYLE_PASSWORD, "Saisie d'informations secrètes", "Veuillez entrer votre mot de passe actuel pour continuer :", "Suivant", "Annuler");
	return 1;
}

ALTX:mm("/menu");
CMD:mm(playerid, params[])
{
	Dialog_Show(playerid, MainMenu, DIALOG_STYLE_LIST, "{FFFFFF}Menu", "1. Inventaire\n\
	2. Statistiques du personnage\n\
	3. Signaler un joueur {ffc46a}(/report){FFFFFF}\n\
	4. Contacter l'administration {ffc46a}(/irc){FFFFFF}\n\
	5. Aide sur les commandes du serveur\n\
	6. Paramètres\n\
	7. Boutique / Don", 
	"Choisir", "Annuler");
	return 1;
}

CMD:settings(playerid, params[])
{
	ShowSettings(playerid);
	return 1;
}

Dialog:MainMenu(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;
	    
	switch(listitem)
	{
		case 0: cmd::inventory(playerid, "");
		case 1: cmd::stats(playerid, "");
		case 2: Dialog_Show(playerid, MakeReport, DIALOG_STYLE_INPUT, "{FFFFFF}Plainte contre un joueur", "{FFFFFF}Veuillez entrer l'ID du joueur et le contenu de votre plainte dans le champ ci-dessous :", "Envoyer", "Annuler");
		case 3: Dialog_Show(playerid, MakeHelp, DIALOG_STYLE_INPUT, "{FFFFFF}Question à l'administration", "{FFFFFF}Veuillez écrire votre question dans le champ ci-dessous :", "Envoyer", "Annuler");
		case 4: cmd::help(playerid, "");
		case 5: ShowSettings(playerid);
		case 6: cmd::donate(playerid, "");
		//case 7: return cmd::mm(playerid, "");
	}
	return 1;
}

Dialog:MakeReport(playerid, response, listitem, inputtext[])
{
	if (!response)			return cmd::mm(playerid, "");
    if (!strlen(inputtext))	return Dialog_Show(playerid, MakeReport, DIALOG_STYLE_INPUT, "{FFFFFF}Plainte contre un joueur", "{FFFFFF}Veuillez entrer l'ID du joueur et le contenu de la plainte ci-dessous :\n{C3C3C3}Format : [ID du joueur] [plainte]", "Envoyer", "Annuler");
        
	cmd::report(playerid, inputtext);
	return 1;
}

Dialog:CmdServer2(playerid, response, listitem, inputtext[])
{
	if (response) cmd::help(playerid, "");
	    
	return 1;
}

Dialog:CmdServer(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;
	    
	switch(listitem)
	{
	    case 0: cmd::techhelp(playerid, "");
	    case 1: cmd::mainhelp(playerid, "");
	    case 2: cmd::chathelp(playerid, "");
	    case 3: cmd::factionhelp(playerid, "");
	    case 4: cmd::househelp(playerid, "");
	    case 5: cmd::bizhelp(playerid, "");
		case 6: cmd::vehiclehelp(playerid, "");
	    case 7: cmd::bankhelp(playerid, "");
	    case 8: cmd::cellhelp(playerid, "");
		case 9: cmd::weaponhelp(playerid, "");
		case 10: cmd::drughelp(playerid, "");
	    case 11: cmd::jobhelp(playerid, "");
		case 12: cmd::fishhelp(playerid, "");
		case 13: cmd::radiohelp(playerid, "");
		case 14: cmd::breakhelp(playerid, "");
	}
	return 1;
}

Dialog:MakeHelp(playerid, response, listitem, inputtext[])
{
	if (!response)			return cmd::mm(playerid, "");
	if (!strlen(inputtext))	return Dialog_Show(playerid, MakeHelp, DIALOG_STYLE_INPUT, "{FFFFFF}Question à l'administration", "{FFFFFF}Veuillez entrer le texte de votre question ci-dessous :", "Envoyer", "Annuler");
	    
	cmd::irc(playerid, inputtext);
	return 1;
}

Dialog:ChangePass(playerid, response, listitem, inputtext[])
{
	if (!response) 																return 1;
	if (isnull(inputtext) || strlen(inputtext) < 6 || strlen(inputtext) > 64)	return Dialog_Show(playerid, ChangePass, DIALOG_STYLE_PASSWORD, "{FFFFFF}Saisie d'information secrète", "{FFFFFF}Veuillez entrer votre mot de passe actuel pour le changer :", "Continuer", "Annuler");

	new query[128 + 129], buf[129];

	WP_Hash(buf, sizeof (buf), inputtext);
	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `accounts` WHERE `id`=%i AND `password`='%e'", PlayerInfo[playerid][pAccID], buf);
	mysql_tquery(dbHandle, query, "OnPlayerChangePass", "i", playerid);
	return 1;
}

Dialog:ChangePass2(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    if (strlen(inputtext) < 6 || strlen(inputtext) > 32)
    {
        Dialog_Show(playerid, ChangePass2, DIALOG_STYLE_PASSWORD, "{FFFFFF}Nouveau mot de passe", "{FFFFFF}Veuillez entrer un nouveau mot de passe :", "Changer", "Annuler");
		return SendErrorMessage(playerid, "Le mot de passe doit comporter entre 6 et 32 caractères.");
	}

	for(new i = 0; i < strlen(inputtext); i++)
	{
		switch(inputtext[i])
		{
			case '!', '@', '#', '$','%','^','&','*','(',')','_','+','=','|','[',']','{','}','-','.','`','~','<','>','?',',','/':
			{
			    Dialog_Show(playerid, ChangePass2, DIALOG_STYLE_PASSWORD, "{FFFFFF}Nouveau mot de passe", "{FFFFFF}Veuillez entrer un nouveau mot de passe :", "Changer", "Annuler");
				return SendErrorMessage(playerid, "Vous avez utilisé un caractère interdit.");
			}
			default: continue;
		}
	}

	new query[128 + 129], buf[129];

	WP_Hash(buf, sizeof (buf), inputtext);
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `password`='%e' WHERE `id` = %i", buf, PlayerInfo[playerid][pAccID]);
	mysql_tquery(dbHandle, query);

	SendSyntaxMessage(playerid, "Votre mot de passe a été changé.");
	return 1;
}

Dialog:ConfirmWord(playerid, response, listitem, inputtext[])
{
	if (!response || isnull(inputtext))
	{
		Dialog_Show(playerid, ConfirmWord, DIALOG_STYLE_PASSWORD, "{c7c3c3}Google Authenticator", "{FFFFFF}Vous vous connectez depuis une adresse IP non confirmée.\nVeuillez entrer le code depuis l'application Google Authenticator pour confirmer que vous êtes le propriétaire du compte.", "Suivant", "");
		return 1;
	}

    if (strval(inputtext) != GoogleAuthenticatorCode(PlayerInfo[playerid][pGoogle], gettime()))
    {
        if (pTemp[playerid][pLoggedTry]++ >= 2)
		{
		    SendErrorMessage(playerid, "Vous avez saisi un code erroné 3 fois.");
			KickEx(playerid);
			return 1;
		}

		SendErrorMessage(playerid, "Code incorrect.");
		Dialog_Show(playerid, ConfirmWord, DIALOG_STYLE_PASSWORD, "{c7c3c3}Google Authenticator", "{FFFFFF}Vous vous connectez depuis une adresse IP non confirmée.\nVeuillez entrer le code depuis l'application Google Authenticator pour confirmer que vous êtes le propriétaire du compte.", "Suivant", "");
        return 1;
    }

    static const str_size[] = "{c7c3c3}SERVEUR: {FFFFFF}Vous avez 5 minutes pour vous connecter\n{c7c3c3}ASTUCE: {FFFFFF}Vous pouvez récupérer votre mot de passe\n                       sur le site : {c7c3c3}"WeburlName"\n                       {FFFFFF}puis accéder à votre compte\n\nVeuillez entrer votre mot de passe:";
    new mes[sizeof(str_size)];

    format(mes, sizeof(mes), str_size);
	Dialog_Show(playerid, Login, DIALOG_STYLE_PASSWORD, "{c7c3c3}Fenêtre de connexion:", mes, "Se connecter", "Annuler");
	pTemp[playerid][pLoggedTime] = 0;
	pTemp[playerid][pLoggedTry] = 0;
	return 1;
}

Dialog:ChangeFight(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	SendAdmMessage("%s a changé le style de combat de %s en %s.", GetAccName(playerid), GetNameEx(GetPVarInt(playerid, "ChangeFightTo")), fight_name[listitem]);
	SendErrorMessage(GetPVarInt(playerid, "ChangeFightTo"), "Votre style de combat a été changé en %s par l'administrateur %s.", fight_name[listitem], GetAccName(playerid));
	SetPlayerFightingStyle(GetPVarInt(playerid, "ChangeFightTo"), fight_id[listitem]);
	PlayerInfo[GetPVarInt(playerid, "ChangeFightTo")][pFightStyle] = fight_id[listitem];
	DeletePVar(playerid, "ChangeFightTo");
	return 1;
}

Dialog:AcceptGoogle(playerid, response, listitem, inputtext[])
{
    if (!IsPlayerLogged(playerid)) return 1;
	if (!response) return ShowSettings(playerid);
	    
    if (strval(inputtext) != GoogleAuthenticatorCode(pTemp[playerid][pGoogle], gettime()))
    {
        SendErrorMessage(playerid, "Code incorrect, veuillez réessayer.");
        return ShowSettings(playerid);
    }

    format(PlayerInfo[playerid][pGoogle], 17, pTemp[playerid][pGoogle]);

    new query[128];

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `accounts` SET `google_auth`='%s' WHERE `id` = %i", PlayerInfo[playerid][pGoogle], PlayerInfo[playerid][pAccID]);
	mysql_tquery(dbHandle, query);
	SendSyntaxMessage(playerid, "Authentification à deux facteurs via Google Authenticator activée avec succès.");
	return 1;
}

Dialog:SendPM(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new userid = GetPVarInt(playerid, #pm_to), text[128];

	GetPVarString(playerid, #pm_text, text, 128);
	SendPMToPlayer(playerid, userid, text);
	return 1;
}

CMD:saluer(playerid, params[])
{
	new userid, type;

	if (sscanf(params,"ud", userid, type))
	{
		SendSyntaxMessage(playerid, "/saluer [ID/Nom du joueur] [type]");
		SendHelpSyntaxMessage(playerid, "[1] Embrasser [2-9] Serrer la main");
		return 1;
	}

	if (!IsPlayerLogged(userid)) return SendNotConnected(playerid);
	if (userid == playerid) return 1;
	if (type > 9 || type < 1) return SendErrorMessage(playerid, "Le type doit être entre 1 et 9.");
	if (!PlayerNearPlayer(2.0, playerid, userid)) return SendNotNearPlayer(playerid);

    pTemp[userid][pGreetPlayer] = playerid;
	pTemp[userid][pGreetType] = type;
    SetPVarInt(userid, #accept_time, gettime() + 30);

	SendHelpMessage(playerid, "Vous avez envoyé une demande de salutation à %s.", GetNameWithMask(userid));
	SendHelpMessage(userid, "%s veut vous saluer (%s).", GetNameWithMask(playerid), (type == 1)?("un baiser"):("une poignée de main"));

	InfoMSG(playerid, "~h~~g~Y~w~ - accepter\n~h~~r~N~w~ - refuser", 3);
	return 1;
}

forward OnPlayerChangePass(playerid);
public OnPlayerChangePass(playerid)
{ 
	if (!cache_num_rows()) return Dialog_Show(playerid, ChangePass, DIALOG_STYLE_PASSWORD, "{FFFFFF}Saisie d'information secrète", "{FFFFFF}Entrez votre mot de passe actuel pour le changer :", "Continuer", "Annuler");
	Dialog_Show(playerid, ChangePass2, DIALOG_STYLE_PASSWORD, "Nouveau mot de passe", "Entrez votre nouveau mot de passe :", "Changer", "Annuler");
	return 1;
}

Dialog:BuyClothes(playerid, response, listitem, inputtext[])
{
    new modelid = GetPVarInt(playerid, "clothes_model");

    if (modelid < 1 || modelid > 311)  return 1;
	if (GetCash(playerid) < 100)        return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent. $100 requis.");
	
	if (response)
		PlayerInfo[playerid][pTog][listitem] = PlayerInfo[playerid][pSkin];
    
	SendSyntaxMessage(playerid, "Vous avez acheté des vêtements pour $100.");
    SetPlayerSkinEx(playerid, modelid);
	GiveCash(playerid, -100);
	
	return 1;
}

Dialog:EditClothes(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

    if (PlayerInfo[playerid][pTog][listitem] == 0)
    {
        PlayerInfo[playerid][pTog][listitem] = PlayerInfo[playerid][pSkin];
        cmd::offclothes(playerid, "1");
        PlayerInfo[playerid][pSkin] = 0;
        return 1;
    }

    new title[32];
    format(title, 32, "{FFFFFF}Vêtement %i.", PlayerInfo[playerid][pTog][listitem]);

    Dialog_Show(playerid, EditClothes2, DIALOG_STYLE_LIST, title, "- Utiliser\n- Jeter", "Choisir", "Annuler");
	SetPVarInt(playerid, "select_tog", listitem);

	return 1;
}

Dialog:EditClothes2(playerid, response, listitem, inputtext[])
{
	if (!response) return 1;

	new slot = GetPVarInt(playerid, "select_tog");

    if (PlayerInfo[playerid][pTog][slot] == 0) return 1;

	if (!listitem)
	{
	    new skin = PlayerInfo[playerid][pSkin];
    	SetPlayerSkinEx(playerid,PlayerInfo[playerid][pTog][slot]);
    	PlayerInfo[playerid][pTog][slot] = skin;
    	return 1;
	}
	
    PlayerInfo[playerid][pTog][slot] = 0;
    return 1;
}