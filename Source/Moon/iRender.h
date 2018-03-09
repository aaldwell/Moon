#pragma once
//interface to the renderer project

//dependancy projects
#include "Renderer.h"

// clear the buffers
void ClearBuffers()
{
	//TODO: abstract
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render()
{
	ClearBuffers();

	Moon_Renderer::Renderer r;
	r.StartRender();
}