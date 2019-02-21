#pragma once

#ifdef VERTEX
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
gl_Position = vec4(aPos, 1.0);
ourColor = aColor;
TexCoord = aTexCoord;
}
#endif //VERTEX


#ifdef FRAGMENT
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, -TexCoord); //*vec4(ourColor, 1.0);
}
#endif //FRAGMENT