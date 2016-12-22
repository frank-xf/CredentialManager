#include <QtGui/QGuiApplication>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "credential_qt_utils.h"

QT_BEGIN_NAMESPACE

namespace ui_utils
{

	QFont MakeFont(unsigned int size, bool bold)
	{
		QFont font = QGuiApplication::font();
		font.setPointSize(ui_utils::def_text_size);
		font.setBold(bold);

		return font;
	}

    void SetBackgroundColor(QWidget * pView, const QColor & color)
    {
        pView->setAutoFillBackground(true);

        QPalette palette;
        palette.setColor(QPalette::Background, color);
        pView->setPalette(palette);
    }

    QLabel* MakeLabel(QWidget* parent, unsigned int h, Qt::Alignment align, QColor clr, bool bold, unsigned int size)
    {
        QLabel* lab = new QLabel(parent);
        lab->setAlignment(align);
        lab->setFixedHeight(h);
		lab->setFont(MakeFont(size, bold));
        lab->setStyleSheet("QLabel{ background:transparent; color:" + clr.name() + "; }");

        return lab;
    }

    QLabel * MakeMarkLabel(QWidget * parent, unsigned int w)
    {
        QLabel* lab = new QLabel("*", parent);
        lab->setAlignment(Qt::AlignCenter);
        lab->setFixedSize(w, def_widget_h);
		lab->setFont(MakeFont());
        lab->setStyleSheet("QLabel{ background:transparent; color:red; }");

        return lab;
    }

    QLabel* MakeStaticLabel(QWidget* parent, const QColor& clr)
    {
        return MakeLabel(parent, def_widget_h, Qt::AlignVCenter | Qt::AlignRight, clr, false, def_text_size);
    }

    QLabel * MakeDynamicLabel(QWidget * parent, const QColor& clr, Qt::Alignment align)
    {
        return MakeLabel(parent, def_widget_h, align, clr, false, def_text_size);
    }

    QLineEdit * MakeLineEdit(QWidget * parent, unsigned int w, const QColor& clr)
    {
        QLineEdit* line = new QLineEdit(parent);
        line->setFixedSize(w, def_widget_h);
		line->setFont(MakeFont());
        line->setStyleSheet("QLineEdit{ color:" + clr.name() + "; }");

        return line;
    }

    QLineEdit * MakePasswordLine(QWidget * parent, unsigned int w)
    {
        QLineEdit* line = new QLineEdit(parent);
        line->setFixedSize(w, def_widget_h);
		line->setFont(MakeFont());
        line->setEchoMode(QLineEdit::Password);
        line->setContextMenuPolicy(Qt::NoContextMenu);
        line->setStyleSheet("QLineEdit{ lineedit-password-character:42; }");

        return line;
    }

    QLineEdit * MakeShowLine(QWidget * parent, const QColor & clr)
    {
        QLineEdit* line = new QLineEdit(parent);
        line->setReadOnly(true);
		line->setFont(MakeFont());
        line->setFixedHeight(def_widget_h);
        line->setStyleSheet("QLineEdit{ border:none; background:transparent; color:" + clr.name() + "; }");

        return line;
    }

    QPushButton* MakeButton(QWidget* parent, unsigned int w)
    {
        QPushButton* button = new QPushButton(parent);
        button->setFixedSize(w, def_widget_h);
		button->setFont(MakeFont());
        button->setStyleSheet("QPushButton{ border:none; background-color:#40B040; color:white; }\n"
            "QPushButton:hover{ background-color:#FF4000; color:white; }\n"
            "QPushButton:pressed{ background-color:#4080FF; color:white; }");

        return button;
    }

    QHBoxLayout * MakeButtonLayout(QPushButton * btnOK, QPushButton * btnCancel)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->setContentsMargins(0, 12, 0, 12);
        phLayout->setSpacing(0);
        phLayout->addStretch(1);
        phLayout->addWidget(btnOK);
        phLayout->addStretch(1);
        phLayout->addWidget(btnCancel);
        phLayout->addStretch(1);

        return phLayout;
    }

    bool ValidateName(const QString & strName)
    {
        if (strName.isEmpty()) return false;

        if (def_text_length < strName.size()) return false;

        for (auto character : strName)
        {
            if ('~' == character) return false;
            if ('!' == character) return false;
            if ('#' == character) return false;
            if ('$' == character) return false;
            if ('%' == character) return false;
            if ('^' == character) return false;
            if ('&' == character) return false;
            if ('*' == character) return false;
            if ('(' == character) return false;
            if (')' == character) return false;
            if ('[' == character) return false;
            if (']' == character) return false;
            if ('{' == character) return false;
            if ('}' == character) return false;
            if ('<' == character) return false;
            if ('>' == character) return false;
            if ('+' == character) return false;
            if ('|' == character) return false;
            if (':' == character) return false;
            if (';' == character) return false;
            if (',' == character) return false;
            if ('?' == character) return false;
            if ('/' == character) return false;
            if ('\\' == character) return false;
            if ('\'' == character) return false;
            if ('\"' == character) return false;
            if ('\n' == character) return false;
            if ('\t' == character) return false;
        }

        return true;
    }

}

QT_END_NAMESPACE
