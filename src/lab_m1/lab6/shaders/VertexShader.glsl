#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Radians;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;

void main()
{
    // TODO(student): Send output to fragment shader
    // frag_color = vertex_color;
    frag_color = vec3(abs(sin(Radians)), abs(cos(Radians - 3.14/6)), abs(sin(Radians - 3.14/2))) + vertex_normal;
    frag_normal = vertex_normal;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    gl_Position.x += sin(Radians) * 2;
    gl_Position.y += cos(Radians) * 2;
}
