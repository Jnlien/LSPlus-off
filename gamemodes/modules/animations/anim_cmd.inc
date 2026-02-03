ALTX:stopanim("/sa");
CMD:stopanim(playerid, params[])
{
	if (OnAnim{playerid}) StopPlayerAnims(playerid);
	return 1;
}

CMD:animlist(playerid, params[])
{
	SendClientMessage(playerid,COLOR_GREEN,"____________Liste des animations____________");
	SendClientMessage(playerid,COLOR_WHITE,"/levermain /viser /decrochertel /raccrochertel /tituber /coupdepied");
	SendClientMessage(playerid,COLOR_WHITE,"/stoptaxi /rigoler /croiserbras /vomir /manger /sirote /fesses /parler");
	SendClientMessage(playerid,COLOR_WHITE,"/fuck /blesse /non /oui /ramper /pousser /taguer /acr /blessetete /blesseventre");
	SendClientMessage(playerid,COLOR_WHITE,"/pisser /plonger /bousculer /inspecter /tomber /soufler /boxer /encourager");
	SendClientMessage(playerid,COLOR_WHITE,"/frapper /observer /lancer /gang /pleurer /montrer /assis /danser /joie");
	SendClientMessage(playerid,COLOR_WHITE,"/fumer /batte /pose /allonger /marcher /salut /zieuter /tenir /ninja /regarder");
	SendClientMessage(playerid,COLOR_WHITE,"/halteres /serelever /echauffement /fouille /pencher /stop /seproteger /suspendu");
	SendClientMessage(playerid,COLOR_WHITE,"/marcherarme /otage /bar /follow /posearme /sexe /ingurgiter /debout /contrelemur");
	SendClientMessage(playerid,COLOR_WHITE,"/car /gazer /melanger /loose /admirer /crier /admirerarme /win /donnerpoche /croiserjambes");
	SendClientMessage(playerid,COLOR_WHITE,"/ecrasersol /hands /ramasse /posersol /rap /rechargerarme /tapoter /peur /manif /basket");
	SendClientMessage(playerid,COLOR_WHITE,"/gratter /superkick");
	SendClientMessage(playerid,COLOR_WHITE,"");
	SendClientMessage(playerid,COLOR_WHITE,"Utiliser /stopanim ou /sa pour stoper les animations.");
	SendClientMessage(playerid,COLOR_GREEN,"_____________________________________________");
	return 1;
}

ALTX:strip("/dance", "/danser");
CMD:strip(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type)) return SendSyntaxMessage(playerid, "/danser [1-26]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"STRIP", "strip_A", 4.1, 1, 1, 1, 1, 1 );
		case 2: OnPlayAnim(playerid,"STRIP", "strip_B", 4.1, 1, 1, 1, 1, 1 );
		case 3: OnPlayAnim(playerid,"STRIP", "strip_C", 4.1, 1, 1, 1, 1, 1 );
		case 4: OnPlayAnim(playerid,"STRIP", "strip_D", 4.1, 1, 1, 1, 1, 1 );
		case 5: OnPlayAnim(playerid,"STRIP", "strip_E", 4.1, 1, 1, 1, 1, 1 );
		case 6: OnPlayAnim(playerid,"STRIP", "strip_F", 4.1, 1, 1, 1, 1, 1 );
		case 7: OnPlayAnim(playerid,"STRIP", "strip_G", 4.1, 1, 1, 1, 1, 1 );
		case 8: OnPlayAnim(playerid,"STRIP","STR_A2B",4.1,1,1,1,1,1);
		case 9: OnPlayAnim(playerid,"STRIP","STR_B2C",4.1,1,1,1,1,1);
		case 10: OnPlayAnim(playerid,"STRIP","STR_C1",4.1,1,1,1,1,1);
		case 11: OnPlayAnim(playerid,"STRIP","STR_C2",4.1,1,1,1,1,1);
		case 12: OnPlayAnim(playerid,"STRIP","STR_Loop_A",4.1,1,1,1,1,1);
		case 13: OnPlayAnim(playerid,"STRIP","STR_Loop_B",4.1,1,1,1,1,1);
		case 14: OnPlayAnim(playerid,"STRIP","STR_Loop_C",4.1,1,1,1,1,1);
		case 15: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
		case 16: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
		case 17: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
		case 18: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE4);
		case 19: OnPlayAnim(playerid,"DANCING","bd_clap",4.1,1,1,1,1,1);
		case 20: OnPlayAnim(playerid,"DANCING","bd_clap1",4.1,1,1,1,1,1);
		case 21: OnPlayAnim(playerid,"DANCING","DAN_Down_A",4.1,1,1,1,1,1);
		case 22: OnPlayAnim(playerid,"DANCING","DAN_Left_A",4.1,1,1,1,1,1);
		case 23: OnPlayAnim(playerid,"DANCING","DAN_Loop_A",4.1,1,1,1,1,1);
		case 24: OnPlayAnim(playerid,"DANCING","DAN_Right_A",4.1,1,1,1,1,1);
		case 25: OnPlayAnim(playerid,"DANCING","DAN_Up_A",4.1,1,1,1,1,1);
		case 26: OnPlayAnim(playerid,"DANCING","dnce_M_a",4.1,1,1,1,1,1);
		default: SendSyntaxMessage(playerid, "/danser [1-26]");
	}
	return 1;
}

ALTX:liftup("/ramasse");
CMD:liftup(playerid, params[])
{
	return OnPlayAnim(playerid, "CARRY", "liftup", 3.0, 0, 0, 0, 0, 0);
}

