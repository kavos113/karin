#include <iostream>
#include <print>
#include <string>
#include <stdexcept>
#include <vector>
#include <ranges>

#include <slang.h>
#include <slang-com-ptr.h>
#include <slang-com-helper.h>

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
    diagnose(diagnostics);
    if (!module)
    {
        throw std::runtime_error("failed to load module");
    }

    std::vector<Slang::ComPtr<slang::IComponentType>> componentsToLink;

    int definedEntryPoint = module->getDefinedEntryPointCount();
    std::println("defined entry point: {}", definedEntryPoint);
    for (int i = 0; i < definedEntryPoint; i++)
    {
        Slang::ComPtr<slang::IEntryPoint> entry;
        SlangResult r = module->getDefinedEntryPoint(i, entry.writeRef());
        if (SLANG_FAILED(r))
        {
            throw std::runtime_error("failed to get entry point");
        }

        std::println("[EntryPoint] {}", entry->getFunctionReflection()->getName());

        componentsToLink.emplace_back(entry.get());
    }

    Slang::ComPtr<slang::IComponentType> composedProgram;
    SlangResult r = session->createCompositeComponentType(
        reinterpret_cast<slang::IComponentType**>(componentsToLink.data()),
        componentsToLink.size(),
        composedProgram.writeRef(),
        diagnostics.writeRef()
    );
    diagnose(diagnostics);
    if (SLANG_FAILED(r))
    {
        throw std::runtime_error("failed to create composite component type");
    }

    Slang::ComPtr<slang::IComponentType> program;
    r = composedProgram->link(program.writeRef(), diagnostics.writeRef());
    diagnose(diagnostics);
    if (SLANG_FAILED(r))
    {
        throw std::runtime_error("failed to link program");
    }

    Slang::ComPtr<slang::IBlob> spirvCode;
    r = program->getEntryPointCode(0, 0, spirvCode.writeRef(), diagnostics.writeRef());
    diagnose(diagnostics);
    if (SLANG_FAILED(r))
    {
        throw std::runtime_error("failed to get spirv code");
    }

    std::println("compiled size: {} bytes", spirvCode->getBufferSize());

    return 0;
}