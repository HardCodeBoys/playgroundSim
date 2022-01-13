#pragma once

#include <iostream>
#include "scene/model.h"


#include "utils/log.h"

class Mesh : public Model
{
private:

public:
	// create a (random) vector of vertices for the mesh
	Mesh() {}
	void CreateVertexData() override {
		PL_NOT_IMPLEMENTED();
	}

};