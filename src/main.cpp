#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
// glm 라이브러리 include
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Texture.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] = { // float 형식을 담는 배열. OpenGL의 float는 기존 float와 차지하는 크기가 다르기 때문에 GLfloat를 사용한다.
        //    Coordinates |       Colors        |    Texture Coordinates // 텍스쳐 좌표는 좌측 하단 (0,0) 부터 시작해 양쪽 축으로 (1,1) 까지 존재, 1보다 높은 값주면 이미지가 반복
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	    0.0f, 0.0f,
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	    5.0f, 0.0f,
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	    2.5f, 5.0f
        // OpenGL은 NDC(Normalized Device Coordinate)라는 좌표 정규화 방식을 이용하는데 -1~1까지의 값을 window화면 비율에 맞춰 재설정 해준다.
    };

// OpenGL에서 primitive가 각 정점의 색상 데이타를 가지고 있으면 자동으로 그라데이션을 적용시킨다.(보간)
GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    }; // 정점들의 순서에 따라 번호를 부여해 재사용할 때 사용


int main()
{
    glfwInit(); // glfw 라이브러리를 호출해 함수를 사용하기 위함
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 생성할 OpenGL 컨텍스트의 버전을 GLFW에 요청
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL의 최신 기능을 가지고 있는 core profile 사용


    GLFWwindow* window = glfwCreateWindow(width,height,"Graphics",NULL,NULL); // window 창을 생성하는 함수. 매개변수는 순서대로 넓이, 높이, 이름, 풀스크린 여부
    if(!window){
        std::cerr<<"윈도우 생성 오류 발생"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // 이 window의 OpenGL 컨텍스트를 현재 쓰레드에서 활성화

    gladLoadGL(); // glad로 내 그래픽 드라이버에서 사용할 수 있는 OpenGL의 최신 기능들을 가져옴

    glViewport(0,0,width,height); // OpenGL이 렌더링할 화면 영역 설정

    Shader shaderProgram("resources/default.vert","resources/default.frag");
    
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float))); // Color 정보는 layout 1에 보내고 offset은 3, stride는 6으로 설정.
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));

    VAO1.UnBind();  
    VBO1.UnBind();
    //EBO1.UnBind(); //VAO 가 UnBind 되기전에 EBO 를 unBind 하면 오류 발생
    
    
    
    Texture backGround("resources/Textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
    
    backGround.texUnit(shaderProgram, "tex0", 0);
   
    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0f,0.0f,0.0f,1.0f); // 배경을 채울 색상 설정
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 위에서 설정한 색상으로 화면을 밀어버림. 화면에 변화가 있을 때마다 호출해줘야 잔상이 남지 않음.
        shaderProgram.Activate();
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);
        camera.Matrix(shaderProgram, "camMatrix");
        
        
  
        backGround.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT,0); // 인덱스를 그리는 함수, indices 개수만큼 전달해야됨.
        glfwSwapBuffers(window); // BackBuffer에서 그린 후 다 그려지면 FrontBuffer와 변경해서 다 그려진 결과만 화면에 뜨게끔.
        glfwPollEvents(); // GLFW 내부 이벤트 큐를 확인하고 처리.
    }
    

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    backGround.Delete();
    glfwDestroyWindow(window); // window 객체 제거
    glfwTerminate(); // 작업을 마치고 호출한 glfw 라이브러리를 종료 및 자원 해제
}