#include "Simulator.h"

int main() {
	int width = 700, height = 700, particle_count = 500000;
	Simulator sim(width, height, particle_count, olc::Pixel(255, 0, 255));
	sim.Construct(width, height, 1, 1);
	sim.Start();
	return 0;
}