#include <glm/gtx/transform.hpp>
#include "camera.h"

using namespace glm;

Camera::Camera(): projection(1.0), view(1.0), combined(1.0), aspect_ratio(1) {
	update();
}

void Camera::update() {
	combined = projection * view;
}

OrthographicCamera::OrthographicCamera(vec2 pos, float rotation, float zoom):
		Camera(), pos(pos), rotation(rotation), zoom(zoom) {
	update();
}

void OrthographicCamera::update() {
	view = translate(vec3(-pos, 0)) * rotate(rotation, vec3(0, 0, -1));
	projection = ortho(-zoom * aspect_ratio, zoom * aspect_ratio, -zoom, zoom);
	Camera::update();
}