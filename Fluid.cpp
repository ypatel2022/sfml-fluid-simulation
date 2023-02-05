#include "Fluid.h "
#include <array>
#include "Constants.h"
#include "Physics.h"

Fluid::Fluid(int diffusion, int viscosity, float dt)
{
	this->size = size;
	this->dt = dt;
	this->diff = diffusion;
	this->visc = viscosity;

	this->s.reserve(N * N);

	this->density.reserve(N * N);

	this->Vx.reserve(N * N);
	this->Vy.reserve(N * N);

	this->Vx0.reserve(N * N);
	this->Vy0.reserve(N * N);
}


void Fluid::AddDensity(int x, int y, float amount)
{
	int index = IX(x, y);
	this->density[index] += amount;
}

void Fluid::AddVelocity(int x, int y, float amountX, float amountY)
{
	int N = this->size;
	int index = IX(x, y);

	this->Vx[index] += amountX;
	this->Vy[index] += amountY;
}

void Fluid::Step()
{
	int N = this->size;
	float visc = this->visc;
	float diff = this->diff;
	float dt = this->dt;
	std::vector<float> Vx = this->Vx;
	std::vector<float> Vy = this->Vy;
	std::vector<float> Vx0 = this->Vx0;
	std::vector<float> Vy0 = this->Vy0;
	std::vector<float> s = this->s;
	std::vector<float> density = this->density;

	Physics::Diffuse(1, Vx0, Vx, visc, dt);
	Physics::Diffuse(2, Vy0, Vy, visc, dt);

	Physics::Project(Vx0, Vy0, Vx, Vy);

	Physics::Advection(1, Vx, Vx0, Vx0, Vy0, dt);
	Physics::Advection(2, Vy, Vy0, Vx0, Vy0, dt);

	Physics::Project(Vx, Vy, Vx0, Vy0);

	Physics::Diffuse(0, s, density, diff, dt);
	Physics::Advection(0, density, s, Vx, Vy, dt);
}

