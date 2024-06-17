#include "inputline.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QInputMethodEvent>
#include <QTextLayout>

InputLine::InputLine(QWidget *parent) : QLineEdit(parent), trigger_length_(0)
{
    connect(this, &QLineEdit::textEdited, this, [this] {
        history_.resetIterator();
        user_text_ = text();
    });

    // Clear input hint on text change
    connect(this, &QLineEdit::textChanged, this, [this]() { input_hint_ = QString(); });

    // Trigger repaint on cursor movement
    connect(this, &QLineEdit::cursorPositionChanged, this, [this]() { update(); });
}

void InputLine::setInputHint(const QString &text)
{
    input_hint_ = text;
    setToolTip(text);
    update();
}

void InputLine::setTriggerLength(uint len) { trigger_length_ = len; }

void InputLine::next(bool search)
{
    auto t = history_.next(search ? user_text_ : QString());

    // Without ClearOnHide the text is already in the input
    // I.e. the first item in history equals the input text
    if (t == text())
        t = history_.next(search ? user_text_ : QString());

    setText(t);
}

void InputLine::previous(bool search)
{
    auto t = history_.prev(search ? user_text_ : QString());
    if (!t.isEmpty())
        setText(t);
}

void InputLine::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    QStyleOptionFrame panel;
    initStyleOption(&panel);

    QRect content_rect = style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);
    content_rect = content_rect.marginsRemoved(textMargins());
    content_rect.adjust(2, 1, -2, -1); // https://codebrowser.dev/qt5/qtbase/src/widgets/widgets/qlineedit_p.cpp.html#QLineEditPrivate::verticalMargin

    QPainter p(this);
    p.setPen(panel.palette.placeholderText().color());
    p.setBackgroundMode(Qt::TransparentMode);

    if (!input_hint_.isNull())
    {
        QString hint;
        if (input_hint_.startsWith(text()))
            hint = input_hint_.mid(text().length());
        else
            hint = QString(" %1").arg(input_hint_);

        auto fm = fontMetrics();
        auto r = content_rect;

        // Get the width of the text in pixels, including pre-edit text for IME
        QString full_text = text() + preedit_text_;
        int text_width = fm.horizontalAdvance(full_text);

        // Adjust hint starting position to avoid overlap
        int offset = 20; // Increased offset to avoid overlap during input
        r.setLeft(r.left() + text_width + offset);

        auto t = fm.elidedText(hint, Qt::ElideRight, r.width());

        p.drawText(r, Qt::TextSingleLine, t);
    }
}

void InputLine::hideEvent(QHideEvent *)
{
    history_.add(text());
    history_.resetIterator();
    user_text_.clear();
    preedit_text_.clear();
}

void InputLine::inputMethodEvent(QInputMethodEvent *event)
{
    preedit_text_ = event->preeditString();
    QLineEdit::inputMethodEvent(event);
    update();
}

QVariant InputLine::inputMethodQuery(Qt::InputMethodQuery query) const
{
    if (query == Qt::ImCursorRectangle)
        return cursorRect();
    if (query == Qt::ImSurroundingText)
        return QVariant(text() + preedit_text_);
    return QLineEdit::inputMethodQuery(query);
}
