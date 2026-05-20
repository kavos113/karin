#include "com_github_kavos113_karin_engine_jni_JniContainerNode.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

using namespace karin::gui;

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_create__
    (JNIEnv *, jclass)
{
    auto *container = new ContainerNode();
    return reinterpret_cast<jlong>(container);
}

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_create__FF
    (JNIEnv *env, jclass cls, jfloat width, jfloat height)
{
    karin::Size size(width, height);
    auto *container = new ContainerNode(size);
    return reinterpret_cast<jlong>(container);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_addChild
    (JNIEnv *env, jclass cls, jlong containerPtr, jlong childPtr)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    auto *child = reinterpret_cast<ViewNode *>(childPtr);
    container->addChild(std::unique_ptr<ViewNode>(child));
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_setLayoutDirection
    (JNIEnv *env, jclass cls, jlong containerPtr, jint direction)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    container->setLayoutDirection(static_cast<ContainerNode::LayoutDirection>(direction));
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_setGap
    (JNIEnv *env, jclass cls, jlong containerPtr, jfloat gap)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    container->setGap(gap);
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_setWrapMode
    (JNIEnv *env, jclass cls, jlong containerPtr, jint mode)
{
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    container->setWrapMode(static_cast<ContainerNode::WrapMode>(mode));
}

