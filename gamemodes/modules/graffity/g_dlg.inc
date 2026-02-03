Dialog:Graffity_CE(playerid, response, listitem, inputtext[])
{
    if (!response) return Dialog_Show(playerid, Graffity_CE_Cancel, DIALOG_STYLE_MSGBOX, "{FFFFFF}Annulation de la création de graffiti", "{FFFFFF}Êtes-vous sûr de vouloir annuler la création du graffiti ?", "Oui", "Non");

    new grafid = Graffity_GetPlayerEdit(playerid);
    switch(listitem)
    {
        case 0:
        {
            SetPVarInt(playerid, "Graffity:Showing", 1);
            SetPVarInt(playerid, "Graffity:Show", 8);
            SendSyntaxMessage(playerid, "La fenêtre d'édition réapparaîtra dans 8 secondes.");
            SendSyntaxMessage(playerid, "Ne vous éloignez pas trop du graffiti.");
        }
        case 1:
        {
            Dialog_Show(playerid, Graffity_CE_Done, DIALOG_STYLE_MSGBOX, "{FFFFFF}Création de graffiti", "{FFFFFF}Êtes-vous sûr de vouloir dessiner le graffiti avec ces paramètres ?", "Oui", "Non");
        }
        case 2:
        {
            EditDynamicObject(playerid, E_GRAFFITY[grafid][gObject]);
            SetPVarInt(playerid, "Graffity:EditPos", 1);
            SendSyntaxMessage(playerid, "Vous pouvez utiliser {FF6347}ESPACE {FFFFFF}pour déplacer la caméra autour.");
        }
        case 3:
        {
            new graf_string[256];
            format(graf_string, sizeof(graf_string), "{FFFFFF}Texte actuel : %s\n\n* Le texte doit contenir au moins %i et au maximum %i caractères.\n\nEntrez le nouveau texte dans la fenêtre ci-dessous :", E_GRAFFITY[grafid][gText], MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);
            Dialog_Show(playerid, Graffity_CE_Text, DIALOG_STYLE_INPUT, "{FFFFFF}Graffiti - modification du texte", graf_string, "Continuer", "Retour");
        }
        case 4:
        {
            Graffity_ShowPlayerEditDialog(playerid);
        }
        case 5:
        {
            new graf_string[196];
            graf_string[0] = EOS;

            for(new i = 0; i < MAX_GRAFFITY_COLORS; i++)
            {
                format(graf_string, sizeof(graf_string), "%s%s%s\n", graf_string, GRAFFITY_COLOR[i][color_dlg], GRAFFITY_COLOR[i][color_name]);
            }
            Dialog_Show(playerid, Graffity_CE_Color, DIALOG_STYLE_LIST, "{FFFFFF}Graffiti - choix de la couleur", graf_string, "Choisir", "Retour");
        }
        case 6:
        {
            new graf_string[256];
            graf_string[0] = EOS;

            for(new i = 0; i < MAX_GRAFFITY_BACKS; i++)
            {
                format(graf_string, sizeof(graf_string), "%s%s%s\n", graf_string, GRAFFITY_BACK[i][color_dlg], GRAFFITY_BACK[i][color_name]);
            }
            Dialog_Show(playerid, Graffity_CE_BackColor, DIALOG_STYLE_LIST, "{FFFFFF}Graffiti - choix de l'arrière-plan", graf_string, "Choisir", "Retour");
        }
        case 7:
        {
            new graf_string[256];
            graf_string[0] = EOS;

            for(new i = 0; i < MAX_GRAFFITY_FONTS; i++)
            {
                format(graf_string, sizeof(graf_string), "%s%s\n", graf_string, GRAFFITY_FONT[i][font_name]);

            }
            Dialog_Show(playerid, Graffity_CE_Font, DIALOG_STYLE_LIST, "{FFFFFF}Graffiti - choix de la police", graf_string, "Choisir", "Retour");
        }
        case 8:
        {
            new graf_string[196];
            format(graf_string, sizeof(graf_string), "{FFFFFF}Taille actuelle : %i\n\n* La taille de la police doit être comprise entre %i et %i.\n\nEntrez la nouvelle valeur dans la fenêtre ci-dessous :", E_GRAFFITY[grafid][gFontSize], MIN_GRAFFITY_FONTSIZE, MAX_GRAFFITY_FONTSIZE);
            Dialog_Show(playerid, Graffity_CE_FontSize, DIALOG_STYLE_INPUT, "{FFFFFF}Graffiti - taille de la police", graf_string, "Choisir", "Retour");
        }
        case 9:
        {
            Graffity_SetBold(grafid, !E_GRAFFITY[grafid][gBold]);
            Graffity_ShowPlayerEditDialog(playerid);
        }
    }
    return 1;
}

