#include "cube.h"
#include "../opengl/vertex.h"
#include "simple_shader_program.h"

namespace {
    // NOTE should really check the packing of this - opengl assumes zero-padding I believe
    static const glarses::opengl::Vertex g_CubeVertices[] = {
            //    x      y      z       r     g     b
            // red square
            {{ -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
            {{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
            {{  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
            {{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
            {{ -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }},
            {{ -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }},

            // green square
            {{ -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
            {{  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
            {{  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
            {{  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
            {{ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},
            {{ -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }},

            // blue square
            {{ -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }},
            {{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }},
            {{ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }},
            {{ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }},
            {{ -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }},
            {{ -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }},

            // yellow square
            {{  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f }},
            {{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f }},
            {{  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f }},
            {{  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f }},
            {{  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f }},
            {{  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f }},

            // purple square
            {{ -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f }},
            {{  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f }},
            {{  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f }},
            {{  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f }},
            {{ -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f }},
            {{ -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f }},

            //
            {{ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f }},
            {{  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f }},
            {{  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f }},
            {{  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f }},
            {{ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f }},
            {{ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f }},
    };
}

namespace glarses::scene {
    void Cube::init() {
        assert(m_Vertices.get_handle() == 0);

        m_Vertices.init();
        m_Vertices.store(std::span<const opengl::Vertex>(g_CubeVertices));

        m_VAO.init();

        m_VAO.attach_buffer(
                m_Vertices,
                SimpleShaderProgram::instance().get_attribute_location("vertex_position"),
                opengl::Vertex::s_PositionAttribute
        );

        m_VAO.attach_buffer(
                m_Vertices,
                SimpleShaderProgram::instance().get_attribute_location("vertex_color"),
                opengl::Vertex::s_ColorAttribute
        );
    }
}