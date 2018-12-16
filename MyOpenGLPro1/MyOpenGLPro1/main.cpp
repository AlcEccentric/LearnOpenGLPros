#include <iostream>
#include <math3d.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

char infoLog[512];

// framebuffer_size_callback is a callback function to adjust to the resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// when escape is pressed
// we tell glfw the window should close
void processInput(GLFWwindow *window);

// check whether shader is successfully compiled
int compileShaderSuccess(GLint shader);

const char * vertexShaderCode = "#version 330 core\n\
layout (location = 0) in vec3 inPos;\n\
void main(){ \n\
    gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);\n\
}\0";

// uniform is global
// which is accessible anywhere
// we can set the value in the application
// it is particularly useful when you need to pass data concerning iteration to the shader
const char * fragmentShaderCode = "#version 330 core\n\
out vec4 col;\n\
uniform vec4 ourColor; \n\
void main(){ \n\
    col = ourColor;\n\
}\0";


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
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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
    
    
    // compile the shaders
    unsigned int vertexShader, fragShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // bind shader with source code and check the result
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glShaderSource(fragShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragShader);
    if( !compileShaderSuccess(vertexShader) || !compileShaderSuccess(fragShader)){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link the compiled shader into pipeline and delete compiled shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    int proSuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &proSuccess);
    if(!proSuccess){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left lower
        0.5f, -0.5f, 0.0f, // right lower
        -0.5f,  0.5f, 0.0f,  // left upper
        0.5f,  0.5f, 0.0f// right upper
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    // unbind vbo and vao
    // bind vao when needed
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // remember to unbind ebo after unbind vao, otherwise the ebo will not be bound
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // set wireframe
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
        
        glUseProgram(shaderProgram);
        float timeValue = glfwGetTime();
        float varvalue = sin(timeValue)*0.5 + 0.5;
        // Note: location of uniforms could be accessed before use pipeline
        // but set uniform values must be declared after use program
        int ourColorLoc = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(ourColorLoc, 0.5f, varvalue, 0.5f, 1.0f);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        
        // SHOW
        // swap the render buffer to the screen buffer every iteration
        glfwSwapBuffers(window);
        // check the events happen in this iteration
        // updates the window state, and calls the corresponding functions
        glfwPollEvents();
    }
    
    // release the resources occupied by glfw
    glfwTerminate();
    return 0;
}

int compileShaderSuccess(GLint shader){
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return success;
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
