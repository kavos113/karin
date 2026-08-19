#include "com_github_kavos113_karin_engine_jni_JniViewNode.h"

#include <karin/gui.h>
#include <karin/common.h>
#include <memory>

#include "jni_global_ref.h"
#include "jni_resource.h"

using namespace karin::gui;
using namespace karin::jni;

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setSize
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat width, jfloat height)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([width, height, node]
    {
        node->setSize({width, height});
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setWidth
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat width)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node, width]
    {
        node->setWidth(width);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setHeight
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat height)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node, height]
    {
        node->setHeight(height);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setBackgroundColor
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat r, jfloat g, jfloat b, jfloat a)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node, r, g, b, a]
    {
        node->setBackgroundColor({r, g, b, a});
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setOpacity
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat opacity)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node, opacity]
    {
        node->setOpacity(opacity);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setMargin
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar flags, jfloat left, jfloat top, jfloat right, jfloat bottom)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([flags, node, left, top, bottom, right]
    {
        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_LEFT)
        {
            node->setMargin(ViewNode::Side::Left, left);
        }

        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_TOP)
        {
            node->setMargin(ViewNode::Side::Top, top);
        }

        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_RIGHT)
        {
            node->setMargin(ViewNode::Side::Right, right);
        }

        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_BOTTOM)
        {
            node->setMargin(ViewNode::Side::Bottom, bottom);
        }
    });

}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPadding
    (JNIEnv *env, jclass cls, jlong viewPtr, jchar flags, jfloat left, jfloat top, jfloat right, jfloat bottom)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([flags, node, left, top, bottom, right]
    {
        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_LEFT)
        {
            node->setPadding(ViewNode::Side::Left, left);
        }

        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_TOP)
        {
            node->setPadding(ViewNode::Side::Top, top);
        }

        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_RIGHT)
        {
            node->setPadding(ViewNode::Side::Right, right);
        }

        if (flags & com_github_kavos113_karin_engine_jni_JniViewNode_SIDE_BOTTOM)
        {
            node->setPadding(ViewNode::Side::Bottom, bottom);
        }
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setMarginSide
    (JNIEnv *env, jclass cls, jlong viewPtr, jint side, jfloat margin)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node, side, margin]
    {
        node->setMargin(static_cast<ViewNode::Side>(side), margin);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPaddingSide
    (JNIEnv *env, jclass cls, jlong viewPtr, jint side, jfloat padding)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node, side, padding]
    {
        node->setPadding(static_cast<ViewNode::Side>(side), padding);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setBorder
    (JNIEnv *env, jclass cls, jlong viewPtr, jint side, jfloat width, jfloat r, jfloat g, jfloat b, jfloat a, jint style)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    NodeBorder::LineStyle lineStyle = static_cast<NodeBorder::LineStyle>(style);
    karin::Color color(r, g, b, a);

    Application::sendTaskEvent([node, side, width, color, lineStyle]
    {
        node->setBorder(static_cast<ViewNode::Side>(side), width, color, lineStyle);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setShadow
    (JNIEnv *env, jclass cls, jlong viewPtr, jfloat offsetX, jfloat offsetY, jfloat r, jfloat g, jfloat b, jfloat a, jfloat blurRadius, jfloat spreadRadius)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    karin::Color color(r, g, b, a);

    Application::sendTaskEvent([node, offsetX, offsetY, color, blurRadius, spreadRadius]
    {
        node->setShadow(offsetX, offsetY, color, blurRadius, spreadRadius);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_requestRelayout
    (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->requestRelayout();
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_requestRedraw
    (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->requestRedraw();
    });
}


JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_destroy
    (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        delete node;
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPointerMoveListener
    (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setPointerMoveHandler(
            [target](karin::Point point)
            {
                target->invoke(
                    [point](JNIEnv* env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchPointerMove", "(FF)V");
                        if (methodId)
                        {
                            env->CallVoidMethod(obj, methodId, point.x, point.y);
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPointerDownListener
  (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setPointerDownHandler(
            [target](karin::Point point, karin::MouseButtonType type)
            {
                target->invoke(
                    [point](JNIEnv* env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchPointerDown", "(FF)V");
                        if (methodId)
                        {
                            env->CallVoidMethod(obj, methodId, point.x, point.y);
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPointerUpListener
  (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setPointerUpHandler(
            [target](karin::Point point, karin::MouseButtonType type)
            {
                target->invoke(
                    [point](JNIEnv* env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchPointerUp", "(FF)V");
                        if (methodId)
                        {
                            env->CallVoidMethod(obj, methodId, point.x, point.y);
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPointerEnterListener
  (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setPointerEnterHandler(
            [target](karin::Point point)
            {
                target->invoke(
                    [](JNIEnv* env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchPointerEnter", "()V");
                        if (methodId)
                        {
                            env->CallVoidMethod(obj, methodId);
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setPointerLeaveListener
  (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setPointerLeaveHandler(
            [target](karin::Point point)
            {
                target->invoke(
                    [](JNIEnv* env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchPointerLeave", "()V");
                        if (methodId)
                        {
                            env->CallVoidMethod(obj, methodId);
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setMouseWheelListener
  (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setMouseWheelHandler(
            [target](karin::Point point, int delta)
            {
                target->invoke(
                    [delta](JNIEnv* env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchMouseWheel", "(I)V");
                        if (methodId)
                        {
                            env->CallVoidMethod(obj, methodId, delta);
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setKeyListener
    (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setKeyHandler(
            [target](karin::KeyEvent event)
            {
                target->invoke(
                    [&event](JNIEnv* env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchKeyEvent", "(IIII)V");
                        if (methodId)
                        {
                            env->CallVoidMethod(
                                obj,
                                methodId,
                                static_cast<int>(event.type),
                                static_cast<int>(event.keyCode),
                                static_cast<int>(event.scanCode),
                                static_cast<int>(event.modifierState)
                            );
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setKeyTypeListener
    (JNIEnv *env, jclass cls, jlong viewPtr, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, target]
    {
        node->setKeyTypeHandler(
            [target](std::string character)
            {
                target->invoke(
                    [&character](JNIEnv *env, jobject obj)
                    {
                        jclass listenerClass = env->GetObjectClass(obj);
                        jmethodID methodId = env->GetMethodID(listenerClass, "dispatchKeyType", "(Ljava/lang/String;)V");
                        if (methodId)
                        {
                            jstring str = env->NewStringUTF(character.c_str());
                            env->CallVoidMethod(obj, methodId, str);
                        }

                        env->DeleteLocalRef(listenerClass);
                    }
                );
            }
        );
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_setIsFocusable
    (JNIEnv *env, jclass cls, jlong viewPtr, jboolean isFocusable, jobject listener)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);
    auto target = std::make_shared<JniGlobalRef>(env, listener);

    Application::sendTaskEvent([node, isFocusable, target]
    {
        node->setFocusable(isFocusable);

        if (isFocusable)
        {
            node->setChangeFocusStateHandler(
                [target](bool focusState)
                {
                    target->invoke(
                        [focusState](JNIEnv *env, jobject obj)
                        {
                            jclass listenerClass = env->GetObjectClass(obj);
                            jmethodID methodId = env->GetMethodID(listenerClass, "onChangeFocusState", "(Z)V");
                            if (methodId)
                            {
                                env->CallVoidMethod(obj, methodId, focusState);
                            }

                            env->DeleteLocalRef(listenerClass);
                        }
                    );
                }
            );
        }
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearPointerMoveListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setPointerMoveHandler(nullptr);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearPointerDownListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setPointerDownHandler(nullptr);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearPointerUpListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setPointerUpHandler(nullptr);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearPointerEnterListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setPointerEnterHandler(nullptr);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearPointerLeaveListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setPointerLeaveHandler(nullptr);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearMouseWheelListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setMouseWheelHandler(nullptr);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearKeyListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setKeyHandler(nullptr);
    });
}

JNIEXPORT void JNICALL Java_com_github_kavos113_karin_engine_jni_JniViewNode_clearKeyTypeListener
  (JNIEnv *env, jclass cls, jlong viewPtr)
{
    CHECK_JNI_PTR(viewPtr);
    auto *node = reinterpret_cast<ViewNode *>(viewPtr);

    Application::sendTaskEvent([node]
    {
        node->setKeyTypeHandler(nullptr);
    });
}