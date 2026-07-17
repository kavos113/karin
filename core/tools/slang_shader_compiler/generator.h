#ifndef CORE_TOOLS_SLANG_SHADER_COMPILER_GENERATOR_H
#define CORE_TOOLS_SLANG_SHADER_COMPILER_GENERATOR_H

#include <string>
#include <vector>
#include <iostream>

#include "shader_module.h"

class CodeGenerator
{
public:
    explicit CodeGenerator(std::string  outFile);
    ~CodeGenerator();

    // include guard: like "GRAPHICS_VULKAN_SHADERS_SHADER_H"
    void generate(const std::vector<ShaderModule>& modules, const std::string& includeGuard) const;

private:
    static void writeHeader(std::ostream& os, const std::string& includeGuard);
    static void writeFooter(std::ostream& os, const std::string& includeGuard);
    static void writeCode(std::ostream& os, const ShaderModule& module);

    std::string m_outFile;

    static constexpr size_t CODE_WIDTH = 10;
};

class LayoutGenerator
{
public:
    explicit LayoutGenerator(const std::string& outFile);
    ~LayoutGenerator();

    // include guard: like "GRAPHICS_VULKAN_SHADERS_SHADER_H"
    void generate(const std::vector<ShaderModule>& modules, const std::string& includeGuard) const;

private:
    static void writeHeader(std::ostream& os, const std::string& includeGuard);
    static void writeFooter(std::ostream& os, const std::string& includeGuard);
    static void writeLayout(std::ostream& os, const ShaderModule& module);

    std::string m_outFile;
};

#endif //CORE_TOOLS_SLANG_SHADER_COMPILER_GENERATOR_H
