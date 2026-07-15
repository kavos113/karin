#include <cctype>

#include <print>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>
#include <ranges>
#include <algorithm>

#include "generator.h"
#include "slang_context.h"
#include "shader_module.h"

namespace fs = std::filesystem;

void usage(char *program)
{
    std::println("Compile shader per file.");
    std::println("Usage: {} <code output file> <layout output file> <project root dir> [.slang files...]", program);
}

inline const std::regex pathRe{"[/\\\\.]"};
inline const std::regex stripRe{"^_*(.*)"};

std::string buildIncludeGuard(const std::string& location)
{
    std::string underbar = std::regex_replace(location, pathRe, "_");
    std::string lower = std::regex_replace(underbar, stripRe, "$&");

    return underbar
        | std::views::transform([](unsigned char c){ return std::isalpha(c) ? std::toupper(c) : c; })
        | std::ranges::to<std::string>();
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        usage(argv[0]);
        return 1;
    }

    std::string codeOutFile(argv[1]);
    std::string layoutOutFile(argv[2]);
    std::string projRootDir(argv[3]);

    if (!fs::exists(codeOutFile))
    {
        std::ofstream{codeOutFile}.close();
    }
    if (!fs::exists(layoutOutFile))
    {
        std::ofstream{layoutOutFile}.close();
    }
    if (!fs::exists(projRootDir))
    {
        std::println("file not found: {}", projRootDir);
        usage(argv[0]);
        return 1;
    }

    std::vector<std::string> files(argc - 4);
    for (int i = 4; i < argc; i++)
    {
        files[i - 4] = argv[i];

        if (!fs::exists(files[i - 4]))
        {
            std::println("file not found: {}", files[i - 4]);
            usage(argv[0]);
            return 1;
        }
    }

    fs::path codePath = fs::canonical(codeOutFile);
    fs::path layoutPath = fs::canonical(layoutOutFile);
    fs::path projRootPath = fs::canonical(projRootDir);

    std::string codeLocation = fs::relative(codePath, projRootPath).string();
    std::string layoutLocation = fs::relative(layoutPath, projRootPath).string();

    std::string codeInclude = buildIncludeGuard(codeLocation);
    std::string layoutInclude = buildIncludeGuard(layoutLocation);

    SlangContext ctx;
    CompileSession session(ctx);

    std::vector<ShaderModule> modules;

    for (const auto& file : files)
    {
        ShaderModule module(file);
        module.compile(session);

        modules.push_back(std::move(module));
    }

    CodeGenerator gen(codeOutFile);
    gen.generate(modules, codeInclude);

    return 0;
}