#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class Table
{
	
public:
	virtual void DrawTable() = 0;
	virtual ~Table() {}
};