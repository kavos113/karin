package com.github.kavos113.karin.engine.jni;

import com.github.kavos113.karin.engine.handle.ViewNodeHandle;

class JniViewNode {
    static native void setClickListener(long nodePtr, ViewNodeHandle node);
    static native void destroy(long nodePtr);
}
