#include "GravitySource.h"

GravitySource::GravitySource(const olc::vf2d &position, const float &mass)
    : m_position{position}, m_mass{mass} {}
