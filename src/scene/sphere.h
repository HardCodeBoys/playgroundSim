#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene/model.h"
#include <vector>

class Sphere : public Model {
private:
    std::vector<glm::vec3> normals;
public:
	Sphere(const glm::vec3& _position, float size, GLuint _shaderID)
        : Model(_position, _shaderID)
	{
        std::cout << "sphere ctor" << std::endl;

        const unsigned int sectorCount = 32;
        const unsigned int stackCount = 16;

		float radius = size / 2;
        const float PI = 3.14159265358979323846f;

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                vertices.push_back(glm::vec3(x, y, z) + position);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(glm::vec3(nx, ny, nz));
            }
        }
        // generate CCW index list of sphere triangles
        // k1--k1+1
        // |  / |
        // | /  |
        // k2--k2+1
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
        CreateVertexData();
		GenerateBuffers();
	}

    Sphere(const Sphere& other) {
        std::cout << "sphere copy ctor" << std::endl;
        position = other.position;

        vertexBuffer = 0;
        indexBuffer = 0;
        
        vertices = other.vertices;
        vertexData = other.vertexData;

        normals = other.normals;

        indices = other.indices;

        shaderID = other.shaderID;

        CreateVertexData();
        GenerateBuffers();
    }
    Sphere(Sphere&& other) noexcept {
        Log::Info("sphere move ctor");
        position = other.position;

        vertexBuffer = other.vertexBuffer;
        indexBuffer = other.indexBuffer;

        vertices = other.vertices;
        vertexData = other.vertexData;

        normals = other.normals;

        indices = other.indices;

        shaderID = other.shaderID;

        other.Destroy();

        CreateVertexData();
        GenerateBuffers();
    }

    void Destroy() override {
        Model::Destroy();
        normals.clear();

    }

	void CreateVertexData() override {
        vertexData.clear();
        const unsigned int sectorCount = 32;
        const unsigned int stackCount = 16;
        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertexData.push_back(vertices[i]);
            vertexData.push_back(normals[i]);
        }
	}
};