// ћассив AC_CODE хранит в себе текстовые форматы номеров кода анти-чита  
new AC_CODE[AC_MAX_CODES][AC_MAX_CODE_LENGTH] =  
{ 
    "000", 
    "001", 
    "002", 
    "003", 
    "004", 
    "005", 
    "006", 
    "007", 
    "008", 
    "009", 
    "010", 
    "011", 
    "012", 
    "013", 
    "014", 
    "015", 
    "016", 
    "017", 
    "018", 
    "019", 
    "020", 
    "021", 
    "022", 
    "023", 
    "024", 
    "025", 
    "026", 
    "027", 
    "028", 
    "029", 
    "030", 
    "031", 
    "032", 
    "033", 
    "034", 
    "035", 
    "036", 
    "037", 
    "038", 
    "039", 
    "040", 
    "041", 
    "042", 
    "043", 
    "044", 
    "045", 
    "046", 
    "047", 
    "048", 
    "049", 
    "050", 
    "051", 
    "052" 
}; 

// ћассив AC_CODE_NAME хранит в себе названи€ читов, которые соответствуют кодам анти-чита  
new AC_CODE_NAME[AC_MAX_CODES][AC_MAX_CODE_NAME_LENGTH] =  
{ 
    {"AirBreak (onfoot)"}, 
    {"AirBreak (in vehicle)"}, 
    {"Teleport (onfoot)"}, 
    {"Teleport (in vehicle)"}, 
    {"Teleport (into/between vehicles)"}, 
    {"Teleport (vehicle to player)"}, 
    {"Teleport (pickups)"}, 
    {"FlyHack (onfoot)"}, 
    {"FlyHack (in vehicle)"}, 
    {"SpeedHack (onfoot)"}, 
    {"SpeedHack (in vehicle)"}, 
    {"Health hack (in vehicle)"}, 
    {"Health hack (onfoot)"}, 
    {"Armour hack"}, 
    {"Money hack"}, 
    {"Weapon hack"}, 
    {"Ammo hack (add)"}, 
    {"Ammo hack (infinite)"}, 
    {"Special actions hack"}, 
    {"GodMode from bullets (onfoot)"}, 
    {"GodMode from bullets (in vehicle)"}, 
    {"Invisible hack"}, 
    {"Lagcomp-spoof"}, 
    {"Tuning hack"}, 
    {"Parkour mod"}, 
    {"Quick turn"}, 
    {"Rapid fire"}, 
    {"FakeSpawn"}, 
    {"FakeKill"}, 
    {"Pro Aim"}, 
    {"CJ run"}, 
    {"CarShot"}, 
    {"CarJack"}, 
    {"UnFreeze"}, 
    {"AFK Ghost"}, 
    {"Full Aiming"}, 
    {"Fake NPC"}, 
    {"Reconnect"}, 
    {"High ping"}, 
    {"Dialog hack"}, 
    {"Sandbox"}, 
    {"Invalid version"}, 
    {"Rcon hack"}, 
    {"Tuning crasher"}, 
    {"Invalid seat crasher"}, 
    {"Dialog crasher"}, 
    {"Attached object crasher"}, 
    {"Weapon Crasher"}, 
    {"Connects to one slot"}, 
    {"Flood callback functions"}, 
    {"Flood change seat"}, 
    {"DDos"}, 
    {"NOP's"} 
}; 

// ћассив AC_TRIGGER_TYPE_NAME хранит в себе названи€ типов срабатываний (наказаний) анти-чита.  
new AC_TRIGGER_TYPE_NAME[AC_MAX_TRIGGER_TYPES][AC_MAX_TRIGGER_TYPE_NAME_LENGTH] =  
{ 
    {"Dйsactivй"}, 
    {"Warn"}, 
    {"Kick"} 
};

new 
    AC_CODE_TRIGGER_TYPE[AC_MAX_CODES], // ћассив AC_CODE_TRIGGER_TYPE хранит в себе наказани€ каждого кода анти-чита, которые мы загрузим из базы данных.
    AC_CODE_TRIGGERED_COUNT[AC_MAX_CODES] = {0, ...}; // ћассив, хран€щий количество срабатываний того или иного кода анти-чита на прот€жении текущей сессии

new 
    pAntiCheatLastCodeTriggerTime[MAX_PLAYERS][AC_MAX_CODES], // ћассив, хран€щий последнее врем€ срабатывани€ каждого кода анти-чита на игрока
    pAntiCheatSettingsPage[MAX_PLAYERS char], // ћассив, хран€щий номер страницы настроек, на которой сейчас находитс€ игрок
    pAntiCheatSettingsMenuListData[MAX_PLAYERS][AC_MAX_CODES_ON_PAGE], // ћассив, хран€щий в себе идентификаторы (ID) отображЄнных анти-читов на текущей странице, при их настройке в диалоге
    pAntiCheatSettingsEditCodeId[MAX_PLAYERS]; // ћассив, хран€щий номер кода анти-чита, который редактируетс€ игроком 