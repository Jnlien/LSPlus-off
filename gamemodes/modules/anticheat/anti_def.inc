/*
    Настройки, связанные с базой данных (MySQL)
*/
#define AC_TABLE_SETTINGS               "anticheat_settings" // Название таблицы в базе данных с настройками анти-чита
#define AC_TABLE_FIELD_CODE             "ac_code" // Название поля с кодом анти-чита в таблице
#define AC_TABLE_FIELD_TRIGGER          "ac_code_trigger_type" // Название поля со значением срабатывания кода анти-чита в таблице

/*
    Общие настройки, касающиеся анти-чита
*/
#define AC_MAX_CODES                    53 // Количество кодов анти-чита (на данный момент их 53)
#define AC_MAX_CODE_LENGTH              (3 + 1) // Максимальное количество символов в "коде" анти-чита (001/002/003, etc.)
#define AC_MAX_CODE_NAME_LENGTH         (33 + 1) // Максимальное количество символов в полном названии чита, за который отвечает какой-либо код
    
#define AC_MAX_TRIGGER_TYPES            3 // Количество типов срабатываний (наказаний) анти-чита. По мере добавления типов срабатываний (наказаний), увеличивайте данное значение.
#define AC_MAX_TRIGGER_TYPE_NAME_LENGTH (9 + 1) // Максимальное количество символов в названии типа срабатывания (наказания) анти-чита

#define AC_GLOBAL_TRIGGER_TYPE_PLAYER   0
#define AC_GLOBAL_TRIGGER_TYPE_IP       1

// Типы срабатываний объявлены макросами, чтобы было проще ориентироваться в OnCheatDetected.
#define AC_CODE_TRIGGER_TYPE_DISABLED   0 // AC_CODE_TRIGGER_TYPE_DISABLED - Тип наказания: Отключён
#define AC_CODE_TRIGGER_TYPE_WARNING    1 // AC_CODE_TRIGGER_TYPE_WARNING - Тип наказания: Warning
#define AC_CODE_TRIGGER_TYPE_KICK       2 // AC_CODE_TRIGGER_TYPE_KICK - Тип наказания: Kick

#define AC_TRIGGER_ANTIFLOOD_TIME       20 // Время для анти-флуда срабатываниями (в секундах)

/*
    Настройки визуальной части системы (диалогов)
*/
#define AC_MAX_CODES_ON_PAGE            15 // Максимальное количество элементов на странице настроек анти-чита
#define AC_DIALOG_NEXT_PAGE_TEXT        ">>> Page suivante"        // Texte du bouton pour afficher la page suivante de la liste
#define AC_DIALOG_PREVIOUS_PAGE_TEXT    "<<< Page prйcйdente"      // Texte du bouton pour afficher la page prйcйdente de la liste