ALTX:putdown("/posersol");
CMD:putdown(playerid, params[])
{
	return OnPlayAnim(playerid, "CARRY", "putdwn", 3.0, 0, 0, 0, 0, 0);
}

ALTX:aim("/viser");
CMD:aim(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/viser [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0);
		case 2: OnPlayAnim(playerid,"PED","gang_gunstand",3.0,0,1,1,1,0);
		case 3: OnPlayAnim(playerid,"PED","Driveby_L",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"PED","Driveby_R",4.1,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"SHOP","SHP_Duck_Fire",4.0,0,0,1,1,0);
		case 6: OnPlayAnim(playerid,"SHOP","SHP_Gun_Threat",4.1,0,1,1,1,0);
		case 7: OnPlayAnim(playerid,"TEC","TEC_reload",4.1,0,1,1,1,1);
		default: SendSyntaxMessage(playerid, "/viser [1-2]");
	}
	return 1;
}

ALTX:lay("/allonger");
CMD:lay(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/allonger [1-9]");

	switch(type)
	{
		case 1:	OnPlayAnim(playerid,"BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
		case 2:	OnPlayAnim(playerid,"SUNBATHE","Lay_Bac_in",3.0,0,1,1,1,0);
		case 3:	OnPlayAnim(playerid,"BEACH","ParkSit_W_loop",3.0,0,1,1,1,0);
		case 4:	OnPlayAnim(playerid,"BEACH","SitnWait_loop_W",4.0, 1, 0, 0, 0, 0);
		case 5:	OnPlayAnim(playerid,"SUNBATHE","SBATHE_F_LieB2Sit",3.0,0,1,1,1,0);
		case 6: OnPlayAnim(playerid,"INT_HOUSE","BED_In_L",4.0,0,0,1,1,0);
		case 7: OnPlayAnim(playerid,"INT_HOUSE","BED_In_R",4.0,0,0,1,1,0);
		case 8: OnPlayAnim(playerid,"INT_HOUSE","BED_Loop_L",4.0,1,0,0,0,0);
		case 9: OnPlayAnim(playerid,"INT_HOUSE","BED_Loop_R",4.0,1,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/allonger [1-9]");
	}
	return 1;
}

CMD:tomber(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/tomber [1-11]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PARACHUTE","FALL_skyDive_DIE",3.5,0,0,0,1,0);
		case 2: OnPlayAnim(playerid,"CRACK","crckdeth2",3.0,1,1,1,1,0);
		case 3: OnPlayAnim(playerid,"CRACK","crckdeth4",3.0,0,1,1,1,0);
		case 4: OnPlayAnim(playerid,"CRACK","crckidle1",3.0,0,1,1,1,0);
		case 5: OnPlayAnim(playerid,"CRACK","crckidle2",3.0,0,1,1,1,0);
		case 6: OnPlayAnim(playerid,"CRACK","crckidle3",3.0,0,1,1,1,0);
		case 7: OnPlayAnim(playerid,"CRACK","crckdeth1",3.0,0,1,1,1,0);
		case 8: OnPlayAnim(playerid,"CRACK","crckdeth3",3.0,0,1,1,1,0);
		case 9: OnPlayAnim(playerid,"PED","KD_left",4.1,0,1,1,1,0);
		case 10: OnPlayAnim(playerid,"HEIST9","CAS_G2_GasKO",4.1,0,1,1,1,0);
		case 11: OnPlayAnim(playerid,"WUZI","CS_Dead_Guy",4.1,1,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/tomber [1-11]");
	}
	return 1;
}

ALTX:areload("/rechargerarme");
CMD:areload(playerid, params[])
{
	return OnPlayAnim(playerid,"TEC","TEC_reload",4.0,0,0,0,0,0);
}

ALTX:bat("/batte");
CMD:bat(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/batte [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CRACK","Bbalbat_Idle_02",4.0, 1, 0, 0, 0, 0);
		case 2: OnPlayAnim(playerid,"CRACK","Bbalbat_Idle_01",4.0, 1, 0, 0, 0, 0);
		case 3: OnPlayAnim(playerid,"BASEBALL","Bat_IDLE",4.1,1,0,0,1,0);
		default: SendSyntaxMessage(playerid, "/batte [1-3]");
	}
	return 1;
}

ALTX:tapcig("/tapoter");
CMD:tapcig(playerid, params[])
{
	return OnPlayAnim(playerid,"SMOKING","M_smk_tap",3.0,0,0,0,0,0);
}

ALTX:laugh("/rigoler");
CMD:laugh(playerid, params[])
{
	return OnPlayAnim(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0, 0);
}

ALTX:vomit("/vomir");
CMD:vomit(playerid, params[])
{
	return OnPlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0);
}

ALTX:cry("/pleurer");
CMD:cry(playerid, params[])
{
	return OnPlayAnim(playerid,"GRAVEYARD","mrnf_loop",4.0,1,0,0,0,0);
}

ALTX:cover("/peur");
CMD:cover(playerid, params[])
{
	return OnPlayAnim(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0);
}

ALTX:stance("/debout");
CMD:stance(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/debout [1-14]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","WOMAN_IDLESTANCE",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"PED","CAR_HOOKERTALK",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"FAT","FatIdle",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"WUZI","Wuzi_Stand_Loop",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"GRAVEYARD","mrnm_loop",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"GRAVEYARD","prst_loopa",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"PED","idlestance_fat",4.0,1,0,0,0,0);
		case 8: OnPlayAnim(playerid,"PED","idlestance_old",4.0,1,0,0,0,0);
		case 9: OnPlayAnim(playerid,"PED","turn_l",4.0,1,0,0,0,0);
		case 10: OnPlayAnim(playerid,"BAR","Barcustom_loop",4.0,1,0,0,0,0);
		case 11: OnPlayAnim(playerid,"BAR","Barserve_loop",4.0,1,0,0,0,0);
		case 12: OnPlayAnim(playerid,"COP_AMBIENT","Coplook_shake",4.1,0,1,1,1,0);
		case 13: OnPlayAnim(playerid,"DEALER","DEALER_IDLE",4.0,1,0,0,0,0);
		case 14: OnPlayAnim(playerid,"PED","IDLE_HBHB",3.0,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/debout [1-14]");
	}
	return 1;
}

ALTX:yell("/manif");
CMD:yell(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/manif [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"ON_LOOKERS","shout_loop",3.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"ON_LOOKERS","shout_01",3.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"ON_LOOKERS","shout_02",3.0,1,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/manif [1-3]");
	}
	return 1;
}

ALTX:gsign("/gang");
CMD:gsign(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type))	return SendSyntaxMessage(playerid, "/gang [1-13]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"GHANDS","gsign1",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"GHANDS","gsign1LH",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"GHANDS","gsign2",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"GHANDS","gsign2LH",4.1,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"GHANDS","gsign3",4.1,0,1,1,1,1);
		case 6: OnPlayAnim(playerid,"GHANDS","gsign3LH",4.1,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"GHANDS","gsign4",4.1,0,1,1,1,1);
		case 8: OnPlayAnim(playerid,"GHANDS","gsign4LH",4.1,0,1,1,1,1);
		case 9: OnPlayAnim(playerid,"GHANDS","gsign5",4.1,0,1,1,1,1);
		case 10: OnPlayAnim(playerid,"GHANDS","gsign5LH",4.1,0,1,1,1,1);
		case 11: OnPlayAnim(playerid,"RIOT","RIOT_ANGRY",4.0,0,0,0,0,0);
		case 12: OnPlayAnim(playerid,"benchpress","gym_bp_celebrate",4.0,0,0,0,0,0);
		case 13: OnPlayAnim(playerid,"Freeweights","gym_free_celebrate",4.0,0,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/gang [1-13]");
	}
	return 1;
}

