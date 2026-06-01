#include "solver.h"
#include <utility>
#include <span>
#include <algorithm>

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
	}

	void Solver::swap_buffers(){
		std::swap(v_x_curr, v_x_prev);
		std::swap(v_y_curr, v_y_prev);
		std::swap(ink_rho_curr, ink_rho_prev);
	}

	inline int Solver::map_2d_to_1d_index(int x, int y) const {
		return y*m_width + x;
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
			v_x_curr[i] += m_DT*f_x[i];
			v_y_curr[i] += m_DT*f_y[i];
		}
		clear_force_sources();
	}

	void Solver::advect(std::vector<float>& target, std::vector<float>& source, std::vector<float>& v_x, std::vector<float>& v_y){
		
	}

	void Solver::diffuse(std::vector<float>& target, std::vector<float>& source, float diffusion_rate){

	}

	void Solver::project(){

	}

	void Solver::add_ink_source(int x, int y, float rho){
		ink_rho_curr[map_2d_to_1d_index(x, y)] += rho;
	}

	std::span<const float> Solver::get_ink_density() const {
		return ink_rho_curr;
	}

	void Solver::add_force_source(int x, int y, float df_x, float df_y){
		int i = map_2d_to_1d_index(x, y);
		f_x[i] += df_x;
		f_y[i] += df_y;
	}

	void Solver::clear_force_sources(){
		std::fill(f_x.begin(), f_x.end(), 0.0f);
		std::fill(f_y.begin(), f_y.end(), 0.0f);
	}
}