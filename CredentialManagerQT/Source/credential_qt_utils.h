#ifndef _bnb_Credential_QT_Utils_H_
#define _bnb_Credential_QT_Utils_H_

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

namespace ui_utils
{
    enum ui_size
    {
        def_text_length = 64,
        def_text_size = 10,
        def_widget_h = 24,

        lab_text_w = 72,
        lab_mark_w = 16,
        lab_credential_w = 92,
        lab_password_w = 100,
        lab_username_w = 80,
        lab_account_w = 64,
        btn_default_w = 64,
        tree_item_w = 160,
        edit_default_w = 280,
        edit_password_w = 240,

        tree_view_min_w = 160,
        tree_view_max_w = tree_view_min_w << 1
    };

    const QColor g_clrManView{ 80, 216, 80 };
    const QColor g_clrCredential{ 128, 128, 0 };
    const QColor g_clrPlatform{ 255, 64, 0 };
    const QColor g_clrAccount{ 160, 128, 255 };
    const QColor g_clrProperty{ 32, 160, 32 };
    const QColor g_clrLabel{ 0, 64, 255 };

    void SetBackgroundColor(QWidget* pView, const QColor& color);

    QLabel* MakeLabel(QWidget* parent, unsigned int w, unsigned int h, Qt::Alignment align, QColor clr, bool bold, unsigned int size);
    QLabel* MakeStaticLabel(QWidget* parent, unsigned int w = lab_text_w, const QColor& clr = g_clrLabel);
    QLabel* MakeDynamicLabel(QWidget * parent, const QColor& clr, Qt::Alignment align);
    QLabel* MakeMarkLabel(QWidget* parent, unsigned int w = lab_mark_w);

    QLineEdit* MakeLineEdit(QWidget* parent, unsigned int w = edit_default_w, const QColor& clr = Qt::black);
    QLineEdit* MakePasswordLine(QWidget* parent, unsigned int w = edit_password_w);
    QLineEdit* MakeShowLine(QWidget* parent, const QColor& clr = Qt::black);

    QPushButton* MakeButton(QWidget* parent, unsigned int w = btn_default_w);

}

QT_END_NAMESPACE

#endif // _bnb_Credential_QT_Utils_H_
