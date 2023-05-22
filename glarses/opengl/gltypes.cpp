#include "gltypes.h"
#include <ostream>

namespace glarses::opengl {
    std::ostream& operator << (std::ostream& os, e_ImageType type) {
        os << "e_ImageType(";
        switch (type) {
            case e_ImageType::TEX_1D:                   os << "TEX_1D";
            case e_ImageType::TEX_2D:                   os << "TEX_2D";
            case e_ImageType::TEX_3D:                   os << "TEX_3D";
            case e_ImageType::TEX_1D_ARRAY:             os << "TEX_1D_ARRAY";
            case e_ImageType::TEX_2D_ARRAY:             os << "TEX_2D_ARRAY";
            case e_ImageType::TEX_CUBEMAP:              os << "TEX_CUBEMAP";
            case e_ImageType::TEX_2D_MULTISAMPLE:       os << "TEX_2D_MULTISAMPLE";
            case e_ImageType::TEX_2D_MULTISAMPLE_ARRAY: os << "TEX_2D_MULTISAMPLE_ARRAY";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_ComponentSwizzle swizzle) {
        os << "e_ComponentSwizzle(";
        switch (swizzle) {
            case e_ComponentSwizzle::ZERO: os << "ZERO";
            case e_ComponentSwizzle::ONE:  os << "ONE";
            case e_ComponentSwizzle::R:    os << "R";
            case e_ComponentSwizzle::G:    os << "G";
            case e_ComponentSwizzle::B:    os << "B";
            case e_ComponentSwizzle::A:    os << "A";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_Format fmt) {
        os << "e_Format(";
        switch (fmt) {
            case e_Format::UNDEFINED:          os << "UNDEFINED";
            case e_Format::R8_UNORM:           os << "R8_UNORM";
            case e_Format::R8_SNORM:           os << "R8_SNORM";
            case e_Format::R16_UNORM:          os << "R16_UNORM";
            case e_Format::R16_SNORM:          os << "R16_SNORM";
            case e_Format::R8G8_UNORM:         os << "R8G8_UNORM";
            case e_Format::R8G8_SNORM:         os << "R8G8_SNORM";
            case e_Format::R16G16_UNORM:       os << "R16G16_UNORM";
            case e_Format::R16G16_SNORM:       os << "R16G16_SNORM";
            case e_Format::R3G3B2_UNORM:       os << "R3G3B2_UNORM";
            case e_Format::R4G4B4_UNORM:       os << "R4G4B4_UNORM";
            case e_Format::R5G5B5_UNORM:       os << "R5G5B5_UNORM";
            case e_Format::R8G8B8_UNORM:       os << "R8G8B8_UNORM";
            case e_Format::R8G8B8_SNORM:       os << "R8G8B8_SNORM";
            case e_Format::R10G10B10_UNORM:    os << "R10G10B10_UNORM";
            case e_Format::R12G12B12_UNORM:    os << "R12G12B12_UNORM";
            case e_Format::R16G16B16_SNORM:    os << "R16G16B16_SNORM";
            case e_Format::R2G2B2A2_UNORM:     os << "R2G2B2A2_UNORM";
            case e_Format::R4G4B4A4_UNORM:     os << "R4G4B4A4_UNORM";
            case e_Format::R5G5B5A1_UNORM:     os << "R5G5B5A1_UNORM";
            case e_Format::R8G8B8A8_UNORM:     os << "R8G8B8A8_UNORM";
            case e_Format::R8G8B8A8_SNORM:     os << "R8G8B8A8_SNORM";
            case e_Format::R10G10B10A2_UNORM:  os << "R10G10B10A2_UNORM";
            case e_Format::R10G10B10A2_UINT:   os << "R10G10B10A2_UINT";
            case e_Format::R12G12B12A12_UNORM: os << "R12G12B12A12_UNORM";
            case e_Format::R16G16B16A16_UNORM: os << "R16G16B16A16_UNORM";
            case e_Format::R16G16B16A16_SNORM: os << "R16G16B16A16_SNORM";
            case e_Format::R8G8B8_SRGB:        os << "R8G8B8_SRGB";
            case e_Format::R8G8B8A8_SRGB:      os << "R8G8B8A8_SRGB";
            case e_Format::R16_FLOAT:          os << "R16_FLOAT";
            case e_Format::R16G16_FLOAT:       os << "R16G16_FLOAT";
            case e_Format::R16G16B16_FLOAT:    os << "R16G16B16_FLOAT";
            case e_Format::R16G16B16A16_FLOAT: os << "R16G16B16A16_FLOAT";
            case e_Format::R32_FLOAT:          os << "R32_FLOAT";
            case e_Format::R32G32_FLOAT:       os << "R32G32_FLOAT";
            case e_Format::R32G32B32_FLOAT:    os << "R32G32B32_FLOAT";
            case e_Format::R32G32B32A32_FLOAT: os << "R32G32B32A32_FLOAT";
            case e_Format::R11G11B10_FLOAT:    os << "R11G11B10_FLOAT";
            case e_Format::R9G9B9_E5:          os << "R9G9B9_E5";
            case e_Format::R8_SINT:            os << "R8_SINT";
            case e_Format::R8_UINT:            os << "R8_UINT";
            case e_Format::R16_SINT:           os << "R16_SINT";
            case e_Format::R16_UINT:           os << "R16_UINT";
            case e_Format::R32_SINT:           os << "R32_SINT";
            case e_Format::R32_UINT:           os << "R32_UINT";
            case e_Format::R8G8_SINT:          os << "R8G8_SINT";
            case e_Format::R8G8_UINT:          os << "R8G8_UINT";
            case e_Format::R16G16_SINT:        os << "R16G16_SINT";
            case e_Format::R16G16_UINT:        os << "R16G16_UINT";
            case e_Format::R32G32_SINT:        os << "R32G32_SINT";
            case e_Format::R32G32_UINT:        os << "R32G32_UINT";
            case e_Format::R8G8B8_SINT:        os << "R8G8B8_SINT";
            case e_Format::R8G8B8_UINT:        os << "R8G8B8_UINT";
            case e_Format::R16G16B16_SINT:     os << "R16G16B16_SINT";
            case e_Format::R16G16B16_UINT:     os << "R16G16B16_UINT";
            case e_Format::R32G32B32_SINT:     os << "R32G32B32_SINT";
            case e_Format::R32G32B32_UINT:     os << "R32G32B32_UINT";
            case e_Format::R8G8B8A8_SINT:      os << "R8G8B8A8_SINT";
            case e_Format::R8G8B8A8_UINT:      os << "R8G8B8A8_UINT";
            case e_Format::R16G16B16A16_SINT:  os << "R16G16B16A16_SINT";
            case e_Format::R16G16B16A16_UINT:  os << "R16G16B16A16_UINT";
            case e_Format::R32G32B32A32_SINT:  os << "R32G32B32A32_SINT";
            case e_Format::R32G32B32A32_UINT:  os << "R32G32B32A32_UINT";

            case e_Format::D32_FLOAT:          os << "D32_FLOAT";
            case e_Format::D32_UNORM:          os << "D32_UNORM";
            case e_Format::D24_UNORM:          os << "D24_UNORM";
            case e_Format::D16_UNORM:          os << "D16_UNORM";
            case e_Format::D32_FLOAT_S8_UINT:  os << "D32_FLOAT_S8_UINT";
            case e_Format::D24_UNORM_S8_UINT:  os << "D24_UNORM_S8_UINT";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_SampleCount count) {
        os << "e_SampleCount(";
        switch (count) {
            case e_SampleCount::SAMPLES_1: os << "SAMPLES_1";
            case e_SampleCount::SAMPLES_2: os << "SAMPLES_2";
            case e_SampleCount::SAMPLES_4: os << "SAMPLES_4";
            case e_SampleCount::SAMPLES_8: os << "SAMPLES_8";
            case e_SampleCount::SAMPLES_16: os << "SAMPLES_16";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_UploadDimension dimension) {
        os << "e_UploadDimension(";
        switch (dimension) {
            case e_UploadDimension::ONE: os << "ONE";
            case e_UploadDimension::TWO: os << "TWO";
            case e_UploadDimension::THREE: os << "THREE";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_UploadFormat fmt) {
        os << "e_UploadFormat(";
        switch (fmt) {
            case e_UploadFormat::UNDEFINED: os << "UNDEFINED";
            case e_UploadFormat::R:               os << "R";
            case e_UploadFormat::RG:              os << "RG";
            case e_UploadFormat::RGB:             os << "RGB";
            case e_UploadFormat::BGR:             os << "BGR";
            case e_UploadFormat::RGBA:            os << "RGBA";
            case e_UploadFormat::BGRA:            os << "BGRA";
            case e_UploadFormat::R_INTEGER:       os << "R_INTEGER";
            case e_UploadFormat::RG_INTEGER:      os << "RG_INTEGER";
            case e_UploadFormat::RGB_INTEGER:     os << "RGB_INTEGER";
            case e_UploadFormat::BGR_INTEGER:     os << "BGR_INTEGER";
            case e_UploadFormat::RGBA_INTEGER:    os << "RGBA_INTEGER";
            case e_UploadFormat::BGRA_INTEGER:    os << "BGRA_INTEGER";
            case e_UploadFormat::DEPTH_COMPONENT: os << "DEPTH_COMPONENT";
            case e_UploadFormat::STENCIL_INDEX:   os << "STENCIL_INDEX";
            case e_UploadFormat::DEPTH_STENCIL:   os << "DEPTH_STENCIL";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_UploadType type) {
        os << "e_UploadType(";
        switch (type) {
            case e_UploadType::UNDEFINED:           os << "UNDEFINED";
            case e_UploadType::UBYTE:               os << "UBYTE";
            case e_UploadType::SBYTE:               os << "SBYTE";
            case e_UploadType::USHORT:              os << "USHORT";
            case e_UploadType::SSHORT:              os << "SSHORT";
            case e_UploadType::UINT:                os << "UINT";
            case e_UploadType::SINT:                os << "SINT";
            case e_UploadType::FLOAT:               os << "FLOAT";
            case e_UploadType::UBYTE_3_3_2:         os << "UBYTE_3_3_2";
            case e_UploadType::UBYTE_2_3_3_REV:     os << "UBYTE_2_3_3_REV";
            case e_UploadType::USHORT_5_6_5:        os << "USHORT_5_6_5";
            case e_UploadType::USHORT_5_6_5_REV:    os << "USHORT_5_6_5_REV";
            case e_UploadType::USHORT_4_4_4_4:      os << "USHORT_4_4_4_4";
            case e_UploadType::USHORT_4_4_4_4_REV:  os << "USHORT_4_4_4_4_REV";
            case e_UploadType::USHORT_5_5_5_1:      os << "USHORT_5_5_5_1";
            case e_UploadType::USHORT_1_5_5_5_REV:  os << "USHORT_1_5_5_5_REV";
            case e_UploadType::UINT_8_8_8_8:        os << "UINT_8_8_8_8";
            case e_UploadType::UINT_8_8_8_8_REV:    os << "UINT_8_8_8_8_REV";
            case e_UploadType::UINT_10_10_10_2:     os << "UINT_10_10_10_2";
            case e_UploadType::UINT_2_10_10_10_REV: os << "UINT_2_10_10_10_REV";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_Filter filter) {
        os << "e_Filter(";
        switch (filter) {
            case e_Filter::NONE:    os << "NONE";
            case e_Filter::NEAREST: os << "NEAREST";
            case e_Filter::LINEAR:  os << "LINEAR";

        default:
            os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_AddressMode mode) {
        os << "e_AddressMode(";
        switch (mode) {
            case e_AddressMode::REPEAT:               os << "REPEAT";
            case e_AddressMode::MIRRORED_REPEAT:      os << "MIRRORED_REPEAT";
            case e_AddressMode::CLAMP_TO_EDGE:        os << "CLAMP_TO_EDGE";
            case e_AddressMode::CLAMP_TO_BORDER:      os << "CLAMP_TO_BORDER";
            case e_AddressMode::MIRROR_CLAMP_TO_EDGE: os << "MIRROR_CLAMP_TO_EDGE";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_BorderColor color) {
        os << "e_BorderColor(";
        switch (color) {
            case e_BorderColor::FLOAT_TRANSPARENT_BLACK: os << "FLOAT_TRANSPARENT_BLACK";
            case e_BorderColor::INT_TRANSPARENT_BLACK:   os << "INT_TRANSPARENT_BLACK";
            case e_BorderColor::FLOAT_OPAQUE_BLACK:      os << "FLOAT_OPAQUE_BLACK";
            case e_BorderColor::INT_OPAQUE_BLACK:        os << "INT_OPAQUE_BLACK";
            case e_BorderColor::FLOAT_OPAQUE_WHITE:      os << "FLOAT_OPAQUE_WHITE";
            case e_BorderColor::INT_OPAQUE_WHITE:        os << "INT_OPAQUE_WHITE";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_AspectMaskBit bit) {
        os << "e_AspectMaskBit(";
        switch (bit) {
            case e_AspectMaskBit::COLOR_BUFFER_BIT:   os << "COLOR_BUFFER_BIT";
            case e_AspectMaskBit::DEPTH_BUFFER_BIT:   os << "DEPTH_BUFFER_BIT";
            case e_AspectMaskBit::STENCIL_BUFFER_BIT: os << "STENCIL_BUFFER_BIT";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, const AspectMask& mask) {
        os << "AspectMask(";
        if (mask & e_AspectMaskBit::COLOR_BUFFER_BIT)
            os << "COLOR ";
        if (mask & e_AspectMaskBit::DEPTH_BUFFER_BIT)
            os << "DEPTH ";
        if (mask & e_AspectMaskBit::STENCIL_BUFFER_BIT)
            os << "STENCIL ";
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_PrimitiveTopology topology) {
        os << "e_PrimitiveTopology(";
        switch (topology) {
            case e_PrimitiveTopology::POINT_LIST:     os << "POINT_LIST";
            case e_PrimitiveTopology::LINE_LIST:      os << "LINE_LIST";
            case e_PrimitiveTopology::LINE_STRIP:     os << "LINE_STRIP";
            case e_PrimitiveTopology::TRIANGLE_LIST:  os << "TRIANGLE_LIST";
            case e_PrimitiveTopology::TRIANGLE_STRIP: os << "TRIANGLE_STRIP";
            case e_PrimitiveTopology::TRIANGLE_FAN:   os << "TRIANGLE_FAN";
            case e_PrimitiveTopology::PATCH_LIST:     os << "PATCH_LIST";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_PolygonMode mode) {
        os << "e_PolygonMode(";
        switch (mode) {
            case e_PolygonMode::FILL:  os << "FILL";
            case e_PolygonMode::LINE:  os << "LINE";
            case e_PolygonMode::POINT: os << "POINT";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_CullMode mode) {
        os << "e_CullMode(";
        switch (mode) {
            case e_CullMode::NONE:           os << "NONE";
            case e_CullMode::FRONT:          os << "FRONT";
            case e_CullMode::BACK:           os << "BACK";
            case e_CullMode::FRONT_AND_BACK: os << "FRONT_AND_BACK";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_FrontFace winding) {
        os << "e_FrontFace(";
        switch (winding) {
            case e_FrontFace::CLOCKWISE:        os << "CLOCKWISE";
            case e_FrontFace::COUNTERCLOCKWISE: os << "COUNTERCLOCKWISE";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_CompareOp operation) {
        os << "e_CompareOp(";
        switch (operation) {
            case e_CompareOp::NEVER:            os << "NEVER";
            case e_CompareOp::LESS:             os << "LESS";
            case e_CompareOp::EQUAL:            os << "EQUAL";
            case e_CompareOp::LESS_OR_EQUAL:    os << "LESS_OR_EQUAL";
            case e_CompareOp::GREATER:          os << "GREATER";
            case e_CompareOp::NOT_EQUAL:        os << "NOT_EQUAL";
            case e_CompareOp::GREATER_OR_EQUAL: os << "GREATER_OR_EQUAL";
            case e_CompareOp::ALWAYS:           os << "ALWAYS";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_LogicOp operation) {
        os << "e_LogicOp(";
        switch (operation) {
            case e_LogicOp::CLEAR:         os << "CLEAR";
            case e_LogicOp::SET:           os << "SET";
            case e_LogicOp::COPY:          os << "COPY";
            case e_LogicOp::COPY_INVERTED: os << "COPY_INVERTED";
            case e_LogicOp::NO_OP:         os << "NO_OP";
            case e_LogicOp::INVERT:        os << "INVERT";
            case e_LogicOp::AND:           os << "AND";
            case e_LogicOp::NAND:          os << "NAND";
            case e_LogicOp::OR:            os << "OR";
            case e_LogicOp::NOR:           os << "NOR";
            case e_LogicOp::XOR:           os << "XOR";
            case e_LogicOp::EQUIVALENT:    os << "EQUIVALENT";
            case e_LogicOp::AND_REVERSE:   os << "AND_REVERSE";
            case e_LogicOp::OR_REVERSE:    os << "OR_REVERSE";
            case e_LogicOp::AND_INVERTED:  os << "AND_INVERTED";
            case e_LogicOp::OR_INVERTED:   os << "OR_INVERTED";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_BlendFactor actor) {
        os << "e_BlendFactor(";
        switch (actor) {
            case e_BlendFactor::ZERO:                     os << "ZERO";
            case e_BlendFactor::ONE:                      os << "ONE";
            case e_BlendFactor::SRC_COLOR:                os << "SRC_COLOR";
            case e_BlendFactor::ONE_MINUS_SRC_COLOR:      os << "ONE_MINUS_SRC_COLOR";
            case e_BlendFactor::DST_COLOR:                os << "DST_COLOR";
            case e_BlendFactor::ONE_MINUS_DST_COLOR:      os << "ONE_MINUS_DST_COLOR";
            case e_BlendFactor::SRC_ALPHA:                os << "SRC_ALPHA";
            case e_BlendFactor::ONE_MINUS_SRC_ALPHA:      os << "ONE_MINUS_SRC_ALPHA";
            case e_BlendFactor::DST_ALPHA:                os << "DST_ALPHA";
            case e_BlendFactor::ONE_MINUS_DST_ALPHA:      os << "ONE_MINUS_DST_ALPHA";
            case e_BlendFactor::CONSTANT_COLOR:           os << "CONSTANT_COLOR";
            case e_BlendFactor::ONE_MINUS_CONSTANT_COLOR: os << "ONE_MINUS_CONSTANT_COLOR";
            case e_BlendFactor::CONSTANT_ALPHA:           os << "CONSTANT_ALPHA";
            case e_BlendFactor::ONE_MINUS_CONSTANT_ALPHA: os << "ONE_MINUS_CONSTANT_ALPHA";
            case e_BlendFactor::SRC_ALPHA_SATURATE:       os << "SRC_ALPHA_SATURATE";
            case e_BlendFactor::SRC1_COLOR:               os << "SRC1_COLOR";
            case e_BlendFactor::ONE_MINUS_SRC1_COLOR:     os << "ONE_MINUS_SRC1_COLOR";
            case e_BlendFactor::SRC1_ALPHA:               os << "SRC1_ALPHA";
            case e_BlendFactor::ONE_MINUS_SRC1_ALPHA:     os << "ONE_MINUS_SRC1_ALPHA";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_BlendOp operation) {
        os << "e_BlendOp(";
        switch (operation) {
            case e_BlendOp::ADD:              os << "ADD";
            case e_BlendOp::SUBTRACT:         os << "SUBTRACT";
            case e_BlendOp::REVERSE_SUBTRACT: os << "REVERSE_SUBTRACT";
            case e_BlendOp::MIN:              os << "MIN";
            case e_BlendOp::MAX:              os << "MAX";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_ColorComponentFlag flag) {
        os << "e_ColorComponentFlag(";
        switch (flag) {
            case e_ColorComponentFlag::NONE:      os << "NONE";
            case e_ColorComponentFlag::R_BIT:     os << "R_BIT";
            case e_ColorComponentFlag::G_BIT:     os << "G_BIT";
            case e_ColorComponentFlag::B_BIT:     os << "B_BIT";
            case e_ColorComponentFlag::A_BIT:     os << "A_BIT";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, const ColorComponentMask& mask) {
        os << "ColorComponentMask(";
        if (mask & e_ColorComponentFlag::NONE)
            os << "NONE ";
        if (mask & e_ColorComponentFlag::R_BIT)
            os << "R";
        if (mask & e_ColorComponentFlag::G_BIT)
            os << "G";
        if (mask & e_ColorComponentFlag::B_BIT)
            os << "B";
        if (mask & e_ColorComponentFlag::A_BIT)
            os << "A";

        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_IndexType type) {
        os << "e_IndexType(";
        switch (type) {
            case e_IndexType::UNSIGNED_BYTE:  os << "UNSIGNED_BYTE";
            case e_IndexType::UNSIGNED_SHORT: os << "UNSIGNED_SHORT";
            case e_IndexType::UNSIGNED_INT:   os << "UNSIGNED_INT";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_MemoryBarrierBit bit) {
        os << "e_MemoryBarrierBit(";
        switch (bit) {
            case e_MemoryBarrierBit::NONE:               os << "NONE";
            case e_MemoryBarrierBit::VERTEX_BUFFER_BIT:  os << "VERTEX_BUFFER_BIT";
            case e_MemoryBarrierBit::INDEX_BUFFER_BIT:   os << "INDEX_BUFFER_BIT";
            case e_MemoryBarrierBit::UNIFORM_BUFFER_BIT: os << "UNIFORM_BUFFER_BIT";
            case e_MemoryBarrierBit::TEXTURE_FETCH_BIT:  os << "TEXTURE_FETCH_BIT";
            case e_MemoryBarrierBit::IMAGE_ACCESS_BIT:   os << "IMAGE_ACCESS_BIT";
            case e_MemoryBarrierBit::COMMAND_BUFFER_BIT: os << "COMMAND_BUFFER_BIT";
            case e_MemoryBarrierBit::TEXTURE_UPDATE_BIT: os << "TEXTURE_UPDATE_BIT";
            case e_MemoryBarrierBit::BUFFER_UPDATE_BIT:  os << "BUFFER_UPDATE_BIT";
            case e_MemoryBarrierBit::MAPPED_BUFFER_BIT:  os << "MAPPED_BUFFER_BIT";
            case e_MemoryBarrierBit::FRAMEBUFFER_BIT:    os << "FRAMEBUFFER_BIT";
            case e_MemoryBarrierBit::SHADER_STORAGE_BIT: os << "SHADER_STORAGE_BIT";
            case e_MemoryBarrierBit::QUERY_COUNTER_BIT:  os << "QUERY_COUNTER_BIT";
            case e_MemoryBarrierBit::ALL_BITS:           os << "ALL_BITS";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, MemoryBarrierMask mask) {
        os << "MemoryBarrierMask(";
        if (mask & e_MemoryBarrierBit::VERTEX_BUFFER_BIT)
            os << "VertexBuffer ";
        if (mask & e_MemoryBarrierBit::INDEX_BUFFER_BIT)
            os << "IndexBuffer ";
        if (mask & e_MemoryBarrierBit::UNIFORM_BUFFER_BIT)
            os << "UniformBuffer ";
        if (mask & e_MemoryBarrierBit::TEXTURE_FETCH_BIT)
            os << "TextureFetch ";
        if (mask & e_MemoryBarrierBit::IMAGE_ACCESS_BIT)
            os << "ImageAccess ";
        if (mask & e_MemoryBarrierBit::COMMAND_BUFFER_BIT)
            os << "CommandBuffer ";
        if (mask & e_MemoryBarrierBit::TEXTURE_UPDATE_BIT)
            os << "TextureUpdate ";
        if (mask & e_MemoryBarrierBit::BUFFER_UPDATE_BIT)
            os << "BufferUpdate ";
        if (mask & e_MemoryBarrierBit::MAPPED_BUFFER_BIT)
            os << "MappedBuffer ";
        if (mask & e_MemoryBarrierBit::FRAMEBUFFER_BIT)
            os << "FrameBuffer ";
        if (mask & e_MemoryBarrierBit::SHADER_STORAGE_BIT)
            os << "ShaderStorage ";
        if (mask & e_MemoryBarrierBit::QUERY_COUNTER_BIT)
            os << "QueryCounter ";

        os << ')';
        return os;
    }

    std::ostream& operator << (std::ostream& os, e_StencilOp operation) {
        os << "e_StencilOp(";
        switch (operation) {
            case e_StencilOp::KEEP:                os << "KEEP";
            case e_StencilOp::ZERO:                os << "ZERO";
            case e_StencilOp::REPLACE:             os << "REPLACE";
            case e_StencilOp::INCREMENT_AND_CLAMP: os << "INCREMENT_AND_CLAMP";
            case e_StencilOp::DECREMENT_AND_CLAMP: os << "DECREMENT_AND_CLAMP";
            case e_StencilOp::INVERT:              os << "INVERT";
            case e_StencilOp::INCREMENT_AND_WRAP:  os << "INCREMENT_AND_WRAP";
            case e_StencilOp::DECREMENT_AND_WRAP:  os << "DECREMENT_AND_WRAP";

            default:
                os << "UNKNOWN";
        }
        os << ')';
        return os;
    }
}