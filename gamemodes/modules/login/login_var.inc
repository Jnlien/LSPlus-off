new login_char_id[MAX_PLAYERS][MAX_CHARACTERS],
	login_char_name[MAX_PLAYERS][MAX_CHARACTERS][MAX_PLAYER_NAME+1];


#define MAX_SPAWN_POINT 2
new Float: first_spawn[MAX_SPAWN_POINT][4] =
{
    {1642.8634, -2330.6145, 13.5469, 357.6671}, 	// Los Santos Airport 
    {1743.0873, -1861.2135, 13.5778, 359.1701} 		// Gare de Los Santos
};