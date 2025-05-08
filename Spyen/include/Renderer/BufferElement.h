#pragma once
#include "Renderer/ShaderDataType.h"

namespace Spyen
{
    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        uint32_t Offset;
        bool Normalized;

        BufferElement(const ShaderDataType type, const std::string& name, const bool normalized = false)
            : Name(name), Type(type), Size(GetShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {
        }

        uint32_t GetComponentCount() const {
            switch (Type) {
            case ShaderDataType::None:   return 0;
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat4:	 return 4 * 4;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;
            }
            SPY_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    };
}