#include "includes.h"

// credits to:
// Broihon and 0xDEC0DE for the dummy device template





// data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {	
	if (!pDevice)
		pDevice = o_pDevice;

	// drawing stuff	
	for (int i = 1; i < 32; i++) 
	{
		Ent* curEnt = hack->entList->ents[i].ent;
		if (!hack->CheckValidEnt(curEnt))
			continue;

		D3DCOLOR color=D3DCOLOR_ARGB(255,0,0,255);
		//Vec3 entHead3D = hack->GetBonePos(curEnt, 8);		
		//Vec2 entPos2D, entHead2D;
		//
		////aimbot test
		//hack->WorldToScreen(entHead3D, entHead2D);
		//DrawFilledRect(entHead2D.x-5, entHead2D.y-5, 10, 10, color);
		//end test - works good af

		//aimbot test 2-aim
		
			int closest_index = getclosestentity(curEnt->vecOrigin, 32, hack);
			Ent* target = hack->entList->ents[closest_index].ent;
			//draw_player_skeleton(hack, target);

			//aimbot 
			Vec3 enthead3d = hack->GetBonePos(target,8);
			Vec2 entpos2d, enthead2d;
			hack->WorldToScreen(enthead3d, enthead2d);
			DrawFilledRect(enthead2d.x - 5, enthead2d.y - 5, 10, 10, color);
			aimat(hack->GetBonePos(target,8),hack);
			//end of aimbot
	}

	// crosshair
	Vec2 l, r, t, b;
	l = r = t = b = hack->crosshair2D;
	l.x -= hack->crosshairSize+3;
	r.x += hack->crosshairSize+3;
	b.y += hack->crosshairSize+5;
	t.y -= hack->crosshairSize+5;

	DrawLine(l, r, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawLine(t, b, 2, D3DCOLOR_ARGB(255, 255, 255, 255));

	// call og function
	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {

	// hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	hack = new Hack();
	hack->Init();

	// hack loop
	while (!GetAsyncKeyState(VK_END)) 
	{
		hack->Update();

		Vec3 pAng = hack->localEnt->aimPunchAngle;

		hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
		hack->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);
	}

	// unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);
	Sleep(1000);

	// uninject
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	return TRUE;
}