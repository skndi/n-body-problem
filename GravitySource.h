#pragma once
#include <utility>
#include "olcPixelGameEngine.h"

class GravitySource
{
private:
	olc::vf2d position;
	float mass;
	bool activated;

public:
	GravitySource(const olc::vf2d&  position, const float& mass);
	void update_position(const olc::vf2d& position);
	const float get_mass() const;
	const olc::vf2d  get_position() const;
	void activate();
	void deactivate();
	bool is_active() const;
};

