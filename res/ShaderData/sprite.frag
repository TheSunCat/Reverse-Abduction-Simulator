#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D image;
uniform bool flip;

void main()
{
    vec2 coords = texCoords;
    if(flip)
        coords.x = 1 - coords.x;

    color = texture(image, coords);
}
