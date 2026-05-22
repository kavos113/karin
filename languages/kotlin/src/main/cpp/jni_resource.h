#ifndef LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_RESOURCE_H
#define LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_RESOURCE_H

#define CHECK_JNI_PTR(ptr) \
    if (ptr == 0) \
    { \
        jclass ex = env->FindClass("java/lang/IllegalStateException"); \
        env->ThrowNew(ex, "Native pointer is null"); \
        return; \
    }

#endif //LANGUAGES_KOTLIN_SRC_MAIN_CPP_JNI_RESOURCE_H
