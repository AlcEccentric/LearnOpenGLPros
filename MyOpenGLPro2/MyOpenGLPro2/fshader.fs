
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform vec2 ourGB;
void main()
{
    FragColor = vec4(ourColor.x, ourGB, 1.0f);
}