CMD:rap(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/rap [1-10]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"RAPPING","RAP_A_Loop",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"RAPPING","RAP_C_Loop",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"GANGS","prtial_gngtlkD",4.0,1,0,0,1,1);
		case 4: OnPlayAnim(playerid,"GANGS","prtial_gngtlkH",4.0,1,0,0,1,1);
		case 5: OnPlayAnim(playerid,"GANGS","prtial_gngtlkF",4.0,1,0,0,1,1);
		case 6: OnPlayAnim(playerid,"GANGS","prtial_gngtlkG",4.0,1,0,0,1,1);
		case 7: OnPlayAnim(playerid,"PED","ATM",4.0,0,0,0,0,0);
		case 8: OnPlayAnim(playerid,"RAPPING","RAP_B_IN",3.0,0,1,1,1,0);
		case 9: OnPlayAnim(playerid,"GYMNASIUM","gym_tread_celebrate",4.1,0,1,1,1,0);
		case 10: OnPlayAnim(playerid,"STRIP","PUN_LOOP",4.1,1,0,0,1,0);
		default: SendSyntaxMessage(playerid, "/rap [1-10]");
	}
	return 1;
}

ALTX:tired("/souffler");
CMD:tired(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/souffler [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","IDLE_tired",3.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"FAT","IDLE_tired",4.0,0,0,1,1,0);
		default: SendSyntaxMessage(playerid, "/souffler [1-2]");
	}
	return 1;
}

ALTX:sit("/assis");
CMD:sit(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/assis [1-13]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","SEAT_down",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"MISC","seat_lr",2.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"MISC","seat_talk_01",2.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"MISC","seat_talk_02",2.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Crash",4.0,1,0,1,1,0);
		case 6: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Drink",4.0,1,0,1,1,0);
		case 7: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Watch",4.0,0,0,1,1,0);
		case 8: OnPlayAnim(playerid,"BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0);
		case 9: OnPlayAnim(playerid,"INT_HOUSE","LOU_In",4.0,0,0,1,1,0);
		case 10: OnPlayAnim(playerid,"TATTOOS", "TAT_ArmR_Pose_P", 4.1, 0, 1, 1, 1, 0 );
		case 11: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Bored_Loop",4.0,1,0,1,1,0);
		case 12: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Type_Loop",4.0,1,0,1,1,0);
		case 13: OnPlayAnim(playerid,"TATTOOS","TAT_Idle_Loop_T",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/assis [1-13]");
	}
	return 1;
}

ALTX:injured("/blesse");
CMD:injured(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/injured [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid, "WUZI", "CS_Dead_Guy", 4.0, 1, 1, 1, 1, 0);
		default: SendSyntaxMessage(playerid, "/injured [1-2]");
	}
	return 1;
}

ALTX:handsup("/levermain", "/serendre");
CMD:handsup(playerid, params[])
{
	if (pTemp[playerid][pTaserTime] || PlayerInfo[playerid][pInjured] || IsPlayerInAnyVehicle(playerid) || JackingEngine[playerid] != 0)	return 1;

	return OnPlayAnim(playerid, "ROB_BANK","SHP_HandsUp_Scr", 4.0, 0, 1, 1, 1, 0);
}

CMD:deal(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/deal [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, 0, 0, 0, 0, 0);
		case 2: OnPlayAnim(playerid, "DEALER", "DEALER_IDLE", 3.0,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/deal [1-2]");
	}
	return 1;
}

