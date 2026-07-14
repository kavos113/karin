#include <print>
#include <string>
#include <vector>

#include "print.h"
#include "slang_context.h"
#include "shader_module.h"

void usage(char *program)
{
    std::println("Compile shader per file.");
    std::println("Usage: {} [.slang files...]", program);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }

    std::vector<std::string> files(argc - 1);
    for (int i = 1; i < argc; i++)
    {
        files[i - 1] = argv[i];
    }

    SlangContext ctx;
    CompileSession session(ctx);

    for (const auto& file : files)
    {
        ShaderModule module(file);
        module.compile(session);

        auto code = module.spirvCode();
        std::println("compile {}, code size: {} bytes", file, code->getBufferSize());
    }

    return 0;
}