#pragma once

#include <vector>
#include <initializer_list>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "texture.h"
#include "vertex.h"
#include "frame_buffer.h"

namespace bsg2 {
class Batch {
public:
    glm::mat4 combined;
    Batch();
    Batch(GLuint shader_program);
    Batch(const Shader& shader);
    Batch(const Batch& other) = delete;
    ~Batch();
    void restart();
    void begin();
    void begin(GLuint texture_id);
    void begin(const Texture& texture);
    void begin(const FrameBuffer& frame_buffer);
    void end();
    void prepare(int vertices, int indices);
    void set_tint(const glm::vec4& colour);
    void set_tint(const glm::vec3& colour);
    void set_tint(float r, float g, float b, float a = 1.f);
    void set_texture(GLuint texture_id);
    void set_texture(const Texture& texture);
    void use_default_texture();
    void set_shader(GLuint shader_program);
    void set_shader(const Shader& shader);
    void use_default_shader();
    void read_frame_buffer(const FrameBuffer& frame_buffer);
    GLuint add_vertex(const glm::vec2& pos, const glm::vec4& colour, const glm::vec2& tex_coords, float depth = 0);
    GLuint add_vertex(const Vertex& v);
    void draw_vertex(GLuint vertex_index);
    void draw_rect(const Vertex& low, const Vertex& high);
    void draw_rect(const glm::vec2& centre, float width, float height, float rotation = 0, const glm::vec4& colour = glm::vec4(1),
        const glm::vec2& tex_coords_low = glm::vec2(0), const glm::vec2& tex_coords_high = glm::vec2(1), float depth = 0);
    void draw_tri(const Vertex& v0, const Vertex& v1, const Vertex& v2);
    void draw_quad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3);
    void draw_tri_strip(const std::vector<Vertex>& vertices);
    void draw_tri_strip(const std::initializer_list<Vertex>& vertices);
    void draw_tri_fan(const std::vector<Vertex>& vertices);
    void draw_tri_fan(const std::initializer_list<Vertex>& vertices);
private:
    GLuint vao, vbo, ibo, texture, shader, default_texture, default_shader;
    int vertex_count, indices_drawn;
    Vertex* vbo_mapped;
    GLuint* ibo_mapped;
    glm::vec4 tint;
};
}