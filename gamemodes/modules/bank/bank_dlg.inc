Dialog:ATMMenu(playerid, response, listitem, inputtext[])
{
    if (!response) return 1;
    
    switch(listitem)
    {
        case 0:
        {
            static const str_size[] = "Compte bancaire : {"COL_GREEN"}$%i.\nCompte épargne : {"COL_GREEN"}$%i";
            new mes[sizeof(str_size) - 4 + 24];
            
            format(mes, sizeof(mes), str_size, PlayerInfo[playerid][pBank], PlayerInfo[playerid][pSavings]);
            Dialog_Show(playerid, ATMBack, DIALOG_STYLE_LIST, "{FFFFFF}Solde", mes, "Retour", "");
        }

        case 1: Dialog_Show(playerid, ATMWithdraw, DIALOG_STYLE_INPUT, "{FFFFFF}Retirer de l'argent", "{FFFFFF}Entrez le montant que vous souhaitez retirer :", "Retirer", "Retour");
		case 2: Dialog_Show(playerid, ATMDeposit, DIALOG_STYLE_INPUT, "{FFFFFF}Déposer de l'argent", "{FFFFFF}Entrez le montant que vous souhaitez déposer :", "Déposer", "Retour");
		case 3: Dialog_Show(playerid, ATMTransfer, DIALOG_STYLE_INPUT, "{FFFFFF}Transfert d'argent", "{FFFFFF}Entrez le nom / ID du destinataire ci-dessous :", "Suivant", "Retour");        
    }
    
    return 1;
}


Dialog:ATMBack(playerid, response, listitem, inputtext[])
{
	if (response)	cmd::atm(playerid, "");
	
	return 1;
}

Dialog:ATMTransfer(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::atm(playerid, "");

    new userid;

    sscanf(inputtext, "u", userid);

    if (!IsPlayerLogged(userid) || userid == playerid)
    {
        Dialog_Show(playerid, ATMTransfer, DIALOG_STYLE_INPUT, "{FFFFFF}Transfert d'argent", "{FFFFFF}Entrez le nom / ID du destinataire ci-dessous :", "Suivant", "Retour");
        return SendErrorMessage(playerid, "Destinataire introuvable.");
    }

    static const str_size[] = "{FFFFFF}Transfert d'argent - {FFFF00}%s";
    new mes[sizeof(str_size) - 2 + MAX_PLAYER_NAME + 1];

    format(mes, sizeof(mes), str_size, GetNameEx(userid));
    SetPVarInt(playerid, #transfer_player, userid);
    Dialog_Show(playerid, ATMTransfer2, DIALOG_STYLE_INPUT, mes, "{FFFFFF}Entrez le montant que vous souhaitez transférer :", "Transférer", "Annuler");
    
    return 1;
}


Dialog:ATMTransfer2(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::atm(playerid, "");

    new userid = GetPVarInt(playerid, #transfer_player), amount = strval(inputtext);

    DeletePVar(playerid, #transfer_player);

    if (!IsPlayerLogged(userid) || userid == playerid)
    {
        Dialog_Show(playerid, ATMTransfer, DIALOG_STYLE_INPUT, "{FFFFFF}Transfert d'argent", "{FFFFFF}Entrez le nom / ID du destinataire ci-dessous :", "Suivant", "Retour");
        return SendErrorMessage(playerid, "Destinataire introuvable.");
    }

    if (GetBankCash(playerid) < amount || amount < 1)
    {
        static const str_size[] = "{FFFFFF}Transfert d'argent - {FFFF00}%s";
        new mes[sizeof(str_size) - 2 + MAX_PLAYER_NAME + 1];

        format(mes, sizeof(mes), str_size, GetNameEx(userid));
        SetPVarInt(playerid, #transfer_player, userid);
        Dialog_Show(playerid, ATMTransfer2, DIALOG_STYLE_INPUT, mes, "{FFFFFF}Entrez le montant que vous souhaitez transférer :", "Transférer", "Annuler");
        return SendErrorMessage(playerid, "Vous n'avez pas cette somme sur votre compte bancaire.");
    }

    BankTransfer(playerid, userid, amount);
    cmd::atm(playerid, "");
    return 1;
}


Dialog:ATMWithdraw(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::atm(playerid, "");
	    
    new amount = strval(inputtext);

    if (amount > GetBankCash(playerid) || amount < 1)
    {
        Dialog_Show(playerid, ATMWithdraw, DIALOG_STYLE_INPUT, "{FFFFFF}Retirer de l'argent", "{FFFFFF}Entrez le montant que vous souhaitez retirer :", "Retirer", "Retour");
        return SendErrorMessage(playerid, "Vous n'avez pas cette somme sur votre compte bancaire.");
    }

    BankWithdraw(playerid, amount);
    cmd::atm(playerid, "");
    return 1;
}


Dialog:ATMDeposit(playerid, response, listitem, inputtext[])
{
    if (!response) return cmd::atm(playerid, "");

    new amount = strval(inputtext);

    if (amount > GetCash(playerid) || amount < 1)
    {
        Dialog_Show(playerid, ATMDeposit, DIALOG_STYLE_INPUT, "{FFFFFFF}Déposer de l'argent", "{FFFFFF}Entrez le montant que vous souhaitez déposer :", "Déposer", "Retour");
        return SendErrorMessage(playerid, "Vous n'avez pas cette somme sur vous.");
    }

    BankDeposit(playerid, amount);
    cmd::atm(playerid, "");
    return 1;
}
