#include <stdio.h>
#include <dlfcn.h>
#include <node_api.h>

napi_value GetSystemInfo(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value result;

    void* handle = dlopen("./libsysinfo.so", RTLD_LAZY);
    if (!handle) {
        const char* error_message = dlerror();
        napi_throw_error(env, NULL, error_message);
        return NULL;
    }

    char* (*getSystemInfoJSON)() = dlsym(handle, "getSystemInfoJSON");  
    if (!getSystemInfoJSON) {
        const char* error_message = dlerror();
        napi_throw_error(env, NULL, error_message);
        dlclose(handle);
        return NULL;
    }

    char* systemInfoJSON = getSystemInfoJSON(); 
    dlclose(handle);

    status = napi_create_string_utf8(env, systemInfoJSON, NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to create string");
        return NULL;
    }

    free(systemInfoJSON); 

    return result;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value fn;

    // Create the GetSystemInfo function
    status = napi_create_function(env, NULL, 0, GetSystemInfo, NULL, &fn);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to create function");
        return NULL;
    }

    // Export the function GetSystemInfo
    status = napi_set_named_property(env, exports, "getSystemInfo", fn);
    if (status != napi_ok) {
        napi_throw_error(env, NULL, "Failed to set named property");
        return NULL;
    }

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
