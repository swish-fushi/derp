#include "includes.h"
#include "hack.h"



float GetDistance(Vec3 other,Vec3 myPos)
{	
	Vec3 delta;
	delta.x = abs(other.x - myPos.x);
	delta.y = abs(other.y - myPos.y);
	delta.z = abs(other.z - myPos.z);
	return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

int getclosestentity(Vec3 mypos, int maxentities,Hack * hack)
{
	int min = 9999,temp=0,minindex=1;
	for (int i = 1; i < maxentities; i++)
	{
		Ent* localplayer = hack->entList->ents[0].ent;
		Ent* curEnt = hack->entList->ents[i].ent;//if valid entity
		if (!hack->CheckValidEnt(curEnt))
			continue;
		
		if (curEnt->iTeamNum == localplayer->iTeamNum)//checking for same team
			continue;
		
		if ((temp=GetDistance(curEnt->vecOrigin, localplayer->vecOrigin))&&temp<min)
		{
			min = temp;
			minindex = i;//we return the player we wanna aim at not the min distance
		}
	}

	return minindex;
}

bool aimat(Vec3 target,Hack* hack)
{	

	float PI = 3.14159265359;

	uintptr_t clientstate = *(uintptr_t*)(hack->engine + hack->dwState);	
	Vec3* viewAngles = (Vec3*)((clientstate)+hack->dwState_ViewAngles);	

	uintptr_t clientstate1 = *(uintptr_t*)(hack->client +hack->dwLocalPlayer);
	Vec3* myPos = (Vec3*)(clientstate1+ hack->m_vecOrigin);

	Vec3* myviewoff = (Vec3*)(clientstate1 + hack->m_vecViewOffset);
	myPos->x+= myviewoff->x;
	myPos->y += myviewoff->y;
	myPos->z += myviewoff->z;

	Vec3 deltaVec = { target.x - myPos->x, target.y - myPos->y,target.z - myPos->z };
	float deltaVecLength = GetDistance(target,*myPos);

	float pitch = ((-asin(deltaVec.z / deltaVecLength)) * (180 / PI));
	float yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);
	//float pitch = atan2f(-aimVec.z, mag) * 180 / 3.14159265358979323846f;
	//
	float headhightdifference = target.z - myPos->z;	
	float angle1 = -asin(headhightdifference / deltaVecLength);
	//
	if ((pitch >= -30 && pitch <= 30) && (yaw >= -180 && yaw <= 180))
	{			
			viewAngles->x = pitch;	
			viewAngles->y = yaw;	
			Sleep(5);
	}
	return true;
}


