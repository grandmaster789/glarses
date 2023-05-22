#ifndef GLARSES_OPENGL_GLTYPES_H
#define GLARSES_OPENGL_GLTYPES_H

#include <iosfwd>
#include <cstdint>

#include "../util/flags.h"

namespace glarses::opengl {
    enum class e_ImageType: uint32_t {
        TEX_1D,
        TEX_2D,
        TEX_3D,
        TEX_1D_ARRAY,
        TEX_2D_ARRAY,
        TEX_CUBEMAP,
        // cubemap array also exists, but it's bad enough to just not support it
        TEX_2D_MULTISAMPLE,
        TEX_2D_MULTISAMPLE_ARRAY
    };

    enum class e_ComponentSwizzle: uint32_t {
        ZERO,
        ONE,
        R,
        G,
        B,
        A
    };

    enum class e_Format: uint32_t {
        UNDEFINED,
        R8_UNORM,
        R8_SNORM,
        R16_UNORM,
        R16_SNORM,
        R8G8_UNORM,
        R8G8_SNORM,
        R16G16_UNORM,
        R16G16_SNORM,
        R3G3B2_UNORM,
        R4G4B4_UNORM,
        R5G5B5_UNORM,
        R8G8B8_UNORM,
        R8G8B8_SNORM,
        R10G10B10_UNORM,
        R12G12B12_UNORM,
        R16G16B16_SNORM,
        R2G2B2A2_UNORM,
        R4G4B4A4_UNORM,
        R5G5B5A1_UNORM,
        R8G8B8A8_UNORM,
        R8G8B8A8_SNORM,
        R10G10B10A2_UNORM,
        R10G10B10A2_UINT,
        R12G12B12A12_UNORM,
        R16G16B16A16_UNORM,
        R16G16B16A16_SNORM,
        R8G8B8_SRGB,
        R8G8B8A8_SRGB,
        R16_FLOAT,
        R16G16_FLOAT,
        R16G16B16_FLOAT,
        R16G16B16A16_FLOAT,
        R32_FLOAT,
        R32G32_FLOAT,
        R32G32B32_FLOAT,
        R32G32B32A32_FLOAT,
        R11G11B10_FLOAT,
        R9G9B9_E5,
        R8_SINT,
        R8_UINT,
        R16_SINT,
        R16_UINT,
        R32_SINT,
        R32_UINT,
        R8G8_SINT,
        R8G8_UINT,
        R16G16_SINT,
        R16G16_UINT,
        R32G32_SINT,
        R32G32_UINT,
        R8G8B8_SINT,
        R8G8B8_UINT,
        R16G16B16_SINT,
        R16G16B16_UINT,
        R32G32B32_SINT,
        R32G32B32_UINT,
        R8G8B8A8_SINT,
        R8G8B8A8_UINT,
        R16G16B16A16_SINT,
        R16G16B16A16_UINT,
        R32G32B32A32_SINT,
        R32G32B32A32_UINT,

        D32_FLOAT,
        D32_UNORM,
        D24_UNORM,
        D16_UNORM,
        D32_FLOAT_S8_UINT,
        D24_UNORM_S8_UINT

        // compressed formats are still missing
        // 64-bit component formats are also missing
    };

    enum class e_SampleCount: uint8_t {
        SAMPLES_1,
        SAMPLES_2,
        SAMPLES_4,
        SAMPLES_8,
        SAMPLES_16
    };

    enum class e_UploadDimension: uint32_t {
        ONE,
        TWO,
        THREE
    };

    enum class e_UploadFormat: uint32_t {
        UNDEFINED,
        R,
        RG,
        RGB,
        BGR,
        RGBA,
        BGRA,
        R_INTEGER,
        RG_INTEGER,
        RGB_INTEGER,
        BGR_INTEGER,
        RGBA_INTEGER,
        BGRA_INTEGER,
        DEPTH_COMPONENT,
        STENCIL_INDEX,
        DEPTH_STENCIL,
    };

    enum class e_UploadType: uint32_t {
        UNDEFINED,
        UBYTE,
        SBYTE,
        USHORT,
        SSHORT,
        UINT,
        SINT,
        FLOAT,
        UBYTE_3_3_2,
        UBYTE_2_3_3_REV,
        USHORT_5_6_5,
        USHORT_5_6_5_REV,
        USHORT_4_4_4_4,
        USHORT_4_4_4_4_REV,
        USHORT_5_5_5_1,
        USHORT_1_5_5_5_REV,
        UINT_8_8_8_8,
        UINT_8_8_8_8_REV,
        UINT_10_10_10_2,
        UINT_2_10_10_10_REV
    };

    enum class e_Filter: uint8_t {
        NONE,
        NEAREST,
        LINEAR
    };

    enum class e_AddressMode: uint8_t {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRROR_CLAMP_TO_EDGE
    };

    enum class e_BorderColor: uint8_t {
        FLOAT_TRANSPARENT_BLACK,
        INT_TRANSPARENT_BLACK,
        FLOAT_OPAQUE_BLACK,
        INT_OPAQUE_BLACK,
        FLOAT_OPAQUE_WHITE,
        INT_OPAQUE_WHITE
    };

    enum class e_AspectMaskBit: uint32_t {
        COLOR_BUFFER_BIT   = 0x01 << 0,
        DEPTH_BUFFER_BIT   = 0x01 << 1,
        STENCIL_BUFFER_BIT = 0x01 << 2
    };
    using AspectMask = util::Flags<e_AspectMaskBit>;

    enum class e_PrimitiveTopology: uint32_t {
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
        PATCH_LIST      // this is only available in pipelines with tesselation shaders
    };

    enum class e_PolygonMode: uint32_t {
        FILL,
        LINE,
        POINT
    };

