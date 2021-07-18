#include "batch.h"

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

constexpr int MAX_VERTEX_COUNT = 16384, MAX_INDEX_COUNT = 32768;

Batch::Batch(Shader* shader)
    : combined(1.0),
      vertex_count(0),
      indices_drawn(0),
      vbo_pos_mapped(nullptr),
      vbo_colour_mapped(nullptr),
      vbo_tex_coords_mapped(nullptr),
      ibo_mapped(nullptr),
      texture(-1),
      shader(shader) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_pos);
    glGenBuffers(1, &vbo_colour);
    glGenBuffers(1, &vbo_tex_coords);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(vec3), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colour);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(vec4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_tex_coords);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(vec2), nullptr, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDEX_COUNT * sizeof(GLuint), nullptr, GL_STATIC_DRAW);
}

Batch::~Batch() {
    // glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo_pos);
    glDeleteBuffers(1, &vbo_colour);
    glDeleteBuffers(1, &vbo_tex_coords);
    glDeleteBuffers(1, &ibo);
}

void Batch::begin() {
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

void Batch::set_texture(GLuint texture_id) {
    if (texture_id == texture) return;
    texture = texture_id;

    end();
    begin();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader->program, "tex"), 0);
}

void Batch::set_texture(Texture* texture) { set_texture(texture->id); }

GLuint Batch::add_vertex(Vertex v) {
    vbo_pos_mapped[vertex_count] = v.pos;
    vbo_colour_mapped[vertex_count] = v.colour;
    vbo_tex_coords_mapped[vertex_count] = v.tex_coords;
    return vertex_count++;
}

GLuint Batch::add_vertex(vec3 pos, vec4 colour, vec2 tex_coords) {
    vbo_pos_mapped[vertex_count] = pos;
    vbo_colour_mapped[vertex_count] = colour;
    vbo_tex_coords_mapped[vertex_count] = tex_coords; 
    return vertex_count++;
}

void Batch::draw_vertex(GLuint vertex_index) {
    ibo_mapped[indices_drawn] = vertex_index;
    indices_drawn++;
}

void Batch::draw_rect(Vertex low, Vertex high) {
	GLint v1 = add_vertex(low);
	GLint v2 = add_vertex({ high.pos.x, low.pos.y, (low.pos.z + high.pos.z) * 0.5f }, (low.colour + high.colour) * 0.5f, { high.tex_coords.x, low.tex_coords.y });
	GLint v3 = add_vertex({ low.pos.x, high.pos.y, (low.pos.z + high.pos.z) * 0.5f }, (low.colour + high.colour) * 0.5f, { low.tex_coords.x, high.tex_coords.y });
	GLint v4 = add_vertex(high);

	draw_vertex(v1);
	draw_vertex(v2);
	draw_vertex(v3);
	draw_vertex(v2);
	draw_vertex(v3);
	draw_vertex(v4);
}