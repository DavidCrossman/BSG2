#include "frame_buffer.h"

#include <cstdlib>

namespace bsg2 {
FrameBuffer::FrameBuffer(GLsizei width, GLsizei height, int msaa_samples) :
		width(width), height(height), msaa_samples(msaa_samples) {
	glGenFramebuffers(1, &fbo);
	glGenFramebuffers(1, &multisample_fbo);
	glGenTextures(1, &colour_texture);
	glGenTextures(1, &multisample_colour_texture);

	resize(width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glBindTexture(GL_TEXTURE_2D, colour_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colour_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::exit(4);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, multisample_fbo);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisample_colour_texture);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisample_colour_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::exit(5);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &fbo);
	glDeleteFramebuffers(1, &multisample_fbo);
	glDeleteTextures(1, &colour_texture);
	glDeleteTextures(1, &multisample_colour_texture);
}

void FrameBuffer::resize(GLsizei width, GLsizei height) {
	glBindTexture(GL_TEXTURE_2D, colour_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisample_colour_texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_samples, GL_RGBA8, width, height, false);
	this->width = width;
	this->height = height;
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, multisample_fbo);
	glViewport(0, 0, width, height);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, multisample_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
}