#include "com_github_kavos113_karin_engine_jni_JniTextNode.h"

#include <karin/gui.h>
#include <karin/graphics.h>
#include <karin/common.h>
#include <memory>

using namespace karin::gui;

JNIEXPORT jlong JNICALL Java_com_github_kavos113_karin_JniTextNode_create
    (JNIEnv *env, jclass cls, jstring text, jstring fontFamily,
     jint fontStyle, jint fontStretch, jint fontWeight, jfloat fontSize, jstring locale,
     jboolean underline, jboolean strikethrough, jint horizontalAlignment, jint verticalAlignment,
     jfloat lineSpacing, jfloat baseline, jint lineSpacingMode, jint trimming, jint wrapping,
     jint flowDirection, jint readingDirection, jfloat r, jfloat g, jfloat b, jfloat a
    )
{
    const char *textChars = env->GetStringUTFChars(text, nullptr);
    const char *fontFamilyChars = env->GetStringUTFChars(fontFamily, nullptr);
    const char *localeChars = env->GetStringUTFChars(locale, nullptr);

    karin::Font font;
    font.family = fontFamilyChars;
    font.style = static_cast<karin::Font::Style>(fontStyle);
    font.stretch = static_cast<karin::Font::Stretch>(fontStretch);
    font.weight = static_cast<karin::Font::Weight>(fontWeight);

    karin::TextStyle textStyle;
    textStyle.font = font;
    textStyle.size = fontSize;
    textStyle.locale = localeChars;
    textStyle.underline = underline;
    textStyle.lineThrough = strikethrough;

    karin::ParagraphStyle paragraphStyle;
    paragraphStyle.horizontalAlignment = static_cast<karin::ParagraphStyle::HorizontalAlignment>(horizontalAlignment);
    paragraphStyle.verticalAlignment = static_cast<karin::ParagraphStyle::VerticalAlignment>(verticalAlignment);
    paragraphStyle.lineSpacing = lineSpacing;
    paragraphStyle.baseline = baseline;
    paragraphStyle.lineSpacingMode = static_cast<karin::ParagraphStyle::LineSpacingMode>(lineSpacingMode);
    paragraphStyle.trimming = static_cast<karin::ParagraphStyle::Trimming>(trimming);
    paragraphStyle.wrapping = static_cast<karin::ParagraphStyle::Wrapping>(wrapping);
    paragraphStyle.flowDirection = static_cast<karin::ParagraphStyle::Direction>(flowDirection);
    paragraphStyle.readingDirection = static_cast<karin::ParagraphStyle::Direction>(readingDirection);

    karin::SolidColorPattern pattern(karin::Color(r, g, b, a));

    auto *textNode = new TextNode(textChars, textStyle, paragraphStyle, pattern);

    env->ReleaseStringUTFChars(text, textChars);
    env->ReleaseStringUTFChars(fontFamily, fontFamilyChars);
    env->ReleaseStringUTFChars(locale, localeChars);

    return reinterpret_cast<jlong>(textNode);
}