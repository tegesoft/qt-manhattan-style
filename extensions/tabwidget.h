#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "qt-manhattan-style_global.hpp"
#include <QVector>
#include <QWidget>

namespace Manhattan {

class QTMANHATTANSTYLESHARED_EXPORT TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = 0);
    ~TabWidget();

    void setTitle(const QString &title);
    QString title() const { return m_title; }

    void addTab(const QString &name);
    void insertTab(int index, const QString &name);
    void removeTab(int index);
    int tabCount() const;

    int currentIndex() const;
    void setCurrentIndex(int index);

signals:
    void currentIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool event(QEvent *event);
    QSize minimumSizeHint() const;

private:
    struct Tab {
        QString name;
    };
    enum HitArea { HITNOTHING, HITOVERFLOW, HITTAB };
    QPair<TabWidget::HitArea, int> convertPosToTab(QPoint pos);

    QString m_title;
    QList<Tab> m_tabs;
    int m_currentIndex;
    QVector<int> m_currentTabIndices;
    int m_lastVisibleIndex;
};

} // namespace Manhattan

#endif // TABWIDGET_H
