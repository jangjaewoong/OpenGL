#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum pixelType){
    // Texture 불러오기
    type = texType;
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true); // OpenGL과 stb는 이미지를 읽는 방향이 달라서 해당 함수를 true로 실행해야 이미지가 거꾸로 뒤집히지 않음.
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0); // 이미지 파일을 읽어서 픽셀 데이터를 메모리에 로드. (파일 경로, 너비, 높이, 채널 수 저장 위치, 원하는 채널 수 0 : 원본 그대로)
    if(!bytes){
        std::cerr << "Failed to load Image" << std::endl;
        return;
    }

    glGenTextures(1, &ID); // 텍스쳐를 저장할 슬롯을 GPU에 생성
    // texture는 texture units 이란 슬롯에 묶여 저장된다. 보통 16개의 텍스처를 담을 수 있고, Fragment Shader가 텍스쳐를 동시에 사용할 수 있게 해준다.
    glActiveTexture(slot); // texture unit에 삽입하기 위해 texture Unit 활성화
    glBindTexture(texType, ID); // texture를 활성화된 유닛에 바인딩 
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // OpenGL은 텍스쳐 데이터가 4바이트 단위로 정렬되어있는데, RGB 3채널을 사용 하는 이미지는 규칙에 맞지 않아 기울어져 보일 수도 있음.
    
    
    // 텍스쳐 샘플링 방식 설정.
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 텍스쳐가 화면보다 작을 때
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 텍스쳐가 화면보다 클 때
 
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT); // 텍스처 좌표가 0~1을 벗어날 때 가로 방향 처리 방식
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT); // 세로 방향 처리 방식
    GLenum format = (numColCh == 4)? GL_RGBA : GL_RGB;
    glTexImage2D(texType, 0, format, widthImg, heightImg, 0, format, pixelType, bytes); // 텍스처 데이터를 GPU 슬롯에 업로드
    glGenerateMipmap(texType); //밈맵생성(원본 텍스쳐를 점점 작게 만든 계산된 이미지 세트), 멀리 있는 물체는 Mimmap사용해 GPU 메모리 절약.


    stbi_image_free(bytes); // 불러온 이미지 자원 해제
    glBindTexture(texType, 0); // 바인딩 해제

}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit){
    GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform); // shader 프로그램에서 tex0 이름의 uniform 변수 위치 가져옴
    shader.Activate(); // shader 프로그램을 활성 상태로 만듬. uniform 값 설정 전 반드시 호출 필요
    glUniform1i(tex0Uni, unit); // 위에서 활성화 한 0번 유닛(GL_TEXTURE0) 번호 전달
}

void Texture::Bind(){
    glBindTexture(type, ID); // texture를 활성화된 유닛에 바인딩 
}

void Texture::UnBind(){
    glBindTexture(type, 0); // texture를 활성화된 유닛에 바인딩 
}

void Texture::Delete(){
    glDeleteTextures(1, &ID);
}