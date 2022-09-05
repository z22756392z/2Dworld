#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec4 color;

out vec4 fragmentColor;

void main() {
	gl_Position = position;

	fragmentColor = color;
}

#shader fragment
#version 330 core

in vec4 fragmentColor;
layout(location = 0) out vec4 color;

void main() {
	color = fragmentColor;
}
