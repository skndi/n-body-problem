#include "GravitySource.h"

GravitySource::GravitySource(const olc::vf2d& position, const float& mass) {
	this->position = position;
	this->mass = mass;
	activated = false;
}

void GravitySource::update_position(const olc::vf2d& position) {
	this->position = position;
}

const float GravitySource::get_mass() const{
	return mass;
}

const olc::vf2d  GravitySource::get_position() const{
	return position;
}

void GravitySource::activate() {
	activated = true;
}

void GravitySource::deactivate() {
	activated = false;
}

bool GravitySource::is_active() const{
	return activated;
}