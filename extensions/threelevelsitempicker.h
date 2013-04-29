#ifndef THREELEVELSITEMPICKER_H
#define THREELEVELSITEMPICKER_H

#include "../qt-manhattan-style_global.hpp"
#include <QWidget>
#include <QListWidget>
#include <QMap>

class QStatusBar;

namespace Manhattan {

class ListWidget : public QListWidget
{
    Q_OBJECT

public:
    ListWidget(QWidget *parent = 0);
    QSize sizeHint() const;
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void setMaxCount(int maxCount);

signals:
    void itemReselected();

protected:
    void mouseReleaseEvent(QMouseEvent* event);

private:
    int m_maxCount;
};

class QTMANHATTANSTYLESHARED_EXPORT ThreeLevelsItemPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ThreeLevelsItemPicker(const QString& level1Title,
                                   const QString& level2Title,
                                   const QString& level3Title,
                                   QMap<QString, QStringList> level2ItemsFromlevel1,
                                   QMap<QString, QStringList> level3ItemsFromLevel2,
                                   QIcon icon,
                                   QWidget* anchorWidget);

    inline QAction* triggerAction() const { return m_triggerAction; }
    void setVisible(bool visible);
    void setMaxVisibleItemCount(int count);

    QString level1() const;
    QString level2() const;
    QString level3() const;

    void setLevel1Item(const QString& name);
    void setLevel2Item(const QString& name);
    void setLevel3Item(const QString& name);

signals:
    void itemChanged();

private slots:
    void setLevel1(const QString& name);
    void setLevel2(const QString& name);
    void setLevel3(const QString& name);
    void smoothHide();

private:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    QWidget* m_anchorWidget;
    QAction* m_triggerAction;
    QMap<QString, QStringList> m_level2ItemsFromlevel1;
    QMap<QString, QStringList> m_level3ItemsFromLevel2;
    ListWidget* m_level1Items;
    ListWidget* m_level2Items;
    ListWidget* m_level3Items;
};

} // namespace Manhattan

#endif // THREELEVELSITEMPICKER_H
