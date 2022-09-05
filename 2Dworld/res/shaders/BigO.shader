#shader vertex
#version 330 core

layout(location = 0)in vec2 position;
layout(location = 1)in vec4 color;

out vec4 fragmentColor;
uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * vec4(position,0.0f,1.0f);

	fragmentColor = color;
}

#shader fragment
#version 330 core

in vec4 fragmentColor;
layout(location = 0) out vec4 color;

void main() {
	color = fragmentColor;
}
