#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform int useMix;
uniform int glob;
uniform float tm;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
	vec4 color = texture2D(texture_1, texcoord);
	vec4 color2 = texture2D(texture_2, texcoord);

	if(glob == 1)
	{
		color = texture2D(texture_1, vec2(texcoord.x - tm * 0.1, texcoord.y));
	}

	if(color.w < 0.5f)
	{
		discard;
	}

	if(useMix == 1)
		out_color = mix(color, color2, 0.5);
	else
		out_color = color;

}
