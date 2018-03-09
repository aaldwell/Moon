// RenderMain.h - Contains highest level of renderer functions
#pragma once  
#include <iostream>
#include "Renderer.h"


namespace Moon_Renderer
{
	void Renderer::StartRender()
	{
		//std::cerr << "StartRender was called. " << std::endl;

		Draw();
	}

	void Renderer::Draw()
	{
		//std::cerr << "DRAWWWWWWW!!!" << std::endl;
	}
}
