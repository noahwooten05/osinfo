#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _OSINFO {
	char OSName[256];
	unsigned long Major, Minor;
	unsigned long Build, Revision;
}OSINFO, * POSINFO;

int main(int argc, char** argv) {

	char* HeaderOut = malloc(2048);
	POSINFO OsInfoData = malloc(sizeof(OSINFO));
	memset(OsInfoData, 0, sizeof(OSINFO));

	unsigned char UpdateMode = 0, IncrementBuild = 0;
	for (int i = 0; i < argc; i++) {
		if (strstr(argv[i], "--update")) {
			UpdateMode = 1;
		}
		if (strstr(argv[i], "--release")) {
			IncrementBuild = 1;
		}
	}

	FILE* OSInfo = fopen("osinfo.dat", "rb+");
	if (!OSInfo) {
		printf("[WARN]: No OS Info data file was found, a new one will be created.\n");
		OSInfo = fopen("osinfo.dat", "wb+");
		UpdateMode = 1;
	} else {
		fread(OsInfoData, sizeof(OSINFO), 1, OSInfo);
	}

	if (UpdateMode) {
		// allow for name, major, minor, etc changes
		printf("OS Name (Current %s): ", OsInfoData->OSName);
		scanf("%s", OsInfoData->OSName);
		printf("OS Major (Current %i): ", OsInfoData->Major);
		scanf("%i", &OsInfoData->Major);
		printf("OS Minor (Current %i): ", OsInfoData->Minor);
		scanf("%i", &OsInfoData->Minor);
		printf("OS Build (Current %i): ", OsInfoData->Build);
		scanf("%i", &OsInfoData->Build);
	}

	if (IncrementBuild) {
		OsInfoData->Build++;
		OsInfoData->Revision = 0;
	}
	else {
		OsInfoData->Revision++;
	}

	fclose(OSInfo);
	OSInfo = fopen("osinfo.dat", "wb");
	fwrite(OsInfoData, sizeof(OSINFO), 1, OSInfo);
	fclose(OSInfo);

	// Get Branch Name
	char Buffer[128];
	char Branch[128] = { 0 };
	FILE* Pipe = _popen("git branch --show-current", "r");
	while (fgets(Buffer, 128, Pipe)) {
		strcat(Branch, Buffer);
	}
	_pclose(Pipe);
	Branch[strcspn(Branch, "\n")] = 0x00;

	// Get Time/Date
	unsigned long Time, Date;
	time_t Now = time(NULL);
	struct tm* Local = localtime(&Now);
	
	Date = ((Local->tm_year + 1900) - 2000) * 10000 +  // Calculate year
		(Local->tm_mon + 1) * 100 + // Calculate month
		(Local->tm_mday); // Calculate day
	Time = Local->tm_hour * 100 + Local->tm_min;

	sprintf(HeaderOut,
		"#pragma once\n" \
		"/*\n" \
		" osinfo.h\n" \
		" (c)Noah Wooten 2023 - 2025, All Rights Reserved\n" \
		" */\n\n" \
		"#define OSINFO_NAME \"%s\"\n" \
		"#define OSINFO_MAJOR %i\n" \
		"#define OSINFO_MINOR %i\n" \
		"#define OSINFO_BUILD %i\n" \
		"#define OSINFO_REVSN %i\n\n" \
		"#define OSINFO_BRANCH \"%s\"\n" \
		"#define OSINFO_DATE %06i\n" \
		"#define OSINFO_TIME %04i\n",

		OsInfoData->OSName, OsInfoData->Major,
		OsInfoData->Minor, OsInfoData->Build,
		OsInfoData->Revision, Branch, Date,
		Time);

	FILE* OsInfoHdr = fopen("osinfo.h", "w");
	fwrite(HeaderOut, strlen(HeaderOut), 1, OsInfoHdr);
	free(HeaderOut);

	// Output the build string
	printf("%s Version %i.%i Build %i.%i.%s.%06i-%04i", OsInfoData->OSName,
		OsInfoData->Major, OsInfoData->Minor, OsInfoData->Build,
		OsInfoData->Revision, Branch, Date, Time);

	free(OsInfoData);
	return;
}