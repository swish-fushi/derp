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

int getclosestentitybyview(Hack* hack)
{
	float PI = 3.14159265359;
	Vec2 minyawpitch{ 999,999 };
	int minindex = 0;


	Vec3 viewAngles = *hack->viewAngles;
	Vec3 myPos = hack->myheadview;

	
	for (int i = 1; i < 32; i++)
	{
		Ent* curEnt = hack->entList->ents[i].ent;

		if (!hack->CheckValidEnt(curEnt))
			continue;
		if (curEnt->iTeamNum==hack->localEnt->iTeamNum)		
			continue;

		Vec3 target = hack->GetBonePos(curEnt, 8);

		Vec3 deltaVec = { target.x - myPos.x, target.y - myPos.y,target.z - myPos.z };
		float deltaVecLength = GetDistance(target, myPos);

		float pitch = ((-asin(deltaVec.z / deltaVecLength)) * (180 / PI));
		float yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);

		if ((pitch >= -89 && pitch <= 89) && (yaw >= -180 && yaw <= 180))//CHECK if angles are ok
		{
			if (((float)abs(pitch - viewAngles.x))+ ((float)abs(yaw - viewAngles.y)) < (minyawpitch.x)+minyawpitch.y)
			{
				minyawpitch.x = ((float)abs(pitch - viewAngles.x));
				minyawpitch.y = ((float)abs(pitch - viewAngles.x));
				minindex = i;
			}
			
		}
	}
	return minindex;
}

bool literlyaimat(Vec2 yawpitch,Hack* hack)
{
	float pitch = yawpitch.x;
	float yaw = yawpitch.y;
	Vec3* viewAngles = hack->viewAngles;
	Vec3* viewAngles1 = (Vec3*)(((hack->engine + hack->dwState)) + hack->dwState_ViewAngles);
	if ((pitch >= -89 && pitch <= 89) && (yaw >= -180 && yaw <= 180))
	{
		if (viewAngles->x != pitch)
		{
			viewAngles->x = pitch;
		}
		if (viewAngles->y != yaw)
		{
			viewAngles->y = yaw;
		}
	}


	return true;
}

bool softaimat(Vec3 target,Hack* hack)
{	
	/*if (GetAsyncKeyState(VK_MENU))
	{*/
		
		int flag = 0;
		Vec3* viewAngles = hack->viewAngles;					
		Vec2 pitchyaw = getyawpitch(target, hack);
		float pitch = pitchyaw.x;
		float yaw = pitchyaw.y;
		
		if ((pitch >= -89 && pitch <= 89) && (yaw >= -180 && yaw <= 180))
		{
			if (((float)abs(pitch-viewAngles->x))<2.f && viewAngles->x != pitch)
			{
				flag++;
			}
			if (((float)abs(yaw - viewAngles->y)) < 2.f && viewAngles->y != yaw)
			{
				flag++;
			}
		}
		if (flag==1)
		{
			literlyaimat(pitchyaw, hack);
		}
	//}
	return true;
}

bool aimat2(Vec3 target, Hack* hack)
{
	/*if (GetAsyncKeyState(VK_MENU))
	{*/
		Vec2 yawpitch = getyawpitch(target, hack);
		if ((yawpitch.x >= -89 && yawpitch.x <= 89) && (yawpitch.y >= -180 && yawpitch.y <= 180))
		{
			literlyaimat(yawpitch, hack);
		}		
	//}	
	return true;
}

bool aimat(Vec3 target, Hack* hack)
{

	float PI = 3.14159265359;

	uintptr_t clientstate = *(uintptr_t*)(hack->engine + hack->dwState);
	Vec3* viewAngles = (Vec3*)((clientstate)+hack->dwState_ViewAngles);
	clientstate = *(uintptr_t*)(hack->client + hack->dwLocalPlayer);
	Vec3 myPos = *(Vec3*)(clientstate + hack->m_vecOrigin);
	
	
	Vec3* myviewoff = (Vec3*)(clientstate + hack->m_vecViewOffset);
	myPos.x += myviewoff->x;
	myPos.y += myviewoff->y;
	myPos.z += myviewoff->z;	

	Vec3 deltaVec = { target.x - myPos.x, target.y - myPos.y,target.z - myPos.z };
	float deltaVecLength = GetDistance(target, myPos);

	float pitch = ((-asin(deltaVec.z / deltaVecLength)) * (180 / PI));
	float yaw = atan2f(deltaVec.y, deltaVec.x) * (180 / PI);

	
	if ((pitch >= -89 && pitch <= 89) && (yaw >= -180 && yaw <= 180))
	{
		if (viewAngles->x != pitch)
		{
			viewAngles->x = pitch;
		}
		if (viewAngles->y != yaw)
		{
			viewAngles->y = yaw;
		}
	}
	return true;
}

Vec2 getyawpitch(Vec3 target,Hack* hack)
{
	float PI = 3.14159265359;
	//Vec3 myPos = hack->myheadview;
	Vec3 myPos = *(Vec3*)(hack->client + hack->dwLocalPlayer + hack->m_vecOrigin);
	Vec3* myviewoff = (Vec3*)(hack->client + hack->dwLocalPlayer + hack->m_vecViewOffset);
	myPos.x += myviewoff->x; myPos.y += myviewoff->y; myPos.z += myviewoff->z;


	Vec3 deltaVec = { target.x - myPos.x, target.y - myPos.y,target.z - myPos.z };
	float deltaVecLength = GetDistance(target, myPos);
	Vec2 pitchyaw = { ((-asin(deltaVec.z / deltaVecLength)) * (180 / PI)) ,atan2f(deltaVec.y, deltaVec.x) * (180 / PI) };
	return pitchyaw;
}


