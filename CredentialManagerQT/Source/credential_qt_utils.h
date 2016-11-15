#ifndef _bnb_Credential_QT_Utils_H_
#define _bnb_Credential_QT_Utils_H_

#define _DeclareNoCopy(_ClassName)      private:            \
        _ClassName(const _ClassName&) = delete;             \
        _ClassName& operator=(const _ClassName&) = delete;

QT_BEGIN_NAMESPACE

class QLabel;
class QPushButton;

namespace ui_utils
{
    enum ui_size
    {
        tree_item_w = 64,
        tree_item_h = 24,

        view_default_h = 520,

        edit_default_w = 160,
        edit_default_h = 20,

        btn_default_w = 60,
        btn_default_h = 20,

        lab_default_w = 60,
        lab_default_h = 20,

        lab_mark_w = 16,
        lab_mark_h = 20,

        item_account_w = 240,
        item_account_h = 28,

        item_platform_w = 400,
        item_platform_h = 28,

        dlg_credential_w = 280,
		dlg_credential_h = 160,

        dlg_default_w = 320,
		dlg_default_h = 180,

    };

    const QColor g_clrManView{ 168, 240, 255 };


    template<typename _Ty>
    constexpr unsigned int to_uint(_Ty value) { return static_cast<unsigned int>(value); }

    void SetBackgroundColor(QWidget* pView, const QColor& color);
    QLabel* MakeMarkLabel(QWidget* parent, unsigned int w = to_uint(lab_mark_w), unsigned int h = to_uint(lab_mark_h));
    QLabel* MakeStaticLabel(QWidget* parent, unsigned int w = to_uint(lab_default_w), unsigned int h = to_uint(lab_default_h), QColor c = Qt::black, unsigned int u = 12);
	QLabel* MakeDynamicLabel(QWidget* parent, QColor c = Qt::black, unsigned int u = 12);
    QPushButton* MakeButton(QWidget* parent, unsigned int w = to_uint(btn_default_w), unsigned int h = to_uint(btn_default_h));

    QLabel* MakeLabel(QWidget* p, unsigned int w, unsigned int h, Qt::Alignment a, QColor c, unsigned int u);
}

QT_END_NAMESPACE

#endif // _bnb_Credential_QT_Utils_H_
