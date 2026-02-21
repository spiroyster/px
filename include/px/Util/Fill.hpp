#ifndef PX_UTIL_FILL_HPP
#define PX_UTIL_FILL_HPP

#include <vector>
#include <list>
#include <functional>
#include <optional>

namespace px
{
	template<class T>
	std::vector<T> Fill(unsigned int numberOfItems, std::function<T(unsigned int n)> predicate)
	{
		std::vector<T> result(numberOfItems);
		for (unsigned int n = 0; n < numberOfItems; ++n)
			result[n] = predicate(n);
		return result;
	}

	// Fill vector of one type from another type...
	template<class A, class B>
	std::vector<A> Fill(const std::vector<B>& a, std::function<A(unsigned int n, const B&)> predicate)
	{
		std::vector<A> result(a.size());
		for (unsigned int n = 0; n < a.size(); ++n)
			result[n] = predicate(n, a[n]);
		return result;
	}

	template<class T>
	std::vector<T> Fill(const std::vector<T>& values, std::function<T(const T&)> predicate)
	{
		std::vector<T> result(values.size());
		for (unsigned int n = 0; n < values.size(); ++n)
			result[n] = predicate(values[n]);
		return result;
	}

	template<class T>
	std::vector<T> Fill(const std::vector<T>& values, std::function<T(unsigned int n, const T&)> predicate)
	{
		std::vector<T> result(values.size());
		for (unsigned int n = 0; n < values.size(); ++n)
			result[n] = predicate(n, values[n]);
		return result;
	}

	template<class T>
	std::vector<T> Fill(const std::vector<T>& values, std::function<bool(const T&)> predicate)
	{
		std::list<T> result;
		for (unsigned int n = 0; n < values.size(); ++n)
		{
			if (predicate(values[n]))
				result.emplace_back(values[n]);
		}
		return { result.begin(), result.end() };
	}

	// Fill a vector from a list...
	template<class A, class B>
	std::vector<A> Fill(const std::list<B>& b, std::function<A(const B&)> predicate)
	{
		std::vector<A> result(b.size());
		unsigned int index = 0;
		for (auto itr = b.begin(); b.end(); ++itr, ++index)
			result[index] = predicate(*itr);
		return result;
	}

	// Fill a single vector from a vector of vectors...
	template<class T>
	std::vector<T> Fill(const std::vector<std::vector<T>>& values)
	{
		std::vector<T> result;
		for (auto itr = values.begin(); itr != values.end(); ++itr)
			result.insert(result.end(), itr->begin(), itr->end());
		return result;
	}

	// Fill from indexed...
	template<class T>
	std::vector<T> Fill(const std::vector<T>& v, const std::vector<unsigned int>& indexes)
	{
		std::vector<T> result(indexes.size());
		for (unsigned int n = 0; n < indexes.size(); ++n)
			result[n] = v[indexes[n]];
		return result;
	}

	template<class T>
	std::vector<T> Fill(const std::vector<T>& v, const std::vector<int>& indexes)
	{
		std::vector<T> result(indexes.size());
		for (unsigned int n = 0; n < indexes.size(); ++n)
			result[n] = v[indexes[n]];
		return result;
	}

	// Fill from indexed with predicate...
	template<class T>
	std::vector<T> Fill(const std::vector<T>& v, const std::vector<int>& indexes, std::function<T(const T&)> predicate)
	{
		std::vector<T> result(indexes.size());
		for (unsigned int n = 0; n < indexes.size(); ++n)
			result[n] = predicate(v[indexes[n]]);
		return result;
	}

	// Fill from indexed with predicate N.B index = index of values, not index of indexes...
	template<class T>
	std::vector<T> Fill(const std::vector<T>& v, const std::vector<int>& indexes, std::function<T(unsigned int index, const T&)> predicate)
	{
		std::vector<T> result(indexes.size());
		for (unsigned int n = 0; n < indexes.size(); ++n)
			result[n] = predicate(indexes[n], v[indexes[n]]);
		return result;
	}

	template<class T>
	std::vector<T> Fill(const std::vector<T>& v, const std::vector<int>& indexes, std::function<std::optional<T>(unsigned int index, const T&)> predicate)
	{
		std::list<T> result;
		for (unsigned int n = 0; n < indexes.size(); ++n)
			if (auto value = predicate(indexes[n], v[indexes[n]]))
				result.emplace_back(*value);
		return { result.begin(), result.end() };
	}

	// Fill a vector from a list, optionally 
	template<class A, class B>
	std::vector<A> Fill(const std::list<B>& items, std::function<std::optional<A>(const B&)> predicate)
	{
		std::list<A> result;
		for (auto itr = items.begin(); itr != items.end(); ++itr)
		{
			if (auto item = predicate(*itr))
				result.emplace_back(*item);
		}
		return { result.begin(), result.end() };
	}

	template<class T>
	std::vector<T> FillOptionally(const std::vector<T>& items, std::function<std::optional<T>(unsigned int index, const T&)> predicate)
	{
		std::list<T> result;
		unsigned int index = 0;
		for (auto itr = items.begin(); itr != items.end(); ++itr, ++index)
		{
			if (auto item = predicate(index, *itr))
				result.emplace_back(*item);
		}
		return { result.begin(), result.end() };
	}

	template<class A, class B>
	std::vector<A> Fill(const std::vector<B>& items, std::function<std::optional<A>(const B&)> predicate)
	{
		std::list<A> result;
		for (auto itr = items.begin(); itr != items.end(); ++itr)
		{
			if (auto item = predicate(*itr))
				result.emplace_back(*item);
		}
		return { result.begin(), result.end() };
	}

	template<class A, class B>
	std::vector<A> Fill(const std::vector<B>& items, std::function<std::optional<A>(unsigned int n, const B&)> predicate)
	{
		std::list<A> result;
		for (unsigned int v = 0; v < items.size(); ++v)
		{
			if (auto item = predicate(v, items[v]))
				result.emplace_back(*item);
		}
		return { result.begin(), result.end() };
	}

	template<class A, class B>
	std::list<A> FillList(const std::vector<B>& items, std::function<std::optional<A>(const B&)> predicate)
	{
		std::list<A> result;
		for (auto itr = items.begin(); itr != items.end(); ++itr)
		{
			if (auto item = predicate(*itr))
				result.emplace_back(*item);
		}
		return result;
	}

	template<class A, class B>
	std::list<A> FillList(const std::list<B>& items, std::function<std::optional<A>(const B&)> predicate)
	{
		std::list<A> result;
		for (auto itr = items.begin(); itr != items.end(); ++itr)
		{
			if (auto item = predicate(*itr))
				result.emplace_back(*item);
		}
		return result;
	}

	// Fill repating the value n times sequentially...
	template<class A>
	std::vector<A> FillSequentialRepeat(const std::vector<A>& items, unsigned int sequentialRepitition)
	{
		std::vector<A> result(items.size() * sequentialRepitition);
		for (unsigned int i = 0; i < items.size(); ++i)
			for (unsigned int n = 0; n < sequentialRepitition; ++n)
				result[(i * sequentialRepitition) + n] = items[i];

		return result;
	}

	template<class A>
	std::vector<A> FillRepeat(const std::vector<A>& items, unsigned int repeat)
	{
		std::vector<A> result(items.size() * repeat);
		for (unsigned int n = 0; n < repeat; ++n)
			result.insert(result.end(), items.begin(), items.end());

		return result;
	}
}

#endif // PX_UTIL_FILL_HPP
