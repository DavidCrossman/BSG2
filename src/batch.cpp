#include "batch.h"

#include <vector>
#include <glm/gtc/type_ptr.hpp>

using glm::vec2, glm::vec3, glm::vec4, glm::mat2;

namespace bsg2 {
static constexpr int maxVertexCount = 16384, maxIndexCount = 32768;

Batch::Batch() : combined(1.0), vertex_count(0), indices_drawn(0),
        vbo_mapped(nullptr), ibo_mapped(nullptr), texture(-1), tint(1) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, maxVertexCount * sizeof(Vertex), nullptr, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndexCount * sizeof(GLuint), nullptr, GL_STATIC_DRAW);

    float pixels[4] = { 1.f, 1.f, 1.f, 1.f };
    glGenTextures(1, &default_texture);
    glBindTexture(GL_TEXTURE_2D, default_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, pixels);

    GLuint vertex_id = glCreateShader(GL_VERTEX_SHADER),
        fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertex_source = R"(
        #version 330 core
        layout(location = 0) in vec3 a_pos;
        layout(location = 1) in vec4 a_colour;
        layout(location = 2) in vec2 a_tex_coords;

        out vec3 v_pos;
        out vec4 v_colour;
        out vec2 v_tex_coords;

        uniform mat4 combined;

        void main() {
            v_pos = a_pos;
            v_colour = a_colour;
            v_tex_coords = a_tex_coords;
            gl_Position = combined * vec4(a_pos, 1.0);
        }
    )", * fragment_source = R"(
        #version 330 core

        in vec3 v_pos;
        in vec4 v_colour;
        in vec2 v_tex_coords;

        out vec4 f_colour;

        uniform sampler2D tex;

        void main() {
	        f_colour = v_colour * texture(tex, v_tex_coords);
        }
    )";

    glShaderSource(vertex_id, 1, &vertex_source, NULL);
    glCompileShader(vertex_id);

    glShaderSource(fragment_id, 1, &fragment_source, NULL);
    glCompileShader(fragment_id);

    default_shader = glCreateProgram();
    glAttachShader(default_shader, vertex_id);
    glAttachShader(default_shader, fragment_id);
    glLinkProgram(default_shader);

    glDetachShader(default_shader, vertex_id);
    glDetachShader(default_shader, fragment_id);

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    shader = default_shader;
}

Batch::Batch(GLuint shader_program) : Batch() {
    set_shader(shader_program);
}

Batch::Batch(const Shader& shader) : Batch() {
    set_shader(shader);
}

Batch::~Batch() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteTextures(1, &default_texture);
    glDeleteProgram(default_shader);
}

void Batch::restart() {
    vertex_count = indices_drawn = 0;

    glUseProgram(shader);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    vbo_mapped = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
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
    glUniform1i(glGetUniformLocation(shader, "tex"), 0);
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
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    if (!indices_drawn) return;

    glUniformMatrix4fv(glGetUniformLocation(shader, "combined"), 1, GL_FALSE, glm::value_ptr(combined));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec3) + sizeof(vec4)));
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

void Batch::set_tint(const glm::vec4& colour) {
    tint = glm::clamp(colour, vec4(0), vec4(1));
}

void Batch::set_tint(const glm::vec3& colour) {
    set_tint({ colour, 1.f });
}

void Batch::set_tint(float r, float g, float b, float a) {
    set_tint({ r, g, b, a });
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

void Batch::set_shader(GLuint shader_program) {
    shader = shader_program;
}

void Batch::set_shader(const Shader& shader) {
    set_shader(shader.program);
}

void Batch::use_default_shader() {
    set_shader(default_shader);
}

void Batch::read_frame_buffer(const FrameBuffer& frame_buffer) {
    end();
    begin(frame_buffer);
}

GLuint Batch::add_vertex(const Vertex& v) {
    return add_vertex(v.pos, v.colour, v.tex_coords, v.depth);
}

GLuint Batch::add_vertex(const vec2& pos, const vec4& colour, const vec2& tex_coords, float depth) {
    vbo_mapped[vertex_count] = { pos, colour * tint, tex_coords, depth };
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
    if (count < 3 || count > maxVertexCount) return;

    prepare(count, (count - 2) * 3);

    GLuint first = add_vertex(vertices[0]), second = add_vertex(vertices[1]), third;

    for (size_t i = 2; i < count; ++i) {
        third = add_vertex(vertices[i]);
        draw_vertex(first);
        first = second;
        draw_vertex(second);
        second = third;
        draw_vertex(third);
    }
}

void Batch::draw_tri_strip(const std::initializer_list<Vertex>& vertices) {
    draw_tri_strip(std::vector(vertices));
}

void Batch::draw_tri_fan(const std::vector<Vertex>& vertices) {
    int count = (int)vertices.size();
    if (count < 3 || count > maxVertexCount) return;

    prepare(count, (count - 2) * 3);

    GLuint origin = add_vertex(vertices[0]), previous = add_vertex(vertices[1]), current;

    for (size_t i = 2; i < count; ++i) {
        current = add_vertex(vertices[i]);
        draw_vertex(origin);
        draw_vertex(previous);
        draw_vertex(current);
        previous = current;
    }
}

void Batch::draw_tri_fan(const std::initializer_list<Vertex>& vertices) {
    draw_tri_fan(std::vector(vertices));
}
}