#include "VAO.h"

 // 이제 GPU에 정점 데이터는 올렸지만, 이 데이터를 어떻게 해석해서 그려야 할지는 모른다.
    // 이때 Vertex Array Object를 이용하여 해석 방법을 설정한다.

VAO::VAO(){
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset){
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset); // (Vertex Shader의 몇번째 입력 변수에 연결할지, 정점데이터가 몇개씩 읽을 건지, 데이터의 타입, 데이터를 0~1로 정규화 할지, 다음 정점까지의 간격, VBO 내에서 데이터가 시작하는 위치)
    // 이 함수가 GL_ARRAY_BUFFER에 바인딩된 VBO의 ID를 VAO에 저장하기 때문에 VBO를 바인드해줘야된다.
    glEnableVertexAttribArray(layout); // layout번 속성을 활성화하겠다. (위에서 정한 Vertex Shader)
    VBO.UnBind();
}   

void VAO::Bind(){
    glBindVertexArray(ID);
}

void VAO::UnBind(){
    glBindVertexArray(0);
}

void VAO::Delete(){
    glDeleteBuffers(1,&ID);
}