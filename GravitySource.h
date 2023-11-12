#pragma once
#include "olcPixelGameEngine.h"
#include <utility>

class GravitySource {
private:
  olc::vf2d m_position;
  float m_mass;
  bool m_activated{false};

public:
  GravitySource(const olc::vf2d &position, const float &mass);
  inline void update_position(const olc::vf2d &position) {
    this->m_position = position;
  };
  inline float get_mass() const { return m_mass; };
  inline const olc::vf2d &get_position() const { return m_position; };
  inline void activate() { m_activated = true; };
  inline void deactivate() { m_activated = false; };
  inline bool is_active() const { return m_activated; };
};
