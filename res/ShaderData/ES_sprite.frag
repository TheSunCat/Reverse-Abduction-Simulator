precision mediump float;

varying vec2 texCoords;

uniform sampler2D image;
uniform float opacity;
uniform bool flip;

void main()
{
    vec2 coords = texCoords;
    if(flip)
        coords.x = 1.0 - coords.x;

    gl_FragColor = texture2D(image, coords);

    gl_FragColor.a *= opacity;
}
