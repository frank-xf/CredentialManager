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
        lab_account_w = 240,
        lab_account_h = 28,

        lab_platform_w = 400,
        lab_platform_h = 28,
    };

    template<typename _Ty>
    constexpr unsigned int to_uint(_Ty value) { return static_cast<unsigned int>(value); }

    void SetBackgroundColor(QWidget* pView, const QColor& color);

}

QT_END_NAMESPACE

#endif // _bnb_Credential_QT_Utils_H_
