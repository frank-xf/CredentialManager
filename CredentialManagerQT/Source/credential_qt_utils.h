#ifndef _bnb_Credential_QT_Utils_H_
#define _bnb_Credential_QT_Utils_H_

#include <QtWidgets/QStyledItemDelegate>

QT_BEGIN_NAMESPACE

class QLabel;
class QLineEdit;
class QPushButton;

namespace ui_utils
{
    enum ui_size
    {
        def_size_text = 10,

        lab_default_w = 72,
        lab_default_h = 20,

        lab_mark_w = 16,
        lab_mark_h = 20,

        tree_view_min_w = 200,
        tree_view_max_w = tree_view_min_w << 1,

        tree_item_w = 160,
        tree_item_h = 24,

        edit_default_w = 200,
        edit_default_h = 24,

        edit_password_w = edit_default_w,
        edit_password_h = 20,

        btn_default_w = 64,
        btn_default_h = 24,

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
    const QColor g_clrCredential{ 16,128,16 };
    const QColor g_clrPlatform{ 224, 32, 32 };
    const QColor g_clrAccount{ 64, 64, 255 };
    const QColor g_clrProperty{ 32, 160, 32 };
    const QColor g_clrLabel{ 128, 128, 0 };


    template<typename _Ty>
    constexpr unsigned int to_uint(_Ty value) { return static_cast<unsigned int>(value); }

    void SetBackgroundColor(QWidget* pView, const QColor& color);
    QLabel* MakeMarkLabel(QWidget* parent, unsigned int w = to_uint(lab_mark_w), unsigned int h = to_uint(lab_mark_h));
    QLabel* MakeStaticLabel(QWidget* parent, unsigned int w = lab_default_w, unsigned int h = lab_default_h, QColor c = Qt::black, bool b = false, unsigned int u = def_size_text);
	QLabel* MakeDynamicLabel(QWidget* parent, QColor c = Qt::black, bool b = false, unsigned int u = def_size_text);
    QPushButton* MakeButton(QWidget* parent, unsigned int w = btn_default_w, unsigned int h = btn_default_h, unsigned int u = def_size_text);

    QLabel* MakeLabel(QWidget* p, unsigned int w, unsigned int h, Qt::Alignment a, QColor c, bool b, unsigned int u);
    QLineEdit* MakeLineEdit(QWidget* p, unsigned int w = edit_default_w, unsigned int h = edit_default_h, unsigned int u = def_size_text);
}

class NoFocusDelegate : public QStyledItemDelegate
{
    void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
    {
        QStyleOptionViewItem itemOption(option);
        if (itemOption.state & QStyle::State_HasFocus)
            itemOption.state ^= QStyle::State_HasFocus;

        QStyledItemDelegate::paint(painter, itemOption, index);
    }
};

QT_END_NAMESPACE

#endif // _bnb_Credential_QT_Utils_H_
