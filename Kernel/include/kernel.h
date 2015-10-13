#ifndef KERNEL_H
#define KERNEL_H

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define kdebug(str) _kdebug(__FILE__ ":" STR(__LINE__) ": " str)

void _kdebug(const char* s);
void active_screensaver();

#endif
