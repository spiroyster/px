#ifndef PX_DATA_HPP
#define PX_DATA_HPP

#include "type.hpp"
#include "Parameter.hpp"

#include <memory>
#include <functional>
#include <list>

namespace px
{
	// Entity gets tracked (added).
	// Has bidirectional connections to any other entity (graph)
	// 
	class Entity
	{
	public:
		Entity(const String& name);
		virtual ~Entity() {}

		const String& Name() const { return name_; }

	private:
		String name_;
		std::list<std::shared_ptr<Entity>> entities_;
	};

	//std::list<const Entity*> DataImutable(std::function<bool(const Entity* entity)> predicate);
	//std::list<Entity*> DataMutable(std::function<bool(const Entity* entity)> predicate); 

	class DataModel
	{
		String name_;
		std::list<std::shared_ptr<Entity>> entities_;
	protected:
		Parameters parameters_;
	public:

		DataModel(const String& name);

		virtual ~DataModel();

		const px::Parameters& Parameters() const { return parameters_; }
		px::Parameters& Parameters() { return parameters_; }

		const String& Name() const { return name_; }

		// Entities
		std::shared_ptr<const Entity> AddEntity(const std::shared_ptr<Entity>& node);
		void RemoveEntity(const String& name);
		std::list<const Entity*> Entities(const std::function<bool(const Entity&)>& predicate = [](const Entity&) { return true; }) const;
		std::list<Entity*> Entities(const std::function<bool(const Entity&)>& predicate = [](const Entity&) { return true; });
		std::shared_ptr<Entity> GetEntity(const String& entityName) const;
		
		template <class T>
		std::list<const T*> Entities(const std::function<bool(const T&)>& predicate = [](const T&) { return true; }) const
		{
			std::list<const T*> result;
			auto entities = Entities([](const Entity& entity) { return dynamic_cast<const T*>(&entity); });
			for (auto itr = entities.begin(); itr != entities.end(); ++itr)
			{
				auto ent = dynamic_cast<const T*>(*itr);
				if (predicate(*ent))
					result.emplace_back(ent);
			}
			return result;
		}

		template <class T>
		std::list<T*> Entities(const std::function<bool(const T&)>& predicate = [](const T&) { return true; })
		{
			std::list<T*> result;
			auto entities = Entities([](const Entity& entity) { return dynamic_cast<const T*>(&entity); });
			for (auto itr = entities.begin(); itr != entities.end(); ++itr)
			{
				auto ent = dynamic_cast<T*>(*itr);
				if (predicate(*ent))
					result.emplace_back(ent);
			}
			return result;
		}

		// Flush...
		void FlushEntities();
		void FlushRelationships();
	};

	// Add a project...
	void AddDataModel(const std::shared_ptr<DataModel>& project);

	// Remove a project...
	void RemoveDataModel(const String& projectName);

	// List Projects
	std::list<String> DataModels();

	class DataModelHandle
	{
		std::shared_ptr<DataModel> handle_;
	public:
		DataModelHandle(const String& projectName);

		~DataModelHandle();

		const DataModel& Immutable() const { return *handle_; }
		DataModel& Mutable() const { return *handle_; }

		template <class T>
		const T* Immutable() const { return dynamic_cast<const T*>(handle_.get()); }

		template <class T>
		T* Mutable() const { return dynamic_cast<T*>(handle_.get()); }
	};
}

#endif // PX_DATA_HPP