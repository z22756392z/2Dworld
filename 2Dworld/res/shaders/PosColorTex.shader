#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec4 color;
layout(location = 2)in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 v_color;
void main() {
	v_TexCoord = texCoord;
	v_color = color;
	gl_Position = position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_color;

uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor * v_color;
}
