#include "TextBox.h"

// Default events
void TextBox::OnTypingEvent(void* sender, void* args)
{
    auto obj = reinterpret_cast<TextBox*>(sender);
    obj->InputChar((const wchar_t*)args);

}

void TextBox::OnFocusEvent(void* sender, void* args)
{
    auto obj = reinterpret_cast<TextBox*>(sender);
    obj->text.append(L"|");
}

void TextBox::OnLostFocusEvent(void* sender, void* args)
{
    auto obj = reinterpret_cast<TextBox*>(sender);
    obj->text.pop_back();
}


TextBox::TextBox(int x, int y, int width, int height, unsigned char align, ElementStyle style):
Element(x, y, width == AUTO ? height * 5.10 : width, height, align), text(text), style(style), fontSize(20.0f)
{
    padding = { 10, 0, 0, 0 };
    AddEvent(ON_TYPING, OnTypingEvent);
    AddEvent(ON_FOCUS, OnFocusEvent);
    AddEvent(LOST_FOCUS, OnLostFocusEvent);
}

void TextBox::Create(HINSTANCE hInstance, HWND hParent, ID2D1RenderTarget* pRenderTarget)
{
    CreateResouces(pRenderTarget);
}

void TextBox::SetOpacity(float opacity)
{
}

float TextBox::GetOpacity()
{
    return 0.0f;
}

void TextBox::Move(int x, int y)
{
    roundRect = D2D1::RoundedRect(D2D1::RectF(posX, posY, posX + width, posY + height), style.getCornerRadius(), style.getCornerRadius());
}

HWND TextBox::Show(HWND hParent, HINSTANCE hInstance)
{
    return HWND();
}

static int getTextSize(const WCHAR* text)
{
    int i = 0;
    for (; text[i] != '\0'; i++);
    return i;
}

void TextBox::OnPaint(ID2D1RenderTarget* pRenderTarget)
{
   pRenderTarget->SetTransform(translate * transform);

   pRenderTarget->FillRoundedRectangle(roundRect, pSolidColorBrush);
   pRenderTarget->DrawTextA(text.c_str(), text.size(), pWriteFormat, D2D1::RectF(posX + padding.left, posY, width + posX, height + posY),
       pForegroundBrush);

    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void TextBox::SetText(const WCHAR* text)
{
}

void TextBox::InputChar(const wchar_t* character)
{
    if (text.size() > 1 && (char)*character == '\b')
        text.erase(text.size() - 2, 1);
    else if (*character == 127)
        return;
    else
        //text.append((const wchar_t*)character);
        text.insert(text.size() - 1, (const wchar_t*)character);
}

void TextBox::RemoveChar()
{
    //stream.seekp(1, std::ios_base::end);
}



void TextBox::CreateResouces(ID2D1RenderTarget* pRenderTarget)
{
    if (style.getTypeOfBrush() == SOLID_COLOR)
        pRenderTarget->CreateSolidColorBrush(style.getBackgroundColors()[0], &pSolidColorBrush);

    roundRect = D2D1::RoundedRect(D2D1::RectF(posX, posY, posX + width, posY + height), style.getCornerRadius(), style.getCornerRadius());

    pRenderTarget->CreateSolidColorBrush(style.getForegroundColor(), &pForegroundBrush);

    const WCHAR fontName[] = L"Arial";
    HRESULT hr;

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pWriteFactory), (IUnknown**)&pWriteFactory);

    hr = pWriteFactory->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &pWriteFormat);

    pWriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    pWriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

}
