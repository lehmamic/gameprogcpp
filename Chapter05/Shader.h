//
//  Shader.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 14.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader {
public:
    Shader();
    ~Shader();
    
    // Load the vertex/fragemnt shaders with the given names
    bool Load(const std::string& vertName, const std::string& fragName);
    void Unload();
    
    // Set this as the active shader program
    void SetActive();
    
    // Sets a Matrix uniform
    void SetMatrixUniform(const char* name, const Matrix4& matrix);
    
private:
    // Tries to compile the specified shader
    bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);

    // Tests whether shader compiled sucessfully
    bool IsCompiled(GLuint shader);
    
    // tests whether vertex/fragment programs link
    bool IsValidProgram();
    
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};

#endif /* Shader_hpp */
