#ifndef CORE_TOOLS_SLANG_SHADER_COMPILER_UTIL_H
#define CORE_TOOLS_SLANG_SHADER_COMPILER_UTIL_H

#include <print>
#include <stdexcept>
#include <string>

#include <slang.h>
#include <slang-com-ptr.h>

inline void diagnose(const Slang::ComPtr<slang::IBlob>& diagnosticBlob)
{
    if (diagnosticBlob != nullptr)
    {
        std::println("{}", static_cast<const char*>(diagnosticBlob->getBufferPointer()));
    }
}

inline void checkResult(SlangResult result, const Slang::ComPtr<slang::IBlob>& diagnosticBlob, const std::string& errMessage)
{
    diagnose(diagnosticBlob);
    if (SLANG_FAILED(result))
    {
        throw std::runtime_error(errMessage);
    }
}

#endif //CORE_TOOLS_SLANG_SHADER_COMPILER_UTIL_H
