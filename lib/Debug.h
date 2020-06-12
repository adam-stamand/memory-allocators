#pragma once

#include <cassert>
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"


#define ASSERT1(cond) assert(cond)
#define ASSERT2(cond, message) \
    if (!(cond)) { \
        SPDLOG_CRITICAL(message); \
        assert(cond);}
    

#define GET_ASSERT_MACRO(_1,_2,NAME,...) NAME
#define ASSERT(...) GET_ASSERT_MACRO(__VA_ARGS__, ASSERT2, ASSERT1)(__VA_ARGS__)

