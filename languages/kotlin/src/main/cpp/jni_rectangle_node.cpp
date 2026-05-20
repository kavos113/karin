#include "com_github_kavos113_karin_engine_jni_JniRectangleNode.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

using namespace karin::gui;

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_engine_jni_JniRectangleNode_create
    (JNIEnv *env, jclass cls, jfloat width, jfloat height, jfloat r, jfloat g, jfloat b, jfloat a)
{
    karin::Size size(width, height);
    karin::Color color(r, g, b, a);
    auto *rectangle = new RectangleNode(size, color);
    return reinterpret_cast<jlong>(rectangle);
}
