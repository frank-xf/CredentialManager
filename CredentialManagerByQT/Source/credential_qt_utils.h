#ifndef _bnb_Credential_QT_Utils_H_
#define _bnb_Credential_QT_Utils_H_

#define _DeclareNoCopy(_ClassName)      private:            \
        _ClassName(const _ClassName&) = delete;             \
        _ClassName& operator=(const _ClassName&) = delete;

QT_BEGIN_NAMESPACE

namespace ui_utils
{
    enum ui_size
    {
        view_default_h = 520,

        edit_default_w = 160,
        edit_default_h = 20,

        lab_default_w = 60,
        lab_default_h = 28,

        item_account_w = 240,
        item_account_h = 28,

        item_platform_w = 400,
        item_platform_h = 28,

        dlg_username_w = 256,
        dlg_username_h = 108,

        dlg_password_w = 300,
        dlg_password_h = 152,

    };

    const QColor g_defColor{ 168, 240, 255};


    template<typename _Ty>
    constexpr unsigned int to_uint(_Ty value) { return static_cast<unsigned int>(value); }

    void SetBackgroundColor(QWidget* pView, const QColor& color);
    QLabel* MakeMarkLabel(QWidget* parent);

}

QT_END_NAMESPACE

#endif // _bnb_Credential_QT_Utils_H_
