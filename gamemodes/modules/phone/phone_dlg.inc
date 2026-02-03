Dialog:SMSMenu(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		DeletePVar(playerid, #sms_number);
		DeletePVar(playerid, #select_sms);
	    return 1;
	}
	new mes[MAX_PHONE_STR],
		count = 0,
		msg[256];

	GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);
			
	for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
	{
		if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
		{
			if (CompareStrings(PlayerContacts[playerid][i][contactName], mes)) 
			{
				count = 1;
				break;
			}
		}
	}

	if (!count)
		format(msg, sizeof(msg), "Répondre\nAppeler\nTransférer\nEnregistrer le numéro\nAjouter aux archives\nSupprimer le message\nSupprimer {ff0303}TOUS LES MESSAGES {FFFFFF}du numéro");
	else
		format(msg, sizeof(msg), "Répondre\nAppeler\nTransférer\nVoir le contact\nAjouter aux archives\nSupprimer le message\nSupprimer {ff0303}TOUS LES MESSAGES {FFFFFF}du contact");

	Dialog_Show(playerid, SMSMenu2, DIALOG_STYLE_LIST, "Options", msg, "Choisir", "Annuler");
	return 1;
}

Dialog:SMSMenu2(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;
	
	switch(listitem)
	{
		case 0: //Îòâåòèòü
		{
			new mes[MAX_PHONE_STR+62];
			GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);

			format(mes, sizeof(mes), "Message >>> {33AA33}%s{A9C4E4}\n\n\tEntrez le texte du message :", mes);
			Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, "Écrire un message", mes, "Envoyer", "Annuler");
			
			DeletePVar(playerid, #select_sms);
			return 1;
		}
		case 1: //Ïîçâîíèòü
		{
			new mes[MAX_PHONE_STR];
			GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);

			format(mes, sizeof(mes), "%s", mes);
			cmd::call(playerid, mes);

			DeletePVar(playerid, #sms_number);
			DeletePVar(playerid, #select_sms);
			return 1;
		}
		case 2: //Ïåðåñëàòü
		{
			Dialog_Show(playerid, ForwardSMS, DIALOG_STYLE_INPUT, "Transférer un message", "Entrez le numéro ou le nom du contact :", "Envoyer", "Annuler");
			return 1;
		}
		case 3: //Ñîõðàíèòü íîìåð || Ïîñìîòðåòü êîíòàêò
		{
			new msg[MAX_PHONE_STR], count = 0;
			GetPVarString(playerid, #sms_number, msg, MAX_PHONE_STR);
			
			for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
			{
				if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
				{
					if (CompareStrings(PlayerContacts[playerid][i][contactName], msg)) 
					{
						count = 1;
						break;
					}
				}
			}

			if (CompareStrings(msg, "Vice Bank")) 
				return SendCellMessage(playerid, "Vous ne pouvez pas enregistrer ce numéro !");
			else if (CompareStrings(msg, "Police Department")) 
				return SendCellMessage(playerid, "Vous ne pouvez pas enregistrer ce numéro !");

			//else if (CompareStrings(msg, "")) 

			if (!count)
			{
				SetPVarInt(playerid, #contact_number, strval(msg));
				
				new mes[128];
				format(mes, sizeof(mes), "Numéro : {33AA33}%i{A9C4E4}\n\n\tEntrez le nom du contact :", GetPVarInt(playerid, #contact_number));
				Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Ajouter un contact", mes, "Suivant", "Annuler");

				DeletePVar(playerid, #sms_number);
				DeletePVar(playerid, #select_sms);
			}
			else 
			{
				ph_showpage(playerid, PAGE_CONTACT_INFO);
				DeletePVar(playerid, #sms_number);
			}
			return 1;
		}
		case 4: ConfirmDialog(playerid, "Confirmation", "Êtes-vous sûr de vouloir archiver ce message ?", "PhoneArchAdd");
		case 5: ConfirmDialog(playerid, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce message ?", "DeleteSMS");
		case 6: ConfirmDialog(playerid, "Confirmation", "{ffffff}Êtes-vous sûr de vouloir supprimer {ff0303}TOUS LES MESSAGES {ffffff}du contact/numéro ?", "DeleteAllSMS");
	}
	return 1;
}

Dialog:ForwardSMS(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		new mes[MAX_PHONE_STR],
			count = 0,
			msg[256];
			
		GetPVarString(playerid, #sms_number, mes, MAX_PHONE_STR);
		
		for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
		{
			if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
			{
				if (CompareStrings(PlayerContacts[playerid][i][contactName], mes)) 
				{
					count = 1;
					break;
				}
			}
		}

		if (!count)
			format(msg, sizeof(msg), "Répondre\nAppeler\nTransférer\nEnregistrer le numéro\nAjouter aux archives\nSupprimer le message\nSupprimer {ff0303}TOUS LES MESSAGES {FFFFFF}du numéro");
		else
			format(msg, sizeof(msg), "Répondre\nAppeler\nTransférer\nVoir le contact\nAjouter aux archives\nSupprimer le message\nSupprimer {ff0303}TOUS LES MESSAGES {FFFFFF}du contact");

		Dialog_Show(playerid, SMSMenu2, DIALOG_STYLE_LIST, "Options", msg, "Choisir", "Annuler");
		return 1;
	}

	if (strlen(inputtext) < 1 || strlen(inputtext) > 24)
 		return Dialog_Show(playerid, ForwardSMS, DIALOG_STYLE_INPUT, "Transférer le message", "Entrez le numéro ou le nom du contact :", "Envoyer", "Annuler");

	new number = strval(inputtext);
	if (!number)
	{
		for(new i = 0; i < GetMaxPlayerContacts(playerid); i++)
		{
			if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
			{
				if (CompareStrings(PlayerContacts[playerid][i][contactName], inputtext)) 
				{
					number = PlayerContacts[playerid][i][contactNumber];
				}
			}
		}
	}
	
	new query[128], string[16];
	valstr(string, number);

	mysql_format(dbHandle, query, sizeof(query), "SELECT * FROM `sms` WHERE `id`=%i", GetPVarInt(playerid, #select_sms));
	mysql_tquery(dbHandle, query, "ForwardMessage", "is", playerid, string);

	DeletePVar(playerid, #sms_number);
	DeletePVar(playerid, #select_sms);

	return 1;
}

Dialog:SendSMS(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 24)
 		return Dialog_Show(playerid, SendSMS, DIALOG_STYLE_INPUT, "Écrire un message", "Nouveau message\n\n\tEntrez le numéro ou le nom du contact :", "Suivant", "Annuler");

    SetPVarString(playerid, #sms_number, inputtext);
    new mes[62+23];
	format(mes, sizeof(mes), "Message >>> {33AA33}%s{A9C4E4}\n\n\tEntrez le texte du message :", inputtext);
	Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, "Écrire un message", mes, "Envoyer", "Annuler");
	return 1;
}

Dialog:SendSMS2(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127)
	{
	    new mes[32];
	    GetPVarString(playerid, #sms_number, mes, 32);
	    Dialog_Show(playerid, SendSMS2, DIALOG_STYLE_INPUT, mes, "Entrez le texte du message :", "Envoyer", "Annuler");
	    return 1;
	}

	new mes[128 + 32],
	    name[32];

    GetPVarString(playerid, #sms_number, name, 32);

	format(mes, sizeof(mes), "%s %s", name, inputtext);
	cmd::sms(playerid, mes);
	return 1;
}

Dialog:SendSMS3(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	new contact = GetPVarInt(playerid, #select_contact);

	if (contact < 0 || contact >= MAX_PLAYER_CONTACTS || PlayerContacts[playerid][contact][contactOn] == 0)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 127)
	{
	    new mes[62+MAX_PHONE_STR];
	    format(mes, sizeof(mes), "Message >>> {33AA33}%s{A9C4E4}\n\n\tEntrez le texte du message :", PlayerContacts[playerid][contact][contactName]);
		Dialog_Show(playerid, SendSMS3, DIALOG_STYLE_INPUT, "Écrire un message", mes, "Envoyer", "Annuler");
	    return 1;
	}

	new mes[128 + 12];
	format(mes, sizeof(mes), "%i %s", PlayerContacts[playerid][contact][contactNumber], inputtext);
	cmd::sms(playerid, mes);
	return 1;
}

Dialog:DeleteContact(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	new contact = GetPVarInt(playerid, #select_contact);

	if (contact < 0 || contact >= MAX_PLAYER_CONTACTS || PlayerContacts[playerid][contact][contactOn] == 0)
	    return 1;

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "DELETE FROM `player_contacts` WHERE `id` = %i", PlayerContacts[playerid][contact][contactID]);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `sms` SET `from_name` = `from_number` WHERE `to_number` = %i AND `from_number`=%i", PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

    mysql_format(dbHandle, query, sizeof(query), "UPDATE `history_calls` SET `from_name` = `from_number` WHERE `to_number` = %i AND `from_number`=%i", PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

	SM2(playerid, "Le contact {33AA33}%s (%i){FFFFFF} a été supprimé avec succès.", PlayerContacts[playerid][contact][contactName], PlayerContacts[playerid][contact][contactNumber]);
	
	PlayerContacts[playerid][contact][contactName] = EOS;
	PlayerContacts[playerid][contact][contactNumber] = 0;
	PlayerContacts[playerid][contact][contactOn] = 0;
	PlayerContacts[playerid][contact][contactID] = 0;

	PhoneInfo[playerid][current_page] = PAGE_MENU_CONTACTS;
	ph_showpage(playerid, PAGE_LIST_CONTACTS);

	DeletePVar(playerid, #select_contact);
	return 1;
}

Dialog:AddContact(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	new number = strval(inputtext);

	if (number < 1 || number > 99999999)
	    return SendErrorMessage(playerid, "Le numéro de contact doit contenir entre 1 et 8 chiffres.");

	for (new i = 0; i < GetMaxPlayerContacts(playerid); i++)
	{
		if (PlayerContacts[playerid][i][contactNumber] == number)
		{
			Dialog_Show(playerid, AddContact, DIALOG_STYLE_INPUT, "Ajouter un contact", "Nouveau contact\n\n\tEntrez le numéro du contact :", "Suivant", "Annuler");
			return SendErrorMessage(playerid, "Un contact avec ce numéro existe déjà : {33AA33}%s{FFFFFF} !", PlayerContacts[playerid][i][contactName]);
		}
	}

	SetPVarInt(playerid, #contact_number, number);

	new mes[128];
	format(mes, sizeof(mes), "Numéro : {33AA33}%i{A9C4E4}\n\n\tEntrez le nom du contact :", number);
	Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Ajouter un contact", mes, "Suivant", "Annuler");
	return 1;
}

Dialog:AddContact2(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	if (strlen(inputtext) < 1 || strlen(inputtext) > 16 || isnull(inputtext))
	{
	    new mes[128];
		format(mes, sizeof(mes), "Numéro : {33AA33}%i{A9C4E4}\n\n\tEntrez le nom du contact :", GetPVarInt(playerid, #contact_number));
		Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Ajouter un contact", mes, "Suivant", "Annuler");
		return SendErrorMessage(playerid, "Le nom du contact doit contenir entre 1 et 16 caractères et inclure au moins une lettre.");
	}

	new contact = -1, count;

	for (new i = 0; i < GetMaxPlayerContacts(playerid); i++)
	{
		if (strlen(PlayerContacts[playerid][i][contactName]) > 0)
		{
			if (CompareStrings(PlayerContacts[playerid][i][contactName], inputtext)) 
			{
				new mes[128];
				format(mes, sizeof(mes), "Numéro : {33AA33}%i{A9C4E4}\n\n\tEntrez le nom du contact :", GetPVarInt(playerid, #contact_number));
				Dialog_Show(playerid, AddContact2, DIALOG_STYLE_INPUT, "Ajouter un contact", mes, "Suivant", "Annuler");
				return SendErrorMessage(playerid, "Un contact portant ce nom existe déjà : {33AA33}%s (%d){FFFFFF} !", inputtext, PlayerContacts[playerid][i][contactNumber]);
			}
		}

		if (PlayerContacts[playerid][i][contactOn] == 0)
		{
			contact = i;
			break;
		}
		else count++;            
	}

	if (count == GetMaxPlayerContacts(playerid)-1 || contact == -1)
	    return SendErrorMessage(playerid, "Mémoire insuffisante dans le téléphone.");

	PlayerContacts[playerid][contact][contactNumber] = GetPVarInt(playerid, #contact_number);
	format(PlayerContacts[playerid][contact][contactName], 32, "%s", inputtext);
	PlayerContacts[playerid][contact][contactOn] = 1;

	new query[128];
	mysql_format(dbHandle, query, sizeof(query), "UPDATE `sms` SET `from_name` = '%e' WHERE `to_number` = %i AND `from_number`=%i", inputtext, PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `sms` SET `to_name` = '%e' WHERE `to_number` = %i AND `from_number`=%i", inputtext, PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `history_calls` SET `from_name` = '%e' WHERE `to_number` = %i AND `from_number`=%i", inputtext, PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

	mysql_format(dbHandle, query, sizeof(query), "UPDATE `history_calls` SET `to_name` = '%e' WHERE `to_number` = %i AND `from_number`=%i", inputtext, PlayerInfo[playerid][pNumber], PlayerContacts[playerid][contact][contactNumber]);
	mysql_tquery(dbHandle, query);

	PlacePlayerContact(playerid, contact, inputtext, GetPVarInt(playerid, #contact_number));

	ph_showpage(playerid, PAGE_LIST_CONTACTS);
	return 1;
}


Dialog:DialogCall(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	cmd::call(playerid, inputtext);
	return 1;
}