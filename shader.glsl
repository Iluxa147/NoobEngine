#pragma once

#ifdef VERTEX
// layout (location = 1) in vec3 aColor;
// uniform vec4 ourColor;
// out vec3 ourColor;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

// uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() { // ourColor = aColor;
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
#endif // VERTEX

#ifdef FRAGMENT
// in vec3 ourColor;

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord),
                  0.05); //*vec4(ourColor, 1.0);
}
#endif // FRAGMENT