ALTX:piss("/pisser");
CMD:piss(playerid, params[])
{
	return OnPlayAnim(playerid,"PAULNMAC","Piss_in",4.1,0,1,1,1,0);
}

ALTX:slapass("/fesses");
CMD:slapass(playerid, params[])
{
	return OnPlayAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0);
}

ALTX:wave("/salut");
CMD:wave(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/salut [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "KISSING","gfwave2", 4.1,0,1,1,0,0);
		case 2: OnPlayAnim(playerid, "ON_LOOKERS","wave_loop", 4.1,1,1,1,0,0);
		case 3: OnPlayAnim(playerid,"CAMERA","camstnd_cmon",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"PED","endchat_01",4.1,0,1,1,0,0);
		default: SendSyntaxMessage(playerid, "/salut [1-4]");
	}
	return 1;
}

ALTX:crossarms("/croiserbras");
CMD:crossarms(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/croiserbras [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1);
		case 2: OnPlayAnim(playerid,"COP_AMBIENT","Coplook_think",4.0,1,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/croiserbras [1-2]");
	}
	return 1; 
}

ALTX:lean("/pose");
CMD:lean(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/pose [1-6]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"GANGS","leanIDLE",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"MISC","Plyrlean_loop",4.0,0,1,1,1,0);
		case 3: OnPlayAnim(playerid,"CAR_CHAT","car_talkm_loop",4.0,0,1,1,1,0);
		case 4: OnPlayAnim(playerid,"SCRATCHING","sclng_r",4.1,0,1,1,1,0);
		case 5: OnPlayAnim(playerid,"SCRATCHING","scdrdlp",4.0,0,0,1,1,0);
		case 6: OnPlayAnim(playerid,"PED","Jetpack_Idle",3.0,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/pose [1-6]");
	}
	return 1;
}

CMD:basket(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/basket [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BSKTBALL","BBALL_idleloop",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"BSKTBALL","BBALL_Jump_Shot",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"BSKTBALL","BBALL_pickup",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"BSKTBALL","BBALL_run",4.1,1,1,1,1,1);
		case 5: OnPlayAnim(playerid,"BSKTBALL","BBALL_def_loop",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"BSKTBALL","BBALL_Dnk",4.0,0,0,0,0,0);
		case 7: OnPlayAnim(playerid,"BSKTBALL","BBALL_walk",4.1,1,1,1,1,1);
		default: SendSyntaxMessage(playerid, "/basket [1-7]");
	}
	return 1;
}

ALTX:lookout("/regarder");
CMD:lookout(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/regarder [1-6]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SWAT","swt_wllpk_L",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"SWAT","swt_wllpk_R",4.0,0,0,1,1,0);
		case 3: OnPlayAnim(playerid,"SWAT","swt_lkt",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"BAR","Barserve_order",4.0,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"ON_LOOKERS","panic_in",4.0,0,0,1,1,0);
		case 6: OnPlayAnim(playerid,"FINALE","FIN_Cop1_Loop",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/regarder [1-6]");
	}
	return 1;
}

CMD:dunk(playerid, params[])
{
	return OnPlayAnim(playerid,"BSKTBALL","BBALL_Dnk",4.0,0,0,0,0,0);
}

CMD:win(playerid, params[])
{
	new type;
	if (sscanf(params,"d",type)) return SendSyntaxMessage(playerid, "/win [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"OTB","wtchrace_win",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"CASINO","manwinb",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"STRIP","PUN_CASH",4.0,0,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/win [1-3]");
	}
	return 1;
}

