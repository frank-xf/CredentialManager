#ifndef _bnb_Drop_Table_H_
#define _bnb_Drop_Table_H_

QT_BEGIN_NAMESPACE

class DropTable : public QTableWidget
{
public:

    DropTable(QWidget * parent = nullptr);

private:

    void dropEvent(QDropEvent *event) override;

};

QT_END_NAMESPACE

#endif  // _bnb_Drop_Table_H_
