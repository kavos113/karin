#ifndef CORE_TOOLS_SLANG_SHADER_COMPILER_SLANG_CONTEXT_H
#define CORE_TOOLS_SLANG_SHADER_COMPILER_SLANG_CONTEXT_H

#include <stdexcept>

#include <slang.h>
#include <slang-com-ptr.h>

class SlangContext
{
public:
    SlangContext()
    {
        SlangResult r = slang::createGlobalSession(m_globalSession.writeRef());
        if (SLANG_FAILED(r))
        {
            throw std::runtime_error("failed to create global session");
        }
    }

    ~SlangContext() = default;

    Slang::ComPtr<slang::IGlobalSession> globalSession() const
    {
        return m_globalSession;
    }

private:
    Slang::ComPtr<slang::IGlobalSession> m_globalSession;
};

class CompileSession
{
public:
    CompileSession(const SlangContext& ctx)
    {
        auto globalSession = ctx.globalSession();

        slang::TargetDesc targetDesc = {
            .format = SLANG_SPIRV,
            .profile = globalSession->findProfile("spirv_1_5")
        };
        slang::SessionDesc sessionDesc = {
            .targets = &targetDesc,
            .targetCount = 1,
        };

        globalSession->createSession(sessionDesc, m_session.writeRef());
    }

    ~CompileSession() = default;

    Slang::ComPtr<slang::ISession> session() const
    {
        return m_session;
    }

private:
    Slang::ComPtr<slang::ISession> m_session;
};

#endif //CORE_TOOLS_SLANG_SHADER_COMPILER_SLANG_CONTEXT_H
