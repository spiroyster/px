#ifndef PX_ERROR_HPP
#define PX_ERROR_HPP

#include "parameter.hpp"

namespace px
{

	class Exception
	{
	public:
		using ID = String;

		Exception();
		Exception(const ID& id, const Parameters& meta = {});
		Exception(const std::exception& e, const Parameters& meta = {});
			
		const ID& Id() const { return id_; }
		const Parameters& Meta() const { return meta_; }

	private:
		ID id_;
		Parameters meta_;
	};
}

#define PX_ERROR(err) \
namespace px { namespace ErrorID { static const Exception::ID err = PX_STRING(#err); } }

#endif // PX_ERROR_HPP