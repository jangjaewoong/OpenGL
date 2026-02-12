#include "VBO.h"
// 위 정점 데이터는 CPU에 존재하기 때문에 GPU에서 그래픽을 그릴 때 프레임마다 CPU에 요청해야 되기 때문에 효율이 낮음
// 따라서 Vertex Buffer Object를 이용해 정점 데이터를 GPU에 올린다.
VBO::VBO(GLfloat* vertices, GLsizeiptr size){
    glGenBuffers(1, &ID); // GPU 메모리에 버퍼를 하나 생성. (버퍼는 슬롯과 동일)
    glBindBuffer(GL_ARRAY_BUFFER, ID); // 여기서 Bind란 특정 객체를 현재 객체로 만드는 것을 의미하고, 그 타입의 객체를 수정할 함수를 실행하면 현재 객체가 수정되게 된다.
    //여기서 GL_ARRAY_BUFFER에 작업을 하면 VBO에 실행되게 된다.
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // CPU의 정점데이터를 VBO 슬롯에 올림 (타겟 버퍼, 정점데이터 크기, 데이터 주소 포인터, 사용 방식(한번 설정, 여러번 그림));
}

void VBO::Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::UnBind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete(){
    glDeleteBuffers(1, &ID);
}