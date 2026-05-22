#include "com_github_kavos113_karin_engine_jni_JniWindow.h"

#include <karin/gui.h>
#include <memory>

#include "jni_resource.h"

using namespace karin::gui;

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniWindow_setRootView
    (JNIEnv *env, jclass cls, jlong windowPtr, jlong viewPtr)
{
    CHECK_JNI_PTR(windowPtr);
    CHECK_JNI_PTR(viewPtr);

    auto *window = reinterpret_cast<std::shared_ptr<Window> *>(windowPtr);
    auto *view = reinterpret_cast<ViewNode *>(viewPtr);

    (*window)->setRootView(std::unique_ptr<ViewNode>(view));
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniWindow_destroy
    (JNIEnv *env, jclass cls, jlong windowPtr)
{
    CHECK_JNI_PTR(windowPtr);
    auto *window = reinterpret_cast<std::shared_ptr<Window> *>(windowPtr);
    delete window;
}