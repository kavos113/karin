package com.github.kavos113.karin.engine.jni;

import com.github.kavos113.karin.engine.handle.EventManager;

class JniViewNode {
    static {
        KarinLoader.load();
    }

    static native void setSize(long nodePtr, float width, float height);
    static native void setWidth(long nodePtr, float width);
    static native void setHeight(long nodePtr, float height);
    static native void setBackgroundColor(long nodePtr, float r, float g, float b, float a);
    static native void setOpacity(long nodePtr, float opacity);

    @SuppressWarnings("unused") static final char SIDE_LEFT = 1;
    @SuppressWarnings("unused") static final char SIDE_TOP = 2;
    @SuppressWarnings("unused") static final char SIDE_RIGHT = 4;
    @SuppressWarnings("unused") static final char SIDE_BOTTOM = 8;

    static native void setMargin(long nodePtr, char flags, float left, float top, float right, float bottom);
    static native void setPadding(long nodePtr, char flags, float left, float top, float right, float bottom);
    static native void setMarginSide(long nodePtr, int side, float value);
    static native void setPaddingSide(long nodePtr, int side, float value);

    static native void setBorder(long nodePtr, int side, float width, float r, float g, float b, float a, int style);
    static native void setShadow(long nodePtr, float offsetX, float offsetY, float r, float g, float b, float a, float blurRadius, float spreadRadius);

    static native void requestRelayout(long nodePtr);
    static native void requestRedraw(long nodePtr);

    static native void setPointerMoveListener(long nodePtr, EventManager target);
    static native void setPointerDownListener(long nodePtr, EventManager target);
    static native void setPointerUpListener(long nodePtr, EventManager target);
    static native void setPointerEnterListener(long nodePtr, EventManager target);
    static native void setPointerLeaveListener(long nodePtr, EventManager target);
    static native void setMouseWheelListener(long nodePtr, EventManager target);
    static native void setKeyListener(long nodePtr, EventManager target);
    static native void setKeyTypeListener(long nodePtr, EventManager target);
    static native void setIsFocusable(long nodePtr, boolean isFocusable);

    static native void destroy(long nodePtr);
}
