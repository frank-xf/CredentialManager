#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "credential_qt_utils.h"

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

    QLabel * MakeMarkLabel(QWidget * parent, unsigned int w, unsigned int h)
    {
        QLabel* lab = new QLabel("*", parent);
        lab->setAlignment(Qt::AlignCenter);
        lab->setFixedSize(w, h);
        lab->setStyleSheet("QLabel{ background:transparent; color:red; }");

        return lab;
    }

    QLabel* MakeStaticLabel(QWidget* parent, unsigned int w, unsigned int h, QColor c, unsigned int u)
    {
		return MakeLabel(parent, w, h, Qt::AlignVCenter | Qt::AlignRight, c, u);
    }

	QLabel * MakeDynamicLabel(QWidget * parent, QColor c, unsigned int u)
	{
		QLabel* lab = new QLabel(parent);
		lab->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		lab->setStyleSheet("QLabel{ background:transparent; color:" + c.name() + "; }");

		QFont font;
		if (0 < u) font.setPointSize(u);
		lab->setFont(font);

		return lab;
	}

    QLabel* MakeLabel(QWidget* p, unsigned int w, unsigned int h, Qt::Alignment a, QColor c, unsigned int u)
    {
        QLabel* lab = new QLabel(p);
        lab->setAlignment(a);
        lab->setFixedSize(w, h);
        lab->setStyleSheet("QLabel{ background:transparent; color:" + c.name() + "; }");

        QFont font;
        if (0 < u) font.setPointSize(u);
        lab->setFont(font);

        return lab;
    }

    QLineEdit * MakeLineEdit(QWidget * p, unsigned int w, unsigned int h, unsigned int u)
    {
        QLineEdit* line = new QLineEdit(p);
        line->setFixedSize(w, h);

        QFont font;
        if (0 < u) font.setPointSize(u);
        line->setFont(font);

        return line;
    }

    QPushButton* MakeButton(QWidget* parent, unsigned int w, unsigned int h, unsigned int u)
    {
        QPushButton* button = new QPushButton(parent);
        button->setFixedSize(w, h);
        button->setStyleSheet("QPushButton{ border:none; background-color:#60e460; color:#2040ff; }\n"
            "QPushButton:hover{ background-color:#C02060; color:white; }\n"
            "QPushButton:pressed{ background-color:#b00020; color:white; }");

        QFont font;
        if (0 < u) font.setPointSize(u);
        button->setFont(font);

        return button;
    }
}

QT_END_NAMESPACE
