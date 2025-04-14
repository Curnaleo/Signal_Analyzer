#include "customlistview.h"

CustomListView::CustomListView(QWidget *parent)
    : QListView{parent}
{
    setAcceptDrops(true);
    setAutoFillBackground(true);
    clear();
}

void CustomListView::dragEnterEvent(QDragEnterEvent *event)
{
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();

}

void CustomListView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void CustomListView::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
    emit changed(event->mimeData());
}

void CustomListView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if ( event == nullptr )
        return;

    QModelIndex *idx = new QModelIndex(QListView::indexAt(event->pos()));

    if ( not idx->isValid() )
        return;

    emit selectedItem(idx);

}

void CustomListView::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}

void CustomListView::clear()
{
    setBackgroundRole(QPalette::Dark);

    emit changed();
}
