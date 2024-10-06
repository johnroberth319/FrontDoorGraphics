#version 330 core
in vec2 TexCoord;

uniform int textureType; // 0 = Wall, 1 = Floor, 2 = Ceiling, 3 = Seal, 4 = Guard, 5 = Handle

out vec4 color;

uniform sampler2D wallTexture;
uniform sampler2D floorTexture;
uniform sampler2D ceilingTexture;
uniform sampler2D citroTexture;
uniform sampler2D sealTexture;
uniform sampler2D guardTexture;
uniform sampler2D handleTexture;

void main()
{
    if (textureType == 0) {
        color = texture(wallTexture, TexCoord);
    } else if (textureType == 1) {
        color = texture(floorTexture, TexCoord);
    } else if (textureType == 2) {
        color = texture(ceilingTexture, TexCoord);
    } else if (textureType == 3) {
        color = texture(sealTexture, TexCoord);
    } else if (textureType == 4) {
        color = texture(guardTexture, TexCoord);
    } else if (textureType == 5) {
        color = texture(handleTexture, TexCoord);
    } else {
        color = texture(citroTexture, TexCoord);
    }
}