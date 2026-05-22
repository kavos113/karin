package com.github.kavos113.karin.engine.jni;

import com.github.kavos113.karin.engine.handle.ViewNodeHandle;

class JniViewNode {
    static {
        KarinLoader.load();
    }

    static native void setClickListener(long nodePtr, ViewNodeHandle node);
    static native void setSize(long nodePtr, float width, float height);
    static native void setWidth(long nodePtr, float width);
    static native void setHeight(long nodePtr, float height);
    static native void setBackgroundColor(long nodePtr, float r, float g, float b, float a);

    // flags: 1 - left, 2 - top, 4 - right, 8 - bottom
    static native void setMargin(long nodePtr, char flags, float left, float top, float right, float bottom);
    static native void setPadding(long nodePtr, char flags, float left, float top, float right, float bottom);

    static native void destroy(long nodePtr);
}
