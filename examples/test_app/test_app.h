#pragma once

#include <glm/glm.hpp>
#include "../include/application.h"
#include "../include/batch.h"
#include "../include/viewport.h"

class TestApp : public bsg2::Application {
	bsg2::ShaderManager shaders;
	bsg2::Batch batch;
	bsg2::FitViewport view;
	bsg2::TextureManager textures;
	const bsg2::Texture& squares;
	void frame() override;
public:
	TestApp(const bsg2::ApplicationConfiguration& config = bsg2::ApplicationConfiguration());
	~TestApp() override;
	void resize(int width, int height);
};