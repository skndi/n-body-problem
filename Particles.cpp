#include "Particles.h"

Particles::Particles(const int& particle_count, const int& mass, const olc::Pixel& color, const int& screen_width, const int& screen_height) {
	x_velocity = new float[particle_count] {};
	y_velocity = new float[particle_count] {};
	x_position = new float[particle_count] {};
	y_position = new float[particle_count] {};
	this->particle_count = particle_count;
	this->mass = mass;
	this->color = color;
	mass_const = _mm256_set1_ps(mass);
	std::mt19937_64 rng = std::mt19937_64();;
	std::uniform_real_distribution<float> w(0, screen_width);
	std::uniform_real_distribution<float> h(0, screen_height);
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);

	for (int i = 0, j = 0; i < particle_count; i++, j += 2) {
		x_position[i] = w(rng);
		y_position[i] = h(rng);
	}

};

void Particles::update_gravity_velocities(const GravitySource& source, float dt) {

	olc::vf2d src_position = source.get_position();
	float source_mass = source.get_mass();
	__m256 src_x_pos = _mm256_set1_ps(src_position.x);
	__m256 src_y_pos = _mm256_set1_ps(src_position.y);
	__m256 src_mass = _mm256_set1_ps(source_mass);
	__m256 force_const = _mm256_mul_ps(_mm256_mul_ps(grav_const, mass_const), src_mass);
	__m256 delta_time = _mm256_set1_ps(dt);

	for (int i = 0; i < particle_count; i += 8) {
		__m256 particle_x_pos = _mm256_loadu_ps(x_position + i);
		__m256 particle_y_pos = _mm256_loadu_ps(y_position + i);
		__m256 diff_x = _mm256_sub_ps(particle_x_pos, src_x_pos);
		__m256 diff_y = _mm256_sub_ps(particle_y_pos, src_y_pos);
		__m256 magn = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(diff_x, diff_x), _mm256_mul_ps(diff_y, diff_y)));
		__m256 unit_v_x = _mm256_div_ps(diff_x, magn);
		__m256 unit_v_y = _mm256_div_ps(diff_y, magn);
		__m256 gravity_force = _mm256_xor_ps(_mm256_div_ps(force_const, _mm256_mul_ps(magn, magn)), _mm256_set1_ps(-0.0));
		__m256 gravity_acceleration_x = _mm256_mul_ps(_mm256_div_ps(_mm256_mul_ps(unit_v_x, gravity_force), mass_const), delta_time);
		__m256 gravity_acceleration_y = _mm256_mul_ps(_mm256_div_ps(_mm256_mul_ps(unit_v_y, gravity_force), mass_const), delta_time);
		__m256 current_vel_x = _mm256_loadu_ps(x_velocity + i);
		__m256 current_vel_y = _mm256_loadu_ps(y_velocity + i);
		_mm256_storeu_ps(x_velocity + i, _mm256_add_ps(current_vel_x, gravity_acceleration_x));
		_mm256_storeu_ps(y_velocity + i, _mm256_add_ps(current_vel_y, gravity_acceleration_y));
		_mm256_zeroall();
	}
}

void Particles::update_positions(const int& width, const int& height) {
	for (int i = 0; i < particle_count; i += 8) {
		_mm256_storeu_ps(x_position + i, _mm256_add_ps(_mm256_loadu_ps(x_position + i), _mm256_loadu_ps(x_velocity + i)));
		_mm256_storeu_ps(y_position + i, _mm256_add_ps(_mm256_loadu_ps(y_position + i), _mm256_loadu_ps(y_velocity + i)));
	}
}

void Particles::update(const GravitySource& source, const int& width, const int& height, float dt) {
	if (source.is_active()) {
		update_gravity_velocities(source, dt);
	}
	update_positions(width, height);
}

void Particles::render(olc::PixelGameEngine* window) {
	for (int i = 0; i < particle_count; i++) {
		window->Draw(x_position[i], y_position[i], color);
	}
}

olc::Pixel Particles::get_color() const {
	return color;
}

Particles::~Particles() {
	delete[] x_position;
	delete[] y_position;
	delete[] x_velocity;
	delete[] y_velocity;
}

