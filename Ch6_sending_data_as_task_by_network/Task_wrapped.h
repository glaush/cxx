#pragma once
#include <boost/thread/thread.hpp>
#include <iostream>

namespace detail
{
	template <class T>
	struct task_wrapped;

	template <class T>
	task_wrapped<T> make_task_wrapped(const T& task_unwrapped)
	{
		return task_wrapped<T>(task_unwrapped);
	}

	template <class T>
	struct task_wrapped
	{
	private:
		T task_unwrapped_;
	public:
		explicit task_wrapped(const T& f)
			: task_unwrapped_(f)
		{

		}

		void operator() () const
		{
			try
			{
				boost::this_thread::interruption_point();
			}
			catch (const boost::thread_interrupted&)
			{

			}

			try
			{
				task_unwrapped_();
			}
			catch (const std::exception& e)
			{
				std::cerr << "Exception: " << e.what() << "\n";
			}
			catch (const boost::thread_interrupted&)
			{
				std::cerr << "Thread interrupted\n";
			}
			catch (...)
			{
				std::cerr << "Unknown exception\n";
			}
		}
	};
}