#ifndef KERNEL_H
#define KERNEL_H

#define KB(s) ((s) << 10)
#define MB(s) (KB(s) << 10)

#define STACK_SIZE MB(1)


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define kdebug(str) _kdebug(__FILE__ ":" STR(__LINE__) ": " str)
#define kdebugs(str) (_kdebug(""), _kdebug(s))

void _kdebug(const char* s);
void kdebug_char(char c);
void kdebug_base(uint64_t value, uint32_t base);
void kdebug_nl();

void active_screensaver();

void intson();
void intsoff();

#endif
