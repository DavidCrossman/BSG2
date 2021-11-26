#include "shader.h"

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <vector>

namespace bsg2 {
Shader::Shader(GLuint program) : program(program) {}

// Adapted from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path) {
    // Create the shaders
    GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vertex_code;
    std::ifstream vertex_stream(vertex_file_path, std::ios::in);
    if (vertex_stream.is_open()) {
        std::stringstream sstr;
        sstr << vertex_stream.rdbuf();
        vertex_code = sstr.str();
        vertex_stream.close();
    }
    else {
        std::cerr << "ERROR: Could not open " << vertex_file_path << std::endl;
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string fragment_code;
    std::ifstream fragment_stream(fragment_file_path, std::ios::in);
    if (fragment_stream.is_open()) {
        std::stringstream sstr;
        sstr << fragment_stream.rdbuf();
        fragment_code = sstr.str();
        fragment_stream.close();
    }
    else {
        std::cerr << "ERROR: Could not open " << fragment_file_path << std::endl;
        return 0;
    }

    int info_log_length;

    // Compile Vertex Shader
    const char *vertex_source = vertex_code.c_str();
    glShaderSource(vertex_id, 1, &vertex_source, NULL);
    glCompileShader(vertex_id);

    // Check Vertex Shader
    glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> vertex_error_message((size_t)info_log_length + 1);
        glGetShaderInfoLog(vertex_id, info_log_length, NULL, &vertex_error_message[0]);
        std::cerr << &vertex_error_message[0] << std::endl;
    }

    // Compile Fragment Shader
    const char* FragmentSourcePointer = fragment_code.c_str();
    glShaderSource(fragment_id, 1, &FragmentSourcePointer, NULL);
    glCompileShader(fragment_id);

    // Check Fragment Shader
    glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> fragment_error_message((size_t)info_log_length + 1);
        glGetShaderInfoLog(fragment_id, info_log_length, NULL, &fragment_error_message[0]);
        std::cerr << &fragment_error_message[0] << std::endl;
    }

    // Link the program
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_id);
    glAttachShader(program_id, fragment_id);
    glLinkProgram(program_id);

    // Check the program
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> program_error_message((size_t)info_log_length + 1);
        glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
        std::cerr << &program_error_message[0] << std::endl;
    }

    glDetachShader(program_id, vertex_id);
    glDetachShader(program_id, fragment_id);

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    return program_id;
}

ShaderManager::ShaderManager(const std::string& asset_dir) : AssetManager(asset_dir) {}

void ShaderManager::load(std::string path) {
    std::string qualified_path = m_asset_dir + path;
    std::string vertex = qualified_path + "/vertex.glsl";
    std::string fragment = qualified_path + "/fragment.glsl";
    GLuint program = load_shaders(vertex.c_str(), fragment.c_str());
    assets.emplace(path, new Shader(program));
}
}