#include "com_github_kavos113_karin_engine_jni_JniContainerNode.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

#include "jni_resource.h"

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
    CHECK_JNI_PTR(containerPtr);
    CHECK_JNI_PTR(childPtr);

    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    auto *child = reinterpret_cast<ViewNode *>(childPtr);

    Application::sendTaskEvent([container, child]
    {
        container->addChild(child);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_setLayoutDirection
    (JNIEnv *env, jclass cls, jlong containerPtr, jint direction)
{
    CHECK_JNI_PTR(containerPtr);
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);

    Application::sendTaskEvent([container, direction]
    {
        container->setLayoutDirection(static_cast<ContainerNode::LayoutDirection>(direction));
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_setGap
    (JNIEnv *env, jclass cls, jlong containerPtr, jfloat gap)
{
    CHECK_JNI_PTR(containerPtr);
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);

    Application::sendTaskEvent([container, gap]
    {
        container->setGap(gap);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_setWrapMode
    (JNIEnv *env, jclass cls, jlong containerPtr, jint mode)
{
    CHECK_JNI_PTR(containerPtr);
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);

    Application::sendTaskEvent([container, mode]
    {
        container->setWrapMode(static_cast<ContainerNode::WrapMode>(mode));
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_setEnableClip
    (JNIEnv *env, jclass cls, jlong containerPtr, jboolean enable)
{
    CHECK_JNI_PTR(containerPtr);
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);

    Application::sendTaskEvent([container, enable]
    {
        container->setEnableClip(enable);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_removeChild
    (JNIEnv *env, jclass cls, jlong containerPtr, jlong childPtr)
{
    CHECK_JNI_PTR(containerPtr);
    CHECK_JNI_PTR(childPtr);

    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    auto *child = reinterpret_cast<ViewNode *>(childPtr);

    Application::sendTaskEvent([container, child]
    {
        container->removeChild(child);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_clearChildren
    (JNIEnv *env, jclass cls, jlong containerPtr)
{
    CHECK_JNI_PTR(containerPtr);
    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);

    Application::sendTaskEvent([container]
    {
        container->clearChildren();
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniContainerNode_insertChild
    (JNIEnv *env, jclass cls, jlong containerPtr, jlong childPtr, jint index)
{
    CHECK_JNI_PTR(containerPtr);
    CHECK_JNI_PTR(childPtr);

    auto *container = reinterpret_cast<ContainerNode *>(containerPtr);
    auto *child = reinterpret_cast<ViewNode *>(childPtr);

    Application::sendTaskEvent([container, child, index]
    {
        container->insertChild(child, index);
    });
}