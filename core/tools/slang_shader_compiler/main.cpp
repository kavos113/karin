#include <iostream>
#include <print>
#include <string>
#include <stdexcept>

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>
#include <slang/slang-com-helper.h>

#include "print.h"

void usage(char *program)
{
    std::println("Usage: {} <.slang file>", program);
}

void diagnose(slang::IBlob *diagnostics)
{
    if (diagnostics != nullptr)
    {
        std::println("{}", static_cast<const char *>(diagnostics->getBufferPointer()));
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage(argv[0]);
        return 1;
    }

    auto fileName = std::string(argv[1]);

    Slang::ComPtr<slang::IGlobalSession> globalSession;
    slang::createGlobalSession(globalSession.writeRef());

    slang::SessionDesc sessionDesc = {};
    slang::TargetDesc targetDesc = {
        .format = SLANG_SPIRV,
        .profile = globalSession->findProfile("spirv_1_5")
    };
    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;

    Slang::ComPtr<slang::ISession> session;
    globalSession->createSession(sessionDesc, session.writeRef());

    Slang::ComPtr<slang::IBlob> diagnostics;
    Slang::ComPtr<slang::IModule> module;
    module = session->loadModule(fileName.c_str(), diagnostics.writeRef());
    if (!module)
    {
        throw std::runtime_error("failed to load module");
    }

    for (auto decl : module->getModuleReflection()->getChildren())
    {
        slang::VariableReflection *varDecl = decl->asVariable();

        if (varDecl && varDecl->findModifier(slang::Modifier::Const) && varDecl->findModifier(slang::Modifier::Static))
        {
            printVariable(varDecl);
        }
    }

    return 0;
}