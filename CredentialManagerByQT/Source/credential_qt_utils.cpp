#include <QtWidgets/QLabel>

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

    QLabel * MakeMarkLabel(QWidget * parent)
    {
        QLabel* lab = new QLabel("*", parent);
        lab->setAlignment(Qt::AlignCenter);
        lab->setFixedSize(16, 20);
        lab->setStyleSheet("QLabel{ background:transparent; color:red; }");

        return lab;
    }

}

QT_END_NAMESPACE
