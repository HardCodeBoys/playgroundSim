#pragma once

#include <iostream>

class Component
{
protected:
	std::string type;
public:
	// no default constructor to ensure it has its type inserted
	Component(const std::string& _type) : type(_type) {}
	virtual ~Component() {}
	std::string GetType() const { return type; }
	virtual void Move(const glm::vec3& position) = 0;
};

