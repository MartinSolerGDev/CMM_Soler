#pragma once
#include <random>

//Custom
#include "Duck.h"
#include "PlayerProjectile.h"


class FileHelpers
{
public:
	static float RandomFloat(float min, float max);
	static int RandomInt(int min, int max);
	static bool RandomChance(float probability);
	static PlayerProjectileType DuckTypeToProjectileType(DuckType duckType);
	static int ScorePerDuck(DuckType duckType); 
};

