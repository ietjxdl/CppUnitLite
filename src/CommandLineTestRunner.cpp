//
// Copyright (c) 2004 Michael Feathers and James Grenning
// Released under the terms of the GNU General Public License version 2 or later.
//


#include "CommandLineTestRunner.h"
#include "TestResult.h"
#include "TestRegistry.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>

#ifdef ENABLE_MEMORYLEAKWARN
#include "MemoryLeakWarning.h"
#endif

int CommandLineTestRunner::repeat = 0;

void CommandLineTestRunner::RunAllTests(int ac, char** av)
{
#ifdef ENABLE_MEMORYLEAKWARN
	MemoryLeakWarning::Enable();
#endif	

	SetOptions(ac, av);
	
	do
	{
		TestResult tr;
		TestRegistry::runAllTests(tr);
	} while (--repeat);
	
#ifdef ENABLE_MEMORYLEAKWARN
	MemoryLeakWarning::Report();
#endif
}

void CommandLineTestRunner::SetOptions(int ac, char** av)
{
	repeat = 1;
	for (int i = 1; i < ac; i++)
	{
		if (0 == strcmp("-v", av[i]))
			TestRegistry::verbose();
		else if (av[i] == strstr(av[i], "-r"))
			SetRepeatCount(ac, av, i);
		else
		{
			SimpleString f(av[i]);
			TestRegistry::filter(f);
		}
	}
}


void CommandLineTestRunner::SetRepeatCount(int ac, char** av, int& i)
{
	repeat = 0;
	
	if (strlen(av[i]) > 2)
		repeat = atoi(av[i] + 2);
	else if (i + 1 < ac)
	{
		repeat = atoi(av[i+1]);
		if (repeat != 0)
			i++;
	}
	
	if (0 == repeat)
		repeat = 2;
	
}

