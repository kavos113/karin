#include "com_github_kavos113_karin_engine_jni_JniWindow.h"

#include <memory>

#include <karin/gui.h>

#include "jni_resource.h"

using namespace karin::gui;

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniWindow_setRootView
    (JNIEnv *env, jclass cls, jlong windowPtr, jlong viewPtr)
{
    CHECK_JNI_PTR(windowPtr);
    CHECK_JNI_PTR(viewPtr);

    auto *window = reinterpret_cast<std::shared_ptr<Window> *>(windowPtr);
    auto *view = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([window, view]
    {
        (*window)->setRootView(std::unique_ptr<ViewNode>(view));
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniWindow_destroy
    (JNIEnv *env, jclass cls, jlong windowPtr)
{
    CHECK_JNI_PTR(windowPtr);
    auto *window = reinterpret_cast<std::shared_ptr<Window> *>(windowPtr);

    Application::sendTaskEvent([window]
    {
        delete window;
    });
}