/*CMD:walk(playerid, params[])
{
	if (!PlayerInfo[playerid][pWalk])	return SendErrorMessage(playerid, "Activez cette fonction dans les paramètres.");
	    
	switch(PlayerInfo[playerid][pWalk])
	{
		case 1: OnPlayAnim(playerid,"PED","WALK_civi",4.1,1,1,1,1,1);
		case 2: OnPlayAnim(playerid,"PED","WALK_gang1",4.1,1,1,1,1,1);
		case 3: OnPlayAnim(playerid,"PED","WOMAN_walksexy",4.1,1,1,1,1,1);
		case 4: OnPlayAnim(playerid,"PED","WALK_armed",4.1,1,1,1,1,1);
		case 5: OnPlayAnim(playerid,"PED","WALK_csaw",4.1,1,1,1,1,1);
		case 6: OnPlayAnim(playerid,"PED","WALK_gang2",4.1,1,1,1,1,1);
		case 7: OnPlayAnim(playerid,"PED","WALK_drunk",4.1,1,1,1,1,1);
		case 8: OnPlayAnim(playerid,"PED","WALK_fat",4.1,1,1,1,1,1);
		case 9: OnPlayAnim(playerid,"PED","WALK_fatold",4.1,1,1,1,1,1);
		case 10: OnPlayAnim(playerid,"PED","WALK_old",4.1,1,1,1,1,1);
		case 11: OnPlayAnim(playerid,"PED","WALK_rocket",4.1,1,1,1,1,1);
		case 12: OnPlayAnim(playerid,"PED","WALK_shuffle",4.1,1,1,1,1,1);
		case 13: OnPlayAnim(playerid,"PED","WOMAN_walknorm",4.1,1,1,1,1,1);
		case 14: OnPlayAnim(playerid,"PED","WOMAN_walkpro",4.1,1,1,1,1,1);
		case 15: OnPlayAnim(playerid,"PED","WOMAN_walkbusy",4.1,1,1,1,1,1);
		case 16: OnPlayAnim(playerid,"PED","WOMAN_walknorm",4.1,1,1,1,1,1);
		case 17: OnPlayAnim(playerid,"PED","Walk_Wuzi",4.1,1,1,1,1,1);
	}
	return 1;
}

CMD:walking(playerid, params[])
{
	if (!PlayerInfo[playerid][pWalk])	return 1;
	    
	switch(PlayerInfo[playerid][pWalk])
	{
		case 1: OnPlayAnim(playerid,"PED","WALK_civi",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"PED","WALK_gang1",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"PED","WOMAN_walksexy",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"PED","WALK_armed",4.1,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"PED","WALK_csaw",4.1,0,1,1,1,1);
		case 6: OnPlayAnim(playerid,"PED","WALK_gang2",4.1,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"PED","WALK_drunk",4.1,0,1,1,1,1);
		case 8: OnPlayAnim(playerid,"PED","WALK_fat",4.1,0,1,1,1,1);
		case 9: OnPlayAnim(playerid,"PED","WALK_fatold",4.1,0,1,1,1,1);
		case 10: OnPlayAnim(playerid,"PED","WALK_old",4.1,0,1,1,1,1);
		case 11: OnPlayAnim(playerid,"PED","WALK_rocket",4.1,0,1,1,1,1);
		case 12: OnPlayAnim(playerid,"PED","WALK_shuffle",4.1,0,1,1,1,1);
		case 13: OnPlayAnim(playerid,"PED","WOMAN_walknorm",4.1,0,1,1,1,1);
		case 14: OnPlayAnim(playerid,"PED","WOMAN_walkpro",4.1,0,1,1,1,1);
		case 15: OnPlayAnim(playerid,"PED","WOMAN_walkbusy",4.1,0,1,1,1,1);
		case 16: OnPlayAnim(playerid,"PED","WOMAN_walknorm",4.1,0,1,1,1,1);
		case 17: OnPlayAnim(playerid,"PED","Walk_Wuzi",4.1,0,1,1,1,1);
	}
	return 1;
}*/

ALTX:smoking("/fumer");
CMD:smoking(playerid,params[])
{
	new type; 

	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/fumer [1-4]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid, "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0);
		case 2: OnPlayAnim(playerid, "SMOKING", "F_smklean_loop", 4.0, 1, 0, 0, 0, 0);
		case 3: OnPlayAnim(playerid, "SMOKING","M_smkstnd_loop", 4.0, 1, 0, 0, 0, 0);
		case 4: OnPlayAnim(playerid, "SMOKING","M_smk_out", 4.0, 0, 0, 0, 0, 0);
		default: SendSyntaxMessage(playerid, "/fumer [1-4]");
	}
	return 1;
}

ALTX:eating("/manger");
CMD:eating(playerid)
{
	return OnPlayAnim(playerid, "FOOD", "EAT_Burger", 4.1,1,1,1,1,1);
}

ALTX:sipdrink("/sirote");
CMD:sipdrink(playerid)
{
	return OnPlayAnim(playerid, "GANGS", "drnkbr_prtl", 4.1,1,1,1,1,1);
}

ALTX:chat("/parler");
CMD:chat(playerid)
{
	return OnPlayAnim(playerid,"PED","IDLE_CHAT",4.0,1,0,0,1,1);
}

CMD:fuck(playerid,params[])
{
	new type; 
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/fuck [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","fucku",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"RIOT","RIOT_FUKU",2.0,0,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/fuck [1-2]");
	}
	return 1;
}

/* SUPPRIMER INUTILE
CMD:taichi(playerid)
{
	return OnPlayAnim(playerid,"PARK","Tai_Chi_Loop",4.0,1,0,0,0,0);
}*/

ALTX:no("/non");
CMD:no(playerid)
{
	return OnPlayAnim(playerid,"GANGS","Invite_No",4.0,0,0,0,0,0);
}

ALTX:yes("/oui");
CMD:yes(playerid)
{
	return OnPlayAnim(playerid,"GANGS","Invite_Yes",4.0,0,0,0,0,0);
}

ALTX:crawl("/ramper");
CMD:crawl(playerid)
{
	return OnPlayAnim(playerid,"PED","CAR_crawloutRHS",4.0,0,0,0,0,0);
}

ALTX:push("/pousser");
CMD:push(playerid, params[])
{
	new type; 
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/pousser [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"GANGS","shake_cara",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"GANGS","shake_carSH",4.0,0,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/pousser [1-2]");
	}
	return 1;
}

CMD:superkick(playerid)
{
	return OnPlayAnim(playerid,"POLICE","Door_Kick",4.0,0,0,0,0,0);
}

CMD:facepalm(playerid)
{
	return OnPlayAnim(playerid,"MISC","plyr_shkhead",4.0,0,0,0,0,0);
}

ALTX:balls("/grattercouilles", "/gratter");
CMD:balls(playerid)
{
	return OnPlayAnim(playerid,"MISC","scratchballs_01",4.0,0,0,0,0,0);
}

ALTX:spraying("/taguer");
CMD:spraying(playerid)
{
	return OnPlayAnim(playerid,"SPRAYCAN","spraycan_full",4.0,0,0,0,0,0);
}

ALTX:medic("/acr");
CMD:medic(playerid)
{
	return OnPlayAnim(playerid,"MEDIC","CPR",4.0,0,0,0,0,0);
}

ALTX:koface("/blessetete");
CMD:koface(playerid)
{
	return OnPlayAnim(playerid,"PED","KO_shot_face",4.0,0,1,1,1,0);
}

