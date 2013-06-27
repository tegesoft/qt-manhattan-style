#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "../qt-manhattan-style_global.hpp"
#include <QVector>
#include <QWidget>

class QStackedWidget;

namespace Manhattan {

class QTMANHATTANSTYLESHARED_EXPORT TabWidget : public QWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent = 0);
    ~TabWidget();

    void setTitle(const QString &title);
    QString title() const { return m_title; }

    void setFrameVisible(bool visible);

    void addTab(const QString &name, QWidget *widget, const QColor &color = Qt::black);
    void insertTab(int index, const QString &name, QWidget *widget, const QColor &color = Qt::black);
    QWidget* removeTab(int index);
    int tabCount() const;
    QString tabText(int index) const;

    int currentIndex() const;
    void setCurrentIndex(int index);

signals:
    void currentIndexChanged(int index);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool event(QEvent *event);

private:
    struct Tab {
        QString name;
        QColor color;
        QWidget* widget;
    };
    enum HitArea { HITNOTHING, HITOVERFLOW, HITTAB };
    QPair<TabWidget::HitArea, int> convertPosToTab(QPoint pos);

    QString m_title;
    QList<Tab> m_tabs;
    int m_currentIndex;
    QVector<int> m_currentTabIndices;
    int m_lastVisibleIndex;
    QStackedWidget *m_stack;
    bool m_drawFrame;
};

} // namespace Manhattan

#endif // TABWIDGET_H
