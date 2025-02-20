#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>  // For glm functionality
#include <glm/gtc/type_ptr.hpp>  // For passing glm matrices to shaders

// Function to read shader text files
std::string get_file_contents(const char* filename);

class Shader {
public:
    GLuint ID;  // Shader Program ID

    // Constructor that loads shaders from file and compiles them
    Shader(const char* vertexFile, const char* fragmentFile);

    // Activate the shader program
    void Activate() const;

    // Delete the shader program
    void Delete();

    // Utility functions for setting uniforms
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif
