#pragma once
#include "GravitySource.h"
#include <random>
#include <chrono>
#include <immintrin.h>
#include <array>
#include "olcPixelGameEngine.h"

const double PI = 3.14159;
const double G = 0.0006673;
const double drag_coeff = 0.01;
const __m256 grav_const = _mm256_set1_ps(G);

class Particles
{
private:
	float* x_position;
	float* y_position;
	float* x_velocity;
	float* y_velocity;
	int particle_count;
	float mass;
	__m256 mass_const;
	olc::Pixel color;

public:
	Particles(const int& particle_count, const int& mass, const olc::Pixel& color, const int& screen_width, const int& screen_height);
	void update_gravity_velocities(const GravitySource& source);
	void update_drag_velocities();
	void update_positions(const int& width, const int& height);
	void render(olc::PixelGameEngine* window);
	olc::Pixel get_color() const;
	void update(const GravitySource& source, const int& width, const int& height);
	~Particles();

};

