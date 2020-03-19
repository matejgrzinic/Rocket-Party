#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

in vec2 v_TexCoord;
out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_Amount;

void main()
{
	vec4 col = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color = texture(u_Texture, v_TexCoord) * u_Color;
};