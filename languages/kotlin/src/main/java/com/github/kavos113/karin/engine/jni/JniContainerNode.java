package com.github.kavos113.karin.engine.jni;

class JniContainerNode {
    static {
        KarinLoader.load();
    }

    static native long create();
    static native long create(float width, float height);

    static native void addChild(long containerPtr, long childPtr);
    static native void insertChild(long containerPtr, long childPtr, int index);
    static native void removeChild(long containerPtr, long childPtr);
    static native void clearChildren(long containerPtr);

    static native void setLayoutDirection(long containerPtr, int direction);
    static native void setGap(long containerPtr, float gap);
    static native void setWrapMode(long containerPtr, int wrapMode);
    static native void setEnableClip(long containerPtr, boolean enableClip);
}
