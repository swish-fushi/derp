#pragma once

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))

struct Vec2 {
	float x, y;
};
struct Vec3 {
	float x, y, z;
};
struct Vec4 {
	float x, y, z, w;
};

class Ent {
public:
	union {
		// isDormant
		DEFINE_MEMBER_N(bool, isDormant, 0xED);
		// iHealth
		DEFINE_MEMBER_N(int, iHealth, 0x100);
		// vecOrigin
		DEFINE_MEMBER_N(Vec3, vecOrigin, 0x138);
		// iTeamNum
		DEFINE_MEMBER_N(int, iTeamNum, 0xF4);
		// boneMatrix
		DEFINE_MEMBER_N(int, boneMatrix, 0x26A8);
		// armorValue
		DEFINE_MEMBER_N(int, ArmorValue, 0xB368);
		// aimPunchAngle
		DEFINE_MEMBER_N(Vec3, aimPunchAngle, 0x302C);
	};
};

class EntListObj {
public:
	Ent* ent;
	char padding[12];
};

class EntList {
public:
	EntListObj ents[32];
};

class Hack {
public:
	uintptr_t dwEntityList = 0x4D44A04;
	uintptr_t dwViewMatrix = 0x4D36344;
	uintptr_t dwState_ViewAngles = 0x4D88;
	uintptr_t m_vecViewOffset = 0x108;
	uintptr_t m_vecOrigin = 0x138;
	//
	uintptr_t dwState = 0x589D9C;//client state



	uintptr_t engine;
	uintptr_t dwLocalPlayer = 0xD30B84;
	uintptr_t client;
	Ent* localEnt;
	EntList* entList;
	float viewMatrix[16];

	ID3DXLine* LineL;

	Vec2 crosshair2D;
	int crosshairSize = 4;

	Vec3* viewAngles = (Vec3*)((engine + dwState) + dwState_ViewAngles);
	Vec3* myviewoff = (Vec3*)(client + dwLocalPlayer + m_vecViewOffset);
	Vec3 myPos = *(Vec3*)((client + dwLocalPlayer) + m_vecOrigin);
	Vec3 myheadview = { myPos.x += myviewoff->x, myPos.y += myviewoff->y,myPos.z += myviewoff->z };



	void Init();
	void Update();
	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 GetBonePos(Ent* ent, int bone);
};