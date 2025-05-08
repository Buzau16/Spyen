#pragma once
#include <memory>
#include <string>
#include <Math/glm.hpp>

namespace Spyen
{
    class Shader
    {
    public:
        Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind();
        void Unbind();

        static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        const std::string& GetName() const { return m_Name; }

        void SetUniform1i(const std::string& name, int value);
        void SetUniform1iv(const std::string& name, int count, const int* value);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform2f(const std::string& name, const glm::vec2& value);
        void SetUniform3f(const std::string& name, const glm::vec3& value);
        void SetUniform4f(const std::string& name, const glm::vec4& value);
        void SetUniformMat4(const std::string& name, const glm::mat4& value);


    private:
        uint32_t CompileShader(uint32_t type, const std::string& filepath);
        std::string ReadFile(const std::string& filepath);

        uint32_t m_RendererID;
        std::string m_Name;
        std::string m_Filepath;
        std::string m_VertexSrc;
        std::string m_FragmentSrc;
    };
}
