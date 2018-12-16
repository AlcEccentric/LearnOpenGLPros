
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aLoc;

out vec3 ourColor;
out vec2 loc;

uniform mat4 ourMat;

void main()
{
    gl_Position = ourMat * vec4(aPos, 1.0);
    ourColor = aColor;
    loc = vec2(aLoc.x, aLoc.y);
}
