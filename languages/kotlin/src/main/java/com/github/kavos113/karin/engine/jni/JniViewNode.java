package com.github.kavos113.karin.engine.jni;

import com.github.kavos113.karin.ViewNode;

class JniViewNode {
    static native void setClickListener(long nodePtr, ViewNode node);
    static native void destroy(long nodePtr);
}
