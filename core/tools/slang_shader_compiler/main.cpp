#include <print>
#include <string>

#include "print.h"
#include "slang_context.h"
#include "module.h"

void usage(char *program)
{
    std::println("Usage: {} <.slang file>", program);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }

    auto fileName = std::string(argv[1]);

    SlangContext ctx;
    CompileSession session(ctx);

    ShaderModule module(fileName);
    module.compile(session);

    auto code = module.spirvCode();
    std::println("code size: {} bytes", code->getBufferSize());

    return 0;
}