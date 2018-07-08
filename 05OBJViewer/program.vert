#version 410 core



layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 color;

uniform mat4 u_projection;
uniform mat4 u_modelView;


uniform mat4 mvp;

out vec2 v_texCoord;
out vec3 v_color;

void main(void)
{
	
	
   gl_Position =   u_projection * u_modelView * vec4(position, 1.0);
   v_texCoord = texCoords;
}