#pragma once

#include "playground.h"
#include "mesh.h"

class Terrain : public Model
{
private:
	// actual size (the number of squares) is one less, size = number of edges
	int size;
	int subdivision;
public:
	Terrain(const glm::vec3& _position, int _size, GLuint _shaderID)
		: size(_size)
	{
		subdivision = 2;
		vertexBuffer = 0;
		position = _position;
		shaderID = _shaderID;
		GenerateTerrain();
		UpdateVertexData();
		GenerateBuffers();

	}
	virtual void UpdateVertexData() override {
		vertexData.clear();
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertexData.push_back(vertices[i]);
			// hardcoded upward facing normals
			vertexData.push_back(glm::vec3(0, 1, 0));
		}
	}
	void foo(auto i) {

	}
private:
	void GenerateTerrain() {
		// size of the perlin grid tells how big the artifacts in the noise will be
		int perlinSize = 1;
		meth::Perlin2D perlin(perlinSize, perlinSize);

		std::cout << "size = " << size << "perlinSize = " << perlinSize << std::endl;
		LogNS::Init();
		foo(perlinSize);
		for (size_t y = 0; y < size; y++)
		{
			for (size_t x = 0; x < size; x++)
			{
				float px = meth::MapRange(0, size, 0.00001f, 1.99f, x);
				float py = meth::MapRange(0, size, 0.00001f, 1.99f, y);
				//float height = perlin.OctavePerlin(px, py, 4);
				//Log::err("lelel", 5, 'a');
				float height = perlin.GetNoise(px, py);
				if (px == (int)px) {
					std::cout << px << " = "; Log::Err("integer");
				}
				//std::cout << "x, y = " << x << ", " << y << "px, py = " << px << ", " << py << std::endl;
				vertices.push_back(glm::vec3(position.x + x, position.y + height, position.z + y));
			}
		}
		// TODO: copy this algorithm to plane ???
		for (size_t i = 0; i < size - 1; i++)
		{
			for (size_t j = 0; j < (size - 1) * size; j += size)
			{
				indices.push_back(j + i);
				indices.push_back(j + i + 1);
				indices.push_back(j + i + 1 + size);
								  
				indices.push_back(j + i + 1 + size);
				indices.push_back(j + i + size);
				indices.push_back(j + i);
			}			
		}
	}
};