#ifndef PX_MUTEX_HPP
#define PX_MUTEX_HPP

#include "type.hpp"

namespace px
{
	class Mutex
	{
		String owner_;
	public:
		Mutex(const String& owner);
		~Mutex();
	};
}

#endif // PX_MUTEX_HPP