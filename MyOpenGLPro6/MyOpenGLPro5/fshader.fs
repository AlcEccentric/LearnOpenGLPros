
#version 330 core
out vec4 FragColor;

in vec2 loc;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2 ourGB;
void main()
{
    FragColor = mix(texture(texture1, loc), texture(texture2, loc), 0.2) * vec4(1.0, ourGB, 1.0);
}
