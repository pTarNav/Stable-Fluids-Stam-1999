#pragma once
#include <vector>
#include <span>

namespace core {
	class Solver{
		public:
			Solver(int width, int height);
			
			void swap_buffers();

			void solve();
			
			std::span<const float> get_velocity_magnitude() const;

			void add_ink_source(int x, int y, float rho);
			std::span<const float> get_ink_density() const;

		private:
			int m_width;
			int m_height;
			int m_size;
			
			int map_2d_to_1d_index(int x, int y) const;
			
			std::vector<float> v_x_prev;
			std::vector<float> v_x_curr;
			std::vector<float> v_y_prev;
			std::vector<float> v_y_curr;
			std::vector<float> ink_rho_prev;
			std::vector<float> ink_rho_curr;
	};
}