#version 330
#ifdef GL_ARB_shading_language_420pack
#extension GL_ARB_shading_language_420pack : require
#endif

in vec3 pos;
out vec2 texCoord;
in vec2 tex;

void main()
{
    gl_Position = vec4(pos.x, pos.y, 0.5, 1.0);
    texCoord = tex;
}

