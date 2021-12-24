#pragma once
// math library
#include <iostream>
#include <random>

constexpr float M_PI = 3.1415926535f;

namespace meth
{
	static float MapRange(float iStart, float iEnd, float oStart, float oEnd, float value) {
		ASSERT(value >= iStart && value <= iEnd);
		return oStart + ((oEnd - oStart) / (iEnd - iStart) * value - iStart);
	}
	static bool SolveQuadratic(float a, float b, float c, float& x1, float& x2) {
		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0) return false;
		else if (discriminant == 0) x1 = x2 = (-b) / 2 * a;
		else {
			x1 = ((-b) + discriminant) / 2 * a;
			x1 = ((-b) - discriminant) / 2 * a;
			/*float q = (b > 0) ?
				-0.5f * (b + sqrt(discriminant)) :
				-0.5f * (b - sqrt(discriminant));
			x1 = q / a;
			x2 = c / q;*/
		}
		if (x1 > x2) ::std::swap(x1, x2);

		return true;
	}
	static float Lerp(float a, float b, float w) {
		return (b - a) * w + a;
	}


	class Random {
	private:
		inline static std::mt19937 generator;
		inline static unsigned int min;
		inline static unsigned int max;
	public:
		static void Init(int seed) {
			generator.seed(seed);
			min = generator.min();
			max = generator.max();
		}
		static float FloatRange(float a, float b) {
			return MapRange(min, max, a, b, generator());
		}
	};

	class Perlin2D {
	private:
		std::vector<glm::vec2> grid;
		int width;
		int height;
	public:
		Perlin2D(int _width, int _height) : width(_width), height(_height) {
			// or equals because creating squares, not points
			for (size_t y = 0; y <= height; y++)
			{
				for (size_t x = 0; x <= width; x++)
				{
					glm::vec2 v = { Random::FloatRange(-1, 1), Random::FloatRange(-1, 1) };
					//std::cout << glm::to_string(v) << std::endl;
					grid.push_back(v);
				}
			}

		}
		float Fade(float t) {
			return t* t* t* (t * (t * 6 - 15) + 10);
		}
		float GetNoise(float x, float y) {
			int X = ((int)floor(x)) % width;
			int Y = ((int)floor(y)) % height;
			
			/*glm::vec2 displacements[4] = {
				{ x - X, y - Y },
				{ x - X + 1, y - Y},
				{ x - X + 1, y - Y + 1},
				{ x - X, y - Y + 1}
			};*/

			glm::vec2 displacements[4] = {
				{ X - x, Y - y },
				{ x - X + 1, Y - y},
				{ x - X + 1, y - Y + 1},
				{ X - x, y - Y + 1}
			};

			float gradVals[4] = {
				glm::dot(grid[X + Y * width], displacements[0]),
				glm::dot(grid[X + 1 + Y * width], displacements[1]),
				glm::dot(grid[X + 1 + (Y + 1) * width], displacements[2]),
				glm::dot(grid[X + (Y + 1) * width], displacements[0]),
			};
			/*for (size_t i = 0; i < 4; i++)
			{
				std::cout << gradVals[i] << std::endl;
			}*/

			/*float xWeight = x - X;
			float yWeight = y - Y;*/

			// when set to 1 creates these weird plateaus with the same height
			float xWeight = Fade(x - X);
			float yWeight = Fade(y - Y);
			
			float xx = Lerp(gradVals[0], gradVals[1], xWeight);
			float yy = Lerp(gradVals[2], gradVals[3], xWeight);

			float amplitude = 2.f;
			//std::cout << "result = " << result << std::endl;
			return Lerp(xx, yy, yWeight) * amplitude;
		}
		float OctavePerlin(float x, float y, int octaves) {
			float frequency = 1;
			float total = 0;
			float amplitude = 1;
			float maxValue = 0;
			float persistence = 2;
			for (size_t i = 0; i < octaves; i++)
			{
				total += GetNoise(x * frequency, y * frequency) * amplitude;

				maxValue += amplitude;

				amplitude *= persistence;
				frequency *= 2;
			}
			return total / maxValue;
		}
	
	};
}
