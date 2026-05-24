#include "com_github_kavos113_karin_engine_jni_JniViewNode.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

#include "jni_global_ref.h"
#include "jni_resource.h"

using namespace karin::gui;
using namespace karin::jni;

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setClickListener
    (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
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
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setSize({width, height});
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setWidth
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat width)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setWidth(width);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setHeight
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat height)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setHeight(height);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setBackgroundColor
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat r, jfloat g, jfloat b, jfloat a)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setBackgroundColor({r, g, b, a});
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setMargin
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar flags, jfloat left, jfloat top, jfloat right, jfloat bottom)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_LEFT)
    {
        node->setMargin(ViewNode::Side::Left, left);
    }

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_TOP)
    {
        node->setMargin(ViewNode::Side::Top, top);
    }

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_RIGHT)
    {
        node->setMargin(ViewNode::Side::Right, right);
    }

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_BOTTOM)
    {
        node->setMargin(ViewNode::Side::Bottom, bottom);
    }
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPadding
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar flags, jfloat left, jfloat top, jfloat right, jfloat bottom)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_LEFT)
    {
        node->setPadding(ViewNode::Side::Left, left);
    }

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_TOP)
    {
        node->setPadding(ViewNode::Side::Top, top);
    }

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_RIGHT)
    {
        node->setPadding(ViewNode::Side::Right, right);
    }

    if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_MARGIN_BOTTOM)
    {
        node->setPadding(ViewNode::Side::Bottom, bottom);
    }
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setMarginSide
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar side, jfloat margin)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setMargin(static_cast<ViewNode::Side>(side), margin);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPaddingSide
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar side, jfloat padding)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    node->setPadding(static_cast<ViewNode::Side>(side), padding);
}


JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_destroy
    (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    delete node;
}