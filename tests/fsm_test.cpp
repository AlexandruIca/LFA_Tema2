#define MAIN_EXECUTABLE
#include "fsm.hpp"

#include "test.hpp"

TEST("[FSM] basic")
{
    ASSERT(fsm::accepts() == true);
}
