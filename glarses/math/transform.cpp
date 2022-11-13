#include "transform.h"

namespace glarses::math {
	Transform& Transform::set_position(float x, float y, float z) {
		m_Position = glm::vec3(x, y, z);
		return *this;
	}

	Transform& Transform::set_position(const glm::vec3& v) {
		m_Position = v;
		return *this;
	}

	const glm::vec3& Transform::get_position() const {
		return m_Position;
	}

	Transform& Transform::set_orientation(const glm::quat& q) {
		m_Orientation = q;
		return *this;
	}

	Transform& Transform::set_angle_axis(float angle, const glm::vec3& axis) {
		m_Orientation = glm::angleAxis(angle, axis);
		return *this;
	}

	Transform& Transform::set_euler(float roll, float pitch, float yaw) {
		m_Orientation = glm::quat(glm::vec3(roll, pitch, yaw));
		return *this;
	}

	Transform& Transform::set_euler(const glm::vec3& euler_angles) {
		m_Orientation = glm::quat(euler_angles);
		return *this;
	}

	const glm::quat& Transform::get_orientation() const {
		return m_Orientation;
	}

	Transform& Transform::set_scale(float x, float y, float z) {
		m_Scale = glm::vec3(x, y, z);
		return *this;
	}

	Transform& Transform::set_scale(const glm::vec3& v) {
		m_Scale = v;
		return *this;
	}

	const glm::vec3& Transform::get_scale() const {
		return m_Scale;
	}

	glm::vec3 Transform::to_local_frame(const glm::vec3& point) {
		auto result = point - m_Position;
		result /= m_Scale;
		result = glm::rotate(m_Orientation, result);
		
		return result;
	}

	glm::vec3 Transform::to_parent_frame(const glm::vec3& point) {
		auto result = glm::rotate(glm::inverse(m_Orientation), point);
		result *= m_Scale;
		result += m_Position;
		
		return result;
	}

	glm::mat4 Transform::get_matrix_to_local() const {
		auto result = glm::translate(-m_Position);
		result *= glm::scale(1.0f / m_Scale);
		result *= glm::mat4_cast(m_Orientation);
		
		return result;
	}

	glm::mat4 Transform::get_matrix_to_parent() const {
		auto result = glm::mat4_cast(glm::inverse(m_Orientation));
		result *= glm::scale(m_Scale);
		result *= glm::translate(m_Position);

		return result;
	}
}