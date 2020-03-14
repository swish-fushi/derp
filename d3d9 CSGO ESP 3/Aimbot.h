#pragma once

#include "includes.h"

float GetDistance(Vec3 other, Vec3 myPos);

int getclosestentity(Vec3 mypos, int maxentities, Hack* hack);

bool aimat(Vec3 target, Hack* hack);

