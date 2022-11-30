#version 330

#define MAX_LIGHTS 128

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 CarPosition;
uniform float CurveCoefficient;

uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform int nrOfLightSources;

out vec3 frag_color;

void main()
{
    // Lighting calculations
    vec3 world_pos = (Model * vec4(v_position,1)).xyz;

    vec3 N = normalize( mat3(Model) * v_normal );
    vec3 L = normalize( light_position - world_pos );
    vec3 V = normalize( eye_position - world_pos );
    vec3 H = normalize( L + V );
    vec3 R = normalize( reflect (-L, N));

    vec3 newPosition = v_position;
    newPosition.y = v_position.y - pow(length(CarPosition - v_position), 2) * CurveCoefficient;

    gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
}