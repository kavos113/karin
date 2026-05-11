package com.github.kavos113.karin;

class JniApplication {
    static {
        KarinLoader.load();
    }

    static native long create();
    static native long createWindow(long appPtr, String title, int x, int y, int width, int height);
    static native void run(long appPtr);
    static native void destroy(long appPtr);
}
