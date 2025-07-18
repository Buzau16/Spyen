#include "Renderer/VertexArray.h"

#include "glad/glad.h"

namespace Spyen {
	GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::None:     return GL_NONE;
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}
		SPY_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	VertexArray::VertexArray() {
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr <VertexBuffer>& vb) {
		Bind();
		vb->Bind();
		const auto& layout = vb->GetLayout();
		for (const auto& element : layout) {
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					reinterpret_cast<const void*>(element.Offset));
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat4:
			{
				uint32_t count = element.GetComponentCount();
				for (uint32_t i = 0; i < count; i++) {
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						reinterpret_cast<const void*>(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					reinterpret_cast<const void*>(element.Offset));
				m_VertexBufferIndex++;
				break;
			}
			default:
				break;
			}
		}
		m_VertexBuffers.push_back(vb);
	}
	void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

	uint32_t* VertexArray::GetRendererID()
	{
		return &m_RendererID;
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_shared<VertexArray>();
	}
}
