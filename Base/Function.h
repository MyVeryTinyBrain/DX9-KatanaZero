#pragma once
#ifndef __FUNCTION_H__
#define __FUNCTION_H__

// Idea from this code...
// This code from https://ospace.tistory.com/224
// ====================================================================================
//struct TestStruct
//{
//	int iValue;
//	TestStruct(int i) :iValue(i) {}
//	int f(int j)
//	{
//		cout << "f(): " << iValue + j << endl;
//		return iValue + j;
//	}
//};
//
//int main()
//{
//	TestStruct ts(99);
//	typedef int (GenericClass::* genericMemFuncType)(int);
//	GenericClass* pThis = reinterpret_cast<GenericClass*>(&ts);
//	genericMemFuncType pFunc = reinterpret_cast<genericMemFuncType>(&TestStruct::f);
//	int result = (pThis->*pFunc)(1);
//	cout << result << endl;
//}
// ====================================================================================

#define PARAMETER_T_MEM_FUNCTION(NAME) RT(T::* NAME)(PT...)
#define PARAMETER_G_FUNCTION(NAME) RT(*NAME)(PT...)
#define RETURN_T_MEM_FUNCTION(NAME) RT(T::* NAME())(PT...)
#define RETURN_G_FUNTION(NAME) RT(*NAME())(PT...)

#include "FunctionConvert.h"

// 이 명령으로 인해서 상속 구조가 존재하는 함수 포인터의 크기가 고정됩니다.
// 32비트 환경: 16
// 64비트 환경: 24
#pragma pointers_to_members(full_generality, virtual_inheritance)

// Class which is use for to call member function
class Caller {};

// Type which storage a function
template <class RT, class ...PT>
class Function
{
	using This = Function<RT, PT...>;
	using mem_func = RT(Caller::*)(PT...);
	using g_func = RT(*)(PT...);
	// Instance of member function
	// is a instance when it's not nullptr
	// is not a instance when it's nullptr
	Caller* m_objVPtr;
	// is a member function when caller is not nullptr
	// is a global function when caller is nullptr
	mem_func m_funcPtr;
public:
	// A constructor for member function
	template<class T>
	Function(void* objVPtr, PARAMETER_T_MEM_FUNCTION(funcPtr)) :
		m_objVPtr(reinterpret_cast<Caller*>(objVPtr)),
		m_funcPtr(reinterpret_cast<mem_func>(funcPtr))
	{}
	// A constructor for global function
	Function(PARAMETER_G_FUNCTION(funcPtr)) :
		m_objVPtr(nullptr),
		m_funcPtr(fPtrConvert<g_func, mem_func>(funcPtr))
	{}
	// Call a stored function
	RT invoke(PT... args) const
	{
		if (m_objVPtr) return (m_objVPtr->*m_funcPtr)(args...);
		else return fPtrConvert<mem_func, g_func>(m_funcPtr)(args...);
	}
	bool equal(const This& other) const
	{
		return m_objVPtr == other.m_objVPtr && m_funcPtr == other.m_funcPtr;
	}
	template<class T>
	bool eqaul(void* objVPtr, PARAMETER_T_MEM_FUNCTION(funcPtr)) const
	{
		return *this == eqaul(This(objVPtr, funcPtr));
	}
	bool operator == (const This& other) const
	{
		return equal(other);
	}
	// Call a stored function
	RT operator ()(PT ...args) const
	{
		return invoke(args...);
	}
	void* getObjectAddress() const
	{
		return m_objVPtr;
	}
	template<class T>
	T getObject() const
	{
		return reinterpret_cast<T>(m_objVPtr);
	}
	void* getFunctionAddress() const
	{
		return fPtrToVPtr(m_funcPtr);
	}
	// Returns casted member function
	// When use it, if stored function isn't a member function, Can happens an error
	template<class T>
	RETURN_T_MEM_FUNCTION(const getAsMemberFunction)
	{
		return reinterpret_cast<RT(T::*)(PT...)>(m_funcPtr);
	}
	// Returns casted global function
	// When use it, if stored function isn't a global function, Can happens an error
	RETURN_G_FUNTION(const getAsGlobalFunction)
	{
		return fPtrConvert<mem_func, g_func>(m_funcPtr);
	}
	bool isMemberFunction() const
	{
		return m_objVPtr != nullptr;
	}
};
#endif
