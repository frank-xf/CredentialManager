
#include <QtCore/QMimeData>
#include <QtGui/QDropEvent>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QSplitter>

#include "../cm-core/cm_core.h"

#include "cm_view_utils.h"

/*
#include "credential_qt_string.h"
#include "credential_qt_utils.h"

#include "Widget/AboutDialog.h"
#include "Widget/HintDialog.h"
#include "Widget/PasswordInput.h"
#include "Widget/NewDialog.h"
#include "Widget/EditDialog.h"
*/
#include "views/ToolBar.h"
#include "views/StackView.h"
#include "views/TreeView.h"
#include "views/MainView.h"

xf::credential::CredentialMgr& g_Credential()
{
    static xf::credential::CredentialMgr _just_a_credential_object_;
    return (_just_a_credential_object_);
}
/*
static inline std::string QStringToString(const QString& str)
{
    auto strLocal = str.toLocal8Bit();
    return { strLocal.data() };
}

static inline bool ValidPath(const QString& strFile)
{
    QFileInfo qfi(strFile);
    return (qfi.exists() && qfi.isFile() && 0 == qfi.suffix().compare("credential", Qt::CaseInsensitive));
}
*/
QT_BEGIN_NAMESPACE

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAcceptDrops(true);
    

    _ui.SetupUI(this);

    // g_Credential().RegisterHandle(std::bind(&MainView::CredentialUpdated, this, std::placeholders::_2, std::placeholders::_3));
}
/*
void MainView::AddCredential()
{
    _ui.m_treeView->AddCredential(g_Credential());

    _ui.m_viewStack->AddCredential(g_Credential());

    _ui.m_viewToolBar->UpdatePath(m_strFile);
}

void MainView::ClearCredential()
{
    _ui.m_treeView->ClearCredential();
    _ui.m_viewStack->ClearCredential();
}

void MainView::OpenFile(const QString & strFile)
{
    if (g_Credential().GetData().IsValid())
    {
        if (0 == strFile.compare(m_strFile, Qt::CaseInsensitive))
        {
            return;
        }
    }

    bnb::memory_type _xml;

    auto strLocal = QStringToString(strFile);
    if (!bnb::Credential::CheckFile(strLocal.c_str(), &_xml))
    {
        HintDialog(hint_type::ht_error, "You selected file invalid !", this).exec();
        return;
    }

    PasswordInput dlg(this);
    if (QDialog::Accepted == dlg.exec())
    {
        bnb::string_type password = From_QString(dlg.GetPassword());
        if (password.empty())
        {
            HintDialog(hint_type::ht_error, "Please input a password !", this).exec();
            return;
        }

        if (!bnb::Credential::Decoding(_xml, (const unsigned char*)password.c_str(), password.size() * sizeof(bnb::char_type)))
        {
            HintDialog(hint_type::ht_error, "You input password error !", this).exec();
            return;
        }

        if (!g_Credential().FromXml(_xml))
        {
            HintDialog(hint_type::ht_error, "Anaylze file failed !", this).exec();
            return;
        }

        g_Credential().SetWord(password);

        m_strFile = strFile;

        ClearCredential();
        AddCredential();
    }
}
*/
void MainView::dragEnterEvent(QDragEnterEvent *qDrag)
{
    /*
    if (qDrag->mimeData()->hasUrls())
        if (1 == qDrag->mimeData()->urls().size())
            if (ValidPath(qDrag->mimeData()->urls().front().toLocalFile()))
                qDrag->acceptProposedAction();
                */
}

void MainView::dropEvent(QDropEvent *qDrop)
{
    /*
    if (qDrop->mimeData()->hasUrls())
    {
        if (1 == qDrop->mimeData()->urls().size())
        {
            QString strFile = qDrop->mimeData()->urls().front().toLocalFile();
            if (ValidPath(strFile))
                OpenFile(strFile);
        }
    }
    */
}

void MainView::ui_type::SetupUI(MainView* pView)
{
    pView->setObjectName("MainView");
    pView->setWindowTitle("Credential Manager");
    SetBackgroundColor(pView, g_clrManView);

    QSplitter* phSplitter = new QSplitter(Qt::Horizontal, pView);
    phSplitter->setObjectName("MainSplitter");
    phSplitter->setHandleWidth(1);
    phSplitter->setOpaqueResize(true);
    phSplitter->setChildrenCollapsible(false);

    m_treeView = new TreeView(phSplitter);
    m_viewStack = new StackView(phSplitter);
    m_viewToolBar = new ToolBar(pView);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(1);
    pMainLayout->addWidget(m_viewToolBar);
    pMainLayout->addWidget(phSplitter, 1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void MainView::ui_type::RetranslateUI(MainView * pView) { }

QT_END_NAMESPACE
