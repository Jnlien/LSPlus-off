CMD:showcontacts(playerid, params[])
{
	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	if (PlayerInfo[playerid][pPhoneOff] != 0)
		return SendErrorMessage(playerid, "Votre téléphone est éteint.");

	new userid;
	if (sscanf(params, "u", userid))
		return SendHelpMessage(playerid, "/showcontacts [ID/Nom du joueur]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (!PlayerNearPlayer(3.0, playerid, userid))
		return SendNotNearPlayer(playerid);

	new count;
	for (new i = 0; i < MAX_PLAYER_CONTACTS; i++)
	{
		if (PlayerContacts[playerid][i][contactOn] == 0)
			continue;

		count++;
		break;
	}

	if (count == 0)
		return SendErrorMessage(playerid, "Vous n'avez aucun contact.");

	SendClientMessageEx(userid, COLOR_HELPS, "%s vous a montré la liste de ses contacts téléphoniques.", GetNameWithMask(playerid));
	SendClientMessageEx(playerid, COLOR_HELPS, "Vous avez montré votre liste de contacts à %s.", GetNameWithMask(userid));

	count = 0;
	for (new i = 0; i < MAX_PLAYER_CONTACTS; i++)
	{
		if (PlayerContacts[playerid][i][contactOn] == 0)
			continue;

		count++;
		SendClientMessageEx(userid, COLOR_GRAD2, "%i. %s - numéro de téléphone : %i", count, PlayerContacts[playerid][i][contactName], PlayerContacts[playerid][i][contactNumber]);
	}

	new mes[128];
	format(mes, sizeof(mes), "%s montre les contacts de son téléphone à %s", GetNameWithMask(playerid), GetNameWithMask(userid));
	SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 15.0, 3000);
	return 1;
}

CMD:showcontact(playerid, params[])
{
	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	if (PlayerInfo[playerid][pPhoneOff] != 0)
		return SendErrorMessage(playerid, "Votre téléphone est éteint.");

	new userid, contact[32];
	if (sscanf(params, "us[32]", userid, contact))
		return SendHelpMessage(playerid, "/showcontacts [ID/Nom du joueur] [nom du contact]");

	if (!IsPlayerLogged(userid))
		return SendNotConnected(playerid);

	if (!PlayerNearPlayer(3.0, playerid, userid))
		return SendNotNearPlayer(playerid);

	new count = -1;
	for (new i = 0; i < MAX_PLAYER_CONTACTS; i++)
	{
		if (PlayerContacts[playerid][i][contactOn] == 0 || !CompareStrings(contact, PlayerContacts[playerid][i][contactName]))
			continue;

		count = i;
		SendClientMessageEx(userid, COLOR_HELPS, "%s vous a montré le contact %s de son téléphone.", GetNameWithMask(playerid), PlayerContacts[playerid][i][contactName]);
		SendClientMessageEx(playerid, COLOR_HELPS, "Vous avez montré le contact %s à %s depuis votre téléphone.", PlayerContacts[playerid][i][contactName], GetNameWithMask(userid));
		SendClientMessageEx(userid, COLOR_GRAD2, "1. %s - numéro de téléphone : %i", PlayerContacts[playerid][i][contactName], PlayerContacts[playerid][i][contactNumber]);

		new mes[128];
		format(mes, sizeof(mes), "%s montre un contact de son téléphone à %s", GetNameWithMask(playerid), GetNameWithMask(userid));
		SetPlayerChatBubble(playerid, mes, COLOR_PURPLE, 15.0, 3000);
		break;
	}

	if (count == -1)
		return SendWarningMessage(playerid, "Ce contact est introuvable.");

	return 1;
}

