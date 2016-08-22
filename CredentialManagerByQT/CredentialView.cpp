#include <list>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

#include "CredentialView.h"

static inline QLabel* MakeLabel(const QString& strText, unsigned int width, QWidget* parent)
{
    QLabel* label = new QLabel(strText, parent);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    label->setFixedWidth(width);
    label->setMinimumHeight(28);
    label->setStyleSheet("QLabel { background-color:#A0FFA0; border:1px solid #6c6c6c; }");

    return label;
}

CredentialView::CredentialView(QWidget * parent, const bnb::Credential& src)
    : QWidget(parent)
{
    setAutoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255, 128, 128));
    setPalette(palette);
    
    MakeLabelList(src);
    LayoutView();

    setFixedSize(sizeHint());

    qDebug("w: %d, h: %d", width(), height());
}

void CredentialView::MakeLabelList(const bnb::Credential & src)
{
    for (auto ptr_platform = src->Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
    {
        _ui_element item;
        item.m_labPlatform = MakeLabel(QString::fromStdString(ptr_platform->m_Pair.m_Key.m_Key), 400, this);

        for (auto ptr_account = ptr_platform->m_Pair.m_Value.Head(); ptr_account; ptr_account = ptr_account->m_Next)
        {
            QLabel* labAccount = MakeLabel(QString::fromStdString(ptr_account->m_Pair.m_Key.m_Key), 240, this);

            item.m_listAccount.push_back(labAccount);
        }

        m_listLabel.push_back(item);
    }
}

void CredentialView::LayoutView()
{
    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setSpacing(2);
    pMainLayout->setMargin(0);

    if (m_listLabel.empty())
    {
        QLabel* labHint = new QLabel(this);
        labHint->setText(u8R"(You haven't added any credential yet)");
        pMainLayout->addWidget(labHint);

        return;
    }

    for (const auto& item : m_listLabel)
    {
        QHBoxLayout* phLayout = new QHBoxLayout;
        phLayout->addWidget(item.m_labPlatform);
        phLayout->setSpacing(0);
        phLayout->setMargin(0);

        if (!item.m_listAccount.empty())
        {
            QVBoxLayout* pvLayout = new QVBoxLayout;
            pvLayout->setSpacing(0);
            pvLayout->setMargin(0);

            for (const auto& iter : item.m_listAccount)
                pvLayout->addWidget(iter);

            phLayout->addLayout(pvLayout);
        }

        pMainLayout->addLayout(phLayout);
    }

    setLayout(pMainLayout);
}
