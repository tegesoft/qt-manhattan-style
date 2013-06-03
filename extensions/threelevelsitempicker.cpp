#include "threelevelsitempicker.h"
#include "../stylehelper.h"
#include "../styledbar.h"
#include <QAction>
#include <QStyle>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QItemDelegate>
#include <QLabel>
#include <QTimer>

namespace
{

QWidget *createTitleLabel(const QString &text)
{
    Manhattan::StyledBar *bar = new Manhattan::StyledBar;
    bar->setSingleRow(true);
    QVBoxLayout *toolLayout = new QVBoxLayout(bar);
    toolLayout->setMargin(0);
    toolLayout->setSpacing(0);

    QLabel *l = new QLabel(text);
    l->setIndent(6);
    QFont f = l->font();
    f.setBold(true);
    l->setFont(f);
    toolLayout->addWidget(l);

    int panelHeight = l->fontMetrics().height() + 12;
    bar->ensurePolished(); // Required since manhattanstyle overrides height
    bar->setFixedHeight(panelHeight);
    return bar;
}

}

class OnePixelGreyLine : public QWidget
{
public:
    OnePixelGreyLine(QWidget *parent = 0)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        setMinimumWidth(1);
        setMaximumWidth(1);
    }
    void paintEvent(QPaintEvent *e)
    {
        Q_UNUSED(e);
        QPainter p(this);
        p.fillRect(contentsRect(), QColor(160, 160, 160, 255));
    }
};

using namespace Manhattan;

ThreeLevelsItemPicker::ThreeLevelsItemPicker(const QString& level1Title,
                                             const QString& level2Title,
                                             const QString& level3Title,
                                             QMap<QString, QStringList> level2ItemsFromlevel1,
                                             QMap<QString, QStringList> level3ItemsFromLevel2,
                                             QIcon icon,
                                             QWidget* anchorWidget)
    : QWidget()
    , m_anchorWidget(anchorWidget)
    , m_triggerAction(0)
    , m_level2ItemsFromlevel1(level2ItemsFromlevel1)
    , m_level3ItemsFromLevel2(level3ItemsFromLevel2)
{
    // Style
    QPalette p = palette();
    p.setColor(QPalette::Text, QColor(255, 255, 255, 160));
    setPalette(p);
    setProperty("panelwidget", true);
    setContentsMargins(QMargins(0, 0, 1, 8));
    setWindowFlags(Qt::Popup);

    // List widgets
    m_level1Items = new ListWidget;
    m_level1Items->setMaxCount(5);
    m_level1Items->addItems(m_level2ItemsFromlevel1.keys());
    connect(m_level1Items, SIGNAL(currentTextChanged(QString)), this, SLOT(setLevel1(QString)));
    m_level2Items = new ListWidget;
    m_level2Items->setMaxCount(5);
    connect(m_level2Items, SIGNAL(currentTextChanged(QString)), this, SLOT(setLevel2(QString)));
    m_level3Items = new ListWidget;
    m_level3Items->setMaxCount(5);
    connect(m_level3Items, SIGNAL(currentTextChanged(QString)), this, SLOT(setLevel3(QString)));
    connect(m_level3Items, SIGNAL(itemReselected()), this, SLOT(hide()));

    QGridLayout *grid = new QGridLayout(this);
    grid->setMargin(0);
    grid->setSpacing(0);

    // Level 1 column
    {
        QWidget* title = ::createTitleLabel(level1Title);
        grid->addWidget(title, 0, 0);
        grid->addWidget(new OnePixelGreyLine, 0, 1);
        grid->addWidget(m_level1Items, 1, 0);
        grid->addWidget(new OnePixelGreyLine, 1, 1);
    }

    // Level 2 column
    {
        QWidget* title = ::createTitleLabel(level2Title);
        grid->addWidget(title, 0, 2);
        grid->addWidget(new OnePixelGreyLine, 0, 3);
        grid->addWidget(m_level2Items, 1, 2);
        grid->addWidget(new OnePixelGreyLine, 1, 3);
    }

    // Level 3 column
    {
        QWidget* title = ::createTitleLabel(level3Title);
        grid->addWidget(title, 0, 4);
        grid->addWidget(m_level3Items, 1, 4);
    }

    this->setLayout(grid);

    // Mode selector action
    m_triggerAction = new QAction(this);
    m_triggerAction->setCheckable(true);
    m_triggerAction->setEnabled(true);
    m_triggerAction->setIcon(icon);
    m_triggerAction->setProperty("titledAction", true); // will add the arrow !
    connect(m_triggerAction, SIGNAL(triggered()), this, SLOT(show()));
}

