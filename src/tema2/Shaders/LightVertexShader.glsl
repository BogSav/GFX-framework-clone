#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 CarPosition;
uniform float CurveCoefficient;

out vec3 world_position;
out vec3 world_normal;

void main()
{
    // Lighting calculations
    world_position = (Model * vec4(v_position.xyz, 1)).xyz;
    world_normal = normalize( mat3(Model) * vertex_normal );

    // Apply the curve
    vec3 newPosition = v_position;
    newPosition.y = v_position.y - pow(length(CarPosition - v_position), 2) * CurveCoefficient;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}