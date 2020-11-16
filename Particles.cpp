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

void Particles::update_gravity_velocities(const GravitySource& source) {

	olc::vf2d src_position = source.get_position();
	float source_mass = source.get_mass();
	__m256 src_x_pos = _mm256_set1_ps(src_position.x);
	__m256 src_y_pos = _mm256_set1_ps(src_position.y);
	__m256 src_mass = _mm256_set1_ps(source_mass);
	__m256 force_const = _mm256_mul_ps(_mm256_mul_ps(grav_const, mass_const), src_mass);

	for (int i = 0; i < particle_count; i += 8) {
		__m256 particle_x_pos = _mm256_loadu_ps(x_position + i);
		__m256 particle_y_pos = _mm256_loadu_ps(y_position + i);
		__m256 diff_x = _mm256_sub_ps(particle_x_pos, src_x_pos);
		__m256 diff_y = _mm256_sub_ps(particle_y_pos, src_y_pos);
		__m256 magn = _mm256_sqrt_ps(_mm256_add_ps(_mm256_mul_ps(diff_x, diff_x), _mm256_mul_ps(diff_y, diff_y)));
		__m256 unit_v_x = _mm256_div_ps(diff_x, magn);
		__m256 unit_v_y = _mm256_div_ps(diff_y, magn);
		__m256 gravity_force = _mm256_xor_ps(_mm256_div_ps(force_const, _mm256_mul_ps(magn, magn)), _mm256_set1_ps(-0.0));
		__m256 gravity_acceleration_x = _mm256_div_ps(_mm256_mul_ps(unit_v_x, gravity_force), mass_const);
		__m256 gravity_acceleration_y = _mm256_div_ps(_mm256_mul_ps(unit_v_y, gravity_force), mass_const);
		__m256 current_vel_x = _mm256_loadu_ps(x_velocity + i);
		__m256 current_vel_y = _mm256_loadu_ps(y_velocity + i);
		_mm256_storeu_ps(x_velocity + i, _mm256_add_ps(current_vel_x, gravity_acceleration_x));
		_mm256_storeu_ps(y_velocity + i, _mm256_add_ps(current_vel_y, gravity_acceleration_y));
		_mm256_zeroall();
	}

}

void Particles::update_drag_velocities() {
	/*for (int i = 0; i < particle_count; i++) {
		float speed = magnitude(velocities[i]);
		if (speed) {
			sf::Vector2f unit_v_drag = velocities[i] / speed;
			float drag_force = -1 * speed * speed * drag_coeff;
			sf::Vector2f drag_acceleraton = unit_v_drag * drag_force;
			velocities[i] += drag_acceleraton;
		}
		particles->operator[](i).position += (sf::Vector2f)velocities[i];
	}*/
}

void Particles::update_positions(const int& width, const int& height) {
	for (int i = 0; i < particle_count; i += 8) {
		_mm256_storeu_ps(x_position + i, _mm256_add_ps(_mm256_loadu_ps(x_position + i), _mm256_loadu_ps(x_velocity + i)));
		_mm256_storeu_ps(y_position + i, _mm256_add_ps(_mm256_loadu_ps(y_position + i), _mm256_loadu_ps(y_velocity + i)));
		/*if (x_position[i] > width) x_position[i] = 0;
		else if (x_position[i] < 0) x_position[i] = width;
		if (x_position[i + 1] > width) x_position[i + 1] = 0;
		else if (x_position[i + 1] < 0) x_position[i + 1] = width;
		if (x_position[i + 2] > width) x_position[i + 2] = 0;
		else if (x_position[i + 2] < 0) x_position[i + 2] = width;
		if (x_position[i + 3] > width) x_position[i + 3] = 0;
		else if (x_position[i + 3] < 0) x_position[i + 3] = width;
		if (x_position[i + 4] > width) x_position[i + 4] = 0;
		else if (x_position[i + 4] < 0) x_position[i + 4] = width;
		if (x_position[i + 5] > width) x_position[i + 5] = 0;
		else if (x_position[i + 5] < 0) x_position[i + 5] = width;
		if (x_position[i + 6] > width) x_position[i + 6] = 0;
		else if (x_position[i + 6] < 0) x_position[i + 6] = width;
		if (x_position[i + 7] > width) x_position[i + 7] = 0;
		else if (x_position[i + 7] < 0) x_position[i + 7] = width;
		if (y_position[i] > height) y_position[i] = 0;
		else if (y_position[i] < 0) y_position[i] = height;
		if (y_position[i + 1] > height) y_position[i + 1] = 0;
		else if (y_position[i + 1] < 0) y_position[i + 1] = height;
		if (y_position[i + 2] > height) y_position[i + 2] = 0;
		else if (y_position[i + 2] < 0) y_position[i + 2] = height;
		if (y_position[i + 3] > height) y_position[i + 3] = 0;
		else if (y_position[i + 3] < 0) y_position[i + 3] = height;
		if (y_position[i + 4] > height) y_position[i + 4] = 0;
		else if (y_position[i + 4] < 0) y_position[i + 4] = height;
		if (y_position[i + 5] > height) y_position[i + 5] = 0;
		else if (y_position[i + 5] < 0) y_position[i + 5] = height;
		if (y_position[i + 6] > height) y_position[i + 6] = 0;
		else if (y_position[i + 6] < 0) y_position[i + 6] = height;
		if (y_position[i + 7] > height) y_position[i + 7] = 0;
		else if (y_position[i + 7] < 0) y_position[i + 7] = height;*/
	}
}

void Particles::update(const GravitySource& source, const int& width, const int& height) {
	if (source.is_active()) {
		update_gravity_velocities(source);
	}

	//update_drag_velocities();
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

