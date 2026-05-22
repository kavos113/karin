#include "com_github_kavos113_karin_engine_jni_JniViewNode.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

#include "jni_global_ref.h"

using namespace karin::gui;
using namespace karin::jni;

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setClickListener
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

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setSize
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat width, jfloat height)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setSize({width, height});
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setWidth
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat width)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setWidth(width);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setHeight
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat height)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setHeight(height);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setBackgroundColor
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat r, jfloat g, jfloat b, jfloat a)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setBackgroundColor({r, g, b, a});
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setMargin
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar flags, jfloat left, jfloat top, jfloat right, jfloat bottom)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    if (flags & 0x01) node->setMargin(ViewNode::Side::Left, left);
    if (flags & 0x02) node->setMargin(ViewNode::Side::Top, top);
    if (flags & 0x04) node->setMargin(ViewNode::Side::Right, right);
    if (flags & 0x08) node->setMargin(ViewNode::Side::Bottom, bottom);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPadding
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar flags, jfloat left, jfloat top, jfloat right, jfloat bottom)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    if (flags & 0x01) node->setPadding(ViewNode::Side::Left, left);
    if (flags & 0x02) node->setPadding(ViewNode::Side::Top, top);
    if (flags & 0x04) node->setPadding(ViewNode::Side::Right, right);
    if (flags & 0x08) node->setPadding(ViewNode::Side::Bottom, bottom);
}


JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_destroy
    (JNIEnv *env, jclass cls, jlong viewPtr)
{
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    delete node;
}