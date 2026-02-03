//////////////////////////////// MISC ////////////////////////////////
#define SetInterior(%0,%1) \
	SetPlayerInterior(%0,%1)

#define GetInterior(%0) \
	GetPlayerInterior(%0)

#define SetWorld(%0,%1) \
	SetPlayerVirtualWorld(%0,%1)

#define GetWorld(%0) \
	GetPlayerVirtualWorld(%0)

#define PlayerToPoint(%0,%1,%2,%3,%4) \
	IsPlayerInRangeOfPoint(%1,%0,%2,%3,%4)

#define GetName(%0) \
	PlayerInfo[%0][pUsername]

#define GetNameEx(%0) \
	PlayerInfo[%0][pUsername_]

#define GetNameWithMask(%0) \
	PlayerInfo[%0][pMaskname]

#define GetAccName(%0) \
	PlayerInfo[%0][pAccname]

//////////////////////////////////////////////////////////////////////
//////////////////////////////// TEXT DEFINES ////////////////////////

#define SendServerMessage(%0,%1)			SendClientMessageEx(%0, COLOR_WHITE, 	""%1)
#define SendErrorMessage(%0,%1)				SendClientMessageEx(%0, COLOR_GREY, 	""%1)

#define SendSyntaxMessage(%0,%1)			SendClientMessageEx(%0, COLOR_GENERAL, 	"Usage: {FFFFFF}"%1)
#define SendHelpSyntaxMessage(%0,%1) 		SendClientMessageEx(%0, COLOR_GENERAL, 	"Commandes: {FFFFFF}"%1)
#define SendHelpSyntaxAdminMessage(%0,%1)	SendClientMessageEx(%0, COLOR_REDTWO,	"Admin: {FFFFFF}"%1)

#define SendHelpMessage(%0,%1)				SendClientMessageEx(%0, COLOR_GENERAL, 	"Info: {FFFFFF}"%1)
#define SendFlecheMessage(%0,%1)			SendClientMessageEx(%0, COLOR_REDTWO, 	"> {FFFFFF}"%1)




#define SendMedicMessage(%0,%1) \
	SendClientMessageEx(%0, 0xFF8282AA, "INFO:{FFFFFF} "%1)

#define SendSpecialMessage(%0,%1) \
	SendClientMessageEx(%0, COLOR_YELLOWMY, "--> "%1)
	
#define SendWarningMessage(%0,%1) \
	SendClientMessageEx(%0, COLOR_LIGHTRED, "[ ! ]{FFFFFF} "%1)

#define SM1(%0,%1) \
	SendClientMessageEx(%0, COLOR_LIGHTRED, ">{FFFFFF} "%1)	
	
#define SM2(%0,%1) \
	SendClientMessageEx(%0, COLOR_SERVER2, ">{FFFFFF} "%1)	
	
#define SM3(%0,%1) \
	SendClientMessageEx(%0, COLOR_LIGHTRED, "-> "%1)

#define SM4(%0,%1) \
	SendClientMessageEx(%0, COLOR_WHITE, "> "%1)	

///PHONE && PAYPHONE && RADIO///
#define SendCellMessage(%0,%1) \
	SendClientMessageEx(%0, COLOR_GRAD2, "[ ! ] "%1)

#define SendPhoneMessage(%0,%1) \
	SendClientMessageEx(%0, COLOR_YELLOWG, "Téléphone: "%1)

#define SendRadioMsg(%0,%1) \
	SendClientMessageEx(%0, COLOR_YELLOWMY, "Radio: "%1)

//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// ADM CMD //////////////////

#define SendAdmMessage(%0) \          
	AMSG(COLOR_REDONE, "(Admin) "%0) 
	// Âñåì Àäìèíàì

#define SAM \
	SendAdmMessage

#define SendAdmMessageToAll(%0) \
	SendClientMessageToAllEx(COLOR_REDONE, "(Admin) "%0)
	// Âñåì èãðîêàì

#define SAMALL \
	SendAdmMessageToAll

#define SendAdmMessageToPlayer(%0,%1) \
	SendClientMessageEx(%0, COLOR_REDONE, "(Admin) "%1)
	// Èãðîêó

#define SAMPL\
	SendAdmMessageToPlayer

//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// ERRORS ///////////////////
#define SendCuffedMessage(%0)     SendClientMessageEx(%0, COLOR_GREY, "ERREUR:{FFFFFF} Vous êtes menotté.")
#define SendNotConnected(%0)      SendClientMessageEx(%0, COLOR_GREY, "ERREUR:{FFFFFF} Le joueur n'est pas en ligne.")
#define SendNotNearPlayer(%0)     SendClientMessageEx(%0, COLOR_GREY, "ERREUR:{FFFFFF} Le joueur est trop loin de vous.")
#define SendNotInVehicle(%0)      SendClientMessageEx(%0, COLOR_GREY, "ERREUR:{FFFFFF} Vous devez être dans un véhicule.")
#define SendNotCash(%0)           SendClientMessageEx(%0, COLOR_GREY, "ERREUR:{FFFFFF} Vous n'avez pas assez d'argent.")
#define SendNotAdmin(%0)          SendClientMessageEx(%0, COLOR_GREY, "ERREUR:{FFFFFF} Vous ne pouvez pas utiliser cette commande.")
#define SendNotDuty(%0)           SendClientMessageEx(%0, COLOR_GREY, "ERREUR:{FFFFFF} Vous devez être en service admin pour pouvoir utiliser cette commande (( /aduty ))")

///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// OTHER ///////////////////

#define SendNoteMessage(%0,%1) \
	SendClientMessageEx(%0, -1, "Note: "%1)

#define SendErrorRadioMessage(%0,%1) \
	SendClientMessageEx(%0, 0xAFAFAFFF, "[Radio]:{DADADA} "%1)

#define SendFishMessage(%0,%1) \
	SendClientMessageEx(%0, -1, "Pêche: "%1)

#define PRESSING(%0,%1) \
	(%0 & (%1))

#define HOLDING(%0) \
	((newkeys & (%0)) == (%0))

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#define RELEASED(%0) \
	(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))