#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>

#include "credential_qt_utils.h"

#include "Major/ContentView.h"

ContentView::ContentView(QWidget * parent) : QWidget(parent)
{
    ui_utils::SetBackgroundColor(this, Qt::white);

    setMinimumSize(400, 200);

    QHBoxLayout* pMainLayout = new QHBoxLayout;
    pMainLayout->addWidget(new QLabel("nothing", this));

    setLayout(pMainLayout);

}