ALTX:kostomach("/blesseventre");
CMD:kostomach(playerid)
{
	return OnPlayAnim(playerid,"PED","KO_shot_stom",4.0,0,1,1,1,0);
}

ALTX:lifejump("/plonger");
CMD:lifejump(playerid)
{
	return OnPlayAnim(playerid,"PED","EV_dive",4.0,0,1,1,1,0);
}

CMD:frontfall(playerid)
{
	return OnPlayAnim(playerid,"PED","FLOOR_hit_f",4.1,0,1,1,1,0);
}

ALTX:box("/boxer");
CMD:box(playerid)
{
	return OnPlayAnim(playerid,"GYMNASIUM","GYMshadowbox",4.0,1,1,1,1,0);
}

CMD:encourager(playerid, params[])
{
	new type; 
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/encourager [1-3]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"RIOT","RIOT_CHANT",4.0,1,1,1,1,0);
		case 2: OnPlayAnim(playerid,"BSKTBALL","BBALL_react_score",4.1,1,1,1,1,0);
		case 3: OnPlayAnim(playerid,"STRIP","PUN_HOLLER",4.1,1,0,0,1,0);
		default: SendSyntaxMessage(playerid, "/encourager [1-3]");
	}
	return 1;
}

CMD:frapper(playerid, params[])
{
	new type; 
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/frapper [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"FIGHT_B","FightB_G",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"FIGHT_D","FightD_G",4.0,0,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/frapper [1-2]");
	}
	return 1;
}

ALTX:lowthrow("/lancer");
CMD:lowthrow(playerid)
{
	return OnPlayAnim(playerid,"GRENADE","WEAPON_throwu",3.0,0,0,0,0,0);
}

CMD:stoptaxi(playerid)
{
	return OnPlayAnim(playerid,"PED","IDLE_taxi",4.0,0,0,0,0,0);
}

CMD:police(playerid, params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/police [1-4]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"POLICE","CopTraf_Away",3.0,1,0,0,1,0);
		case 2: OnPlayAnim(playerid,"POLICE","CopTraf_Come",3.0,1,0,0,1,0);
		case 3: OnPlayAnim(playerid,"POLICE","CopTraf_Left",3.0,1,0,0,1,0);
		case 4: OnPlayAnim(playerid,"POLICE","CopTraf_Stop",3.0,1,0,0,1,0);
		default: SendSyntaxMessage(playerid, "/police [1-4]");
	}
	return 1;
}

ALTX:lookback("/zieuter");
CMD:lookback(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/zieuter [1-3]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","flee_lkaround_01",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"PED","roadcross_gang",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Hat",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/zieuter [1-3]");
	}
	return 1;
}

ALTX:caim("/tenir");
CMD:caim(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/tenir [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","IDLE_ROCKET",4.0,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"HAIRCUTS","BRB_Cut",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/tenir [1-2]");
	}
	return 1;
}

ALTX:walk("/marcher");
CMD:walk(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/marcher [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","Player_Sneak",4.1,1,1,1,1,1);
		case 2: OnPlayAnim(playerid,"CARRY","crry_prtial",4.1,0,1,1,1,1);
		default: SendSyntaxMessage(playerid, "/marcher [1-2]");
	}
	return 1;
}

ALTX:sroll("/ninja");
CMD:sroll(playerid)
{
	return OnPlayAnim(playerid,"SWAT","JMP_Wall1m_180",4.1,0,1,1,1,0);
}

ALTX:rod("/halteres");
CMD:rod(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/halteres [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"benchpress","gym_bp_up_A",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"Freeweights","gym_free_B",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/halteres [1-2]");
	}
	return 1;
}

ALTX:gog("/observer");
CMD:gog(playerid)
{
	return OnPlayAnim(playerid,"goggles","goggles_put_on",4.1,0,1,1,1,1);
}

ALTX:bracing("/echauffement");
CMD:bracing(playerid)
{
	return OnPlayAnim(playerid,"PLAYIDLES","stretch",4.0,0,0,0,0,0);
}

ALTX:search("/fouille");
CMD:search(playerid)
{
	return OnPlayAnim(playerid,"ROB_BANK","CAT_Safe_Rob",4.0,1,0,0,0,0);
}

ALTX:daim("/pencher");
CMD:daim(playerid)
{
	return OnPlayAnim(playerid,"RYDER","Van_Stand_Crate",3.0,0,1,1,1,0);
}

ALTX:rhand("/stop");
CMD:rhand(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/stop [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SCRATCHING","scdlulp",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"SCRATCHING","scdrulp",4.0,0,0,1,1,0);
		default: SendSyntaxMessage(playerid, "/stop [1-2]");
	}
	return 1;
}

ALTX:hangon("/suspendu");
CMD:hangon(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/suspendu [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"TRAIN","tran_ouch",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"FINALE","FIN_LegsUp_Loop",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/suspendu [1-2]");
	}
	return 1;
}

CMD:sexe(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/sexe [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BLOWJOBZ", "BJ_COUCH_START_P", 4.1, 0, 1, 1, 1, 0 );
		case 2: OnPlayAnim(playerid,"PED","DRIVE_BOAT",4.0,0,0,1,1,0);
		default: SendSyntaxMessage(playerid, "/sexe [1-2]");
	}
	return 1;
}

ALTX:polwalk("/marcherarme");
CMD:polwalk(playerid)
{
	return OnPlayAnim(playerid,"POLICE","Cop_move_FWD",3.0,1,1,1,1,1);
}

