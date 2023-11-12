#pragma once
#include "GravitySource.h"
#include "olcPixelGameEngine.h"
#include <array>
#include <chrono>
#include <immintrin.h>
#include <random>

static constexpr double PI = 3.14159;
static constexpr double G = 0.0006673;
static constexpr double drag_coeff = 0.01;
static const __m256 grav_const = _mm256_set1_ps(G);

class Particles {
private:
  std::vector<float> x_pos_vector;
  std::vector<float> y_pos_vector;
  std::vector<float> x_vel_vector;
  std::vector<float> y_vel_vector;
  float *x_position;
  float *y_position;
  float *x_velocity;
  float *y_velocity;
  int64_t particle_count;
  __m256 mass_const;
  olc::Pixel color;

public:
  Particles(int64_t particle_count, int64_t mass, const olc::Pixel &color,
            int64_t screen_width, int64_t screen_height);

  inline void update_gravity_velocities(const GravitySource &source, float dt) {
    const olc::vf2d &src_position = source.get_position();
    __m256 src_x_pos = _mm256_set1_ps(src_position.x);
    __m256 src_y_pos = _mm256_set1_ps(src_position.y);
    __m256 src_mass = _mm256_set1_ps(source.get_mass());
    __m256 force_const =
        _mm256_mul_ps(_mm256_mul_ps(grav_const, mass_const), src_mass);
    __m256 delta_time = _mm256_set1_ps(dt);

    for (int i = 0; i < particle_count; i += 8) {
      __m256 particle_x_pos = _mm256_loadu_ps(x_position + i);
      __m256 particle_y_pos = _mm256_loadu_ps(y_position + i);
      __m256 diff_x = _mm256_sub_ps(particle_x_pos, src_x_pos);
      __m256 diff_y = _mm256_sub_ps(particle_y_pos, src_y_pos);
      __m256 magn = _mm256_sqrt_ps(_mm256_add_ps(
          _mm256_mul_ps(diff_x, diff_x), _mm256_mul_ps(diff_y, diff_y)));
      __m256 unit_v_x = _mm256_div_ps(diff_x, magn);
      __m256 unit_v_y = _mm256_div_ps(diff_y, magn);
      __m256 gravity_force =
          _mm256_xor_ps(_mm256_div_ps(force_const, _mm256_mul_ps(magn, magn)),
                        _mm256_set1_ps(-0.0));
      __m256 gravity_acceleration_x = _mm256_mul_ps(
          _mm256_div_ps(_mm256_mul_ps(unit_v_x, gravity_force), mass_const),
          delta_time);
      __m256 gravity_acceleration_y = _mm256_mul_ps(
          _mm256_div_ps(_mm256_mul_ps(unit_v_y, gravity_force), mass_const),
          delta_time);
      __m256 current_vel_x = _mm256_loadu_ps(x_velocity + i);
      __m256 current_vel_y = _mm256_loadu_ps(y_velocity + i);
      _mm256_storeu_ps(x_velocity + i,
                       _mm256_add_ps(current_vel_x, gravity_acceleration_x));
      _mm256_storeu_ps(y_velocity + i,
                       _mm256_add_ps(current_vel_y, gravity_acceleration_y));
    }
  }
  void update_drag_velocities();
  inline void update_positions(int64_t width, int64_t height) {
    for (int i = 0; i < particle_count; i += 8) {
      _mm256_storeu_ps(x_position + i,
                       _mm256_add_ps(_mm256_loadu_ps(x_position + i),
                                     _mm256_loadu_ps(x_velocity + i)));
      _mm256_storeu_ps(y_position + i,
                       _mm256_add_ps(_mm256_loadu_ps(y_position + i),
                                     _mm256_loadu_ps(y_velocity + i)));
    }
  }
  inline void render(olc::PixelGameEngine *window) {
    for (int i = 0; i < particle_count; i += 8) {
      window->Draw(x_position[i], y_position[i], color);
      window->Draw(x_position[i + 1], y_position[i + 1], color);
      window->Draw(x_position[i + 2], y_position[i + 2], color);
      window->Draw(x_position[i + 3], y_position[i + 3], color);
      window->Draw(x_position[i + 4], y_position[i + 4], color);
      window->Draw(x_position[i + 5], y_position[i + 5], color);
      window->Draw(x_position[i + 6], y_position[i + 6], color);
      window->Draw(x_position[i + 7], y_position[i + 7], color);
    }
  };

  inline void update(const GravitySource &source, const int &width,
                     const int &height, float dt) {
    if (source.is_active()) {
      update_gravity_velocities(source, dt);
    }
    update_positions(width, height);
  }
};
