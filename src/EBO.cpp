#include "EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size){
    // 중복된 정점을 모두 데이터에 추가하지 않고 점점에 번호를 붙여서
    // Element Buffer Object(EBO)에서 그릴 정점 조합을 올림
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::UnBind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete(){
    glDeleteBuffers(1, &ID);
}