ALTX:wallow("/bousculer");
CMD:wallow(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/bousculer [1-4]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"MD_CHASE","MD_BIKE_Lnd_Roll",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"MD_CHASE","MD_BIKE_Lnd_Roll_F",4.1,0,1,1,1,0);
		case 3: OnPlayAnim(playerid,"PED","BIKE_fallR",4.0,0,1,1,1,0);
		case 4: OnPlayAnim(playerid,"PED","BIKE_fall_off",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/bousculer [1-4]");
	}
	return 1;
}

CMD:loose(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/loose [1-4]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"OTB","wtchrace_lose",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"BSKTBALL","BBALL_react_miss",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CASINO","Roulette_lose",4.1,0,1,1,1,0);
		case 4: OnPlayAnim(playerid,"CASINO","Slot_lose_out",4.0,0,0,0,0,0);
		default: SendSyntaxMessage(playerid, "/loose [1-4]");
	}
	return 1;
}

ALTX:robr("/otage");
CMD:robr(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/otage [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SHOP","SHP_Rob_React",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"SHOP","SHP_Rob_GiveCash",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/otage [1-2]");
	}
	return 1;
}

CMD:bar(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/bar [1-5]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SHOP","SHP_Serve_End",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"HEIST9","Use_SwipeCard",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"BAR","Barserve_give",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"CASINO","Roulette_bet",4.1,0,1,1,1,0);
		case 5: OnPlayAnim(playerid,"BAR","Barserve_bottle",4.0,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/bar [1-5]");
	}
	return 1;
}

CMD:follow(playerid,params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/follow [1-4]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"RYDER","RYD_Beckon_01",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"RYDER","RYD_Beckon_02",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"RYDER","RYD_Beckon_03",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"WUZI","Wuzi_follow",4.1,0,1,1,1,1);
		default: SendSyntaxMessage(playerid, "/follow [1-4]");
	}
	return 1;
}

CMD:posearme(playerid,params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/poserarme [1-8]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SHOTGUN","shotgun_crouchfire",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"SHOTGUN","shotgun_fire",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"SHOTGUN","shotgun_fire_poor",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"COLT45","colt45_fire_2hands",4.1,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"COLT45","colt45_reload",4.1,0,1,1,1,1);
		case 6: OnPlayAnim(playerid,"COLT45","sawnoff_reload",4.1,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"CAMERA","camcrch_cmon",4.0,0,0,1,1,0);
		case 8: OnPlayAnim(playerid,"SWORD","sword_IDLE",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/poserarme [1-8]");
	}
	return 1;
}

CMD:climb(playerid)
{
	return OnPlayAnim(playerid,"PED","CLIMB_jump",4.0,0,0,1,1,0);
}

CMD:pushup(playerid)
{
	return OnPlayAnim(playerid,"PED","getup_front",4.0,1,0,1,1,0);
}

ALTX:swallow("/ingurgiter");
CMD:swallow(playerid)
{
	return OnPlayAnim(playerid,"PED","gum_eat",4.0,0,0,0,0,0);
}

CMD:contrelemur(playerid, params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/contrelemur [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","Shove_Partial",4.1,0,1,1,1,1);	
		case 2: OnPlayAnim(playerid,"PED","CLIMB_jump2fall",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/contrelemur [1-2]");
	}
	return 1;
}

CMD:car(playerid,params[])
{
	new type;

	if (!IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "Vous devez être dans un véhicule.");
	if (sscanf(params,"d", type))		return SendSyntaxMessage(playerid, "/car [1-11]");
	
	switch(type)
	{
		case 1: ApplyAnimation(playerid,"LOWRIDER","lrgirl_bdbnce",4.1,0,1,1,1,0,1);
		case 2: ApplyAnimation(playerid,"LOWRIDER","lrgirl_hair",4.1,0,1,1,1,0,1);
		case 3: ApplyAnimation(playerid,"LOWRIDER","lrgirl_idle_to_l0",4.1,0,1,1,1,0,1);
		case 4: ApplyAnimation(playerid,"PED","Tap_hand",4.1,0,1,1,1,0,1);
		case 5: ApplyAnimation(playerid,"PED","Tap_handP",4.1,0,1,1,1,0,1);
		case 6: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l12_to_l0",4.1,0,1,1,1,0,1);
		case 7: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l2_to_l3",4.1,0,1,1,1,0,1);
		case 8: ApplyAnimation(playerid,"PED","CAR_LB",4.1,0,1,1,1,0,1);
		case 9: ApplyAnimation(playerid,"PED","CAR_sitp",4.1,0,1,1,1,0,1);
		case 10: ApplyAnimation(playerid,"PED","CAR_sitpLO",4.1,0,1,1,1,0,1);
		case 11: ApplyAnimation(playerid,"CAR_CHAT","carfone_loopA",4.1,1,1,1,1,0,1);
		default: SendSyntaxMessage(playerid, "/car [1-11]");
	}
	return 1;
}

CMD:inspecter(playerid,params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/inspecter [1-8]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CAR","Fixn_Car_Loop",3.0,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"CAR","Fixn_Car_Out",3.0,0,1,1,1,0);
		case 3: OnPlayAnim(playerid,"WUZI","Wuzi_grnd_chk",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"COP_AMBIENT","Copbrowse_in",4.1,0,1,1,1,0);
		case 5: OnPlayAnim(playerid,"POLICE","crm_drgbst_01",1.0,0,1,1,1,0);
		case 6: OnPlayAnim(playerid,"BLOWJOBZ", "BJ_COUCH_START_W", 4.1, 0, 1, 1, 1, 0 );
		case 7: OnPlayAnim(playerid,"BD_FIRE","BD_Panic_02",4.1,0,1,1,1,0);
		case 8: OnPlayAnim(playerid,"BD_FIRE","BD_Panic_03",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/inspecter [1-8]");
	}
	return 1;
}

ALTX:backwalk("/gazer");
CMD:backwalk(playerid)
{
	return OnPlayAnim(playerid,"PED","gas_cwr",4.1,1,1,1,1,1);
}

ALTX:taction("/melanger");
CMD:taction(playerid)
{
	return OnPlayAnim(playerid,"BD_FIRE","wash_up",4.0,0,0,0,0,0);
}

/*CMD:catch(playerid) -- supprimer inutile.
{
	return OnPlayAnim(playerid,"MD_CHASE","MD_HANG_Loop",4.1,0,1,1,1,0);
}*/

ALTX:protection("/seproteger");
CMD:protection(playerid,params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/seproteger [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CAMERA","picstnd_take",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"SHOP","SHP_Gun_Duck",4.0,0,0,1,1,0);
		case 3: OnPlayAnim(playerid,"PED","FightShB",4.0,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"AIRPORT","thrw_barl_thrw",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/seproteger [1-4]");
	}
	return 1;
}

ALTX:looksky("/admirer");
CMD:looksky(playerid)
{
	return OnPlayAnim(playerid,"ON_LOOKERS","lkup_loop",4.1,0,1,1,1,0);
}

ALTX:show("/montrer");
CMD:show(playerid,params[])
{
	new type;
	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/montrer [1-9]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"ON_LOOKERS","panic_point",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"ON_LOOKERS","panic_shout",4.1,0,1,1,1,0);
		case 3: OnPlayAnim(playerid,"ON_LOOKERS","point_loop",4.1,0,1,1,1,0);
		case 4: OnPlayAnim(playerid,"MISC","Hiker_Pose",4.0,0,1,1,1,0);
		case 5: OnPlayAnim(playerid,"MISC","Hiker_Pose_L",4.0,0,1,1,1,0);
		case 6: OnPlayAnim(playerid,"POOL","POOL_Long_Start",4.1,0,1,1,1,0);
		case 7: OnPlayAnim(playerid,"SHAMAL","SHAMAL_align",4.1,0,1,1,1,0);
		case 8: OnPlayAnim(playerid,"OTB","wtchrace_cmon",4.1,0,1,1,1,0);
		case 9: OnPlayAnim(playerid,"TEC","TEC_crouchfire",4.1,0,1,1,1,0);
		default: SendSyntaxMessage(playerid, "/montrer [1-9]");
	}
	return 1;
}

