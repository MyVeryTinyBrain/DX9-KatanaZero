#pragma once

// c++ std

#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <string>
#include <algorithm>
#include <filesystem>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>
#include <chrono>
#include <functional>

using std::string;
using std::wstring;

// default

#include "DebugConsole.h"
#include "DefaultDefine.h"
#include "DefaultFunction.h"

// common
#include "Delegate.h"
#include "PriorityQueue.h"

// windows

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// directX9

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <d3d9.h>
#include <d3dx9.h>

#include "DirectXMathHeader.h"
#include "DirectXDefine.h"
#include "DirectXTypedef.h"

// Core

#include "CoreTypedef.h"
#include "CoreDefine.h"

// Memory leak test

#include <crtdbg.h>
#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif