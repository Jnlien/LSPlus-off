APB_OnGameModeInit()
{
    return mysql_tquery(dbHandle, "SELECT * FROM `apbs`", "LoadDynamicAPB");
}