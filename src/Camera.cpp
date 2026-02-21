#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position){
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane){
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, Up);
    // 뷰행렬을 생성하는 함수. (카메라 위치, 카메라가 바라보는 목표 지점, 카메라의 위쪽 방향(카메라의 위가 어느 방향인지 정의))
    projection = glm::perspective(glm::radians(FOVdeg), (float)width/(float)height, nearPlane, farPlane);

    cameraMatrix = projection * view;
   
}

void Camera::Matrix(Shader& shader, const char* uniform){
     // 투영행렬을 생성하는 함수. (수직 시야각, 종횡비(width/height), 가까운 절단면, 먼 절단면)
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    //View-Projection 합성 행렬을 GPU 쉐이더에 전달 (vertex.glsl) (uniform번호, 행렬개수, 전치 여부, 행렬의 데이터포인터)
}

void Camera::Inputs(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        Position += speed * Orientation; // w누르면 카메라가 -Z축쪽으로 이동
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){ // A누르면 카메라가 왼쪽으로 이동
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
        // glm::cross() - 외적, 두 벡터에 모두 수직인 새로운 벡터를 만든다.
        // glm::normalize() - 벡터의 길이를 1로 만들되, 방향은 유지한다. (외적의 결과는 1이 아닐수 있기 때문)
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        Position += speed * -Orientation; // s누르면 카메라가 Z축쪽으로 이동
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        Position += speed * Up; // 스페이스 누르면 카메라 위로 이동
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        Position += speed * -Up; // 왼쪽 컨트롤 누르면 카메라 아래로 이동
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        speed = 0.4f; // 왼쪽 Shift 누르면 이동 속도 올라감
    }
    else if (glfwGetKey(window,  GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
        speed = 0.1f;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){ // 왼쪽 마우스 버튼이 눌려있으면
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // 커서 숨기되 정확한 위치 구함.
        if(firstClick){
            glfwSetCursorPos(window, (width/2), (height/2)); // 화면을 처음 클릭할 때 화면 중앙으로 커서 이동
            firstClick = false;
        }
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY); //마우스의 X,Y 좌표가 변수에 포인터로 전달됨

        float rotX = sensitivity * (float)(mouseY-(height/2))/height; // 마우스가 위아래로 움직이면 카메라가 X축(가로)를 기준으로 회전, 마우스가 화면 중앙 기준으로 얼마나 멀어졌는지 확인하고 다시 height로 나눠 화면 크기에 상관 없이 감도를 일정하게 유지 
        float rotY = sensitivity * (float)(mouseX-(width/2))/width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
        // -rotX를 사용하는 이유. 만약 rotX가 12.5라면 openGL은 오른손 좌표계를 사용하니 양수 각도는 반시계 방향으로 회전해 반대방향으로 회전하게됨. 따라서 -를 붙여야 카메라가 위아래로 회전하게 됨.
        if(!(glm::angle(newOrientation, Up) <= glm::radians(5.0f)) && !(glm::angle(newOrientation, -Up)<= glm::radians(5.0f))){
            Orientation = newOrientation; // up 벡터(하늘)와의 각도가 5도 이상이고 -up 벡터(바닥)과의 각도가 5도 이상일 때만 회전 적용 (카메라가 완전히 수직을 보지 못하도록)
        }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
        // Y축 기준으로 좌우 회전
        glfwSetCursorPos(window, (width/2), (height/2));
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}