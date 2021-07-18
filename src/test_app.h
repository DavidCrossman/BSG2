#pragma once

#include <glm/glm.hpp>
#include "application.h"
#include "batch.h"
#include "texture_manager.h"

class TestApp : public Application {
	ShaderManager shaders;
	Batch* batch;
	OrthographicCamera cam;
	TextureManager textures;
	Texture* squares;
	int width, height;
public:
	TestApp(int width, int height);
	~TestApp() override;

	glm::mat3 screen_to_world();
	void initialise() override;
	void frame() override;
	void framebuffer_size_callback(int width, int height) override;
};