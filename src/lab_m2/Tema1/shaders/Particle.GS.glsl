#version 430

// Input and output topologies
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 eye_position;
uniform float offset;

uniform vec3 cube_positon;
uniform mat4 view_matrices[6];
uniform vec3 up_vectors[6];
uniform mat4 cube_proj;

// Output
layout(location = 0) out vec2 texture_coord;

// Local variables
vec3 vpos = gl_in[0].gl_Position.xyz;
vec3 forward = normalize(eye_position - vpos);
vec3 right = normalize(cross(forward, vec3(0, 1, 0)));
vec3 up = normalize(cross(forward, right));

void EmitPoint(vec2 offset)
{
    vec3 pos = right * offset.x + up * offset.y + vpos;
    gl_Position = Projection * View * vec4(pos, 1.0);
    EmitVertex();
}

void EmitPoint(vec2 offset, int layer)
{
    vec3 forward = normalize(cube_positon - vpos);
    vec3 up = up_vectors[layer];
    vec3 right = normalize(cross(forward, up));

    vec3 pos = right * offset.x + up * offset.y + vpos;

    gl_Position = cube_proj * view_matrices[layer] * vec4(pos, 1.0);
    
    EmitVertex();
}


void main()
{
    float ds = offset;

//    texture_coord = vec2(0, 0);
//    EmitPoint(vec2(-ds, -ds)); // Vertex 0
//
//    texture_coord = vec2(1, 0);
//    EmitPoint(vec2( ds, -ds)); // Vertex 1
//    
//    texture_coord = vec2(0, 1);
//    EmitPoint(vec2(-ds,  ds)); // Vertex 3
//    
//    texture_coord = vec2(1, 1);
//    EmitPoint(vec2( ds,  ds)); // Vertex 2

    int layer;
    for (layer = 0; layer < 6; layer++)
    {
        gl_Layer = layer;
        
        texture_coord = vec2(0, 0);
        EmitPoint(vec2(-ds, -ds), layer); // Vertex 0

        texture_coord = vec2(1, 0);
        EmitPoint(vec2( ds, -ds), layer); // Vertex 1
    
        texture_coord = vec2(0, 1);
        EmitPoint(vec2(-ds,  ds), layer); // Vertex 3
    
        texture_coord = vec2(1, 1);
        EmitPoint(vec2( ds,  ds), layer); // Vertex 2   

        EndPrimitive();
    }
}