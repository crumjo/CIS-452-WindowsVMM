// WindowsVMM.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>


int humanReadableState(char *state, DWORD val)
{
	if (val == 0x1000)
	{
		strncpy(state, "COMMIT", 7);
		return 0;
	}

	else if (val == 0x10000)
	{
		strncpy(state, "FREE", 5);
		return 0;
	}

	else if (val == 0x2000)
	{
		strncpy(state, "RESERVE", 7);
		return 0;
	}

	return 1;
}


int main(int argc, char **argv)
{
	SYSTEM_INFO sys_inf;
	SYSTEM_INFO *sys_p = &sys_inf;
	MEMORY_BASIC_INFORMATION mem_inf;
	MEMORY_BASIC_INFORMATION *mem_p = &mem_inf;
	GetSystemInfo(sys_p);

	/* Size of a page. */
	printf("Page size: %ld\n", sys_inf.dwPageSize);

	int size = 1048576;
	char *mem_chunk = (char *)malloc(size);
	printf("Memory of size %d bytes allocated.\n", size);

	SIZE_T ret = VirtualQuery(mem_chunk, mem_p, size);
	if (ret == 0)
	{
		fprintf(stderr, "Virtual query failed.\n");
		exit(1);
	}

	char *state = (char *)malloc(8 * sizeof(char));
	int error = humanReadableState(state, mem_inf.State);
	if (error == 1)
	{
		fprintf(stderr, "Conversion failed.\n");
		exit(1);
	}

	printf("State before free: %s (0x%x)\n", state, mem_inf.State);

	free(mem_chunk);

	ret = VirtualQuery(mem_chunk, mem_p, size);
	if (ret == 0)
	{
		fprintf(stderr, "Virtual query failed.\n");
		exit(1);
	}

	error = humanReadableState(state, mem_inf.State);
	if (error == 1)
	{
		fprintf(stderr, "Conversion failed.\n");
		exit(1);
	}

	printf("State after free: %s (0x%x)\n", state, mem_inf.State);

	free(state);
    return 0;
}

