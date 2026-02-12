#include "shaderClass.h"

// 파일의 전체 내용을 문자열로 읽어오는 함수.
std::string get_file_contents(const char* filename){
    std::ifstream in(filename, std::ios::binary); // 파일 경로를 바이너리 모드로 읽음
    if(in){ // 파일 열기 실패하면 throw(errno) 실행
        std::string contents;
        in.seekg(0, std::ios::end); // 읽기 포인터가 파일 끝으로 이동
        contents.resize(in.tellg()); // 현재 읽기 포인터의 위치를 반환(바이트 단위), 문자열 크기를 파일 크기만큼 할당.
        in.seekg(0, std::ios::beg); // 읽기 포인터가 파일 맨 처음으로 이동
        in.read(&contents[0], contents.size()); // read(버퍼, 읽을 크기) 읽기 포인터로 파일의 데이터를 읽어와 버퍼에 저장.
        in.close(); // 파일 스트림 닫기
        return(contents); // 읽은 문자열 반환
    }
    throw(errno); // 에러 코드 던짐.
}

Shader::Shader(const char* vertexFile, const char* fragmentFile){
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // GPU 메모리에 Vertex Shader를 담을 슬롯을 생성하여 슬롯 번호를 할당한다.
    glShaderSource(vertexShader, 1, &vertexSource, NULL); // 슬롯에 셰이더파일 소스코드를 연결한다. (셰이더 슬롯, 담을 문자열 개수, 문자열 포인터 배열)
    glCompileShader(vertexShader); // GPU 내부에서 소스코드를 이해하도록 컴파일한다.

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // GPU 메모리에 fragment Shader를 담을 슬롯을 생성하고 슬롯번호를 할당한다.
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    ID = glCreateProgram(); // 만든 셰이더들을 하나로 합쳐주는 프로그램 슬롯을 만든다.
    glAttachShader(ID, vertexShader); // 프로그램 슬롯에 셰이더를 연결한다.
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID); // 프로그램에 연결된 셰이더 슬롯들을 하나로 링크해 실행 가능한 프로그램으로 만든다.

    glDeleteShader(vertexShader); // GPU에 올라간 셰이더 슬롯들을 해제한다.
    glDeleteShader(fragmentShader);
}

void Shader::Activate(){
    glUseProgram(ID); // 사용할 셰이더 프로그램 지정
}

void Shader::Delete(){
    glDeleteProgram(ID); // 프로그램 자원 해제
}