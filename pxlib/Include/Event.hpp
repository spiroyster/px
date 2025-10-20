#ifndef PX_EVENT_HPP
#define PX_EVENT_HPP

#include "type.hpp"
#include "mutex.hpp"

#include <vector>

namespace px
{
	class ObserverInterface
	{
	public:

		typedef std::string ID;

		ObserverInterface(const ID& id);
		
		virtual ~ObserverInterface();
		
		void ToFront();
		void ToBack();
		
		const ID& Id() const { return id_; }

	private:
		ID id_;
	};

	typedef std::vector<ObserverInterface*> ObserverList;

	ObserverList Observers(const ObserverInterface::ID& eventID);

}

#define PX_EVENT_DECLARE_METHOD(x) x ## Event
#define PX_EVENT_DECLARE_NAME(x) x

#define PX_EVENT(name)\
namespace px\
{\
	namespace Observer\
	{\
		class PX_EVENT_DECLARE_NAME(name) : public ObserverInterface\
		{\
			std::function<void()> callback_;\
		public:\
			PX_EVENT_DECLARE_NAME(name)(std::function<void()> callback = 0) : ObserverInterface(#name), callback_(callback) {}\
			virtual void PX_EVENT_DECLARE_METHOD(name)() { callback_(); }\
		};\
	}\
	namespace Dispatch\
	{\
		static void PX_EVENT_DECLARE_NAME(name)(std::function<ObserverList(const ObserverList&)> predicate = nullptr)\
		{\
			Mutex mut(Str("Dispatch: ") + Str(#name));\
			auto observers = predicate ? predicate(Observers(#name)) : Observers(#name);\
			for (auto o = observers.begin(); o != observers.end(); ++o) { dynamic_cast<Observer::PX_EVENT_DECLARE_NAME(name)*>(*o)->PX_EVENT_DECLARE_METHOD(name)(); }\
		}\
	}\
}

#define PX_EVENT_1(name, argt1, argv1)\
namespace px \
{\
	namespace Observer\
	{\
		class PX_EVENT_DECLARE_NAME(name) : public ObserverInterface\
		{\
			std::function<void(argt1)> callback_;\
		public:\
			PX_EVENT_DECLARE_NAME(name)(std::function<void(argt1)> callback = 0) : ObserverInterface(#name), callback_(callback) {}\
			virtual void PX_EVENT_DECLARE_METHOD(name)(argt1 argv1) { callback_(argv1); }\
		};\
	}\
	namespace Dispatch\
	{\
		static void PX_EVENT_DECLARE_NAME(name)(argt1 argv1, std::function<ObserverList(const ObserverList&)> predicate = nullptr)\
		{\
			Mutex mut(Str("Dispatch: ") + Str(#name));\
			auto observers = predicate ? predicate(Observers(#name)) : Observers(#name);\
			for (auto o = observers.begin(); o != observers.end(); ++o) { dynamic_cast<Observer::PX_EVENT_DECLARE_NAME(name)*>(*o)->PX_EVENT_DECLARE_METHOD(name)(argv1); }\
		}\
	}\
}

#define PX_EVENT_2(name, argt1, argv1, argt2, argv2)\
namespace px \
{\
namespace Observer\
	{\
	class PX_EVENT_DECLARE_NAME(name) : public ObserverInterface\
	{\
		std::function<void(argt1, argt2)> callback_; \
	public:\
		PX_EVENT_DECLARE_NAME(name)(std::function<void(argt1, argt2)> callback = 0) : ObserverInterface(#name), callback_(callback) {}\
		virtual void PX_EVENT_DECLARE_METHOD(name)(argt1 argv1, argt2 argv2) { callback_(argv1, argv2); }\
	}; \
}\
namespace Dispatch\
{\
	static void PX_EVENT_DECLARE_NAME(name)(argt1 argv1, argt2 argv2, std::function<ObserverList(const ObserverList&)> predicate = nullptr)\
	{\
		Mutex mut(Str("Dispatch: ") + Str(#name)); \
		auto observers = predicate ? predicate(Observers(#name)) : Observers(#name); \
		for (auto o = observers.begin(); o != observers.end(); ++o) { dynamic_cast<Observer::PX_EVENT_DECLARE_NAME(name)*>(*o)->PX_EVENT_DECLARE_METHOD(name)(argv1, argv2); }\
	}\
}\
}

#define PX_EVENT_3(name, argt1, argv1, argt2, argv2, argt3, argv3)\
namespace px \
{\
namespace Observer\
	{\
	class PX_EVENT_DECLARE_NAME(name) : public ObserverInterface\
	{\
		std::function<void(argt1, argt2, argt3)> callback_; \
	public:\
		PX_EVENT_DECLARE_NAME(name)(std::function<void(argt1, argt2, argt3)> callback = 0) : ObserverInterface(#name), callback_(callback) {}\
		virtual void PX_EVENT_DECLARE_METHOD(name)(argt1 argv1, argt2 argv2, argt3 argv3) { callback_(argv1, argv2, argv3); }\
	}; \
}\
namespace Dispatch\
{\
	static void PX_EVENT_DECLARE_NAME(name)(argt1 argv1, argt2 argv2, argt3 argv3, std::function<ObserverList(const ObserverList&)> predicate = nullptr)\
	{\
		Mutex mut(Str("Dispatch: ") + Str(#name)); \
		auto observers = predicate ? predicate(Observers(#name)) : Observers(#name); \
		for (auto o = observers.begin(); o != observers.end(); ++o) { dynamic_cast<Observer::PX_EVENT_DECLARE_NAME(name)*>(*o)->PX_EVENT_DECLARE_METHOD(name)(argv1, argv2, argv3); }\
	}\
}\
}

//
//#define PX_EVENT_3(name, argt1, argv1, argt2, argv2, argt3, argv3)\
//namespace EventObserver\
//{\
//	class PX_EVENT_DECLARE_NAME(name) : public AxW::System::ObserverInterface\
//	{\
//		std::function<void(argt1, argt2, argt3)> callback_;\
//	public:\
//		PX_EVENT_DECLARE_NAME(name)(std::function<void(argt1, argt2, argt3)> callback = 0) : AxW::System::ObserverInterface(#name), callback_(callback) {}\
//		virtual void PX_EVENT_DECLARE_METHOD(name)(argt1 argv1, argt2 argv2, argt3 argv3) { callback_(argv1, argv2, argv3); }\
//	};\
//}\
//namespace EventDispatch\
//{\
//	static void PX_EVENT_DECLARE_NAME(name)(argt1 argv1, argt2 argv2, argt3 argv3, std::function<AxW::System::ObserverList(const AxW::System::ObserverList&)> predicate = nullptr)\
//	{\
//		AxW::System::Mutex mut(AxW::Str("Dispatch: ") + AxW::Str(#name));\
//		auto observers = predicate ? predicate(AxW::System::Observers(#name)) : AxW::System::Observers(#name);\
//		for (auto o = observers.begin(); o != observers.end(); ++o) { dynamic_cast<EventObserver::PX_EVENT_DECLARE_NAME(name)*>(*o)->PX_EVENT_DECLARE_METHOD(name)(argv1, argv2, argv3); }\
//	}\
//}
//
//#define PX_EVENT_4(name, argt1, argv1, argt2, argv2, argt3, argv3, argt4, argv4)\
//namespace EventObserver\
//{\
//	class PX_EVENT_DECLARE_NAME(name) : public AxW::System::ObserverInterface\
//	{\
//		std::function<void(argt1, argt2, argt3, argt4)> callback_;\
//	public:\
//		PX_EVENT_DECLARE_NAME(name)(std::function<void(argt1, argt2, argt3, argt4)> callback = 0) : AxW::System::ObserverInterface(#name), callback_(callback) {}\
//		virtual void PX_EVENT_DECLARE_METHOD(name)(argt1 argv1, argt2 argv2, argt3 argv3, argt4 argv4) { callback_(argv1, argv2, argv3, argv4); }\
//	};\
//}\
//namespace EventDispatch\
//{\
//	static void PX_EVENT_DECLARE_NAME(name)(argt1 argv1, argt2 argv2, argt3 argv3, argt4 argv4, std::function<AxW::System::ObserverList(const AxW::System::ObserverList&)> predicate = nullptr)\
//	{\
//		AxW::System::Mutex mut(AxW::Str("Dispatch: ") + AxW::Str(#name));\
//		auto observers = predicate ? predicate(AxW::System::Observers(#name)) : AxW::System::Observers(#name);\
//		for (auto o = observers.begin(); o != observers.end(); ++o) { dynamic_cast<EventObserver::PX_EVENT_DECLARE_NAME(name)*>(*o)->PX_EVENT_DECLARE_METHOD(name)(argv1, argv2, argv3, argv4); }\
//	}\
//}


#endif // PX_EVENT_HPP