#include "Simulator.h"

Simulator::Simulator(int64_t width, int64_t height, int64_t particle_count,
                     const olc::Pixel &color)
    : width{width}, height{height}, particle_count{particle_count},
      mouse(Simulator::GetMousePos(), 1000000), particles{particle_count, 1,
                                                          color, width,
                                                          height} {
  sAppName = "Particles";
  this->width = width;
  this->height = height;
  this->particle_count = particle_count;
  paused = false;
  mouse_pressed = false;
  ;
}

void Simulator::render() {
  Simulator::Clear(olc::BLACK);
  particles.render(this);
}

void Simulator::update(float dt) {
  particles.update(mouse, width, height, dt);
  if (mouse_pressed) {
    mouse.activate();
    mouse.update_position(Simulator::GetMousePos());
  } else
    mouse.deactivate();
}

bool Simulator::OnUserCreate() { return true; }

bool Simulator::OnUserUpdate(float fElapsedTime) {
  check_events();
  update(fElapsedTime);
  render();
  return true;
}

void Simulator::check_events() {

  if (Simulator::GetKey(olc::Key::SPACE).bPressed) {
    paused = true;
  }
  if (Simulator::GetMouse(0).bHeld) {
    mouse_pressed = true;
  }
  if (Simulator::GetMouse(0).bReleased) {
    mouse_pressed = false;
  }
}