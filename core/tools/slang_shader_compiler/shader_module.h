#ifndef CORE_TOOLS_SLANG_SHADER_COMPILER_MODULE_H
#define CORE_TOOLS_SLANG_SHADER_COMPILER_MODULE_H

#include <cctype>

#include <string>
#include <stdexcept>
#include <utility>
#include <vector>
#include <sstream>
#include <filesystem>
#include <ranges>
#include <algorithm>

#include <slang.h>
#include <slang-com-ptr.h>

#include "slang_context.h"
#include "util.h"

class ShaderModule
{
public:
    explicit ShaderModule(std::string filePath)
        : m_filePath(std::move(filePath))
    {}

    ~ShaderModule() = default;

    void compile(const CompileSession& session)
    {
        Slang::ComPtr<slang::ISession> sess = session.session();

        Slang::ComPtr<slang::IBlob> diagnosticBlob;

        Slang::ComPtr<slang::IModule> module;
        module = sess->loadModule(m_filePath.string().c_str(), diagnosticBlob.writeRef());
        diagnose(diagnosticBlob);
        if (!module)
        {
            throw std::runtime_error("failed to load module");
        }

        std::vector<Slang::ComPtr<slang::IComponentType>> components;

        int numEntryPoints = module->getDefinedEntryPointCount();
        for (int i = 0; i < numEntryPoints; i++)
        {
            Slang::ComPtr<slang::IEntryPoint> entry;
            SlangResult r = module->getDefinedEntryPoint(i, entry.writeRef());
            if (SLANG_FAILED(r))
            {
                throw std::runtime_error("failed to get entry point");
            }

            if (i == 0)
            {
                m_entryPoint = entry->getFunctionReflection()->getName();
            }

            components.emplace_back(entry.get());
        }

        Slang::ComPtr<slang::IComponentType> composedProgram;
        SlangResult r = sess->createCompositeComponentType(
            reinterpret_cast<slang::IComponentType**>(components.data()),
            components.size(),
            composedProgram.writeRef(),
            diagnosticBlob.writeRef()
        );
        checkResult(r, diagnosticBlob, "failed to composite program");

        r = composedProgram->link(m_program.writeRef(), diagnosticBlob.writeRef());
        checkResult(r, diagnosticBlob, "failed to link program");

        // expected each module has 1 entry point
        r = m_program->getEntryPointCode(0, 0, m_spirvCode.writeRef(), diagnosticBlob.writeRef());
        checkResult(r, diagnosticBlob, "failed to get spirv code");
    }

    Slang::ComPtr<slang::IBlob> spirvCode() const
    {
        return m_spirvCode;
    }

    std::string identifier() const
    {
        std::stringstream ss;

        std::string path = m_filePath.stem().string()
            | std::views::transform([](unsigned char c) { return std::isalnum(c) ? std::tolower(c) : '_'; })
            | std::ranges::to<std::string>();
        std::string entry = m_entryPoint
            | std::views::transform([](unsigned char c) { return std::tolower(c); })
            | std::ranges::to<std::string>();

        ss << path << "_" << entry;

        return ss.str();
    }

private:
    std::filesystem::path m_filePath;
    std::string m_entryPoint;

    Slang::ComPtr<slang::IBlob> m_spirvCode = nullptr;
    Slang::ComPtr<slang::IComponentType> m_program = nullptr;
};

#endif //CORE_TOOLS_SLANG_SHADER_COMPILER_MODULE_H