void ThreeLevelsItemPicker::setVisible(bool visible)
{
    if (visible)
    {
        QPoint moveTo = m_anchorWidget->mapToGlobal(QPoint(m_anchorWidget->width() + 1,0));
        move(moveTo);
    }

    QWidget::setVisible(visible);
    m_triggerAction->setChecked(visible);
}

void ThreeLevelsItemPicker::setMaxVisibleItemCount(int count)
{
    m_level1Items->setMaxCount(count);
    m_level2Items->setMaxCount(count);
    m_level3Items->setMaxCount(count);
}

QString ThreeLevelsItemPicker::level1() const
{
    QListWidgetItem* item = m_level1Items->currentItem();
    return item ? item->text() : QString();
}

QString ThreeLevelsItemPicker::level2() const
{
    QListWidgetItem* item = m_level2Items->currentItem();
    return item ? item->text() : QString();
}

QString ThreeLevelsItemPicker::level3() const
{
    QListWidgetItem* item = m_level3Items->currentItem();
    return item ? item->text() : QString();
}

namespace
{

QListWidgetItem* find(const ListWidget* list, const QString& name)
{
    for (int row = 0; row < list->count(); ++row)
    {
        QListWidgetItem* item = list->item(row);
        if (item->text() == name)
            return item;
    }

    return 0;
}

} // Anonymous namespace

void ThreeLevelsItemPicker::setLevel1Item(const QString& name)
{
    QListWidgetItem* item = ::find(m_level1Items, name);
    if (item)
        m_level1Items->setCurrentItem(item);
}

void ThreeLevelsItemPicker::setLevel2Item(const QString& name)
{
    QListWidgetItem* item = ::find(m_level2Items, name);
    if (item)
        m_level2Items->setCurrentItem(item);
}

void ThreeLevelsItemPicker::setLevel3Item(const QString& name)
{
    QListWidgetItem* item = ::find(m_level3Items, name);
    if (item)
        m_level3Items->setCurrentItem(item);
}

void ThreeLevelsItemPicker::setLevel1(const QString& name)
{
    m_level2Items->clear();
    m_level2Items->addItems(m_level2ItemsFromlevel1.value(name));
}

void ThreeLevelsItemPicker::setLevel2(const QString& name)
{
    m_level3Items->clear();
    m_level3Items->addItems(m_level3ItemsFromLevel2.value(name));
}

void ThreeLevelsItemPicker::setLevel3(const QString& name)
{
    if (name.isEmpty())
        return;

    m_triggerAction->setProperty("heading", name);
    QListWidgetItem* item = m_level2Items->currentItem();
    m_triggerAction->setProperty("subtitle", item ? item->text() : "");
    smoothHide();
    emit itemChanged();
}

void ThreeLevelsItemPicker::smoothHide()
{
    QTimer::singleShot(200, this, SLOT(hide()));
}

// This is a workaround for the problem that Windows
// will let the mouse events through when you click
// outside a popup to close it. This causes the popup
// to open on mouse release if you hit the button, which
//
//
// A similar case can be found in QComboBox
void ThreeLevelsItemPicker::mousePressEvent(QMouseEvent *e)
{
    setAttribute(Qt::WA_NoMouseReplay);
    QWidget::mousePressEvent(e);
}

