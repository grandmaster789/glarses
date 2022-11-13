#pragma once

#include "../dependencies.h"

#include <string>
#include <filesystem>

// currently only suited to the basic vtx+frag shader program
class ShaderProgram {
public:
	ShaderProgram() = default;
	~ShaderProgram();

	ShaderProgram             (const ShaderProgram&) = delete;
	ShaderProgram& operator = (const ShaderProgram&) = delete;
	ShaderProgram             (ShaderProgram&& sp) noexcept;
	ShaderProgram& operator = (ShaderProgram&& sp) noexcept;

	// throws on possible errors
	static ShaderProgram load_from_memory(
		const std::string& vtx_shader_source,
		const std::string& vtx_shader_source_name,
		const std::string& frag_shader_source,
		const std::string& frag_shader_source_name
	);

	void use();

private:
	GLuint m_VertexShader   = 0;
	GLuint m_FragmentShader = 0;
	GLuint m_ShaderProgram  = 0;
};

ShaderProgram load_shader_sources(
	const std::filesystem::path& vtx_shader_file, 
	const std::filesystem::path& frag_shader_file
);