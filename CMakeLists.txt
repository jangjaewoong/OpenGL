cmake_minimum_required(VERSION 3.10)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL과 GLFW 찾기
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)  # apt로 설치한 GLFW 찾기

# GLAD 소스
set(GLAD_SOURCES
    external/glad/src/glad.c
)

# 실행 파일
add_executable(graphics
    src/main.cpp
    src/shaderClass.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/stb.cpp
    src/Texture.cpp
    ${GLAD_SOURCES}
)

# 헤더 경로
target_include_directories(graphics PRIVATE
    external/glad/include
    external/stb
    ${OPENGL_INCLUDE_DIR}
)

# 라이브러리 링크
target_link_libraries(graphics PRIVATE
    glfw                    # apt로 설치한 GLFW
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

# 리소스 폴더를 빌드 디렉토리로 복사
file(COPY ${CMAKE_SOURCE_DIR}/resources 
     DESTINATION ${CMAKE_BINARY_DIR})

# 컴파일 옵션
target_compile_options(graphics PRIVATE -Wall)