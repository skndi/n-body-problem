#pragma once
#include "GravitySource.h"
#include "Particles.h"
#include "olcPixelGameEngine.h"
#include <chrono>
#include <memory>
#include <random>

class Simulator : public olc::PixelGameEngine {
private:
  int64_t width, height, particle_count;
  GravitySource mouse;
  Particles particles;
  bool paused;
  bool mouse_pressed;
  olc::Pixel color;

public:
  Simulator(int64_t width, int64_t height, int64_t particle_count,
            const olc::Pixel &color);
  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
  void update(float dt);
  void render();
  void check_events();
};