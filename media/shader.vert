#version 450 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 mvp;

out vec4 fragColour;
out vec2 textCoord;

void main()
{
	gl_Position = mvp * vec4(vPosition, 1.0);

	textCoord = aTexCoord;
}
