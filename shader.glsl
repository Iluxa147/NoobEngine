#pragma once

#ifdef VERTEX
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the position variable has attribute position 0

out vec3 ourColor;

void main()
{
gl_Position = vec4(aPos, 1.0);
ourColor = aColor;
}
#endif //VERTEX


#ifdef FRAGMENT
out vec4 FragColor;
in vec3 ourColor;

//uniform vec4 ourColor;

void main()
{
	FragColor = vec4(ourColor, 1.0);
}
#endif //FRAGMENT
