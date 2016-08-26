#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace ui_utils
{

    void SetBackgroundColor(QWidget * pView, const QColor & color)
    {
        pView->setAutoFillBackground(true);

        QPalette palette;
        palette.setColor(QPalette::Background, color);
        pView->setPalette(palette);
    }

}

QT_END_NAMESPACE
