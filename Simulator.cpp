#include "Simulator.h"

Simulator::Simulator(int width, int height, int particle_count, const olc::Pixel& color) : mouse(Simulator::GetMousePos(), 1000000){
    sAppName = "Particles";
	this->width = width;
	this->height = height;
	this->particle_count = particle_count;
    paused = false;
    mouse_pressed = false;
    particles = std::make_unique<Particles>(particle_count, 1, color, width, height);
}

void Simulator::render() {
    Simulator::Clear(olc::BLACK);
    particles->render(this);
}

void Simulator::update() {
    particles->update(mouse, width, height);
    if (mouse_pressed) {
        mouse.activate();
        mouse.update_position(Simulator::GetMousePos());
    }
    else mouse.deactivate();
}

bool Simulator::OnUserCreate() {
    return true;
}

bool Simulator::OnUserUpdate(float fElapsedTime) {
    check_events();
    update();
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