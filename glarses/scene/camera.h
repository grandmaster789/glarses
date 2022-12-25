#pragma once

namespace glarses::scene {
	class Camera {
	public:

	private:
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		glm::vec3 m_Position;
	};
}