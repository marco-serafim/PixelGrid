#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 outColor;

uniform vec3 inColor;
uniform vec2 position_of_pixel;
uniform int pixels_each_side;

void main()
{
    gl_Position = vec4(aPos.x/pixels_each_side + position_of_pixel.x/pixels_each_side,
    aPos.y/pixels_each_side + position_of_pixel.y/pixels_each_side,
    1.0f, 1.0f);

    outColor = aColor;
}
