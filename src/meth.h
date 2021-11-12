#pragma once
// math library
#include <cmath>
#include <iostream>

#define M_PI 3.1415926535

namespace meth
{
	class vec2i {
	private:
		int x, y;
	public:
		vec2i(int _x, int _y) : x(_x), y(_y) {};		
	};


	// shortcuts for direction vectors
	vec2i vec2iUp(0, 1);


	class vec3f {
	private:
		float values[3];
	public:
		vec3f() : values{ 0, 0, 0 } {}
		vec3f(float _x, float _y, float _z) : values{ _x, _y, _z } {};

		float &operator[](int i) {
			return values[i];
		}
		float operator[](int i) const {
			return values[i];
		}

		friend vec3f operator-(const vec3f& u, const vec3f& v) {
			return vec3f(u.values[0] - v.values[0], u.values[1] - v.values[1], u.values[2] - v.values[2]);
		}

		friend std::ostream& operator<<(std::ostream& os, const vec3f& m) {
			os << "{";
			for (size_t i = 0; i < 4; ++i)
				os << m.values[i] << " ";
			os << "}" << std::endl;
			return os;
		}
	};

	class vec4f {
	private:
		float values[4];
	public:
		vec4f() : values{ 0, 0, 0, 0 } {}
		vec4f(float _x, float _y, float _z, float _w) : values{ _x, _y, _z, _w } {};

		float* get_values() {
			return values;
		}

		friend std::ostream& operator<<(std::ostream& os, const vec4f& m) {
			os << "{";
			for (size_t i = 0; i < 4; ++i)
				os << m.values[i] << " ";
			os << "}" << std::endl;
			return os;
		}

	};

	class mat4f {
	private:
		float values[16];
	public:
		mat4f() : values{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}
		mat4f(float f) : values{f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f} {}
		mat4f(float val[16]) {
			std::memcpy(values, val, sizeof(values));
		}

		float* get_values() {
			return values;
		}
		void fill_ascending() {
			for (size_t i = 1; i < 17; ++i)
			{
				values[i-1] = i;
			}
		}
		void fill_identity(float f) {
			for (size_t i = 0; i < 16; i++)
			{
				if (i % 5 == 0)
					values[i] = f;
				else
					values[i] = 0;
			}
		}


		vec4f operator*(vec4f& v) const {
			vec4f result;
			for (size_t i = 0; i < 4; ++i) {
				for (size_t j = 0; j < 4; ++j) {
					result.get_values()[j] += this->values[i + j * 4] * v.get_values()[i];
				}
			}
			return result;
		}

		mat4f operator*(const mat4f& m) const {
			mat4f result;
			int row = 0;
			int column = 0;
			for (size_t i = 0; i < 16; i++)
			{
				float res = 0;
				//std::cout << row << std::endl;
				//std::cout << column << std::endl;
				for (size_t j = 0; j < 4; j++)
				{
					//std::cout << "this->values[j + row * 4] = " << this->values[j + row * 4] << std::endl;
					//std::cout << "m.values[row * j + 4] = "   << m.values[4 * j + column] << std::endl;
					res += this->values[j + row * 4] * m.values[4 * j + column];
				}
				result.values[i] = res;
				if ((i + 1) % 4 == 0) {
					row++;
				}
				column++;
				if (column == 4)
					column = 0;
			}
			return result;
		}
		float &operator[](int i){
			return values[i];
		}

		friend std::ostream& operator<<(std::ostream& os, const mat4f& m)
		{
			os << "{";
			for (int i = 1; i < 17; ++i) {
				os << m.values[i-1] << " ";
				if (i == 16) {
					os << "}";
					break;
				}
				if (i % 4 == 0)
					os << "}" << std::endl << "{";
			}
			os << std::endl;
			return os;
		}
	};

	static mat4f get_view_matrix() {

	}

	static mat4f get_perspective_matrix(float b, float t, float l, float r, float n, float f) {
		mat4f result;

		result[0] = 2 * n / (r - l);
		result[1] = 0;
		result[2] = 0;
		result[3] = 0;
		result[4] = 0;
		result[5] = 2 * n / (t - b);
		result[6] = 0;
		result[7] = 0;
		result[8] = (r + l) / (r - l);
		result[9] = (t + b) / (t - b);
		result[10] = -(f + n) / (f - n);
		result[11] = -1;
		result[12] = 0;
		result[13] = 0;
		result[14] = -2 * f * n / (f - n);
		result[14] = 0;

		return result;
	}

	vec3f normalize(const vec3f& v) {
		vec3f result = v;
		float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		result[0] = v[0] / length;
		result[1] = v[1] / length;
		result[2] = v[2] / length;
		return result;
	}
	vec4f normalize() {
		return vec4f();

	}
	vec3f cross_product(const vec3f& u, const vec3f& v) {
		float x = u[1] * v[2] - u[2] * v[1];
		float y = u[2] * v[0] - u[0] * v[2];
		float z = u[0] * v[1] - u[1] * v[0];

		return vec3f(x, y, z);
	}

	mat4f look_at(const vec3f& cameraPosition, const vec3f& targetPosition, const vec3f& head) {
		mat4f result;

		vec3f forward = normalize(cameraPosition - targetPosition);
		vec3f right = cross_product(normalize(head), forward);
		vec3f up = cross_product(forward, right);

		result[0] = right[0];
		result[1] = right[1];
		result[2] = right[2];
		result[3] = up[0];
		result[4] = up[1];
		result[5] = up[2];
		result[6] = forward[0];
		result[7] = forward[1];
		result[8] = forward[2];

		result[11] = cameraPosition[0];
		result[12] = cameraPosition[1];
		result[13] = cameraPosition[2];


		return result;
	}

	void gluPerspective(
		const float& angleOfView,
		const float& aspectRatio,
		const float& n, const float& f,
		float& b, float& t, float& l, float& r) 
	{
		float scale = tan(angleOfView * 0.5 * M_PI / 180) * n;
		r = aspectRatio * scale, l = -r;
		t = scale, b = -t;
	}

	static mat4f identityMat4f(float f) {
		mat4f m;
		m.fill_identity(f);
		return m;
	}

}
