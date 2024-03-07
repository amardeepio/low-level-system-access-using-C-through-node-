#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <cjson/cJSON.h>

typedef struct {
    long uptime;
    long totalram;
    long freeram;
} SystemInfo;

// Get system information and store it in the SystemInfo struct
SystemInfo getSystemInfo() {
    SystemInfo info;
    struct sysinfo si;

    if (sysinfo(&si) != 0) {
        perror("sysinfo");
        exit(EXIT_FAILURE);  // Handle errors appropriately
    }

    info.uptime = si.uptime;
    info.totalram = si.totalram;
    info.freeram = si.freeram;

    return info;
}

// Generate a JSON string containing the system information
char* getSystemInfoJSON() {
    SystemInfo info = getSystemInfo();

    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "uptime", info.uptime);
    cJSON_AddNumberToObject(root, "totalram", info.totalram);
    cJSON_AddNumberToObject(root, "freeram", info.freeram);

    char *jsonString = cJSON_Print(root);
    cJSON_Delete(root);

    return jsonString;
}
