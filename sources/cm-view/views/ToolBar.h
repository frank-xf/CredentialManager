#pragma once

QT_BEGIN_NAMESPACE

class ToolBar : public QWidget
{
    struct ui_type
    {
        QLineEdit* m_editPath;
        QPushButton* m_btnNew;
        QPushButton* m_btnOpen;
        QPushButton* m_btnAbout;

        void SetupUI(QWidget* pView);
        void RetranslateUI(QWidget* pView);
    };

public:

    ToolBar(QWidget * parent = nullptr);

    const ui_type& UI() const { return (_ui); }

    void UpdatePath(const QString& strPath);

private:

    void OnClickedNew();
    void OnClickedOpen();
    void OnClickedAbout();

    ui_type _ui;

};

QT_END_NAMESPACE

