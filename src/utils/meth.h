#pragma once
// math library
#include <iostream>
#include <random>

#include <glm/glm.hpp>

#undef max
#undef min

#define METH_ASSERT(x) if(!(x)) __debugbreak()


constexpr float M_PI = 3.1415926535f;

namespace meth
{
	static float MapRange(float iStart, float iEnd, float oStart, float oEnd, float value) {
		METH_ASSERT(value >= iStart && value <= iEnd);
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
		std::vector<glm::vec2> gradients;
		int size;

		std::vector<int> permutationTable = {151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7,
			225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247,
			120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
			88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134,
			139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220,
			105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80,
			73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86,
			164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38,
			147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189,
			28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101,
			155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232,
			178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12,
			191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
			199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236,
			205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

		int hash(int x, int y) const {
			return permutationTable[permutationTable[x] + y];
		}

	public:
		Perlin2D(){
			size = 16;
			// or equals because creating squares, not points
			for (size_t y = 0; y <= size; y++)
			{
				for (size_t x = 0; x <= size; x++)
				{
					glm::vec2 v = { Random::FloatRange(-1, 1), Random::FloatRange(-1, 1) };
					//std::cout << glm::to_string(v) << std::endl;
					gradients.push_back(v);
				}
			}
			for (size_t i = 0; i < 256; i++)
			{
				permutationTable.push_back(permutationTable[i]);
			}

		}
		float Fade(float t) {
			return t* t* t* (t * (t * 6 - 15) + 10);
		}
		float GetNoise(float x, float y) {
			int X = ((int)floor(x)) & size * size - 1;
			int Y = ((int)floor(y)) & size * size - 1;
			
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

			glm::vec2 grads[4] = {
				gradients[hash(X, Y)],
				gradients[hash(X + 1, Y)],
				gradients[hash(X + 1, Y + 1)],
				gradients[hash(X, Y + 1)]
			};

			float gradVals[4] = {
				glm::dot(grads[0], displacements[0]),
				glm::dot(grads[1], displacements[1]),
				glm::dot(grads[2], displacements[2]),
				glm::dot(grads[3], displacements[0]),
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

			float amplitude = 0.5;
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