CMD:loudspeaker(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	if (pTemp[playerid][pCallLine] == INVALID_PLAYER_ID || pTemp[playerid][pCallStatus] != 2)
		return SendWarningMessage(playerid, "Vous n'êtes pas en conversation téléphonique.");

	if (GetPVarInt(playerid, #loud_speak) == 0)
	{
		SetPVarInt(playerid, #loud_speak, 1);
		SM2(playerid, "Vous avez activé le haut-parleur.");
	}
	else
	{
		SetPVarInt(playerid, #loud_speak, 0);
		SM2(playerid, "Vous avez désactivé le haut-parleur.");
	}
	return 1;
}

ALTX:phone("/ph");
CMD:phone(playerid, params[])
{
	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	if (GetPVarInt(playerid, #pDelay))
		return 1;

	if (IsPlayerPhoneCreated(playerid))
	{
		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
			cmd::hangup(playerid, "");

		return Phone_Hide(playerid);
	}

	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (pTemp[playerid][pPayphone] != -1)
		return SendErrorMessage(playerid, "Vous utilisez déjà un téléphone public.");

	if (IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS) || IsPlayerHoldingWeapon(playerid))
		return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

	if (GetPVarInt(playerid, #selfie))
		cmd::selfie(playerid, "");

	PhoneInfo[playerid][current_page] = PAGE_NONE;
	PhoneInfo[playerid][phoneCreated] = 0;
	Phone_Show(playerid);

	SelectTextDraw(playerid, COLOR_LIGHTRED);
	return 1;
}

ALTX:phonecursor("/pc");
CMD:phonecursor(playerid, params[])
{
	SelectTextDraw(playerid, 255);
	return 1;
}

ALTX:sms("/text");
CMD:sms(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	if (PlayerInfo[playerid][pPhoneOff] != 0)
		return SendCellMessage(playerid, "Téléphone éteint.");

	if (0 == GetPlayerNetwork(playerid))
		return SendCellMessage(playerid, "Pas de réseau.");

	if (PlayerInfo[playerid][pPhoneAirplane] == 1)
		return SendCellMessage(playerid, "Appels et SMS indisponibles - mode avion activé.");

	new number_str[24], text[128];
	if (sscanf(params, "s[24]s[128]", number_str, text))
		return SendHelpMessage(playerid, "/sms [numéro ou contact] [message]");

	SmsNumber(playerid, number_str, text);
	return 1;
}

CMD:selfie(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (!IsPlayerPhoneCreated(playerid) && !GetPVarInt(playerid, #selfie) && IsPlayerAttachedObjectSlotUsed(playerid, ATTACH_HANDS))
        return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

    if (!GetPVarInt(playerid, #selfie))
    {
		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || IsPlayerInAnyVehicle(playerid))
		    return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser la caméra pour le moment.");

        new Float:x, Float:y, Float:z;
        new Float:new_x, Float:new_y;
        new Float:radius = 0.9;
        new Float:rot;

        TogglePlayerControllable(playerid, 0);

	    GetPlayerPos(playerid, x, y, z);
	    GetPlayerFacingAngle(playerid, rot);
     	new_x = x + radius * floatcos(rot, degrees);
	    new_y = y + radius * floatsin(rot, degrees);

		RemovePlayerAttachedObject(playerid, ATTACH_HANDS);
	   	SetPlayerAttachedObject(playerid, ATTACH_HANDS, phone_model[PlayerInfo[playerid][pPhoneModel]], 6, 0.06, 0.01, 0.08, 180.0, 90.0, 0.0);

		SetPlayerCameraPos(playerid, new_x, new_y, z + 0.78);
		SetPlayerCameraLookAt(playerid, x, y, z + 0.60);
	    SetPlayerFacingAngle(playerid, rot - 90.0);

		SetPVarFloat(playerid, #selfie_z, 0.60);

		ApplyAnimation(playerid, "PED", "gang_gunstand", 4.1, 1, 1, 1, 1, 1, 1);
  		SetPVarInt(playerid, #selfie, 1);

		SetPlayerArmedWeapon(playerid, 0);

		InfoMSG(playerid, "Appuyez sur ~y~F8~w~ pour faire une capture d'écran, ~y~F7~w~ pour cacher le chat.~n~Maintenez ~r~W, A, S~w~ et ~r~D~w~ pour bouger la caméra, ~y~ENTRÉE~w~ pour revenir.~n~Utilisez ~r~/headmove~w~ pour tourner la tête du personnage.", 6);
		CancelSelectTextDraw(playerid);

		Phone_Hide(playerid, 1);
		return 1;
 	}

	TogglePlayerControllable(playerid, 1);
 	SetCameraBehindPlayer(playerid);

	RemovePlayerAttachedObject(playerid, ATTACH_HANDS);

	if (IsPlayerPhoneCreated(playerid))
 		SetPlayerAttachedObject(playerid, ATTACH_HANDS, phone_model[PlayerInfo[playerid][pPhoneModel]], 6, 0.111, -0.000999, 0.0, 77.9, -171.6, -3.40001, 1.0, 1.0, 1.0);

	ApplyAnimation(playerid, "PED", "ATM", 4.1, 0, 1, 1, 0, 1, 1);
 	DeletePVar(playerid, #selfie);
	
	new selfiepage = PAGE_HOME;
	if (PhoneInfo[playerid][current_page] != PAGE_NONE)
	{
		selfiepage = PhoneInfo[playerid][current_page];
		PhoneInfo[playerid][current_page] = PAGE_NONE;
	}

	Phone_Show(playerid, selfiepage);
	return 1;
}


ALTX:pickup("/p");
CMD:pickup(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	new targetid = pTemp[playerid][pCallLine];

	if (!IsPlayerPhoneCreated(playerid) && targetid == INVALID_PLAYER_ID && IsPlayerAttachedObjectSlotUsed(playerid, 9))
		return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

	if (pTemp[playerid][pCallStatus] == 2)
		return SendCellMessage(playerid, "Vous êtes déjà en communication.");

	if (targetid == INVALID_PLAYER_ID || pTemp[playerid][pCallStatus] != 1)
		return SendCellMessage(playerid, "Personne ne vous appelle.");

	if (pTemp[targetid][pCallLine] != playerid)
		return SendErrorMessage(playerid, "Erreur 404. [F8 > Bug Tracker]");

	if (pTemp[playerid][pPayphone] != -1)
		return SendErrorMessage(playerid, "Vous utilisez un téléphone public.");

	pTemp[playerid][pCallIncoming] = 1;
	pTemp[targetid][pCallIncoming] = 0;

	pTemp[playerid][pCallStatus] = 2;
	pTemp[targetid][pCallStatus] = 2;

	pTemp[playerid][pCallTime] = 0;
	pTemp[targetid][pCallTime] = 0;

	if (pTemp[targetid][pPayphone] == -1)
	{
		if (!IsPlayerPhoneCreated(targetid)) Phone_Show(targetid, PAGE_CALL);
		else ph_showpage(targetid, PAGE_CALL);
	}

	if (!IsPlayerPhoneCreated(playerid)) Phone_Show(playerid, PAGE_CALL);
	else ph_showpage(playerid, PAGE_CALL);

	SM2(playerid, "Vous avez répondu à l'appel.");
	SM2(targetid, "L'appel a été accepté.");

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
	return 1;
}

ALTX:togphone("/togph");
CMD:togphone(playerid, params[])
{
	SendErrorMessage(playerid, "Utilisez l'interface du téléphone pour l'éteindre.");
	return 1;
}

ALTX:hangup("/hg");
CMD:hangup(playerid, params[])
{
	//if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
	//	return SendErrorMessage(playerid, "Âû íå ìîæåòå èñïîëüçîâàòü äàííóþ ôóíêöèþ ïðÿìî ñåé÷àñ.");
	
	if (PlayerInfo[playerid][pNumber] == 0 && pTemp[playerid][pPayphone] == -1) 
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	if (pTemp[playerid][pCallLine] == INVALID_PLAYER_ID && !pTemp[playerid][pEmergency]) 
		return 1;

	
	if (pTemp[playerid][pPayphone] == -1) {
		TextDrawHideForPlayer(playerid, CallBox[playerid]);
		TextDrawHideForPlayer(playerid, PhNote[1][playerid]);
		PhoneInfo[playerid][phoneCallBox] = 0;	
	}
	
	if (pTemp[playerid][pEmergency]) SendCellMessage(playerid, "Vous avez raccroché.");

	new targetid = pTemp[playerid][pCallLine];

	if (!pTemp[playerid][pEmergency] && pTemp[playerid][pCallStatus] == 2)
	{
		SendCellMessage(playerid, "Vous avez terminé la conversation avec %s.", pTemp[playerid][pCallName]);
		SendCellMessage(targetid, "Vous entendez des bips courts.");
		HangupCall(targetid);
	}
	else if (!pTemp[playerid][pEmergency] && pTemp[playerid][pCallStatus] == 1)
	{
		SendCellMessage(targetid, "Vous avez refusé l'appel entrant.");
		SendCellMessage(playerid, "Le téléphone a cessé de sonner.");
		HangupCall(targetid);

		/*for(new i = 0; i < sizeof(PhBtn); i++)
		{
			TextDrawColor(PhBtn[i][targetid], phone_btns_color[PlayerInfo[targetid][pPhoneModel]]);
			TextDrawBackgroundColor(PhBtn[i][targetid], phone_btns_color[PlayerInfo[targetid][pPhoneModel]]);
			if (IsPlayerPhoneCreated(targetid)) TextDrawShowForPlayer(targetid, PhBtn[i][targetid]);
		}*/
 	}

	HangupCall(playerid);
	return 1;
}

CMD:call(playerid, params[])
{		
    if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

    if (PlayerInfo[playerid][pPhoneOff] != 0)
        return SendCellMessage(playerid, "Le téléphone est éteint.");

	if (0 == GetPlayerNetwork(playerid))
	    return SendCellMessage(playerid, "Pas de réseau disponible.");

	if (PlayerInfo[playerid][pPhoneAirplane] == 1)
	    return SendCellMessage(playerid, "Appels et SMS indisponibles - votre téléphone est en mode avion.");

	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || 
		pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

    if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
	    return SendCellMessage(playerid, "Vous avez un appel en cours. Tapez /hangup pour raccrocher.");

    if (!IsPlayerPhoneCreated(playerid) && IsPlayerAttachedObjectSlotUsed(playerid, 9))
        return SendWarningMessage(playerid, "Vos mains sont déjà occupées.");

	if (pTemp[playerid][pPayphone] != -1)
		return SendErrorMessage(playerid, "Vous utilisez déjà une cabine téléphonique.");

	new number_str[32];

	if (sscanf(params,"s[32]", number_str))
	{
		SendHelpMessage(playerid, "/call [numéro / contact / list (numéros des services)]");

		if (CompareStrings(number_str, "list"))
		{
			SM2(playerid, "Service de secours : 911");
			SM2(playerid, "Police (appels généraux) : 991");
			SM2(playerid, "Taxi : 555");
			SM2(playerid, "Service de mécaniciens : 444");
		}
		return 1;
	}

	SetPlayerChatBubble(playerid, "sort son téléphone de sa poche.", COLOR_PURPLE, 20.0, 4000);

	SetPlayerAttachedObject(playerid, ATTACH_HANDS, phone_model[PlayerInfo[playerid][pPhoneModel]], 6, 0.111, -0.000999, 0.0, 77.9, -171.6, -3.40001, 1.0, 1.0, 1.0);
	SetPlayerCellphoneAction(playerid, true);

	CallNumber(playerid, number_str);
	return 1;
}

CMD:dropcell(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || 
		pTemp[playerid][pCuffed] || PhoneInfo[playerid][current_page] == PAGE_NOTIFICATION || PlayerInfo[playerid][pJailed] != 0)
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	if (PlayerInfo[playerid][pNumber] == 0)
		return SendErrorMessage(playerid, "Vous n'avez pas de téléphone.");

	if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency] != 0)
	    return SendErrorMessage(playerid, "Terminez l'appel en cours - utilisez /hangup.");

	if (IsPlayerPhoneCreated(playerid))
		Phone_Hide(playerid);

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `sms` WHERE `to_number` = %i", PlayerInfo[playerid][pNumber]);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `player_contacts` WHERE `userid` = %i", PlayerInfo[playerid][pID]);
	mysql_tquery(dbHandle, query);

	for (new i; i != MAX_PLAYER_CONTACTS; i++)
	{
	    PlayerContacts[playerid][i][contactNumber] = 0;
		PlayerContacts[playerid][i][contactOn] = 0;
		PlayerContacts[playerid][i][contactID] = 0;
	}

	SendCellMessage(playerid, "Vous avez jeté votre téléphone portable.");
	SendHelpMessage(playerid, "Pour pouvoir réutiliser un téléphone, achetez une nouvelle carte SIM.");

	SendNearbyMessage(Range_Normal, playerid, COLOR_PURPLE, "* %s a jeté son téléphone.", GetNameWithMask(playerid));

	PlayerInfo[playerid][pNumber] = 0;
	PlayerInfo[playerid][pPhoneModel] = 0;
	return 1;
}