#version 330

// Input
in vec2 texcoord;
in vec3 vertex_color;

// Uniform properties
uniform sampler2D texture_1;
uniform vec2 ciclare;

uniform int textureActive;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
	vec4 finalColor = vec4(vertex_color, 1);

	if (textureActive == 1)
	{
		finalColor = texture2D(texture_1, texcoord + ciclare);

		if(finalColor.w < 0.9f)
		{
			discard;
		}
	}

	out_color = finalColor;

}