package com.github.kavos113.karin.engine.jni;

class JniContainerNode {
    static native long create();
    static native long create(float width, float height);
    static native void addChild(long containerPtr, long childPtr);
    static native void setLayoutDirection(long containerPtr, int direction);
    static native void setGap(long containerPtr, float gap);
    static native void setWrapMode(long containerPtr, int wrapMode);
}
