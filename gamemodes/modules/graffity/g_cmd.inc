ALTX:gr("/graffity");
CMD:gr(playerid, params[])
{
    new option[16], parameters[145];
    if (sscanf(params, "s[16]S()[144]", option, parameters))
    {
        SendHelpMessage(playerid, "(/gr)affity [create / stop]");
        return 1;
    }
    if (CompareStrings(option, "create"))
    {
        new g_text[33];
        if (sscanf(parameters, "s[32]", g_text))                                      
            return SendHelpMessage(playerid, "/gr create [texte]");

        if (strlen(g_text) > MAX_GRAFFITY_TEXT || strlen(g_text) < MIN_GRAFFITY_TEXT)  
            return SendErrorMessage(playerid, "Le texte du graffiti doit contenir entre %i et %i caractères.", MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);

        if (Graffity_GetFreeID() == -1)                                                  
            return SendErrorMessage(playerid, "Limite de graffitis atteinte. Essayez de créer un graffiti plus tard.");

        if (AntiCheatGetWeapon(playerid) != 41)                                          
            return SendErrorMessage(playerid, "Vous devez avoir une bombe de peinture en main.");

        if (Graffity_GetAccepts(playerid) != -1) 
        {
            SendErrorMessage(playerid, "Vous avez déjà créé un graffiti qui est en cours de validation.");
            SendSyntaxMessage(playerid, "Attendez que votre graffiti soit approuvé ou rejeté avant d'en créer un nouveau.");
            return 1;
        }
        if (Graffity_Nearest(playerid) != -1)        return SendErrorMessage(playerid, "Un graffiti existe déjà à proximité.");
        if (Graffity_GetPlayerEdit(playerid) != -1)  return SendErrorMessage(playerid, "Vous êtes déjà en train de modifier un graffiti.");
        if (IsPlayerInAnyVehicle(playerid))          return SendErrorMessage(playerid, "Vous ne devez pas être dans un véhicule.");
        if (PlayerInfo[playerid][pInjured])          return SendErrorMessage(playerid, "Vous ne pouvez pas créer de graffiti en étant blessé ou mort.");
        
        Graffity_CreateTemporary(playerid, g_text);
        return 1;
    }
    else if (CompareStrings(option, "stop"))
    {
        if (Graffity_GetPlayerEdit(playerid) == -1)      
            return SendErrorMessage(playerid, "Vous n'êtes pas en train de modifier un graffiti.");

        if (!GetPVarInt(playerid, "Graffity:Spraying"))   
            return SendErrorMessage(playerid, "Vous n'êtes pas en train de créer un graffiti.");
        
        SendSyntaxMessage(playerid, "La création du graffiti a été annulée.");
        Graffity_DestroyTemporary(Graffity_GetPlayerEdit(playerid), 1);
        Graffity_ResetPlayer(playerid);
        UnFreezePlayer(playerid);
        cmd::stopanim(playerid, "");
        return 1;
    }
    else
    {
        SendHelpMessage(playerid, "(/gr)affity [create / stop]");
    }
    return 1;
}