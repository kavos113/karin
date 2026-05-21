package com.github.kavos113.karin.engine.jni;

class JniRectangleNode {
    static {
        KarinLoader.load();
    }

    static native long create(float width, float height, float r, float g, float b, float a);
}
