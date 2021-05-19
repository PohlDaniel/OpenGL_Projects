#version 410 core

layout(location = 0) in vec3 i_position;
layout(location = 2) in vec3 i_normal;

uniform mat4 u_projection;
uniform mat4 u_modelView;


void main(void){

  gl_Position =  u_projection  * u_modelView * vec4(i_position, 1.0);
  //gl_Position.z *= gl_Position.w;

}