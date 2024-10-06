#version 330 core
in vec2 TexCoord;

uniform int textureType; // 0 = Wall, 1 = Floor, 2 = Ceiling

out vec4 color;

uniform sampler2D wallTexture;
uniform sampler2D floorTexture;
uniform sampler2D ceilingTexture;
uniform sampler2D citroTexture;

void main()
{
    if (textureType == 0) {
        color = texture(wallTexture, TexCoord);
    } else if (textureType == 1) {
        color = texture(floorTexture, TexCoord);
    } else if (textureType == 2) {
        color = texture(ceilingTexture, TexCoord);
    } else {
        color = texture(citroTexture, TexCoord);
    }
}