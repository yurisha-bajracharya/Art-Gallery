#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp> // Added missing glm include
#include <glm/gtc/type_ptr.hpp> // Needed for setting uniforms

// Function to read shader text files
std::string get_file_contents(const char* filename);

class Shader {
public:
    GLuint ID; // Ensure ID is declared in public scope

    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();

    // Utility functions for setting uniforms
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif
