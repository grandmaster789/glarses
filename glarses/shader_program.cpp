#include "shader_program.h"
#include "io/file.h"

#include <iostream>

ShaderProgram::~ShaderProgram() {
	if (m_ShaderProgram) {
		glUseProgram(0);
		glDeleteProgram(m_ShaderProgram);
	}

	if (m_FragmentShader)
		glDeleteShader(m_FragmentShader);
	if (m_VertexShader)
		glDeleteShader(m_VertexShader);
}

ShaderProgram::ShaderProgram(ShaderProgram&& sp) noexcept :
	m_VertexShader  (sp.m_VertexShader),
	m_FragmentShader(sp.m_FragmentShader),
	m_ShaderProgram (sp.m_ShaderProgram)
{
	sp.m_FragmentShader = 0;
	sp.m_VertexShader   = 0;
	sp.m_ShaderProgram  = 0;
}

ShaderProgram& ShaderProgram::operator = (ShaderProgram&& sp) noexcept {
	if (m_ShaderProgram) {
		glUseProgram(0);
		glDeleteProgram(m_ShaderProgram);
	}

	if (m_FragmentShader)
		glDeleteShader(m_FragmentShader);
	if (m_VertexShader)
		glDeleteShader(m_VertexShader);

	m_ShaderProgram  = sp.m_ShaderProgram;
	m_FragmentShader = sp.m_FragmentShader;
	m_VertexShader   = sp.m_VertexShader;

	sp.m_ShaderProgram  = 0;
	sp.m_FragmentShader = 0;
	sp.m_VertexShader   = 0;

	return *this;
}

ShaderProgram ShaderProgram::load_from_memory(
	const std::string& vtx_shader_source,
	const std::string& vtx_shader_source_name,
	const std::string& frag_shader_source,
	const std::string& frag_shader_source_name
) {
	ShaderProgram result;

	result.m_VertexShader   = glCreateShader(GL_VERTEX_SHADER);
	result.m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	result.m_ShaderProgram  = glCreateProgram();

	// doesnt hurt to check this
	static_assert(sizeof(GLchar) == sizeof(std::string::value_type));

	const char* vtx_shader_raw  = vtx_shader_source.data();
	const char* frag_shader_raw = frag_shader_source.data();

	glShaderSource(result.m_VertexShader,   1, &vtx_shader_raw,  nullptr);
	glShaderSource(result.m_FragmentShader, 1, &frag_shader_raw, nullptr);

	std::cout << "Compiling \"" << vtx_shader_source_name << "\"\n";
	glCompileShader(result.m_VertexShader);

	std::cout << "Compiling \"" << frag_shader_source_name << "\"\n";
	glCompileShader(result.m_FragmentShader);

	// TODO -- report compilation issues etc
	std::cout << "Linking\n";
	glAttachShader(result.m_ShaderProgram, result.m_VertexShader);
	glAttachShader(result.m_ShaderProgram, result.m_FragmentShader);
	glLinkProgram(result.m_ShaderProgram);

	// TODO -- report linking issues etc

	return result;
}

void ShaderProgram::use() {
	glUseProgram(m_ShaderProgram);
}

ShaderProgram load_shader_sources(
	const std::filesystem::path& vtx_shader_file,
	const std::filesystem::path& frag_shader_file
) {
	auto vtx_source  = io::read_text_file(vtx_shader_file);
	auto frag_source = io::read_text_file(frag_shader_file);

	return ShaderProgram::load_from_memory(
		vtx_source,
		vtx_shader_file.stem().string() + vtx_shader_file.extension().string(),
		frag_source,
		frag_shader_file.stem().string() + frag_shader_file.extension().string()
	);
}