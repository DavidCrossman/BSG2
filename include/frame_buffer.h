#pragma once

#include <GL/glew.h>

namespace bsg2 {
class FrameBuffer {
	friend class Batch;
public:
	FrameBuffer(GLsizei width, GLsizei height, int msaa_samples = 4);
	~FrameBuffer();
	void resize(GLsizei width, GLsizei height);
	void bind();
	void unbind();
private:
	GLuint fbo, multisample_fbo, colour_texture, multisample_colour_texture;
	GLsizei width, height;
	int msaa_samples;
};
}