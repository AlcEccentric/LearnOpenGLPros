//
//  Shader.h
//  MyOpenGLPro2
//
//  Created by 李弘毅 on 2018/12/15.
//  Copyright © 2018 李弘毅. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // the program ID
    unsigned int ID;
    
    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    
    void setVec2(const std::string &name, float value1, float value2) const;
};



#endif /* Shader_h */
