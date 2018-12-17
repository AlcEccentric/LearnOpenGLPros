#include <iostream>
#include <math3d.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "headers/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

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
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // left lower
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // right lower
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // left upper
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f // right upper
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    
    // gen VAO & VBO & EBO and configure them
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Note: EBO should be bound to GL_ELEMENT_ARRAY_BUFFER not simple GL_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(2);
    // unbind vbo and vao
    // bind vao when needed
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glBindVertexArray(0);
    
    // set wireframe/fillframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // set texture
    
    unsigned int tex1, tex2;
    glGenTextures(1, &tex1);
    
    // bind
    glBindTexture(GL_TEXTURE_2D, tex1);
    // set texture wrap and texture filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // begin to load page as texuture
    int width1, height1, nchanels1;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data1 = stbi_load("container.jpg", &width1, &height1, &nchanels1, 0);
    if(data1){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glGenTextures(1, &tex2);
    // bind
    glBindTexture(GL_TEXTURE_2D, tex2);
    // set texture wrap and texture filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width2, height2, nchanels2;
    stbi_set_flip_vertically_on_load(true);
    unsigned char * data2 = stbi_load("awesomeface.png", &width2, &height2, &nchanels2, 0);
    if(data2){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);
    glBindTexture(GL_TEXTURE_2D, 0);

    // set texture unit (location) to uniform vars
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    
    // activate and bind
    // when activating the texture
    // use the GL_Texture0/1/... instead of 0/1/...
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2);
    
    glm::mat4 trans;
    
    // glm will multiply the matrix from left to right
    // so the product here is
    // T * S * R
    // since we will put matrix on the left of a vector
    // i.e. T * S * R * v
    // thus, the order of these translations are R, S, T
    trans = glm::translate(trans, glm::vec3(0.5, 0.5, 0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
//    trans = glm::rotate(trans, glm::radians(45.0f ), glm::vec3(0.0, 0.0, 1.0));
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
        float spinSpeed = glfwGetTime()/100000;

        trans = glm::rotate(trans, glm::radians(360.0f * spinSpeed), glm::vec3(0.0, 0.0, 1.0));
       
        ourShader.use();
        ourShader.setGlmValueMat4("ourMat", glm::value_ptr(trans));
        ourShader.setVec2("ourGB", ourGreen, ourBlue);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        
        // SHOW
        // swap the render buffer to the screen buffer every iteration
        glfwSwapBuffers(window);
        // check the events happen in this iteration
        // updates the window state, and calls the corresponding functions
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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
