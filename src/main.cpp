#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
    glfwInit(); // glfw 라이브러리를 호출해 함수를 사용하기 위함
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 생성할 OpenGL 컨텍스트의 버전을 GLFW에 요청
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL의 최신 기능을 가지고 있는 core profile 사용

    GLfloat vertices[] = { // float 형식을 담는 배열. OpenGL의 float는 기존 float와 차지하는 크기가 다르기 때문에 GLfloat를 사용한다.
        -0.5f, -0.5f, 0.0f, // Lower Left Corner
        0.0f, -0.5f, 0.0f, // Inner donw
        -0.25f,0.0f,0.0f, // Inner Left
        0.5f, -0.5f, 0.0f, // Lower Right Corner
        0.25f, 0.0f, 0.0f, // Inner Right
        0.0f, 0.5f, 0.0f  // Upper Corner
        // OpenGL은 NDC(Normalized Device Coordinate)라는 좌표 정규화 방식을 이용하는데 -1~1까지의 값을 window화면 비율에 맞춰 재설정 해준다.
    };

    GLuint indices[] = {
        0, 1, 2,
        1, 3, 4,
        2, 4, 5
    }; // 정점들의 순서에 따라 번호를 부여해 재사용할 때 사용

    GLFWwindow* window = glfwCreateWindow(800,600,"Graphics",NULL,NULL); // window 창을 생성하는 함수. 매개변수는 순서대로 넓이, 높이, 이름, 풀스크린 여부
    if(!window){
        std::cerr<<"윈도우 생성 오류 발생"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // 이 window의 OpenGL 컨텍스트를 현재 쓰레드에서 활성화

    gladLoadGL(); // glad로 내 그래픽 드라이버에서 사용할 수 있는 OpenGL의 최신 기능들을 가져옴

    glViewport(0,0,800,600); // OpenGL이 렌더링할 화면 영역 설정


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // GPU 메모리에 Vertex Shader를 담을 슬롯을 생성하여 슬롯 번호를 할당한다.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 슬롯에 셰이더파일 소스코드를 연결한다.
    glCompileShader(vertexShader); // GPU 내부에서 소스코드를 이해하도록 컴파일한다.

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // GPU 메모리에 fragment Shader를 담을 슬롯을 생성하고 슬롯번호를 할당한다.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram(); // 만든 셰이더들을 하나로 합쳐주는 프로그램 슬롯을 만든다.
    glAttachShader(shaderProgram, vertexShader); // 프로그램 슬롯에 셰이더를 연결한다.
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // 프로그램에 연결된 셰이더 슬롯들을 하나로 링크해 실행 가능한 프로그램으로 만든다.

    glDeleteShader(vertexShader); // GPU에 올라간 셰이더 슬롯들을 해제한다.
    glDeleteShader(fragmentShader);

    // 위 정점 데이터는 CPU에 존재하기 때문에 GPU에서 그래픽을 그릴 때 프레임마다 CPU에 요청해야 되기 때문에 효율이 낮음
    // 따라서 Vertext Buffer Object를 이용해 정점 데이터를 GPU에 올린다.
    // 이제 GPU에 정점 데이터는 올렸지만, 이 데이터를 어떻게 해석해서 그려야 할지는 모른다.
    // 이때 Vertex Array Object를 이용하여 해석 방법을 설정한다.

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // GPU 메모리에 해석 방법을 담을 버퍼를 생성한다.
    glBindVertexArray(VAO); // OpenGL에서 해당 VAO 슬롯의 해석 방법을 사용해 그리라고 지정한다.
    glGenBuffers(1, &VBO); // GPU 메모리에 버퍼를 하나 생성. (버퍼는 슬롯과 동일)
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 여기서 Bind란 특정 객체를 현재 객체로 만드는 것을 의미하고, 그 타입의 객체를 수정할 함수를 실행하면 현재 객체가 수정되게 된다.
    //여기서 GL_ARRAY_BUFFER에 작업을 하면 VBO에 실행되게 된다.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // CPU의 정점데이터를 VBO 슬롯에 올림 (타겟 버퍼, 정점데이터 크기, 데이터 주소 포인터, 사용 방식(한번 설정, 여러번 그림));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // (Vertex Shader의 몇번째 입력 변수에 연결할지, 정점데이터가 몇개씩 읽을 건지, 데이터의 타입, 데이터를 0~1로 정규화 할지, 다음 정점까지의 간격, VBO 내에서 데이터가 시작하는 위치)
    glEnableVertexAttribArray(0); // 0번 속성을 활성화하겠다. (위에서 정한 Vertex Shader)

    // 중복된 정점을 모두 데이터에 추가하지 않고 점점에 번호를 붙여서
    // Element Buffer Object(EBO)에서 그릴 정점 조합을 올림
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Bind 해제
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    while(!glfwWindowShouldClose(window)){
        glClearColor(0.5f,0.5f,1.0f,1.0f); // 배경을 채울 색상 설정
        glClear(GL_COLOR_BUFFER_BIT); // 위에서 설정한 색상으로 화면을 밀어버림. 화면에 변화가 있을 때마다 호출해줘야 잔상이 남지 않음.
        glUseProgram(shaderProgram); // 사용할 프로그램 지정
        glBindVertexArray(VAO); // 사용할 데이터 해석 방법 설정
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT,0); // 인덱스를 그리는 함수
        glfwSwapBuffers(window); // BackBuffer에서 그린 후 다 그려지면 FrontBuffer와 변경해서 다 그려진 결과만 화면에 뜨게끔.
        glfwPollEvents(); // GLFW 내부 이벤트 큐를 확인하고 처리.
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window); // window 객체 제거
    glfwTerminate(); // 작업을 마치고 호출한 glfw 라이브러리를 종료 및 자원 해제
}