ALTX:sh("/cry", "/crier");
CMD:sh(playerid)
{
	return OnPlayAnim(playerid,"ON_LOOKERS","shout_in",4.0,0,0,0,0,0);
}

ALTX:crossfingers("/admirerarme");
CMD:crossfingers(playerid)
{
	return OnPlayAnim(playerid,"CASINO","cards_loop",4.1,0,1,1,1,0);
}

CMD:hdelight(playerid)
{
	return OnPlayAnim(playerid,"CASINO","manwind",4.0,0,0,0,0,0);
}

ALTX:tpull("/donnerpoche");
CMD:tpull(playerid)
{
	return OnPlayAnim(playerid,"CASINO","Slot_in",4.1,0,1,1,1,0);
}

ALTX:joy("/joie");
CMD:joy(playerid)
{
	return OnPlayAnim(playerid,"CASINO","Slot_win_out",4.0,0,0,0,0,0);
}

ALTX:fsit("/croiserjambes");
CMD:fsit(playerid)
{
	return OnPlayAnim(playerid,"JST_BUISNESS","girl_02",4.1,0,1,1,1,0);
}

/* SUPPRIMER INUTILES
CMD:crushjump(playerid)
{
	return OnPlayAnim(playerid,"DODGE","Crush_Jump",4.1,0,1,1,1,0);
}*/

ALTX:tramplefloor("/ecrasersol");
CMD:tramplefloor(playerid)
{
	return OnPlayAnim(playerid,"FINALE","FIN_Cop1_Stomp",4.1,1,1,1,1,0);
}

ALTX:swingback("/serelever");
CMD:swingback(playerid)
{
	return OnPlayAnim(playerid,"PED","getup",4.1, 0, 1, 1, 1, 0);
}

CMD:hands(playerid,params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendSyntaxMessage(playerid, "/hands [1-7]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkB",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkC",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkD",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkE",4.1,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkF",4.1,0,1,1,1,1);
		case 6: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkG",4.1,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkH",4.1,0,1,1,1,1);
		default: SendSyntaxMessage(playerid, "/hands [1-7]");
	}
	return 1;
}

ALTX:cellin("/decrochertel");
CMD:cellin(playerid)
{
	return SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USECELLPHONE);
}

ALTX:cellout("/raccrochertel");
CMD:cellout(playerid)
{
	return SetPlayerSpecialAction(playerid,SPECIAL_ACTION_STOPUSECELLPHONE);
}

ALTX:drunk("/tituber");
CMD:drunk(playerid)
{
	return OnPlayAnim(playerid,"PED","WALK_DRUNK",4.1,1,1,1,1,1);
}

ALTX:kicking("/coupdepied");
CMD:kicking(playerid)
{
	return OnPlayAnim(playerid,"GANGS","shake_carK",4.0,0,0,0,0,0);
}

CMD:bomb(playerid)
{
	return OnPlayAnim(playerid, "BOMBER","BOM_Plant",4.0,0,0,0,0,0);
}