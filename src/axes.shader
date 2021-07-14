#shader vertex
#version 420

layout(location = 0) in vec3 linePos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(linePos, 1.0);
};

#shader fragment
#version 420

out vec4 FragColor;

uniform vec4 ourColor;
uniform int xAxis;
uniform int yAxis;
uniform int zAxis;

void main()
{
	if (xAxis == 1)
	{
		FragColor = vec4(ourColor);
	}
	if (yAxis == 1)
	{
		FragColor = vec4(ourColor);
	}
	if (zAxis == 1)
	{
		FragColor = vec4(ourColor);
	}
};