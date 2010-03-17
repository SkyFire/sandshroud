/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef _FEATHERMOONEMU_GETOPT_H
#define _FEATHERMOONEMU_GETOPT_H

/* getopt() wrapper */
#define feathermoonemu_no_argument            0
#define feathermoonemu_required_argument      1
#define feathermoonemu_optional_argument      2
struct feathermoonemu_option
{
	const char *name;
	int has_arg;
	int *flag;
	int val;
};
extern char feathermoonemu_optarg[514];
int feathermoonemu_getopt_long_only (int ___argc, char *const *___argv, const char *__shortopts, const struct feathermoonemu_option *__longopts, int *__longind);

#endif
