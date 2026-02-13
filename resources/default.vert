// OpenGL에서 사용하는 Shader 언어 (GLSL)
// Vertex Shader source code

#version 330 core // 우리가 사용하는 GLSL 버전
layout (location = 0) in vec3 aPos; // layout은 OpenGL이 정점 데이터를 받을 수 있게 하는 통로다. 정점 데이터를 받아 Vec3 형식의 aPos 변수에 저장한다. // CPU에서 정점데이터를 받기 때문에 in을 사용
layout (location = 1) in vec3 aColor;

out vec3 color; // Fragment에 내보낼 변수

uniform float scale;

void main()
{
   gl_Position = vec4(aPos.x + aPos.x*scale, aPos.y+ aPos.y*scale, aPos.z + aPos.z*scale, 1.0); //gl_Position은 미리 정해진 키워드로서 정점의 위치를 이용하기 위해 사용
   color = aColor;
}