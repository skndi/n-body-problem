#include "Particles.h"

Particles::Particles(int64_t particle_count, int64_t mass,
                     const olc::Pixel &color, int64_t screen_width,
                     int64_t screen_height)
    : particle_count{particle_count}, color{color} {
  x_pos_vector.reserve(particle_count);
  y_pos_vector.reserve(particle_count);
  x_vel_vector.reserve(particle_count);
  y_vel_vector.reserve(particle_count);
  x_velocity = x_pos_vector.data();
  y_velocity = y_pos_vector.data();
  x_position = x_vel_vector.data();
  y_position = y_vel_vector.data();
  mass_const = _mm256_set1_ps(mass);
  std::mt19937_64 rng = std::mt19937_64();

  std::uniform_real_distribution<float> w(0, screen_width);
  std::uniform_real_distribution<float> h(0, screen_height);
  uint64_t timeSeed =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32)};
  rng.seed(ss);

  for (int i = 0, j = 0; i < particle_count; i++, j += 2) {
    x_position[i] = w(rng);
    y_position[i] = h(rng);
  }
};
