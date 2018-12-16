#include <iostream>
#include <math3d.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "headers/Shader.h"


// framebuffer_size_callback is a callback function to adjust to the resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// when escape is pressed
// we tell glfw the window should close
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc,char * argv[]) {
    //tell glfw how to configure the window we want to build
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // only for Mac OS
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // create a window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 为该窗口在当前thread设置上下文环境
    glfwMakeContextCurrent(window);
    
    // glfwGetProcAddress指向当前系统opengl函数的位置
    // 交给GLAD完成其初始化，由GLAD管理opengl函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // tell opengl how to render the window
    // first two specify the location
    // second two specify the size
    glViewport(0, 0, 800, 600);
    
    // framebuffer_size_callback is a callback function to adjust to the resizing
    // tell glfw when window is resized call this function
    //We register the callback functions after we've created the window and before the game loop is initiated.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // link the pipeline
    // in order to read file in xcode
    // check Product->Scheme->Edit Scheme->Options->Working Directory: Using custom working directory
    // and specify the path with main.cpp's path
    Shader ourShader("vshader.vs", "fshader.fs");

    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left lower
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// right lower
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f // left upper
    };
    
    
    // gen VAO & VBO & EBO and configure them
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    // unbind vbo and vao
    // bind vao when needed
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
    
    // set wireframe/fillframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // render loop
    while(!glfwWindowShouldClose(window))
    {
        // INPUT
        processInput(window);
        
        //RENDER HERE
        // tell glClear to clear the color buffer in last iteration with RGBA specified here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);// only clear the color buffer
        
        float ourGreen = sin(glfwGetTime()) * 0.5 + 0.5;
        float ourBlue = cos(glfwGetTime()) * 0.5 + 0.5;
        ourShader.use();
        ourShader.setVec2("ourGB", ourGreen, ourBlue);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
        // SHOW
        // swap the render buffer to the screen buffer every iteration
        glfwSwapBuffers(window);
        // check the events happen in this iteration
        // updates the window state, and calls the corresponding functions
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // release the resources occupied by glfw
    glfwTerminate();
    return 0;
}


// framebuffer_size_callback is a callback function to adjust to the resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// when escape is pressed
// we tell glfw the window should close
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
