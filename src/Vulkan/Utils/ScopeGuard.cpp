#include <Vulkan/Utils/ScopeGuard.hpp>


namespace Utils
{

    class ScopeGuard::DestructorCall
    {
    private:
        const std::function<void()> m_function;

    public:
        DestructorCall() = delete;
        DestructorCall(DestructorCall&&) = delete;
        DestructorCall(const DestructorCall&) = delete;

        DestructorCall& operator=(DestructorCall&&) = delete;
        DestructorCall& operator=(const DestructorCall&) = delete;

        DestructorCall(std::function<void()> function)
            : m_function(std::move(function))
        { }

        ~DestructorCall()
        {
            if (m_function)
            {
                m_function();
            }
        }

    };

    ScopeGuard::ScopeGuard(std::function<void()> cleanupFunction)
        : m_cleanupFunction(std::make_shared<DestructorCall>(std::move(cleanupFunction)))
    { }

    ScopeGuard& ScopeGuard::operator=(std::function<void()> cleanupFunction)
    {
        m_cleanupFunction = std::make_shared<DestructorCall>(std::move(cleanupFunction));
        return *this;
    }

}

