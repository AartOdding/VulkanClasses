#pragma once

#include <memory>
#include <functional>


namespace Utils
{

    class ScopeGuard
    {
    public:

        ScopeGuard() = default;
        ScopeGuard(ScopeGuard&&) = default;
        ScopeGuard(const ScopeGuard&) = default;

        ScopeGuard& operator=(ScopeGuard&&) = default;
        ScopeGuard& operator=(const ScopeGuard&) = default;

        ScopeGuard(std::function<void()> cleanupFunction);
        ScopeGuard& operator=(std::function<void()> cleanupFunction);

    private:

        class DestructorCall;
        std::shared_ptr<DestructorCall> m_cleanupFunction;

    };
}
