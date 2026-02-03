CMD:balance(playerid, params[])
{
    if (!Bank_Nearest(playerid)) return SendErrorMessage(playerid, "Vous n'êtes pas dans une banque.");

    SendClientMessageEx(playerid, COLOR_FADE1, "[Banque] : Votre compte bancaire contient $%i.", PlayerInfo[playerid][pBank]);
    SendClientMessageEx(playerid, COLOR_FADE1, "[Banque] : Compte épargne : $%i.", PlayerInfo[playerid][pSavings]);
    return 1;
}


CMD:virement(playerid, params[])
{
    new userid, amount;

    if (!Bank_Nearest(playerid))
        return SendErrorMessage(playerid, "Vous n'êtes pas dans une banque.");

    if (sscanf(params, "ud", userid, amount))
    {
        new msg[128];
        SendSyntaxMessage(playerid, "/virement [ID/Nom du joueur] [montant]");
        format(msg, sizeof(msg), "[Banque] : Vous avez $%i sur votre compte bancaire.", PlayerInfo[playerid][pBank]);
        SendClientMessage(playerid, -1, msg);
        return 1;
    }

    if (userid == playerid)
        return SendErrorMessage(playerid, "Vous ne pouvez pas vous transférer de l'argent à vous-même.");

    if (!IsPlayerLogged(userid))
        return SendNotConnected(playerid);

    if (GetBankCash(playerid) < amount || amount < 1)
        return SendErrorMessage(playerid, "Vous n'avez pas assez d'argent sur votre compte.");

    BankTransfer(playerid, userid, amount);
    return 1;
}


CMD:deposer(playerid, params[])
{
    new amount;

    if (!Bank_Nearest(playerid))
        return SendErrorMessage(playerid, "Vous n'êtes pas dans une banque.");

    if (sscanf(params, "d", amount))
    {
        new msg[128];
        SendSyntaxMessage(playerid, "/deposer [montant]");
        format(msg, sizeof(msg), "[Banque] : Vous avez $%i sur votre compte bancaire.", PlayerInfo[playerid][pBank]);
        SendClientMessage(playerid, -1, msg);
        return 1;
    }

    if (amount > GetCash(playerid) || amount < 1)
        return SendErrorMessage(playerid, "Vous n'avez pas cette somme sur vous.");

    BankDeposit(playerid, amount);
    return 1;
}

CMD:retirer(playerid, params[])
{
    new amount;

    if (!Bank_Nearest(playerid))
        return SendErrorMessage(playerid, "Vous n'êtes pas dans une banque.");

    if (sscanf(params, "d", amount))
    {
        SendSyntaxMessage(playerid, "/retirer [montant]");
        SendClientMessageEx(playerid, COLOR_FADE1,"[Banque] : Vous avez $%i sur votre compte bancaire.", PlayerInfo[playerid][pBank]);
        return 1;
    }

    if (amount > GetBankCash(playerid) || amount < 1)
        return SendErrorMessage(playerid, "Vous n'avez pas cette somme sur votre compte.");

    BankWithdraw(playerid, amount);
    return 1;
}


CMD:epargne(playerid, params[])
{
    new amount;

    if (!Bank_Nearest(playerid))
        return SendErrorMessage(playerid, "Vous n'êtes pas dans une banque.");

    if (PlayerInfo[playerid][pSavings] == 0)
    {
        if (sscanf(params, "d", amount))
            return SendSyntaxMessage(playerid, "/epargne [montant]");

        if (amount < 35000 || amount > 100000)
            return SendErrorMessage(playerid, "Le montant du dépôt doit être compris entre 35 000 et 100 000.");

        if (GetCash(playerid) < amount)
            return SendNotCash(playerid);

        SendHelpMessage(playerid, "Vous avez déposé $%d sur un compte épargne à 1%% d'intérêt.", amount);
        GiveCash(playerid, -amount);
        PlayerInfo[playerid][pSavings] = amount;
    }
    else
    {
        SendHelpMessage(playerid, "Vous avez retiré vos économies d'un montant de $%i.", PlayerInfo[playerid][pSavings]);
        GiveCash(playerid, PlayerInfo[playerid][pSavings]);
        PlayerInfo[playerid][pSavings] = 0;
    }

    return 1;
}


CMD:atm(playerid, params[])
{
    if (ATM_Nearest(playerid) == -1) return 1;
    Dialog_Show(playerid, ATMMenu, DIALOG_STYLE_LIST, "{FFFFFF}Menu", "- Solde\n- Retirer de l'argent\n- Déposer de l'argent\n- Transférer de l'argent", "Choisir", "Annuler");
    return 1;
}