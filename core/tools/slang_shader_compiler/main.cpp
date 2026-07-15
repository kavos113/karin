#include <print>
#include <string>
#include <vector>

#include "generator.h"
#include "slang_context.h"
#include "shader_module.h"

void usage(char *program)
{
    std::println("Compile shader per file.");
    std::println("Usage: {} <out file> [.slang files...]", program);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage(argv[0]);
        return 1;
    }

    std::string outFile(argv[1]);

    std::vector<std::string> files(argc - 2);
    for (int i = 2; i < argc; i++)
    {
        files[i - 2] = argv[i];
    }

    SlangContext ctx;
    CompileSession session(ctx);

    std::vector<ShaderModule> modules;

    for (const auto& file : files)
    {
        ShaderModule module(file);
        module.compile(session);

        modules.push_back(std::move(module));
    }

    CodeGenerator gen(outFile);
    gen.generate(modules, "SAMPLE_INCLUDE_GUARD");

    return 0;
}