void ThreeLevelsItemPicker::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Manhattan::Utils::StyleHelper::borderColor());
    painter.drawLine(rect().topRight(), rect().bottomRight());

    QRect bottomRect(0, rect().height() - 8, rect().width(), 8);
    static QImage image(QLatin1String(":/projectexplorer/images/targetpanel_bottom.png"));
    Manhattan::Utils::StyleHelper::drawCornerImage(image, &painter, bottomRect, 1, 1, 1, 1);
}

////////
// TargetSelectorDelegate
////////
class TargetSelectorDelegate : public QItemDelegate
{
public:
    TargetSelectorDelegate(QObject *parent) : QItemDelegate(parent) { }
private:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    mutable QImage selectionGradient;
};

QSize TargetSelectorDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(190, 30);
}

void TargetSelectorDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    painter->save();
    painter->setClipping(false);

    if (selectionGradient.isNull())
        selectionGradient.load(QLatin1String(":/projectexplorer/images/targetpanel_gradient.png"));

    if (option.state & QStyle::State_Selected) {
        QColor color =(option.state & QStyle::State_HasFocus) ?
                      option.palette.highlight().color() :
                      option.palette.dark().color();
        painter->fillRect(option.rect, color.darker(140));
        Manhattan::Utils::StyleHelper::drawCornerImage(selectionGradient, painter, option.rect.adjusted(0, 0, 0, -1), 5, 5, 5, 5);
        painter->setPen(QColor(255, 255, 255, 60));
        painter->drawLine(option.rect.topLeft(), option.rect.topRight());
        painter->setPen(QColor(255, 255, 255, 30));
        painter->drawLine(option.rect.bottomLeft() - QPoint(0,1), option.rect.bottomRight() -  QPoint(0,1));
        painter->setPen(QColor(0, 0, 0, 80));
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    }

    QFontMetrics fm(option.font);
    QString text = index.data(Qt::DisplayRole).toString();
    painter->setPen(QColor(255, 255, 255, 160));
    QString elidedText = fm.elidedText(text, Qt::ElideMiddle, option.rect.width() - 12);
    if (elidedText != text)
        const_cast<QAbstractItemModel *>(index.model())->setData(index, text, Qt::ToolTipRole);
    else
        const_cast<QAbstractItemModel *>(index.model())->setData(index, QString(), Qt::ToolTipRole);
    painter->drawText(option.rect.left() + 6, option.rect.top() + (option.rect.height() - fm.height()) / 2 + fm.ascent(), elidedText);

    painter->restore();
}

////////
// ListWidget
////////
ListWidget::ListWidget(QWidget *parent)
    : QListWidget(parent), m_maxCount(0)
{
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAlternatingRowColors(false);
    setFocusPolicy(Qt::WheelFocus);
    setItemDelegate(new TargetSelectorDelegate(this));
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // Style
    QFile file(":/extensions/qss/threelevelsitempicker-listwidget.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);
}

QSize ListWidget::sizeHint() const
{
    int height = m_maxCount * 30;
    int width = 190;
    QSize size(width, height);
    return size;
}

void ListWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        focusPreviousChild();
    else if (event->key() == Qt::Key_Right)
        focusNextChild();
    else
        QListWidget::keyPressEvent(event);
}

void ListWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() != Qt::LeftArrow && event->key() != Qt::RightArrow)
        QListWidget::keyReleaseEvent(event);
}

void ListWidget::setMaxCount(int maxCount)
{
    // Note: the current assumption is that, this is not called while the listwidget is visible
    // Otherwise we would need to add code to MiniProjectTargetSelector reacting to the
    // updateGeometry (which then would jump ugly)
    m_maxCount = maxCount;
    updateGeometry();
}

void ListWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QListWidgetItem* item = currentItem();
    QListWidget::mouseReleaseEvent(event);
    if (item == currentItem())
    {
        emit itemReselected();
    }
}
