#pragma once

#include <functional>


namespace Utils
{

	class DestructCall
	{
	public:

		DestructCall()
			: m_call(false)
		{

		}

		DestructCall(std::function<void()> function)
			: m_func(function)
			, m_call(true)
		{

		}

		~DestructCall()
		{
			tryCall();
		}

		void operator =(std::function<void()> function)
		{
			tryCall();
			m_func = function;
			m_call = true;
		}

		DestructCall(const DestructCall&) = delete;
		DestructCall(DestructCall&&) = delete;

	private:

		void tryCall()
		{
			if (m_call)
			{
				m_func();
				m_call = false;
			}
		}

		std::function<void()> m_func;
		bool m_call;

	};

}
