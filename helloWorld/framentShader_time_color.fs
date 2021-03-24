
#version 330 core
out vec4 color;
in vec3 ourColor;
uniform vec4 ourColors; // 在OpenGL程序代码中设定这个变量

void main()
{
    color = vec4(ourColor, 1.0f) * ourColors;
}  
