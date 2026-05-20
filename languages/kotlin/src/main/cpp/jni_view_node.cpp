#include "com_github_kavos113_karin_engine_jni_JniViewNode.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

#include "jni_global_ref.h"

using namespace karin::gui;
using namespace karin::jni;

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_JniViewNode_setClickListener
    (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto callback = std::make_shared<JniGlobalRef>(env, listener);

    node->setOnClick(
        [callback](karin::Point point)
        {
            callback->invoke(
                [](JNIEnv* env, jobject obj)
                {
                    jclass listenerClass = env->GetObjectClass(obj);
                    jmethodID methodId = env->GetMethodID(listenerClass, "dispatchClickEvent", "()V");
                    if (methodId)
                    {
                        env->CallVoidMethod(obj, methodId);
                    }

                    env->DeleteLocalRef(listenerClass);
                }
            );
        }
    );
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_destroy
    (JNIEnv *env, jclass cls, jlong viewPtr)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    delete node;
}