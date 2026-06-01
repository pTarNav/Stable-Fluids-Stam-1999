#pragma once
#include <vector>
#include <span>

namespace core {
	class Solver{
		public:
			Solver(int width, int height, float DT);
			
			void swap_buffers();

			void step();
			
			std::span<const float> get_velocity_magnitude();

			void add_ink_source(int x, int y, float rho);
			std::span<const float> get_ink_density() const;

			void add_force_source(int x, int y, float df_x, float df_y);

		private:
			int m_width;
			int m_height;
			int m_size;
			float m_DT;
			
			int map_2d_to_1d_index(int x, int y) const;
			std::pair<int, int> map_1d_to_2d_index(int i) const;
			
			std::vector<float> v_x_prev;
			std::vector<float> v_x_curr;
			std::vector<float> v_y_prev;
			std::vector<float> v_y_curr;
			std::vector<float> ink_rho_prev;
			std::vector<float> ink_rho_curr;
			std::vector<float> f_x;
			std::vector<float> f_y;
			std::vector<float> m_velocity_magnitude;

			void add_forces();
			void advect(std::vector<float>& target, std::vector<float>& source, std::vector<float>& v_x, std::vector<float>& v_y);
			void diffuse(std::vector<float>& target, std::vector<float>& source, float diffusion_rate);
			void project();

			void clear_force_sources();
	};
}