#version 330 core\n

in vec3 vertexColor;
out vec4 FragColor;

void main()
{
   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);
};