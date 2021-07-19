#pragma once

#include <glm/glm.hpp>
#include "../include/application.h"
#include "../include/batch.h"
#include "../include/texture_manager.h"

class TestApp : public bsg2::Application {
	bsg2::ShaderManager shaders;
	bsg2::Batch* batch;
	bsg2::OrthographicCamera cam;
	bsg2::TextureManager textures;
	bsg2::Texture* squares;
	int width, height;
public:
	TestApp(GLFWwindow* window, int width, int height);
	~TestApp() override;

	glm::mat3 screen_to_world();
	void frame(int frame_count, float delta) override;
	void resize(int width, int height);
};