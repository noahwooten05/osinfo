#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _OSINFO {
	unsigned long Build, Revision;
}OSINFO, * POSINFO;

int main(int argc, char** argv) {
	char* HeaderOut = malloc(2048);
	sprintf(HeaderOut,
		"#pragma once\n" \
		"/*\n" \
		" osinfo.h\n" \
		" (c)Noah Wooten 2023 - 2025, All Rights Reserved\n" \
		" */\n\n" \
		"#define OSINFO_NAME \"\"\n" \
		"#define OSINFO_MAJOR\n" \
		"#define OSINFO_MINOR\n" \
		"#define OSINFO_BUILD\n" \
		"#define OSINFO_REVSN\n\n" \
		"#define OSINFO_BRANCH \"\"\n" \
		"#define OSINFO_DATE\n" \
		"#define OSINFO_TIME\n",
			);



	return;
}