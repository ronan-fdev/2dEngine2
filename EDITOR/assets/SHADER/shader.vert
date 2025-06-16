#version 430
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec4 aColor;

out vec2 fragUVs;
out vec4 fragColor;

uniform mat4 uProjection;
void main()
{
    gl_Position = uProjection * vec4(aPosition, 1.0);
    fragUVs = aTexCoords;
    fragColor = aColor;
}