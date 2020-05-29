
#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QMimeData>
#include <QtGui/QDropEvent>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QSplitter>

#include "../../cm-core/cm_core.h"
#include "../../cm-encrypt/cm_encrypt.h"

#include "../cm_view_utils.h"

/*"

#include "Widget/AboutDialog.h"
#include "Widget/NewDialog.h"
#include "Widget/EditDialog.h"
*/

#include "../widgets/HintDialog.h"
#include "../widgets/LoginDialog.h"

#include "ToolBar.h"
#include "NodeView.h"
#include "TreeView.h"
#include "MainView.h"

QT_BEGIN_NAMESPACE

inline bool CheckPath(const QString& strFile)
{
    QFileInfo qfi(strFile);
    return (qfi.exists() && qfi.isFile() && 0 == qfi.suffix().compare("credential", Qt::CaseInsensitive));
}

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAcceptDrops(true);
    

    _ui.SetupUI(this);

    // g_Credential().RegisterHandle(std::bind(&MainView::CredentialUpdated, this, std::placeholders::_2, std::placeholders::_3));
}

void MainView::Init()
{
    show();

    if (auto args = QCoreApplication::arguments(); 1 < args.size())
        if (CheckPath(args[1]))
            OpenFile(args[1]);
}

void MainView::OpenFile(const QString& strFile)
{
    if (0 == strFile.compare(m_FilePath, Qt::CaseInsensitive))
        return;

    xf::credential::encrypt::memory_t data;
    if (!xf::credential::encrypt::ValidateFile(FromQString(strFile).c_str(), data))
    {
        HintDialog(hint_type::ht_error, "You selected file invalid !", this).exec();
        return;
    }

    LoginDialog dlg(this);
    if (QDialog::Accepted == dlg.exec())
    {
        auto username = FromQString(dlg.GetUsername());
        if (username.empty())
        {
            HintDialog(hint_type::ht_error, "Please input a username !", this).exec();
            return;
        }

        auto password = FromQString(dlg.GetPassword());
        if (password.empty())
        {
            HintDialog(hint_type::ht_error, "Please input a password !", this).exec();
            return;
        }

        if (!xf::credential::encrypt::Decrypt(data, username.c_str(), username.size(), password.c_str(), password.size()))
        {
            HintDialog(hint_type::ht_error, "The username or password you inputed is invalid !", this).exec();
            return;
        }

        if (!m_Credential.Deserialize(xf::credential::encrypt::memory_to_string(data)))
        {
            HintDialog(hint_type::ht_error, "Anaylze file failed !", this).exec();
            return;
        }

        // g_Credential().SetWord(password);

        m_FilePath = strFile;

        InitCredential();
    }
}

void MainView::InitCredential()
{
    _ui.m_treeView->InitCredential(m_Credential);

    _ui.m_viewStack->Show(m_Credential);

    _ui.m_viewToolBar->UpdatePath(m_FilePath);
}

bool MainView::OnLoad()
{
    QString strFile = QFileDialog::getOpenFileName(
        this, "Please select a credential file", ".", "credential file(*.credential)");

    if (!strFile.isEmpty())
    {
        OpenFile(strFile);
        return true;
    }

    return false;
}



/*
void MainView::ClearCredential()
{
    _ui.m_treeView->ClearCredential();
    _ui.m_viewStack->ClearCredential();
}
*/
void MainView::dragEnterEvent(QDragEnterEvent *qDrag)
{
    if (qDrag->mimeData()->hasUrls())
        if (1 == qDrag->mimeData()->urls().size())
            if (CheckPath(qDrag->mimeData()->urls().front().toLocalFile()))
                qDrag->acceptProposedAction();
}

void MainView::dropEvent(QDropEvent *qDrop)
{
    if (qDrop->mimeData()->hasUrls())
    {
        if (1 == qDrop->mimeData()->urls().size())
        {
            QString strFile = qDrop->mimeData()->urls().front().toLocalFile();
            if (CheckPath(strFile))
                OpenFile(strFile);
        }
    }
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
    m_viewStack = new NodeView(phSplitter);
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
