 #include "Line.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

Line::Line()
{
	float vertices[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	VertexArray va;
	VertexBuffer vb(vertices, 3 * 2 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);

};
