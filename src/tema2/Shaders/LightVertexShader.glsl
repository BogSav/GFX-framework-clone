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

uniform vec3 light_positions[MAX_LIGHTS];

uniform int nrOfLightSources;

out vec3 frag_color;

void main()
{
    // Lighting calculations
    vec3 world_pos = (Model * vec4(v_position,1)).xyz;
    float ambient_light = material_kd * 0.04;
    float culoareObiect = 0;

    for(int i = 0; i < nrOfLightSources; i++)
    {
        vec3 N = normalize( mat3(Model) * vertex_normal );
        vec3 L = normalize( light_positions[i] - world_pos );
        vec3 V = normalize( eye_position - world_pos );
        vec3 H = normalize( L + V );
    
        float diffuse_light = material_kd * max(dot(N, L), 0);

        float specular_light = 0;

        if (diffuse_light > 0)
        {
            specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
        }

        float factorAtenuare = 1 / (0.00009 * pow(distance(world_pos, light_positions[i]), 2) + 1);
        culoareObiect += (ambient_light + factorAtenuare * ( diffuse_light + specular_light )) * 1.1;
    }
    
     frag_color = vertex_color * culoareObiect;

    // Apply the curve
    vec3 newPosition = v_position;
    newPosition.y = v_position.y - pow(length(CarPosition - v_position), 2) * CurveCoefficient;

    gl_Position = Projection * View * Model * vec4(newPosition, 1.0);
}