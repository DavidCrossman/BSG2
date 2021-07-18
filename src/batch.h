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
    Shader* shader;
    glm::vec3* vbo_pos_mapped;
    glm::vec4* vbo_colour_mapped;
    glm::vec2* vbo_tex_coords_mapped;
    GLuint* ibo_mapped;

public:
    glm::mat4 combined;
    Batch(Shader* shader);
    ~Batch();
    void begin();
    void end();
    void set_texture(GLuint texture_id);
    void set_texture(Texture* texture);
    GLuint add_vertex(glm::vec3 pos, glm::vec4 colour, glm::vec2 tex_coords);
    GLuint add_vertex(Vertex v);
    void draw_vertex(GLuint vertex_index);
    void draw_rect(Vertex low, Vertex high);
};
}