#pragma once

#include "playground.h"
#include "model.h"

class Mesh : public Model
{
private:

public:
	// create a (random) vector of vertices for the mesh
	Mesh() {}
	virtual void UpdateVertexData() override {
		std::cout << "calling unimplemented vertexdata on Mesh" << std::endl;
	}

};