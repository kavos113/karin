package com.github.kavos113.karin.engine.jni;

class JniTextNode {
    static {
        KarinLoader.load();
    }

    static native long create(
        String text,
        String fontFamily,
        int fontStyle,
        int fontStretch,
        int fontWeight,
        float fontSize,
        String locale,
        boolean underline,
        boolean strikethrough,
        int horizontalAlignment,
        int verticalAlignment,
        float lineSpacing,
        float baseLine,
        int lineSpacingMode,
        int trimming,
        int wrapping,
        int flowDirection,
        int readingDirection,
        float r,
        float g,
        float b,
        float a
    );
}
