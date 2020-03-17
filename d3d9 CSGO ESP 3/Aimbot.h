#pragma once

#include "includes.h"

float GetDistance(Vec3 other, Vec3 myPos);

int getclosestentity(Vec3 mypos, int maxentities, Hack* hack);

bool aimat(Vec3 target, Hack* hack);

bool softaimat(Vec3 target, Hack* hack);

int getclosestentitybyview(Hack* hack);

bool literlyaimat(Vec2 yawpitch, Hack* hack);

Vec2 getyawpitch(Vec3 target, Hack* hack);

bool aimat2(Vec3 target, Hack* hack);