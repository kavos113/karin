package com.github.kavos113.karin;

class JniWindow {
    static {
        KarinLoader.load();
    }

    static native void setRootView(long windowPtr, long viewPtr);
    static native void destroy(long windowPtr);
}
