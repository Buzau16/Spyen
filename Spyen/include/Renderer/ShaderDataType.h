#pragma once
#include "Core/LogMacros.h"

namespace Spyen
{
    enum class ShaderDataType {
        None = 0, Float, Float2, Float3, Float4, Mat4, Int, Int2, Int3, Int4, Bool
    };

    static uint32_t GetShaderDataTypeSize(const ShaderDataType type) {
        switch (type)
        {
        case ShaderDataType::None:		return 0;
        case ShaderDataType::Float:		return 4;
        case ShaderDataType::Float2:	return 4 * 2;
        case ShaderDataType::Float3:	return 4 * 3;
        case ShaderDataType::Float4:	return 4 * 4;
        case ShaderDataType::Mat4:		return 4 * 4 * 4;
        case ShaderDataType::Int:		return 4;
        case ShaderDataType::Int2:		return 4 * 2;
        case ShaderDataType::Int3:		return 4 * 3;
        case ShaderDataType::Int4:		return 4 * 4;
        case ShaderDataType::Bool:		return 1;
        }
        SPY_CORE_ASSERT(false, "Unknown ShaderDataType");
        return -1;
    }
}