
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aLoc;

out vec2 loc;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main()
{
    gl_Position = project * view * model * vec4(aPos, 1.0);
    loc = vec2(aLoc.x, aLoc.y);
}
