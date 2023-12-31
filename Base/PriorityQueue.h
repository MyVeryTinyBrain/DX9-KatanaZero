#pragma once

template <typename T, typename Comparer>
class PriorityQueue : public std::priority_queue<T, std::vector<T>, Comparer>
{
public:
	inline const std::vector<T>& get_vector() const
	{
		char* address = cast<char*>(this);
		std::vector<T>* pointer = cast<std::vector<T>*>(address);
		return *pointer;
	}
	inline Comparer& get_comparer()
	{
		char* address = cast<char*>(this) + sizeof(std::vector<T>);
		Comparer* pointer = cast<Comparer*>(address);
		return *pointer;
	}
	inline void clear()
	{
		PriorityQueue empty;
		std::swap(*this, empty);
	}
};

template <typename T>
class MinHeap : public PriorityQueue<T, std::greater<T>> {};

template <typename T>
class MaxHeap : public PriorityQueue<T, std::less<T>> {};