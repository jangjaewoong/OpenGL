//Fragment Shader source code
#version 330 core
out vec4 FragColor; // 화면으로 내보내서 out 사용

in vec3 color; // vertex shader에서 명시한 변수 이름과 동일해야한다.

void main()
{
   FragColor = vec4(color, 1.0f);
}