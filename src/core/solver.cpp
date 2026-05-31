#include "solver.h"
#include <utility>

namespace core {

	Solver::Solver(int width, int height): m_width(width), m_height(height), m_size(width*height)
	{
		v_x_prev.resize(m_size, 0.0f);
		v_x_curr.resize(m_size, 0.0f);
		v_y_prev.resize(m_size, 0.0f);
		v_y_curr.resize(m_size, 0.0f);
		ink_rho_prev.resize(m_size, 0.0f);
		ink_rho_curr.resize(m_size, 0.0f);
	}

	void Solver::swap_buffers(){
		std::swap(v_x_curr, v_x_prev);
		std::swap(v_y_curr, v_y_prev);
		std::swap(ink_rho_curr, ink_rho_prev);
	}

	inline int Solver::map_2d_to_1d_index(int x, int y) const {
		return y*m_width + x;
	}

	void Solver::solve(){

	}

	void Solver::add_ink_source(int x, int y, int rho) {
		ink_rho_curr[map_2d_to_1d_index(x, y)] += rho;
	}
}