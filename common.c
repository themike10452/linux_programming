#include <stdlib.h>
#include <stdio.h>
#include "common.h"

void errexit(const char* message)
{
	perror(message);
	exit(1);
}