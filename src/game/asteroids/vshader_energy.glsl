# version 400
layout (location = 0) in vec4 vPosition;

uniform mat4 model;
uniform float color;

out vec4 v2fcolor;

void main ()
{
    gl_Position = model * vPosition ;
    v2fcolor = vec4 (color, color, color, 1);
}
