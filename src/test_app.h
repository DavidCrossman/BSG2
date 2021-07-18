#pragma once

#include <glm/glm.hpp>
#include "application.h"
#include "batch.h"
#include "texture_manager.h"

class TestApp : public bsg2::Application {
	bsg2::ShaderManager shaders;
	bsg2::Batch* batch;
	bsg2::OrthographicCamera cam;
	bsg2::TextureManager textures;
	bsg2::Texture* squares;
	int width, height;
public:
	TestApp(int width, int height);
	~TestApp() override;

	glm::mat3 screen_to_world();
	void initialise() override;
	void frame(int frame_count, float delta) override;
	void framebuffer_size_callback(int width, int height) override;
};