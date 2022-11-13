#pragma once

#include "../dependencies.h"

namespace glarses::math {
	
	class Transform {
	public:
		// full value semantics
		Transform() = default;

		Transform&       set_position(float x, float y, float z);
		Transform&       set_position(const glm::vec3& v);
		const glm::vec3& get_position() const;

		Transform&       set_orientation(const glm::quat& q);
		Transform&       set_angle_axis(float angle, const glm::vec3& axis);
		Transform&       set_euler(float roll, float pitch, float yaw);
		Transform&       set_euler(const glm::vec3& euler_angles);
		const glm::quat& get_orientation() const;

		Transform&       set_scale(float x, float y, float z);
		Transform&       set_scale(const glm::vec3& v);
		const glm::vec3& get_scale() const;

		glm::vec3 to_local_frame(const glm::vec3& point);
		glm::vec3 to_parent_frame(const glm::vec3& point);

		glm::mat4 get_matrix_to_local() const;
		glm::mat4 get_matrix_to_parent() const;

	private:
		glm::vec3 m_Position    = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat m_Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 m_Scale       = glm::vec3(1.0f, 1.0f, 1.0f);
	};
}