#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "vertex.h"

namespace bsg2 {
class Batch {
    GLuint vao, vbo_pos, vbo_colour, vbo_tex_coords, ibo, vertex_count, indices_drawn, texture;
    glm::vec3* vbo_pos_mapped;
    glm::vec4* vbo_colour_mapped;
    glm::vec2* vbo_tex_coords_mapped;
    GLuint* ibo_mapped;

public:
    Shader* shader;
    glm::mat4 combined;
    Batch(Shader* shader);
    Batch(const Batch& other) = delete;
    ~Batch();
    void begin();
    void end();
    void prepare(int vertices, int indices);
    void set_texture(GLuint texture_id);
    void set_texture(Texture* texture);
    GLuint add_vertex(const glm::vec2& pos, const glm::vec4& colour, const glm::vec2& tex_coords, float depth = 0);
    GLuint add_vertex(const Vertex& v);
    void draw_vertex(GLuint vertex_index);
    void draw_rect(const Vertex& low, const Vertex& high);
    void draw_tri(const Vertex& v0, const Vertex& v1, const Vertex& v2);
    void draw_quad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3);
};
}