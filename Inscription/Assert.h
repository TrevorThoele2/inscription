#pragma once

#ifdef INSCRIPTION_DEBUG
#define INSCRIPTION_ASSERT(x) if(!(x)) abort();
#define INSCRIPTION_ASSERT_MESSAGE(x, message) if(!(x)) abort();
#else
#define INSCRIPTION_ASSERT(x) (void)0;
#define INSCRIPTION_ASSERT_MESSAGE(x, message) (void)0;
#endif