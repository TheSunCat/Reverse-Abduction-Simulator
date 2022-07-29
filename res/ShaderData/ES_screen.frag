precision mediump float;

varying vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    gl_FragColor = texture2D(screenTexture, texCoords.st);
}  
