#pragma once
#include <vector>

struct Fluid
{
	int size;
	float dt;
	float diff;
	float visc;

	std::vector<float> s;
	std::vector<float> density;

	std::vector<float> Vx;
	std::vector<float> Vy;

	std::vector<float> Vx0;
	std::vector<float> Vy0;

	Fluid(int diffusion, int viscosity, float dt);

	void AddDensity(int x, int y, float amount);

	void AddVelocity(int x, int y, float amountX, float amountY);

	void Step();
};

typedef struct Fluid Fluid;