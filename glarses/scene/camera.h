#pragma once

#ifndef GLARSES_SCENE_CAMERA_H
#define GLARSES_SCENE_CAMERA_H

#include "../dependencies.h"

namespace glarses::scene {
	class Camera {
	public:

	private:
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		glm::vec3 m_Position;
	};
}

#endif