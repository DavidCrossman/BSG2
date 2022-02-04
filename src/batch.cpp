#include "batch.h"

#include <vector>
#include <glm/gtc/type_ptr.hpp>

using glm::vec2, glm::vec3, glm::vec4, glm::mat2;

namespace bsg2 {
static constexpr int maxVertexCount = 16384, maxIndexCount = 32768;

Batch::Batch(Shader& shader) : combined(1.0), vertex_count(0), indices_drawn(0),
        vbo_pos_mapped(nullptr), vbo_colour_mapped(nullptr), vbo_tex_coords_mapped(nullptr),
        ibo_mapped(nullptr), texture(-1), shader(&shader) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_pos);
    glGenBuffers(1, &vbo_colour);
    glGenBuffers(1, &vbo_tex_coords);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, maxVertexCount * sizeof(vec3), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colour);
    glBufferData(GL_ARRAY_BUFFER, maxVertexCount * sizeof(vec4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coords);
    glBufferData(GL_ARRAY_BUFFER, maxVertexCount * sizeof(vec2), nullptr, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndexCount * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

    float pixels[4] = { 1.f, 1.f, 1.f, 1.f };
    glGenTextures(1, &default_texture);
    glBindTexture(GL_TEXTURE_2D, default_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, pixels);
}

Batch::~Batch() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo_pos);
    glDeleteBuffers(1, &vbo_colour);
    glDeleteBuffers(1, &vbo_tex_coords);
    glDeleteBuffers(1, &ibo);
    glDeleteTextures(1, &default_texture);
}

void Batch::restart() {
    vertex_count = indices_drawn = 0;

    glUseProgram(shader->program);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    vbo_pos_mapped = (vec3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colour);
    vbo_colour_mapped = (vec4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coords);
    vbo_tex_coords_mapped = (vec2*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    ibo_mapped = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void Batch::begin() {
    begin(default_texture);
}

void Batch::begin(GLuint texture_id) {
    restart();

    texture = texture_id;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader->program, "tex"), 0);
}

void Batch::begin(const Texture& texture) {
    begin(texture.id);
}

void Batch::begin(const FrameBuffer& frame_buffer) {
    begin(frame_buffer.colour_texture);
}

void Batch::end() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colour);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coords);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    if (!indices_drawn) return;

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "combined"), 1, GL_FALSE, value_ptr(combined));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colour);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coords);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glDrawElements(GL_TRIANGLES, indices_drawn, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Batch::prepare(int vertices, int indices) {
    if (indices_drawn + indices > maxIndexCount || vertex_count + vertices > maxVertexCount) {
        end();
        restart();
    }
}

void Batch::set_texture(GLuint texture_id) {
    if (texture_id == texture) return;

    end();
    begin(texture_id);
}

void Batch::set_texture(const Texture& texture) {
    set_texture(texture.id);
}

void Batch::use_default_texture() {
    set_texture(default_texture);
}

void Batch::set_shader(Shader& shader) {
    this->shader = &shader;
}

void Batch::read_frame_buffer(const FrameBuffer& frame_buffer) {
    end();
    begin(frame_buffer);
}

GLuint Batch::add_vertex(const Vertex& v) {
    return add_vertex(v.pos, v.colour, v.tex_coords, v.depth);
}

GLuint Batch::add_vertex(const vec2& pos, const vec4& colour, const vec2& tex_coords, float depth) {
    vbo_pos_mapped[vertex_count] = { pos, depth };
    vbo_colour_mapped[vertex_count] = colour;
    vbo_tex_coords_mapped[vertex_count] = tex_coords;
    return vertex_count++;
}

void Batch::draw_vertex(GLuint vertex_index) {
    ibo_mapped[indices_drawn] = vertex_index;
    indices_drawn++;
}

void Batch::draw_rect(const Vertex& low, const Vertex& high) {
    prepare(4, 6);

	GLint i0 = add_vertex(low);
	GLint i1 = add_vertex({ high.pos.x, low.pos.y }, (low.colour + high.colour) * 0.5f,
        { high.tex_coords.x, low.tex_coords.y }, (low.depth + high.depth) * 0.5f);
	GLint i2 = add_vertex({ low.pos.x, high.pos.y }, (low.colour + high.colour) * 0.5f,
        { low.tex_coords.x, high.tex_coords.y }, (low.depth + high.depth) * 0.5f);
	GLint i3 = add_vertex(high);

	draw_vertex(i0);
	draw_vertex(i1);
	draw_vertex(i2);
	draw_vertex(i1);
	draw_vertex(i2);
	draw_vertex(i3);
}

void Batch::draw_rect(const vec2& centre, float width, float height, float rotation,
        const vec4& colour, const vec2& tex_coords_low, const vec2& tex_coords_high, float depth) {
    prepare(4, 6);

    mat2 rot_mat = { { cosf(rotation), sinf(rotation) }, { -sinf(rotation), cosf(rotation) } };

    GLint i0 = add_vertex(centre + 0.5f * rot_mat * vec2(-width, -height), colour, tex_coords_low, depth);
    GLint i1 = add_vertex(centre + 0.5f * rot_mat * vec2(width, -height), colour, { tex_coords_high.x, tex_coords_low.y }, depth);
    GLint i2 = add_vertex(centre + 0.5f * rot_mat * vec2(-width, height), colour, { tex_coords_low.x, tex_coords_high.y }, depth);
    GLint i3 = add_vertex(centre + 0.5f * rot_mat * vec2(width, height), colour, tex_coords_high, depth);

    draw_vertex(i0);
    draw_vertex(i1);
    draw_vertex(i2);
    draw_vertex(i1);
    draw_vertex(i2);
    draw_vertex(i3);
}

void Batch::draw_tri(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    prepare(3, 3);

	draw_vertex(add_vertex(v0));
	draw_vertex(add_vertex(v1));
	draw_vertex(add_vertex(v2));
}

void Batch::draw_quad(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    prepare(4, 6);

    GLint i0 = add_vertex(v0);
    GLint i1 = add_vertex(v1);
    GLint i2 = add_vertex(v2);
    GLint i3 = add_vertex(v3);

    draw_vertex(i0);
    draw_vertex(i1);
    draw_vertex(i2);
    draw_vertex(i0);
    draw_vertex(i2);
    draw_vertex(i3);
}

void Batch::draw_tri_strip(const std::vector<Vertex>& vertices) {
    int count = (int)vertices.size();
    if (count < 3) return;

    prepare(count, (count - 2) * 3);

    std::vector<GLint> indices;
    indices.reserve(count);
    for (const Vertex& v : vertices) {
        indices.push_back(add_vertex(v));
    }
    
    for (size_t i = 2; i < count; ++i) {
        draw_vertex(indices[i - 2]);
        draw_vertex(indices[i - 1]);
        draw_vertex(indices[i]);
    }
}

void Batch::draw_tri_strip(const std::initializer_list<Vertex>& vertices) {
    draw_tri_strip(std::vector(vertices));
}

void Batch::draw_tri_fan(const std::vector<Vertex>& vertices) {
    int count = (int)vertices.size();
    if (count < 3) return;

    prepare(count, (count - 2) * 3);

    std::vector<GLint> indices;
    indices.reserve(count);
    for (const Vertex& v : vertices) {
        indices.push_back(add_vertex(v));
    }

    for (size_t i = 2; i < count; ++i) {
        draw_vertex(indices[0]);
        draw_vertex(indices[i - 1]);
        draw_vertex(indices[i]);
    }
}

void Batch::draw_tri_fan(const std::initializer_list<Vertex>& vertices) {
    draw_tri_fan(std::vector(vertices));
}
}
