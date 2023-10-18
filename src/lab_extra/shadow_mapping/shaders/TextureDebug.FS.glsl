#version 430

// Input
layout(location = 0) in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 color;


void main()
{
    float depth = texture(texture_1, texture_coord).r;
    color = vec4(depth);
}
