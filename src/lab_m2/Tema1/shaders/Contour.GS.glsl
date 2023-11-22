#version 430

layout(triangles) in;
layout(line_strip, max_vertices = 12) out;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 viewMatrices[6];

in vec3 world_normal[3];

const vec3[6] viewVectors = {vec3(1,0,0), vec3(-1,0,0),vec3(0,1,0),vec3(0,-1,0),vec3(0,0,1),vec3(0,0,-1)};

vec4 InterpolateVertexPosition(float dotA, float dotB, vec4 posA, vec4 posB)
{
    float alpha = abs(dotA) / (abs(dotA) + abs(dotB));
    return posA + alpha * (posB - posA);
}

bool IsVisible(float dotProduct)
{
    // Verificam cu toleranta
    return dotProduct <= 0.0 + 0.0001;
}

void EmitContourSegment(int indexA, int indexB, int layer)
{
    vec4 interpolatedPos = InterpolateVertexPosition(
        dot(world_normal[indexA], viewVectors[layer]),
        dot(world_normal[indexB], viewVectors[layer]),
        gl_in[indexA].gl_Position,
        gl_in[indexB].gl_Position
    );

    gl_Position = Projection * viewMatrices[layer] * interpolatedPos;
    EmitVertex();
}

void main()
{
    int layer;

    for(layer = 0; layer < 6; layer++)
    {
        gl_Layer = layer;

        float dot1 = dot(world_normal[0], viewVectors[layer]);
        float dot2 = dot(world_normal[1], viewVectors[layer]);
        float dot3 = dot(world_normal[2], viewVectors[layer]);

        bool v1Visible = IsVisible(dot1);
        bool v2Visible = IsVisible(dot2);
        bool v3Visible = IsVisible(dot3);

        if (v1Visible != v2Visible || v1Visible != v3Visible || v2Visible != v3Visible)
        {
            if (v1Visible != v2Visible)
            {
                EmitContourSegment(0, 1, layer);
            }
            if (v1Visible != v3Visible)
            {
                EmitContourSegment(0, 2, layer);
            }
            if (v2Visible != v3Visible)
            {
                EmitContourSegment(1, 2, layer);
            }
            EndPrimitive();
        }
    }
}
