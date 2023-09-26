#pragma once
#ifndef __FUNCTION_CONVERT_H__
#define __FUNCTION_CONVERT_H__

// Get memory address of member function?
// https://stackoverflow.com/questions/8121320/get-memory-address-of-member-function

// Convert function pointer to void pointer
template <class FX>
inline void* fPtrToVPtr(FX fPtr)
{
	// Example
	// void* vPtr = (void*&)fPtr;
	void* vPtr = (void*&)fPtr;
	return vPtr;
}

// Convert void pointer to function pointer
template <class FX>
inline FX vPtrTofPtr(void* vPtr)
{
	// Example
	// void(CLASS:: * fPtr)() = (void(CLASS::*&)())vPtr;
	FX fPtr = (FX&)vPtr;
	return fPtr;
}

// Convert input function pointer to out function pointer
template <class IN_FX, class OUT_FX>
inline OUT_FX fPtrConvert(IN_FX fPtr)
{
	void* vPtr = fPtrToVPtr(fPtr);
	return vPtrTofPtr<OUT_FX>(vPtr);
}
#endif
