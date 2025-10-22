#ifndef PX_DATA_HPP
#define PX_DATA_HPP

#include "type.hpp"

#include <memory>
#include <map>
#include <list>

namespace px
{
	class Entity
	{
	public:

		void Relate(const std::shared_ptr<Entity>& relationship);
		void Remove(const std::shared_ptr<Entity>& relationship);
		const std::list<std::shared_ptr<Entity>>& Related() const { return relationships_; }

	private:
		std::list<std::shared_ptr<Entity>> relationships_;
	};

	std::list<const Entity*> DataImutable(std::function<bool(const Entity* entity)> predicate);
	std::list<Entity*> DataMutable(std::function<bool(const Entity* entity)> predicate);

}

#endif // PX_DATA_HPP