Dialog:Graffity_CE_Cancel(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (!response)   return Graffity_ShowPlayerEditDialog(playerid);

    Graffity_DestroyTemporary(grafid);
    Graffity_ResetPlayer(playerid);
    SendSyntaxMessage(playerid, "La création du graffiti a été annulée.");
    
    return 1;
}

Dialog:Graffity_CE_Done(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);

    if (!response) return Graffity_ShowPlayerEditDialog(playerid);
    Graffity_DestroyTemporary(playerid);
    SetPVarInt(playerid, "Graffity:Spraying", 1);
    FreezePlayer(playerid);
    ApplyAnimation(playerid,"SPRAYCAN","spraycan_full",4.0,1,0,0,0,0);
    SetPVarInt(playerid, "Graffity:SprayingTime", strlen(E_GRAFFITY[grafid][gText]));
    SendSyntaxMessage(playerid, "Pour arrêter de dessiner le graffiti, entrez /gr stop.");
    return 1;
}

Dialog:Graffity_CE_Text(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    {
        if (strlen(inputtext) > MAX_GRAFFITY_TEXT || strlen(inputtext) < MIN_GRAFFITY_TEXT)
        {
            SendErrorMessage(playerid, "Le texte du graffiti doit contenir au moins %i et au maximum %i caractères.", MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);
            
            new graf_string[256];
            
            format(graf_string, sizeof(graf_string), "{FFFFFF}Texte actuel : %s\n\n* Le texte doit contenir au moins %i et au maximum %i caractères.\n\nEntrez le nouveau texte dans la fenêtre ci-dessous :", E_GRAFFITY[grafid][gText], MIN_GRAFFITY_TEXT, MAX_GRAFFITY_TEXT);
            Dialog_Show(playerid, Graffity_CE_Text, DIALOG_STYLE_INPUT, "{FFFFFF}Graffiti - modification du texte", graf_string, "Continuer", "Retour");
            return 1;
        }

        Graffity_SetText(grafid, inputtext);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_Color(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    {
        Graffity_SetColor(grafid, GRAFFITY_COLOR[listitem][color_data]);
        SetPVarInt(playerid, "Graffity:Color", listitem+1);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_BackColor(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    {
        Graffity_SetBackColor(grafid, GRAFFITY_BACK[listitem][color_data]);
        SetPVarInt(playerid, "Graffity:Back", listitem+1);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_Font(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid);
    if (response)
    { 
        Graffity_SetFont(grafid, GRAFFITY_FONT[listitem][font_name]);
        SetPVarInt(playerid, "Graffity:Font", listitem+1);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}

Dialog:Graffity_CE_FontSize(playerid, response, listitem, inputtext[])
{
    new grafid = Graffity_GetPlayerEdit(playerid), fsz = strval(inputtext);
    if (response)
    {
        if (fsz > MAX_GRAFFITY_FONTSIZE || fsz < MIN_GRAFFITY_FONTSIZE)
        {
            SendErrorMessage(playerid, "La taille de la police doit être comprise entre %i et %i.", MIN_GRAFFITY_FONTSIZE, MAX_GRAFFITY_FONTSIZE);

            new graf_string[196];

            format(graf_string, sizeof(graf_string), "{FFFFFF}Taille actuelle : %i\n\n* La taille de la police doit être comprise entre %i et %i.\n\nEntrez la nouvelle valeur dans la fenêtre ci-dessous :", E_GRAFFITY[grafid][gFontSize], MIN_GRAFFITY_FONTSIZE, MAX_GRAFFITY_FONTSIZE);
            Dialog_Show(playerid, Graffity_CE_FontSize, DIALOG_STYLE_INPUT, "{FFFFFF}Graffiti - taille de la police", graf_string, "Choisir", "Retour");
            return 1;
        }

        Graffity_SetFontSize(grafid, fsz);
        Graffity_ShowPlayerEditDialog(playerid);
    }
    else 
    {
        Graffity_ShowPlayerEditDialog(playerid);
    }
    return 1;
}