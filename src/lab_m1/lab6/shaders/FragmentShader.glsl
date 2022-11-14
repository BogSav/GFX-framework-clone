#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 frag_normal;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    // out_color = vec4(1);
    out_color = vec4(frag_color, 0);
    // out_color = vec4(frag_normal, 1);
    // out_color = vec4(cos(0), 0, 0 , 0);
}
