#include "ObserverManager.hpp"

namespace pxCore
{
	void ObserverManager::RegisterObserver(px::ObserverInterface* observer)
	{
		observers_[observer->Id()].emplace_back(observer);
	}

	void ObserverManager::UnregisterObserver(px::ObserverInterface* observer)
	{
		auto itr = observers_.find(observer->Id());
		if (itr == observers_.end())
			throw px::Exception(px::ErrorID::ObserverInvalid,
				{
					{ px::TagID::ID, px::Str(observer->Id())},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		auto obsItr = std::find_if(itr->second.begin(), itr->second.end(), [&observer](const px::ObserverInterface* o) { return o == observer; });
		if (obsItr == itr->second.end())
			throw px::Exception(px::ErrorID::ObserverNotFound,
				{
					{ px::TagID::ID, px::Str(observer->Id())},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		itr->second.erase(obsItr);

		if (itr->second.empty())
			observers_.erase(itr);
	}

	void ObserverManager::ObserverToBack(px::ObserverInterface* observer)
	{
		// Find the container of sibscribers to the observer to remove event...
		auto itr = observers_.find(observer->Id());
		if (itr == observers_.end())
			throw px::Exception(px::ErrorID::ObserverInvalid,
				{
					{ px::TagID::ID, px::Str(observer->Id())},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		auto obsItr = std::find_if(itr->second.begin(), itr->second.end(), [&observer](const px::ObserverInterface* o) { return o == observer; });
		if (obsItr == itr->second.end())
			throw px::Exception(px::ErrorID::ObserverNotFound,
				{
					{ px::TagID::ID, px::Str(observer->Id())},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		std::rotate(obsItr, obsItr + 1, itr->second.end());
	}

	void ObserverManager::ObserverToFront(px::ObserverInterface* observer)
	{
		// Find the container of sibscribers to the observer to remove event...
		auto itr = observers_.find(observer->Id());
		if (itr == observers_.end())
			throw px::Exception(px::ErrorID::ObserverInvalid,
				{
					{ px::TagID::ID, px::Str(observer->Id())},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		auto obsItr = std::find_if(itr->second.begin(), itr->second.end(), [&observer](const px::ObserverInterface* o) { return o == observer; });
		if (obsItr == itr->second.end())
			throw px::Exception(px::ErrorID::ObserverNotFound,
				{
					{ px::TagID::ID, px::Str(observer->Id())},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});

		std::rotate(itr->second.begin(), obsItr, obsItr + 1);
	}

	std::vector<px::ObserverInterface*> ObserverManager::Subscribers(const px::ObserverInterface::ID& observer)
	{
		auto itr = observers_.find(observer);
		/*if (itr == observers_.end())
			throw px::Exception(px::ErrorID::ObserverInvalid,
				{
					{ px::TagID::ID, px::Str(observer)},
					{ px::TagID::Callee, px::Str(__FUNCTION__)},
				});*/

		return itr == observers_.end() ? std::vector<px::ObserverInterface*>() : itr->second;
	}

}