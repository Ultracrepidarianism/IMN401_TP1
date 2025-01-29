#version 460

uniform float time;

out gl_PerVertex
{
	vec4 gl_Position;
};
out vec2 currentPos;

layout (location = 0) in vec3 position;

void main()
{
	float scaleFactor = .75f + 0.5f * cos(time);
	vec3 pos = position * scaleFactor;
	pos += vec3(sin(time)/2, 0.0, 0.0);
	currentPos = pos.xy;
	gl_Position = vec4(pos, 1.0);
}