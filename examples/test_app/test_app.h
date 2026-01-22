#pragma once

#include <vector>
#include "../include/application.h"
#include "../include/batch.h"
#include "../include/viewport.h"
#include "../include/frame_buffer.h"

class TestApp : public bsg2::Application {
public:
	TestApp(const bsg2::ApplicationConfiguration& config = bsg2::ApplicationConfiguration());
	~TestApp() override;
	void resize(int width, int height);
private:
	bsg2::ShaderManager shaders;
	bsg2::Shader& inversion;
	bsg2::Batch batch;
	bsg2::FitViewport view;
	bsg2::FrameBuffer frame_buffer;
	bsg2::TextureManager textures;
	const bsg2::Texture& squares;
	std::vector<bsg2::Vertex> strip_vertices, fan_vertices;
	void frame() override;
};