    enum class e_CullMode: uint32_t {
        NONE = 0,
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    enum class e_FrontFace: uint32_t {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };

    enum class e_CompareOp: uint32_t {
        NEVER,
        LESS,
        EQUAL,
        LESS_OR_EQUAL,
        GREATER,
        NOT_EQUAL,
        GREATER_OR_EQUAL,
        ALWAYS
    };

    enum class e_LogicOp: uint32_t {
        CLEAR,
        SET,
        COPY,
        COPY_INVERTED,
        NO_OP,
        INVERT,
        AND,
        NAND,
        OR,
        NOR,
        XOR,
        EQUIVALENT,
        AND_REVERSE,
        OR_REVERSE,
        AND_INVERTED,
        OR_INVERTED
    };

    enum class e_BlendFactor : uint32_t {
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA,
        SRC_ALPHA_SATURATE,
        SRC1_COLOR,
        ONE_MINUS_SRC1_COLOR,
        SRC1_ALPHA,
        ONE_MINUS_SRC1_ALPHA,
    };

    enum class e_BlendOp : uint32_t {
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX,
    };

    enum class e_ColorComponentFlag : uint32_t {
        NONE      = 0,
        R_BIT     = 0x01 << 0,
        G_BIT     = 0x01 << 1,
        B_BIT     = 0x01 << 2,
        A_BIT     = 0x01 << 3
    };
    using ColorComponentMask = util::Flags<e_ColorComponentFlag>;

    enum class e_IndexType: uint32_t {
        UNSIGNED_BYTE,
        UNSIGNED_SHORT,
        UNSIGNED_INT,
    };

    enum class e_MemoryBarrierBit : uint32_t
    {
        NONE               = 0,
        VERTEX_BUFFER_BIT  = 0x01 << 0,  // GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT
        INDEX_BUFFER_BIT   = 0x01 << 1,  // GL_ELEMENT_ARRAY_BARRIER_BIT
        UNIFORM_BUFFER_BIT = 0x01 << 2,  // GL_UNIFORM_BARRIER_BIT
        TEXTURE_FETCH_BIT  = 0x01 << 3,  // GL_TEXTURE_FETCH_BARRIER_BIT
        IMAGE_ACCESS_BIT   = 0x01 << 4,  // GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
        COMMAND_BUFFER_BIT = 0x01 << 5,  // GL_COMMAND_BARRIER_BIT
        TEXTURE_UPDATE_BIT = 0x01 << 6,  // GL_TEXTURE_UPDATE_BARRIER_BIT
        BUFFER_UPDATE_BIT  = 0x01 << 7,  // GL_BUFFER_UPDATE_BARRIER_BIT
        MAPPED_BUFFER_BIT  = 0x01 << 8,  // GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT
        FRAMEBUFFER_BIT    = 0x01 << 9,  // GL_FRAMEBUFFER_BARRIER_BIT
        SHADER_STORAGE_BIT = 0x01 << 10, // GL_SHADER_STORAGE_BARRIER_BIT
        QUERY_COUNTER_BIT  = 0x01 << 11, // GL_QUERY_BUFFER_BARRIER_BIT
        ALL_BITS           = static_cast<uint32_t>(~0x0)
    };
    using MemoryBarrierMask = util::Flags<e_MemoryBarrierBit>;

    enum class e_StencilOp : uint32_t {
        KEEP                = 0,
        ZERO,
        REPLACE,
        INCREMENT_AND_CLAMP,
        DECREMENT_AND_CLAMP,
        INVERT,
        INCREMENT_AND_WRAP,
        DECREMENT_AND_WRAP,
    };

    std::ostream& operator << (std::ostream& os, e_ImageType               type);
    std::ostream& operator << (std::ostream& os, e_ComponentSwizzle        swizzle);
    std::ostream& operator << (std::ostream& os, e_Format                  fmt);
    std::ostream& operator << (std::ostream& os, e_SampleCount             count);
    std::ostream& operator << (std::ostream& os, e_UploadDimension         dimension);
    std::ostream& operator << (std::ostream& os, e_UploadFormat            fmt);
    std::ostream& operator << (std::ostream& os, e_UploadType              type);
    std::ostream& operator << (std::ostream& os, e_Filter                  filter);
    std::ostream& operator << (std::ostream& os, e_AddressMode             mode);
    std::ostream& operator << (std::ostream& os, e_BorderColor             color);
    std::ostream& operator << (std::ostream& os, e_AspectMaskBit           bit);
    std::ostream& operator << (std::ostream& os, const AspectMask&         mask);
    std::ostream& operator << (std::ostream& os, e_PrimitiveTopology       topology);
    std::ostream& operator << (std::ostream& os, e_PolygonMode             mode);
    std::ostream& operator << (std::ostream& os, e_CullMode                mode);
    std::ostream& operator << (std::ostream& os, e_FrontFace               winding);
    std::ostream& operator << (std::ostream& os, e_CompareOp               operation);
    std::ostream& operator << (std::ostream& os, e_LogicOp                 operation);
    std::ostream& operator << (std::ostream& os, e_BlendFactor             factor);
    std::ostream& operator << (std::ostream& os, e_BlendOp                 operation);
    std::ostream& operator << (std::ostream& os, e_ColorComponentFlag      flag);
    std::ostream& operator << (std::ostream& os, const ColorComponentMask& mask);
    std::ostream& operator << (std::ostream& os, e_IndexType               type);
    std::ostream& operator << (std::ostream& os, e_MemoryBarrierBit        bit);
    std::ostream& operator << (std::ostream& os, MemoryBarrierMask         mask);
    std::ostream& operator << (std::ostream& os, e_StencilOp               operation);
}

#endif
