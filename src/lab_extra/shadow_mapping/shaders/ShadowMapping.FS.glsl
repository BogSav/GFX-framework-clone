#version 410

// Input
layout(location = 0) in vec3 color;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;
layout(location = 3) in vec4 shadow_pos;

// Uniform properties
uniform sampler2D texture_depth;

uniform vec3 light_direction;
uniform vec3 light_position;
uniform int light_mode;
uniform int light_power;

uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    float closestDepth = texture(texture_depth, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.00005;

    float shadow = currentDepth - bias > closestDepth ? 0.35 : 1.0;

    return shadow;
}

void main()
{
    // Fragment position and normals in world space
    vec3 P = world_position;
    vec3 N = normalize(world_normal);

    // ------------------------------------------------------------------------
    // Shadow 

    // NDC to Screen coordinates
    // TODO(student): Check if pixel is in shadow
    float visibility = ShadowCalculation(shadow_pos);
 
    // ------------------------------------------------------------------------
    // Phong lighting
    vec3 L = normalize(light_position - P);
    vec3 V = normalize(eye_position - P);
    vec3 H = normalize(L + V);

    vec3 ambient_light = vec3(0.15);
    vec3 diffuse_light = vec3(0);
    vec3 specular_light = vec3(0);

    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(1.5708);
    float factorAtenuare = 1.0;
    
    if (spot_light > cos(1.5708))
    {
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        factorAtenuare = pow(linear_att, 2);
        factorAtenuare *= 1.0 / (0.01 * pow(distance(world_position, light_position), 2) + 1);

        diffuse_light = material_kd * vec3(1,0,0) * max(dot(N, L), 0);
        if (dot(N, L) > 0)
        {
            specular_light = material_ks * vec3(1, 0, 0) * pow(max(dot(N, H), 0), material_shininess);
        }
    }

    vec3 light = factorAtenuare * (diffuse_light + specular_light) + ambient_light;

    vec3 color = object_color * light;
    out_color = visibility * vec4(color, 1);
}
