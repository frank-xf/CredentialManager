#ifndef _bnb_Credential_QT_Utils_H_
#define _bnb_Credential_QT_Utils_H_

enum class ui_size : unsigned int
{

};

#define _DeclareNoCopy(_ClassName)      private:            \
        _ClassName(const _ClassName&) = delete;             \
        _ClassName& operator=(const _ClassName&) = delete;

QT_BEGIN_NAMESPACE

namespace ui_utils
{

    void SetBackgroundColor(QWidget* pView, const QColor& color);

}

QT_END_NAMESPACE

#endif // _bnb_Credential_QT_Utils_H_
