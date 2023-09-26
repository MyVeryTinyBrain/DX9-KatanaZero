#pragma once

#define PUBLIC		public:
#define PROTECTED	protected:
#define PRIVATE		private:

#define SAFE_DELETE(P)			{if(P) delete P; P = nullptr;}
#define SAFE_DELETE_ARRAY(P)	{if(P) delete[] P; P = nullptr;}
#define SAFE_RELEASE(P)			{if(P) P->Release(); P = NULL;}

#define DECLARE_SINGLETON(CLASSNAME)					\
private: static CLASSNAME g_instance;					\
public: static CLASSNAME* GetInstance();				\
private: CLASSNAME();									\
private: ~CLASSNAME();

#define IMPLEMENT_SINGLETON(CLASSNAME)					\
CLASSNAME CLASSNAME::g_instance;						\
CLASSNAME* CLASSNAME::GetInstance()						\
{														\
	return &g_instance;									\
}

#define IMPLEMENT_SINGLETON_CONSTRUCTOR(CLASSNAME)		\
CLASSNAME::CLASSNAME()

#define IMPLEMENT_SINGLETON_DESTRUCTOR(CLASSNAME)		\
CLASSNAME::~CLASSNAME()

#define MESSAGE_BOX(W_MSG)								\
MessageBox(nullptr, W_MSG, L"Message", MB_OK)

#define ZERO_MEM(INSTANCE)								\
ZeroMemory(&INSTANCE, sizeof(INSTANCE))