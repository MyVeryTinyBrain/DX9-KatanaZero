#pragma once
#ifndef __DELEGATE_H__
#define __DELEGATE_H__

#include <vector>
#include <utility>
#include <cassert>
#include "Function.h"

// Type which storage multiple functions
template <class RT, class ...PT>
class Delegate
{
public:
	// Function type which storage in
	using func = Function<RT, PT...>;
private:
	using This = Delegate<RT, PT...>;
	using vector = std::vector<func>;
	using iterator = typename vector::iterator;
	// Container of functions
	vector m_functions;
public:
	Delegate() = default;
	Delegate(const Delegate& other) = default;
	~Delegate() = default;
	Delegate(Delegate&& rvalue)
	{
		m_functions = move(rvalue.m_functions);
	}
	void push_back(const func& f)
	{
		m_functions.push_back(f);
	}
	template<class T>
	void push_back(void* objVPtr, PARAMETER_T_MEM_FUNCTION(funcPtr))
	{
		push_back(func(objVPtr, funcPtr));
	}
	bool erase(const func& f)
	{
		for (iterator it = m_functions.begin(); it != m_functions.end(); ++it)
		{
			if (*it == f)
			{
				m_functions.erase(it);
				return true;
			}
		}
		return false;
	}
	template<class T>
	bool erase(void* objVPtr, PARAMETER_T_MEM_FUNCTION(funcPtr))
	{
		return erase(func(objVPtr, funcPtr));
	}
	bool contains(const func& f) const
	{
		for (iterator it = m_functions.begin(); it != m_functions.end(); ++it)
			if (*it == f)
				return true;
		return false;
	}
	template<class T>
	bool contains(void* objVPtr, PARAMETER_T_MEM_FUNCTION(funcPtr)) const
	{
		return contains(func(objVPtr, funcPtr));
	}
	void clear()
	{
		m_functions.clear();
	}
	const vector& getContainer() const
	{
		return m_functions;
	}
	const func& getFunction(const size_t& index) const
	{
		return m_functions[index];
	}
	iterator findFunction(const func& f) const
	{
		for (iterator it = m_functions.begin(); it != m_functions.end(); ++it)
			if (*it == f)
				return it;
		return m_functions.end();
	}
	const size_t& getSize() const
	{
		return m_functions.size();
	}
	RT invoke(PT ...args)
	{
		// 호출 중 삭제되면 반복자의 무효화 오류를 방지하기 위하여
		// 복사 후 연산합니다.
		vector copied_functions = m_functions;
		size_t size = copied_functions.size();
		if (size == 0) return;
		assert(size > 0);
		for (iterator it = copied_functions.begin(); it != copied_functions.end(); ++it)
		{
			if (size > 1)
				it->invoke(args...);
			else return it->invoke(args...);
			--size;
		}
		assert("invoke error");
		return copied_functions[0](args...);
	}
	RT operator ()(PT ...args)
	{
		return invoke(args...);
	}
	This& operator += (const func& f)
	{
		push_back(f);
		return *this;
	}
	This& operator -= (const func& f)
	{
		erase(f);
		return *this;
	}
	const func& operator [] (const size_t& index) const
	{
		return getFunction(index);
	}
};
#endif
