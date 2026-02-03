CMD:cabine(playerid, params[])
{
	static option[24], secoption[128];
	if (sscanf(params, "s[24]S()[128]", option, secoption))
	{
		ClearChatbox(playerid, 1);
		SendSyntaxMessage(playerid, "/cabine [commande]");
		SendHelpSyntaxMessage(playerid, "appeler, (rac)crocher, (dec)rocher");
		if (CheckAdmin(playerid, ADMIN_LEVEL_3)) return SendHelpSyntaxAdminMessage(playerid, "creer, (supp)rimer, (edit)er, goto");
		ClearChatbox(playerid, 1);
		return 1;
	}

	if (CompareStrings(option, "appeler"))
	{
		if (sscanf(secoption, "s[16]", pTemp[playerid][pPayphoneNumStr])) {
			SendSyntaxMessage(playerid, "/cabine appeler [numéro]");
			SendHelpMessage(playerid, "911 - Urgences | 444 - Mécanicien | 555 - Taxi"); // | 991 - Ligne directe police
			return 1;
		}

		if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
			return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

		if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
			return SendErrorMessage(playerid, "Vous avez un appel en cours. Tapez /cabine raccrocher - pour raccrocher.");

		if (IsPlayerPhoneCreated(playerid))
			return SendErrorMessage(playerid, "Vous utilisez déjà un téléphone.");

		if (GetCash(playerid) < 2)
			return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent, vous devez avoir $2 sur vous.");

		new id = Payphone_Nearest(playerid);
		if (id == -1) return SendErrorMessage(playerid, "Il n'y a pas de téléphone public à proximité.");

		if (PayphoneInfo[id][phUsed] != INVALID_PLAYER_ID)
			return SendErrorMessage(playerid, "Le téléphone public est déjà utilisé.");

		pTemp[playerid][pPayphone] = id;
		PayphoneInfo[id][phUsed] = playerid;

		cmd::server_ame(playerid, "a inséré une pièce dans le téléphone public.");
		GiveCash(playerid, -2);
		GameTextForPlayer(playerid, "~r~-$2.", 3000, 1);

		ApplyAnimation(playerid, "PED", "null", 4.0, 0, 1, 1, 0, 0, 1);
		ApplyAnimation(playerid, "PED", "phone_in", 4.0, 0, 1, 1, 0, 0, 1);
		ApplyAnimation(playerid, "PED", "phone_talk", 4.0, 1, 1, 1, 1, 0, 1);
		SetPlayerAttachedObject(playerid, 8, 330, 6, 0.014000, -0.007999, 0.000000);

		SM2(playerid, "Vous appelez le %s. Veuillez patienter...", pTemp[playerid][pPayphoneNumStr]);

		CallNumber(playerid, pTemp[playerid][pPayphoneNumStr], 1);
		pTemp[playerid][pPayphoneNumStr][0] = '\0';
	}

	else if (CompareStrings(option, "decrocher") || CompareStrings(option, "dec"))
	{
	    new id = Payphone_Nearest(playerid);
		if (id == -1) return SendErrorMessage(playerid, "Vous devez être à côté d'une cabine téléphonique.");
		if (PayphoneInfo[id][phState] == PAYPHONE_STATE_NONE) return SendErrorMessage(playerid, "Il n'y a aucun appel venant de ce téléphone public.");
		
		if (PayphoneInfo[id][phState] == PAYPHONE_STATE_RINGING)
		{
			new targetid = PayphoneInfo[id][phCaller];
			PayphoneInfo[id][phState] = PAYPHONE_STATE_INCALL;

			pTemp[playerid][pCallLine] = targetid;
			pTemp[targetid][pCallLine] = playerid;

			pTemp[playerid][pCallStatus] = 2;
			pTemp[playerid][pCallTime] = 0;

			pTemp[targetid][pCallStatus] = 2;
			pTemp[targetid][pCallTime] = 0;

			Update_Payphone(id);

			SM2(playerid, "Vous avez répondu à l'appel.");
			SM2(targetid, "Le correspondant a répondu à l'appel.");
		}

		ApplyAnimation(playerid, "PED", "null", 4.0, 0, 1, 1, 0, 0, 1);
		ApplyAnimation(playerid, "PED", "phone_in", 4.0, 0, 0, 1, 0, 0, 1);
		ApplyAnimation(playerid, "PED", "phone_talk", 4.0, 1, 1, 1, 1, 0, 1);
		SetPlayerAttachedObject(playerid, 8, 330, 6, 0.014000, -0.007999, 0.000000);
	}

	else if (CompareStrings(option, "raccrocher") || CompareStrings(option, "rac"))
	{
	    new id = Payphone_Nearest(playerid);
		if (id == -1) return SendErrorMessage(playerid, "Vous devez être à côté d'une cabine téléphonique.");
		if (PayphoneInfo[id][phState] == -1) return SendErrorMessage(playerid, "Vous n'êtes pas en communication.");

		if (PayphoneInfo[id][phState] == PAYPHONE_STATE_NONE || PayphoneInfo[id][phState] == PAYPHONE_STATE_RINGING || PayphoneInfo[id][phState] == PAYPHONE_STATE_INCALL)
		{
			new targetid = PayphoneInfo[id][phCaller];
			SM2(playerid, "Vous avez raccroché.");
			SM2(targetid, "Le correspondant a raccroché.");

			for (new i = 0; i < 2; i++)
			{
				new pid = (i == 0) ? playerid : targetid;
				pTemp[pid][pCallStatus] = 0;
				pTemp[pid][pCallTime] = 0;

				SetPlayerSpecialAction(pid, SPECIAL_ACTION_STOPUSECELLPHONE);
				cancelPayphone(pid);

				ApplyAnimation(pid, "PED", "null", 4.0, 0, 1, 1, 0, 0, 1);
				ApplyAnimation(pid, "PED", "phone_out", 4.0, 0, 1, 1, 0, 0, 1);
				RemovePlayerAttachedObject(pid, 8);
			}
		}
	}

	/*--------------- COMMANDES ADMINS ---------------*/
	else if (CompareStrings(option, "creer"))
    {
        new number, type;
        if (sscanf(secoption, "iI(0)", number, type))
            return SendSyntaxMessage(playerid, "/cabine creer [numéro]");

        if (!type) type = 1216;

        new Float:x, Float:y, Float:z, Float:a, vw, interior;
        GetXYInFrontOfPlayer(playerid, x, y, z, 2.0);
        GetPlayerFacingAngle(playerid, a);
        vw = GetWorld(playerid);
        interior = GetInterior(playerid);

        new id;
        if ((id = CreatePayphone(true, type, number, x, y, z, 0.0, 0.0, 0.0, vw, interior)) == -1)
            return SendErrorMessage(playerid, "Nombre maximum de téléphones publics atteint.");

        SetPVarInt(playerid, #edit_pp, id + 1);
        EditDynamicObject(playerid, PayphoneInfo[id][phObj]);

        SendAdmMessageToPlayer(playerid, "Vous avez créé un téléphone public avec le numéro : %i [ID : %i].", number, id);
        return 1;
    }

    else if (CompareStrings(option, "supprimer") || CompareStrings(option, "supp"))
    {
        new id;

        if (sscanf(secoption, "i", id))
            return SendSyntaxMessage(playerid, "/cabine (supp)rimer [ID téléphone]");
        if (id < 0 || id >= MAX_PAYPHONES || !PayphoneInfo[id][phID])
            return SendErrorMessage(playerid, "Téléphone public introuvable.");

        if (IsValidDynamicObject(PayphoneInfo[id][phObj]))
            DestroyDynamicObject(PayphoneInfo[id][phObj]);
        if (IsValidDynamic3DTextLabel(PayphoneInfo[id][phLabel]))
            DestroyDynamic3DTextLabel(PayphoneInfo[id][phLabel]);

        new query[128];
        mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `payphones` WHERE `id` = %i", PayphoneInfo[id][phID]);
        mysql_tquery(dbHandle, query);

        PayphoneInfo[id][phID] = 0;
        SendAdmMessageToPlayer(playerid, "Vous avez supprimé le téléphone public [ID : %i].", id);
        return 1;
    }

    else if (CompareStrings(option, "editer") || CompareStrings(option, "edit"))
    {
        new id;

        if (sscanf(secoption, "d", id))
            return SendSyntaxMessage(playerid, "/cabine (edit)er [ID téléphone]");
        if (id < 0 || id > MAX_PAYPHONES || !PayphoneInfo[id][phID])
            return SendErrorMessage(playerid, "Téléphone public introuvable.");
        if (Payphone_Nearest(playerid) != id)
            return SendErrorMessage(playerid, "Vous devez être à proximité du téléphone public.");

        SetPVarInt(playerid, #edit_pp, id + 1);
        EditDynamicObject(playerid, PayphoneInfo[id][phObj]);
        return 1;
    }

    else if (CompareStrings(option, "goto"))
    {
        new id;

        if (sscanf(secoption, "i", id))
            return SendSyntaxMessage(playerid, "/cabine goto [ID téléphone]");
        if (id < 0 || id > MAX_PAYPHONES || !PayphoneInfo[id][phID])
            return SendErrorMessage(playerid, "Téléphone public introuvable.");

        SetPlayerPosEx(playerid, PayphoneInfo[id][phX], PayphoneInfo[id][phY], PayphoneInfo[id][phZ]);
        SetWorld(playerid, PayphoneInfo[id][phVW]);
        SetInterior(playerid, PayphoneInfo[id][phInt]);
        return 1;
    }
	return 1;
}










/*ALTX:payphone("/pp");
CMD:payphone(playerid, params[])
{
	if (PlayerInfo[playerid][pInjured] != 0 || GetPVarInt(playerid, "Freeze") || pTemp[playerid][pTaserTime] || pTemp[playerid][pCuffed])
		return SendErrorMessage(playerid, "Vous ne pouvez pas utiliser cette fonction pour le moment.");

	new id = -1;
	if ((id = Payphone_Nearest(playerid)) == -1)
		return SendErrorMessage(playerid, "Il n'y a pas de téléphone public à proximité.");
		
	if (PayphoneInfo[id][phUsed] != INVALID_PLAYER_ID)
		return SendErrorMessage(playerid, "Le téléphone public est déjà utilisé.");
		
	if (IsPlayerPhoneCreated(playerid))
		return SendErrorMessage(playerid, "Vous utilisez déjà un téléphone.");
	
	if (pTemp[playerid][pCallLine] != INVALID_PLAYER_ID || pTemp[playerid][pEmergency])
		return SendErrorMessage(playerid, "Vous avez un appel en cours. Tapez /hangup pour raccrocher.");

	pTemp[playerid][pPayphone] = id;
	PayphoneInfo[id][phUsed] = playerid;

	new info[128];
	format(info, sizeof(info), "Tapez '~y~/pc~w~' pour revenir au ~y~curseur~w~.~n~Numéro de ce téléphone public :~y~ %i", PayphoneInfo[id][phNumber]);
	InfoMSG(playerid, info, 5);

	SetPlayerFacingAngle(playerid, PayphoneInfo[id][phRZ]); 
	SetCameraBehindPlayer(playerid);
	
	if (PayphoneInfo[id][phState] == PAYPHONE_STATE_RINGING) 
	{
		PayphoneInfo[id][phState] = PAYPHONE_STATE_INCALL;

		new targetid = PayphoneInfo[id][phCaller];

		pTemp[playerid][pCallLine] = targetid;
		pTemp[targetid][pCallLine] = playerid;

		pTemp[playerid][pCallStatus] = 2;
		pTemp[playerid][pCallTime] = 0;

		pTemp[targetid][pCallStatus] = 2;
		pTemp[targetid][pCallTime] = 0;
		
		Update_Payphone(id);

		if (0 == IsPlayerPhoneCreated(targetid))
			Phone_Show(targetid, PAGE_CALL);
		else
		{
			ph_showpage(targetid, PAGE_CALL);
		}

		SM2(playerid,"Vous avez répondu à l'appel.");
		SM2(targetid,"Le correspondant a répondu à l'appel.");
	}
	
	PlayerOnPayphone(playerid);
	return 1;
}*/

