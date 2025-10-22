#ifndef pxCore_OBSERVERMANAGER_HPP
#define pxCore_OBSERVERMANAGER_HPP

#include <include/px.hpp>

namespace pxCore
{
	class ObserverManager
	{
		std::map<px::ObserverInterface::ID, px::ObserverList> observers_;

	public:

		void RegisterObserver(px::ObserverInterface* observer);
		void UnregisterObserver(px::ObserverInterface* observer);
		void ObserverToBack(px::ObserverInterface* observer);
		void ObserverToFront(px::ObserverInterface* observer);
		std::vector<px::ObserverInterface*> Subscribers(const px::ObserverInterface::ID& eventID);

	};
}

#endif // pxCore_OBSERVERMANAGER_HPP