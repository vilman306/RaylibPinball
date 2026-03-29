#version 330

// Simple bloom shader

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;

uniform vec2 resolution;

const float samples = 5.0;
const float quality = 3.0;

void main()
{
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1) / resolution * quality;

    vec4 source = texture(texture0, fragTexCoord);

    const int range = 2; //int((samples - 1) / 2);

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture(texture0, fragTexCoord + vec2(x, y) * sizeFactor);
        }
    }

    finalColor = source + sum / (samples*samples);

}