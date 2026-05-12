#include "com_github_kavos113_karin_JniApplication.h"

#include <karin/gui.h>
#include <memory>

using namespace karin::gui;

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_JniApplication_create
    (JNIEnv *env, jclass cls)
{
    auto *app = new Application();
    return reinterpret_cast<jlong>(app);
}

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_JniApplication_createWindow
    (JNIEnv *env, jclass cls, jlong appPtr, jstring title, jint x, jint y, jint width, jint height)
{
    auto *app = reinterpret_cast<Application *>(appPtr);

    const char *titleChars = env->GetStringUTFChars(title, nullptr);
    auto window = app->createWindow(titleChars, x, y, width, height);
    env->ReleaseStringUTFChars(title, titleChars);

    auto *windowPtr = new std::shared_ptr(window);

    return reinterpret_cast<jlong>(windowPtr);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_JniApplication_run
    (JNIEnv *env, jclass cls, jlong appPtr)
{
    auto *app = reinterpret_cast<Application *>(appPtr);
    app->run();
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_JniApplication_destroy
    (JNIEnv *env, jclass cls, jlong appPtr)
{
    auto *app = reinterpret_cast<Application *>(appPtr);
    delete app;
};