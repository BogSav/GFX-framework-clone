#version 330

struct light_source
{
   int  type;
   vec3 position;
   vec3 color;
   vec3 direction;
   float cutOff;
};

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform light_source lights[3];

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;

    vec3 semiFinalColor = vec3(0);
    vec3 culoareObiect = vec3(0);

    for(int i = 0; i < 2; i++)
    {
        vec3 diffuse_light = vec3(0);
        vec3 specular_light = vec3(0);

        vec3 L = normalize( lights[i].position - world_position );
        vec3 V = normalize( eye_position - world_position );
        vec3 H = normalize( L + V );

        float factorAtenuare;

        // TODO(student): If (and only if) the light is a spotlight, we need to do
        // some additional things.
        switch(lights[i].type)
        {
        case 1:
            float spot_light = dot(-L, lights[i].direction);
            float spot_light_limit = cos(lights[i].cutOff);

            if(spot_light > cos(lights[i].cutOff))
            {
                float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
                factorAtenuare = pow(linear_att, 2);

                diffuse_light = material_kd * lights[i].color * max(dot(world_normal, L), 0);
                if (dot(world_normal, L) > 0)
                {
                    specular_light = material_ks * lights[i].color * pow(max(dot(world_normal, H), 0), material_shininess);
                }
            }
            break;
        case 2:
            diffuse_light = material_kd * lights[i].color * max(dot(world_normal, L), 0);
            if (dot(world_normal, L) > 0)
            {
                specular_light = material_ks * lights[i].color * pow(max(dot(world_normal, H), 0), material_shininess);
            }

            factorAtenuare = 1 / (pow(distance(world_position, lights[i].position), 2) + 1);
            break;      
        }

        semiFinalColor += factorAtenuare * ( diffuse_light + specular_light );
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    culoareObiect = object_color * ambient_light + semiFinalColor;

    // TODO(student): Write pixel out color
    out_color = vec4(culoareObiect.xyz, 1);

}
