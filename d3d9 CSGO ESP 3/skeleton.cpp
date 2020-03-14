#include "includes.h";
#include "hack.h"

void draw_player_skeleton(Hack* hack, Ent* player)
{
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 0);
	if (!hack->CheckValidEnt(player))
		return;
	if (player == hack->localEnt)
		return;
	
	if (player->iTeamNum== hack->localEnt->iTeamNum)
	{
		D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 255, 0);
	}
	else
	{
		D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 0);
	}
	Drawespline(8, 7, player, color);//headtoneck
	Drawespline(7, 11, player, color);//leftshoulder
	Drawespline(7, 41, player, color);//rightshoulder
	Drawespline(11, 12, player, color);//leftelbow
	Drawespline(41, 42, player, color);//rightelbow
	Drawespline(7, 3, player, color);//spine
	Drawespline(3, 71, player, color);//leftupperleg
	Drawespline(3, 78, player, color);//rightupperleg
	//Drawespline(78, 79, player, color);
	//Drawespline(71, 72, player, color);//left toe
	//need to make this work with hitboxes and not boneids
}

