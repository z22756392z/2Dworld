#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec2 texCoord;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = texCoord;
	gl_Position = position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform float threshold;
uniform int mode;
uniform sampler2D u_Texture;

float Inverselerp(float a, float b, float t) {
	return (t - a) / (b - a);
}

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
	float luminance = dot(vec3(color), vec3(0.2126, 0.7152, 0.0722));
	float dx = dFdx(dot(vec3(color), vec3(0, 0, 1)));
	float dy = dFdy(dot(vec3(color), vec3(0, 0, 1)));
	float gradient = fwidth(luminance);
	float isEdge = float(dy > mix(0, 1, threshold));
	float dx_normalized = Inverselerp(0, dx, mix(0, dx, threshold));
	float dy_normalized = Inverselerp(0, dy, mix(0, dy, threshold));
    if (mode == 0) {
        
    }
    else if (mode == 1) {
        color = vec4(dx * threshold, 0, 0, 1);
    }
    else if (mode == 2) {
        color = vec4(0, dy * threshold, 0, 1);
    }
    else if (mode == 3) {
        color = vec4(dx *  threshold, dy * threshold, 0, 1);
    }
    else {
        // Handle other cases or fallback behavior here
        discard;
    }
}

