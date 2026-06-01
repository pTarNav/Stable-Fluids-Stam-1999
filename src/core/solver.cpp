#include "solver.h"
#include <iostream>
#include <utility>
#include <span>
#include <algorithm>
#include <cmath>

namespace core {

	Solver::Solver(int width, int height, float DT): m_width(width), m_height(height), m_size(width*height), m_DT(DT)
	{
		v_x_prev.resize(m_size, 0.0f);
		v_x_curr.resize(m_size, 0.0f);
		v_y_prev.resize(m_size, 0.0f);
		v_y_curr.resize(m_size, 0.0f);
		ink_rho_prev.resize(m_size, 0.0f);
		ink_rho_curr.resize(m_size, 0.0f);
		f_x.resize(m_size, 0.0f);
		f_y.resize(m_size, 0.0f);
		m_velocity_magnitude.resize(m_size, 0.0f);
	}

	void Solver::swap_buffers(){
		std::swap(v_x_curr, v_x_prev);
		std::swap(v_y_curr, v_y_prev);
		std::swap(ink_rho_curr, ink_rho_prev);
	}

	inline int Solver::map_2d_to_1d_index(int x, int y) const {
		return y*m_width + x;
	}

	inline std::pair<int, int> Solver::map_1d_to_2d_index(int i) const {
		return {i%m_width, (int)i/m_width};
	}

	void Solver::step(){
		// Fluid
		add_forces();
		advect(v_x_curr, v_x_prev, v_x_prev, v_y_prev);
		advect(v_y_curr, v_y_prev, v_x_prev, v_y_prev);
		diffuse(v_x_curr, v_x_prev, 0.5f);
		diffuse(v_y_curr, v_y_prev, 0.5f);
		project();

		// Ink
		advect(ink_rho_curr, ink_rho_prev, v_x_curr, v_y_curr);
		diffuse(ink_rho_curr, ink_rho_prev, 0.5f);
	}

	void Solver::add_forces(){
		for (int i = 0; i < m_size; i++)
		{
			v_x_prev[i] += m_DT*f_x[i];
			v_y_prev[i] += m_DT*f_y[i];
		}
		clear_force_sources();
	}

	void Solver::clear_force_sources(){
		std::fill(f_x.begin(), f_x.end(), 0.0f);
		std::fill(f_y.begin(), f_y.end(), 0.0f);
	}

	void Solver::advect(std::vector<float>& target, std::vector<float>& source, std::vector<float>& v_x, std::vector<float>& v_y){
		for (int i = 0; i < m_size; i++){
			auto [curr_x, curr_y] = map_1d_to_2d_index(i);
			float prev_x = curr_x - m_DT*v_x[map_2d_to_1d_index(curr_x, curr_y)];
			float prev_y = curr_y - m_DT*v_y[map_2d_to_1d_index(curr_x, curr_y)];

			// TODO: boundary clamp (this is by AI)
			if (prev_x < 0.5f) prev_x = 0.5f;
			if (prev_x > (float)m_width - 1.5f) prev_x = (float)m_width - 1.5f;
			if (prev_y < 0.5f) prev_y = 0.5f;
			if (prev_y > (float)m_height - 1.5f) prev_y = (float)m_height - 1.5f;
			
			int prev_x_floor = (int)prev_x;
			int prev_x_ceil = prev_x_floor + 1;
			int prev_y_floor = (int)prev_y;
			int prev_y_ceil = prev_y_floor + 1;
			float a_x = prev_x - prev_x_floor;
			float b_x = 1.0f - a_x;
			float a_y = prev_y - prev_y_floor;
			float b_y = 1.0f - a_y;
			
			target[i] = 
				b_x*b_y*source[map_2d_to_1d_index(prev_x_floor, prev_y_floor)] +
				b_x*a_y*source[map_2d_to_1d_index(prev_x_floor, prev_y_ceil)] +
				a_x*b_y*source[map_2d_to_1d_index(prev_x_ceil, prev_y_floor)] +
				a_x*a_y*source[map_2d_to_1d_index(prev_x_ceil, prev_y_ceil)];
		}
	}

	void Solver::diffuse(std::vector<float>& target, std::vector<float>& source, float diffusion_rate){

	}

	void Solver::project(){

	}

	std::span<const float> Solver::get_velocity_magnitude() {
		for (int i = 0; i < m_size; i++) {
			float vx = v_x_curr[i];
			float vy = v_y_curr[i];
			m_velocity_magnitude[i] = (vx*vx) + (vy*vy);		
		}
		return m_velocity_magnitude;
	}


	void Solver::add_ink_source(int x, int y, float rho){
		ink_rho_curr[map_2d_to_1d_index(x, y)] += rho;
	}

	std::span<const float> Solver::get_ink_density() const {
		return ink_rho_curr;
	}

	void Solver::add_force_source(int x, int y, float df_x, float df_y){
		std::cout << "Added force @(" << x << ", " << y << "): (" << df_x << ", " << df_y << ")" << std::endl;
		int i = map_2d_to_1d_index(x, y);
		f_x[i] += df_x;
		f_y[i] += df_y;
	}
}