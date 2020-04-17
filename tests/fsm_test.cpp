#define MAIN_EXECUTABLE
#include "fsm.hpp"
#include "lnfa.hpp"

#include "test.hpp"

TEST("[FSM] basic")
{
    ASSERT(fsm::lambda == '\0');
}
