#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H

#include <QListView>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

class CustomListView : public QListView
{
    Q_OBJECT
public:
    explicit CustomListView(QWidget *parent = nullptr);

public slots:
    void clear();

signals:
    void changed(const QMimeData *mimeData = nullptr);
    void selectedItem(QModelIndex *index);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // CUSTOMLISTVIEW_H
