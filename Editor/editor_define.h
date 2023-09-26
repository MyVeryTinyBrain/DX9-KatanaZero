#pragma once

#define INIT_MAXIMIZE
#ifdef INIT_MAXIMIZE
#define INIT_SIZE SW_SHOWMAXIMIZED
#elif
#define INIT_SIZE SW_SHOW
#endif