//Fragment Shader source code
#version 330 core
out vec4 FragColor; // 화면으로 내보내서 out 사용

in vec3 color; // vertex shader에서 명시한 변수 이름과 동일해야한다.
in vec2 texCoord;

in vec3 Normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
   float ambient = 0.20f; // 최소 Ambient 수치
   vec3 normal = normalize(Normal); // Vertex Data의 Normal이 길이 1이 아닌경우, 외적을 했을때 diffuse값이 잘못된 밝기가 나옴.
   vec3 lightDirection = normalize(lightPos - crntPos); // 현재 오브젝트에서 빛을 바라보는 방향, 위와 동일한 이유로 저규화
   // diffuse는 면이 빛을 얼마나 정면으로 바라보는지 
   float diffuse = max(dot(normal, lightDirection), 0.0f); // 법선 방향과 빛의 방향 사이 코사인 값을 구한다.(내적), 음수는 빛이 뒷면에서 오는 걸 의미하기 때문에 max는 0으로 설정
   
   float specularLight = 0.50f; // 반사광 세기
   vec3 viewDirection = normalize(camPos - crntPos); // 오브젝트가 카메라를 바라보는 방향
   vec3 reflectionDirection = reflect(-lightDirection, normal); // 빛이 면으로 향하는 방향과, 법선 벡터를 입력받아, 어느 방향으로 반사 되는지 구하는 함수이다. 반사 방향과 카메라가 일치할수록 더 크게 빛난다.
   float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8); // 내적으로 카메라 방향과 반사 방향의 코사인값을 받아 max로 음수 제거 후 8 제곱을 한다. 여기서 지수가 클수록 하이라이트가 좁고 강해짐.
   float specular = specAmount * specularLight;
   // ambient  = 0.20  → 빛이 안닿는 면도 최소한 이만큼은 밝게 (완전 검정 방지)
   // diffuse  = 0~1   → 면이 빛을 정면으로 바라볼수록 밝아짐
   // specular = 0~0.5 → 반사광이 카메라와 일치할수록 반짝임
   FragColor = texture(tex0, texCoord) * lightColor * (diffuse+ambient+specular);
}