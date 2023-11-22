#version 430

// Input
layout(location = 0) in vec3 world_position;
layout(location = 1) in vec3 world_normal;

// Uniform properties
uniform samplerCube texture_cubemap;

uniform vec3 camera_position;

// Output
layout(location = 0) out vec4 out_color;


vec3 myReflect()
{
    vec3 incidentRay = normalize(world_position - camera_position);    
    return texture(texture_cubemap, reflect(incidentRay, world_normal)).xyz;
}

void main()
{
    out_color = vec4(myReflect(), 0);
}
