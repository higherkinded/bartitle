#ifndef _BARTITLE_MACROS_H_
#define _BARTITLE_MACROS_H_

#define guard(varname, call) \
	varname = call; \
	if (!varname)

#define set(target, flag) target = target | (1 << flag)
#define unset(target, flag) target = target & ~(1 << flag)
#define getflag(source, flag) ((source >> flag) & 1)

#define ifeq(str1, str2) if (!strcmp(str1, str2))
#define ifset(source, flag) if (getflag(source, flag))
#define ifunset(source, flag) if (!getflag(source, flag))

#endif /* _BARTITLE_MACROS_H_ */
