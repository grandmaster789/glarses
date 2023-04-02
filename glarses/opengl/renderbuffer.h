#pragma once

#ifndef GLARSES_OPENGL_RENDERBUFFER_H
#define GLARSES_OPENGL_RENDERBUFFER_H

#include "../dependencies.h"

namespace glarses::opengl {
    class RenderBuffer {
    public:
        RenderBuffer() = default;
        virtual ~RenderBuffer();

        // no copy, move transfers ownership
        RenderBuffer             (const RenderBuffer&) = delete;
        RenderBuffer& operator = (const RenderBuffer&) = delete;
        RenderBuffer             (RenderBuffer&& rb) noexcept;
        RenderBuffer& operator = (RenderBuffer&& rb) noexcept;

        // setting storage is only valid before calling create()
        void set_color_storage  (GLenum format, GLsizei width, GLsizei height);
        void set_depth_storage  (GLsizei width, GLsizei height);
        void set_stencil_storage(GLsizei width, GLsizei height);

        void create(); // should be called after specifying settings
        void destroy();

        [[nodiscard]] GLuint get_handle() const;

        void bind(GLenum target) const;
        void unbind(GLenum target) const;

    protected:
        GLuint m_Handle = 0;
    };
}

#endif