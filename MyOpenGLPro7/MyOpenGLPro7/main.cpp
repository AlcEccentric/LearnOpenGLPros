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
#include "headers/camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float lastFrame = 0, deltaTime = 0;

// framebuffer_size_callback is a callback function to adjust to the resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// handle mouse move
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// handle scroll move
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// when escape is pressed
// we tell glfw the window should close
void processInput(GLFWwindow *window);
// process WASD keys to allow users to move the camera
void processWASD(GLFWwindow *window, const glm::vec3 &cameraUp, const glm::vec3 &cameraFront, glm::vec3 &cameraPos, float renderTimePerFrame);


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
    // capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);
    
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    // framebuffer_size_callback is a callback function to adjust to the resizing
    // tell glfw when window is resized call this function
    //We register the callback functions after we've created the window and before the game loop is initiated.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // link the pipeline
    // in order to read file in xcode
    // check Product->Scheme->Edit Scheme->Options->Working Directory: Using custom working directory
    // and specify the path with main.cpp's path
    Shader ourShader("vshader.vs", "fshader.fs");
    
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    
    // gen VAO & VBO & EBO and configure them
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Note: EBO should be bound to GL_ELEMENT_ARRAY_BUFFER not simple GL_ARRAY_BUFFER

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    
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
    

    
    
    // used to give positions of different cudes in world coordinate system
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    
    
    // enable depth test
    glEnable(GL_DEPTH_TEST);
    
    // used to compute the time spent on rendering each frame

    
    // render loop
    while(!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        // INPUT processing
        processInput(window);
        
        //RENDER HERE
        // tell glClear to clear the color buffer in last iteration with RGBA specified here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);// clear the color buffer and z-buffer
        
        float ourGreen = sin(glfwGetTime()) * 0.5 + 0.5;
        float ourBlue = cos(glfwGetTime()) * 0.5 + 0.5;

       
        ourShader.use();
        
        
        
        ourShader.setVec2("ourGB", ourGreen, ourBlue);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        
//        float radius = 10.0f;
//        float camX = sin(glfwGetTime()) * radius;
//        float camZ = cos(glfwGetTime()) * radius;
//        glm::mat4 view;
//        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
//        ourShader.setGlmValueMat4("view", glm::value_ptr(view));
        
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setGlmValueMat4("view", glm::value_ptr(view));
        
        glm::mat4 project = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setGlmValueMat4("project", glm::value_ptr(project));
        
        for(unsigned int i = 0; i < 10; i++)
        {
            // let all 10 cubes rotate
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime()/10* glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setGlmValueMat4("model", glm::value_ptr(model));
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
//        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = -(ypos - lastY); // for the positive direction of y axis is down-side
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);

    
}

// when escape is pressed
// we tell glfw the window should close
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
        
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    // limit the fov between 1 and 45 degrees
    camera.ProcessMouseScroll(yoffset);
}
