#include "Fluid.h "
#include <array>
#include "Constants.h"
#include "Physics.h"

#include <iostream>

int IX(int x, int y);
sf::Color Hsv(int hue, float sat, float val, float d);
float MapToRange(float val, float minIn, float maxIn, float minOut, float maxOut);

Fluid::Fluid(float diffusion, float viscosity, float dt)
{
	this->dt = dt;
	this->diff = diffusion;
	this->visc = viscosity;

	// create vectors with size N*N filled with 0
	this->cells.resize(N * N, sf::RectangleShape(sf::Vector2f(SCALE, SCALE)));

	this->s.resize(N * N, 0);

	this->density.resize(N * N, 0);

	this->Vx.resize(N * N, 0);
	this->Vy.resize(N * N, 0);

	this->Vx0.resize(N * N, 0);
	this->Vy0.resize(N * N, 0);

	// set position and size of cells
	for (int j = 0; j < N; j++)
	{
		for (int i = 0; i < N; i++)
		{
			int x = i * SCALE;
			int y = j * SCALE;

			int index = IX(i, j);
			this->cells[index].setPosition(sf::Vector2f(x, y));
		}
	}
}


void Fluid::AddDensity(int x, int y, float amount)
{
	this->density[IX(x, y)] += amount;
}

void Fluid::AddVelocity(int x, int y, float amountX, float amountY)
{
	this->Vx[IX(x, y)] += amountX;
	this->Vy[IX(x, y)] += amountY;
}

void Fluid::Step()
{
	Physics::Diffuse(1, this->Vx0, this->Vx, this->visc, this->dt);
	Physics::Diffuse(2, this->Vy0, this->Vy, this->visc, this->dt);

	Physics::Project(this->Vx0, this->Vy0, this->Vx, this->Vy);

	Physics::Advection(1, this->Vx, this->Vx0, this->Vx0, this->Vy0, this->dt);
	Physics::Advection(2, this->Vy, this->Vy0, this->Vx0, this->Vy0, dt);

	Physics::Project(this->Vx, this->Vy, this->Vx0, this->Vy0);

	Physics::Diffuse(0, this->s, this->density, this->diff, this->dt);
	Physics::Advection(0, this->density, this->s, this->Vx, this->Vy, this->dt);
}

void Fluid::RenderDensity(sf::RenderWindow& window)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int index = IX(i, j);

			int x = i * SCALE;
			int y = j * SCALE;
			float d = this->density[index];

			this->cells[index].setFillColor(Hsv(d, 1, 1, 255));

			window.draw(this->cells[index]);
		}
	}
}

void Fluid::RenderVelocity(sf::RenderWindow& window)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int index = IX(i, j);
			float vx = this->Vx[index];
			float vy = this->Vy[index];

			int r = MapToRange(vx, -0.05f, 0.05f, 0, 255);
			int g = MapToRange(vy, -0.05f, 0.05f, 0, 255);
			this->cells[index].setFillColor(sf::Color(r, g, 255));

			window.draw(this->cells[index]);
		}
	}
}

void Fluid::RenderDefault(sf::RenderWindow& window)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int index = IX(i, j);

			float d = std::min(255.f, this->density[index]);

			this->cells[index].setFillColor(sf::Color(255, 255, 255, d));

			window.draw(this->cells[index]);
		}
	}
}