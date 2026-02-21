// OpenGL에서 사용하는 Shader 언어 (GLSL)
// Vertex Shader source code

#version 330 core // 우리가 사용하는 GLSL 버전
layout (location = 0) in vec3 aPos; // layout은 OpenGL이 정점 데이터를 받을 수 있게 하는 통로다. 정점 데이터를 받아 Vec3 형식의 aPos 변수에 저장한다. // CPU에서 정점데이터를 받기 때문에 in을 사용
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec3 color; // Fragment에 내보낼 변수
out vec2 texCoord; // 텍스쳐 좌표 값
out vec3 Normal;
out vec3 crntPos;
uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
   crntPos = vec3(model * vec4(aPos,1.0f)); // 위치값으로 쓸 때는 w 성분이 필요없어서 vec3()로 캐스팅 해 xyz 성분만 가져옴
   gl_Position = camMatrix * vec4(crntPos, 1.0f); //gl_Position은 미리 정해진 키워드로서 정점의 위치를 이용하기 위해 사용, vec4를 사용하는 이유는 동차 좌표계를 사용하기 때문
   color = aColor;
   texCoord = aTex;
   Normal = aNormal;
}