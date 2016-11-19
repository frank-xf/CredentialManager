#ifndef _bnb_No_Focus_Delegate_H_
#define _bnb_No_Focus_Delegate_H_

QT_BEGIN_NAMESPACE

class NoFocusDelegate : public QStyledItemDelegate
{
    void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem itemOption(option);
        if (itemOption.state & QStyle::State_HasFocus)
            itemOption.state ^= QStyle::State_HasFocus;

        QStyledItemDelegate::paint(painter, itemOption, index);
    }
};

QT_END_NAMESPACE

#endif  // _bnb_No_Focus_Delegate_H_
