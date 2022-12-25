#pragma once

#include "../dependencies.h"
#include <cstdint>

namespace glarses::scene {
	struct Material {
		glm::vec4 m_Emissive  = { 0.0f, 0.0f, 0.0f, 0.0f };
		glm::vec4 m_Albedo    = { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 m_Roughness = { 1.0f, 1.0f, 0.0f, 0.0f };
		
		float m_Opaque    = 1.0f; // transparancy factor
		float m_AlphaTest = 0.0f;
		float m_Metallic  = 0.0f;

		uint64_t m_AmbientOcclusionMap = ~0x0;
		uint64_t m_EmissiveMap         = ~0x0;
		uint64_t m_AlbedoMap           = ~0x0;

		uint64_t m_MetallicRoughnessMap = ~0x0;
		uint64_t m_NormalMap            = ~0x0;
		uint64_t m_OpacityMap           = ~0x0;
	};

	static_assert(sizeof(Material) % 16 == 0, "Material should be 16-bytes aligned");
}