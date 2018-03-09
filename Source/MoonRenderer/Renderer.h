#pragma once
#include <iostream>

#ifdef MOON_RENDERER_EXPORTS  
#define MR_API __declspec(dllexport)   
#else  
#define MR_API __declspec(dllimport)   
#endif  

namespace Moon_Renderer
{
	// This class is exported from the MoonRenderer.dll  
	class Renderer
	{
	public:
		static MR_API void StartRender();

		static void Draw();
		
	};
}

