#pragma once

#ifndef GLARSES_OPENGL_FRAMEBUFFER_H
#define GLARSES_OPENGL_FRAMEBUFFER_H

#include "../dependencies.h"

namespace glarses::opengl {
    class Texture;
    class RenderBuffer;

    class FrameBuffer {
    public:
        FrameBuffer() = default; // object is created in a non-initialized state
        virtual ~FrameBuffer();

        // no copy, move transfers ownership
        FrameBuffer             (const FrameBuffer& fb) = delete;
        FrameBuffer& operator = (const FrameBuffer& fb) = delete;
        FrameBuffer             (FrameBuffer&& fb) noexcept;
        FrameBuffer& operator = (FrameBuffer&& fb) noexcept;

        // setting attachments is only valid before calling the 'create()' function
        void set_color_attachment  (const Texture& t, int mip_level = 0, int color_attachment_idx = 0);
        void set_depth_attachment  (const Texture& t, int mip_level = 0);
        void set_stencil_attachment(const Texture& t, int mip_level = 0);

        void set_color_attachment  (const RenderBuffer& rb, int color_attachment_idx = 0);
        void set_depth_attachment  (const RenderBuffer& rb);
        void set_stencil_attachment(const RenderBuffer& rb);

        void create(); // after setting all the attachments, call this to finalize the actual buffer creation
        void destroy();

        [[nodiscard]] GLuint get_handle()   const;
        [[nodiscard]] GLenum check_status() const;
        [[nodiscard]] bool   is_ready()     const;

        static std::string status_to_string(GLenum status);

        void bind(GLenum target);
        void unbind(GLenum target);

        void draw_buffer(int color_attachment_idx = 0);
        void blit_to_screen(
            GLint      src_left,
            GLint      src_top,
            GLint      src_right,
            GLint      src_bottom,
            GLint      dst_left,
            GLint      dst_top,
            GLint      dst_right,
            GLint      dst_bottom,
            GLbitfield mask,
            GLenum     filter
        );
        void blit_to_framebuffer(
            FrameBuffer& fb,
            GLint        src_left,
            GLint        src_top,
            GLint        src_right,
            GLint        src_bottom,
            GLint        dst_left,
            GLint        dst_top,
            GLint        dst_right,
            GLint        dst_bottom,
            GLbitfield   mask,
            GLenum       filter
        );

    protected:
        GLuint m_Handle = 0;
    };
}

#endif
