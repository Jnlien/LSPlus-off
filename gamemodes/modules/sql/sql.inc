SQL_OnQueryError(errorid, error[], callback[], query[])
{
    if (errorid == CR_SERVER_LOST)
    {
        dbHandle = mysql_connect(SQL_HOST, SQL_USER, SQL_DB, SQL_PASS);
        mysql_log();
        if (mysql_errno() != 0)
        {
            printf("[MySQL]: Échec de reconnexion à %s", SQL_HOST);
        }
        else
        {
            printf("[MySQL]: Reconnecté à %s avec l'utilisateur %s", SQL_HOST, SQL_USER);

            /*mysql_set_charset("utf8mb4");
            mysql_query(dbHandle, "SET NAMES utf8mb4;", false);
            mysql_query(dbHandle, "SET SESSION character_set_server='utf8mb4';", false);

            new name[32];
            mysql_get_charset(name, dbHandle, 32);
            printf("[MySQL]: Encodage actif : %s", name);*/
        }
    }
    printf("[MySQL]: <|| erreur : %s | callback : %s | requête : %s ||>", error, callback, query);
    return 1;
}

SQL_OnGameModeInit()
{
    dbHandle = mysql_connect(SQL_HOST, SQL_USER, SQL_DB, SQL_PASS);
    mysql_log();
    if (mysql_errno() != 0)
    {
        print("[MySQL]: Échec de la connexion à la base de données.");
    }
    else
    {
        print("[MySQL]: Connexion à la base de données réussie.");

        /*mysql_set_charset("utf8mb4");
        mysql_tquery(dbHandle, "SET NAMES utf8mb4;");
        mysql_tquery(dbHandle, "SET SESSION character_set_server='utf8mb4';");

        new name[32];
        mysql_get_charset(name, dbHandle, 32);
        printf("[MySQL]: Encodage actif : %s", name);*/

        // mysql_tquery(dbHandle, "UPDATE `users` SET `online`=0 WHERE `online` = 1");
    }
    return 1;
}