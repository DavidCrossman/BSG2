#pragma once

#include <glm/glm.hpp>
#include "../include/application.h"
#include "../include/batch.h"

class TestApp : public bsg2::Application {
	bsg2::ShaderManager shaders;
	bsg2::Batch batch;
	bsg2::OrthographicCamera cam;
	bsg2::TextureManager textures;
	const bsg2::Texture& squares;
	void frame() override;
public:
	TestApp(const bsg2::WindowConfiguration& config = bsg2::WindowConfiguration());
	~TestApp() override;
	void resize(int width, int height);
};