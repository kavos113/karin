#include "print.h"

#include <cstdint>

#include <string>
#include <print>

void printVariable(slang::VariableReflection *var)
{
    auto name = std::string(var->getName());

    int64_t value;
    SlangResult r = var->getDefaultValueInt(&value);
    if (SLANG_SUCCEEDED(r))
    {
        std::println("{}: {}", name, value);
    }
    else
    {
        std::println("variable: {}", name);
    }
}
