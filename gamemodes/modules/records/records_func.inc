stock SaveRecord(value)
{
    new string[256];
    mysql_format(dbHandle, string, sizeof string,"UPDATE `records` SET `record` = '%d' WHERE `name` = 'Online'", value);
    mysql_tquery(dbHandle, string);
    return true;
}

stock LoadRecord()
{
    new string[192];
    mysql_format(dbHandle, string, sizeof string, "SELECT `record`, DATE_FORMAT(`recorded_at`, '%%d/%%m %%H:%%i') AS `recorded_at` FROM `records` WHERE `name` = 'Online'");
    mysql_tquery(dbHandle, string, "OnRecordLoaded");
    return true;
}

stock UpdateOnlineRecordsMetrics(count)
{
    if (count > DayRecord) DayRecord = count;
    if (count > RecordInfo[rOnline])
    {
        RecordInfo[rOnline] = count;
        SaveRecord(count);
        //LoadRecord();
    }
    return true;
}

this::OnRecordLoaded()
{
    new rows = cache_num_rows();
    if (!rows) return 1;

    for (new i = 0; i < rows; i++)
    {
        cache_get_field_content(i, "record", RecordInfo[rOnline], sizeof RecordInfo[rOnline]);
        cache_get_field_content(i, "recorded_at", RecordInfo[rDate], sizeof RecordInfo[rDate]);
    }

    return true;
}

stock SendOnlineRecordMessage(playerid)
{
    new msg[132];
    format(msg, sizeof msg, "> Le dernier record de joueurs connectés est de %d, celui-ci a été enregistré le %s", RecordInfo[rOnline], to_date(RecordInfo[rDate], 1));
    return SendClientMessage(playerid, COLOR_LIGHTGREEN, msg);
}