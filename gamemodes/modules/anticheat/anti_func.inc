// Функция загрузки данных о настройках кодов анти-чита из базы данных
stock UploadAntiCheatSettings()  
{  
    mysql_tquery(dbHandle, "SELECT * FROM `anticheat_settings`", "UploadAntiCheat");
}

this::UploadAntiCheat()
{
    new rows = cache_num_rows(), tick = GetTickCount();

	if (!rows) {  
        print("[MySQL] : Les paramиtres anti-cheat sont introuvables dans la base de donnйes. Chargement du mode interrompu – veuillez configurer l'anti-cheat.");  
        return GameModeExit();  
    }  

    for(new i = 0; i < AC_MAX_CODES; i++)
    {
        AC_CODE_TRIGGER_TYPE[i] = cache_get_field_content_int(i, "ac_code_trigger_type");

        if (AC_CODE_TRIGGER_TYPE[i] == AC_CODE_TRIGGER_TYPE_DISABLED) {
            EnableAntiCheat(i, 0);
        }
    }

    new mes[128];
    format(mes, sizeof(mes), "*** [ANTICHEAT] : Paramиtres anti-cheat chargйs avec succиs (chargйs : %i). Temps йcoulй : %i ms.", rows, GetTickCount() - tick);  
    print(mes);

    return 1;  
}

stock Anticheat_OnPlayerConnect(playerid)
{
    /*
        В цикле присвамваем значение -1 каждой ячейке массива, которая отвечает за
        время последнего срабатывания каждого кода анти-чита на игрока
    */
    for(new i = 0; i < AC_MAX_CODES; i++)
        pAntiCheatLastCodeTriggerTime[playerid][i] = -1;

    pAntiCheatSettingsPage{playerid} = 0; // Присваиваем значение 0 переменной, хранящей номер страницы настроек анти-чита, на которой находится игрок
    pAntiCheatSettingsEditCodeId[playerid] = -1; // Присваиваем переменной, хранящей идентификатор (ID) кода анти-чита, который редактирует игрок, занчение -1
    return 1;
} 

this::OnCheatDetected(playerid, ip_address[], type, code)
{
    if (type == AC_GLOBAL_TRIGGER_TYPE_PLAYER)
    {
        switch(code)
        {
            case 5, 6, 11, 22: 
            {
                return 1;
            }
            case 32: // CarJack
            {
                new 
                    Float:x, 
                    Float:y, 
                    Float:z;

                AntiCheatGetPos(playerid, x, y, z);
                return SetPlayerPos(playerid, x, y, z);
            }
            case 40: // Sandbox
            {
                SendClientMessage(playerid, -1, MAX_CONNECTS_MSG);
                return AntiCheatKickWithDesync(playerid, code);
            }
            case 41: // Invalid Version
            {
                SendClientMessage(playerid, -1, UNKNOWN_CLIENT_MSG);
                return AntiCheatKickWithDesync(playerid, code);
            }
            default:
            {
                if (gettime() - pAntiCheatLastCodeTriggerTime[playerid][code] < AC_TRIGGER_ANTIFLOOD_TIME)
                    return 1;

                pAntiCheatLastCodeTriggerTime[playerid][code] = gettime();
                AC_CODE_TRIGGERED_COUNT[code]++;

                new
                    string[88 - 10 + MAX_PLAYER_NAME + 5 + AC_MAX_CODE_NAME_LENGTH + AC_MAX_CODE_LENGTH],
                    trigger_type = AC_CODE_TRIGGER_TYPE[code];

                switch (trigger_type)
                {
                    case AC_CODE_TRIGGER_TYPE_DISABLED: return 1;
                    case AC_CODE_TRIGGER_TYPE_WARNING: SendAdmMessage("%s[%d] est suspectй d'utiliser un cheat : %s [code : %03d].", GetNameEx(playerid), playerid, AC_CODE_NAME[code], code); 
                    case AC_CODE_TRIGGER_TYPE_KICK: 
                    {
                        SendAdmMessage("%s[%d] a йtй kick pour suspicion de cheat : %s [code : %03d].", GetNameEx(playerid), playerid, AC_CODE_NAME[code], code);
                        format(string, sizeof(string), "Vous avez йtй kick pour suspicion de cheat : %s [code : %03d].", AC_CODE_NAME[code], code);
                        SendClientMessage(playerid, -1, string);
                        AntiCheatKickWithDesync(playerid, code);
                    }
                }
            }
        }
    }
    else // AC_GLOBAL_TRIGGER_TYPE_IP
    {
        AC_CODE_TRIGGERED_COUNT[code]++;
        SendAdmMessage("<AC-BAN-IP> L'adresse IP %s a йtй bannie : %s [code : %03d].", ip_address, AC_CODE_NAME[code], code);
        BlockIpAddress(ip_address, 0);
    }
    return 1;
} 
