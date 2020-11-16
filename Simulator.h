#pragma once
#include <memory>
#include "Particles.h"
#include "GravitySource.h"
#include <random>
#include <chrono>
#include "olcPixelGameEngine.h"

class Simulator : public olc::PixelGameEngine
{
private:
	int width, height, particle_count;
	GravitySource mouse;
	std::unique_ptr<Particles> particles;
	bool paused;
	bool mouse_pressed;
	olc::Pixel color;

public:
	Simulator(int width, int height, int particle_count, const olc::Pixel& color);
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	void update();
	void render();
	void check_events();

};