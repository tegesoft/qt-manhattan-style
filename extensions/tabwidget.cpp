#include "tabwidget.h"
#include "../stylehelper.h"
#include <QRect>
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QMenu>
#include <QToolTip>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QDebug>

using namespace Manhattan;

static const int MIN_LEFT_MARGIN = 50;
static const int MARGIN = 12;
static const int TAB_HEIGHT = 24;
static const int CONTENT_HEIGHT_MARGIN = 10;
static const int SELECTION_IMAGE_WIDTH = 10;
static const int SELECTION_IMAGE_HEIGHT = 20;
static const int OVERFLOW_DROPDOWN_WIDTH = TAB_HEIGHT;

static void drawFirstLevelSeparator(QPainter *painter, QPoint top, QPoint bottom)
{
    painter->setPen(QPen(QColor(Qt::white).darker(110), 0));
    painter->drawLine(top, bottom);
    painter->setPen(QPen(Qt::gray, 0));
    painter->drawLine(top - QPoint(1,0), bottom - QPoint(1,0));
}

TabWidget::TabWidget(QWidget *parent) :
    QWidget(parent),
    m_currentIndex(-1),
    m_lastVisibleIndex(-1),
    m_stack(0),
    m_drawFrame(false)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, TAB_HEIGHT + CONTENT_HEIGHT_MARGIN + 1, 0, 0);
    m_stack = new QStackedWidget;
    layout->addWidget(m_stack);
    setLayout(layout);

    connect(this, SIGNAL(currentIndexChanged(int)), m_stack, SLOT(setCurrentIndex(int)));
}

TabWidget::~TabWidget()
{
}

int TabWidget::currentIndex() const
{
    return m_currentIndex;
}

void TabWidget::setCurrentIndex(int index)
{
    Q_ASSERT(index < m_tabs.size());
    if (index == m_currentIndex)
        return;
    m_currentIndex = index;
    emit currentIndexChanged(m_currentIndex);
    update();
}

void TabWidget::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void TabWidget::setFrameVisible(bool visible)
{
    if (visible != m_drawFrame) {
        m_drawFrame = visible;
        update();
    }
}

void TabWidget::addTab(const QString &name, QWidget *widget)
{
    Q_ASSERT(widget);
    Tab tab;
    tab.name = name;
    tab.widget = widget;
    m_tabs.append(tab);
    m_stack->addWidget(widget);
    if (m_currentIndex == -1) {
        m_currentIndex = 0;
        emit currentIndexChanged(m_currentIndex);
    }
    update();
}

void TabWidget::insertTab(int index, const QString &name, QWidget *widget)
{
    Q_ASSERT(widget);
    Tab tab;
    tab.name = name;
    tab.widget = widget;
    m_tabs.insert(index, tab);
    m_stack->insertWidget(index, widget);
    if (m_currentIndex >= index) {
        ++m_currentIndex;
        emit currentIndexChanged(m_currentIndex);
    }
    update();
}

void TabWidget::removeTab(int index)
{
    m_tabs.takeAt(index);
    if (index <= m_currentIndex) {
        --m_currentIndex;
        if (m_currentIndex < 0 && m_tabs.size() > 0)
            m_currentIndex = 0;
        if (m_currentIndex < 0) {
            emit currentIndexChanged(-1);
        } else {
            emit currentIndexChanged(m_currentIndex);
        }
    }
    update();
}

int TabWidget::tabCount() const
{
    return m_tabs.size();
}

QString TabWidget::tabText(int index) const
{
    return m_tabs.value(index).name;
}

/// Converts a position to the tab that is undeneath
/// If HitArea is tab, then the second part of the pair
/// is the tab number
QPair<TabWidget::HitArea, int> TabWidget::convertPosToTab(QPoint pos)
{
    if (pos.y() < TAB_HEIGHT) {
        // on the top level part of the bar
        int eventX = pos.x();
        QFontMetrics fm(font());
        int x = m_title.isEmpty() ? 0 :
                2 * MARGIN + qMax(fm.width(m_title), MIN_LEFT_MARGIN);

        if (eventX <= x)
            return qMakePair(HITNOTHING, -1);
        int i;
        for (i = 0; i <= m_lastVisibleIndex; ++i) {
            int otherX = x + 2 * MARGIN + fm.width(m_tabs.at(
                    m_currentTabIndices.at(i)).name);
            if (eventX > x && eventX < otherX) {
                break;
            }
            x = otherX;
        }
        if (i <= m_lastVisibleIndex) {
            return qMakePair(HITTAB, i);
        } else if (m_lastVisibleIndex < m_tabs.size() - 1) {
            // handle overflow menu
            if (eventX > x && eventX < x + OVERFLOW_DROPDOWN_WIDTH) {
                return qMakePair(HITOVERFLOW, -1);
            }
        }
    }

    return qMakePair(HITNOTHING, -1);
}

void TabWidget::mousePressEvent(QMouseEvent *event)
{
    // todo:
    // the even wasn't accepted/ignored in a consistent way
    // now the event is accepted everywhere were it hitted something interesting
    // and otherwise ignored
    // should not make any difference
    QPair<HitArea, int> hit = convertPosToTab(event->pos());
    if (hit.first == HITTAB) {
        if (m_currentIndex != m_currentTabIndices.at(hit.second)) {
            m_currentIndex = m_currentTabIndices.at(hit.second);
            update();
            event->accept();
            emit currentIndexChanged(m_currentIndex);
            return;
        }
    } else if (hit.first == HITOVERFLOW) {
        QMenu overflowMenu;
        QList<QAction *> actions;
        for (int i = m_lastVisibleIndex + 1; i < m_tabs.size(); ++i) {
            actions << overflowMenu.addAction(m_tabs.at(m_currentTabIndices.at(i)).name);
        }
        if (QAction *action = overflowMenu.exec(event->globalPos())) { // todo used different position before
            int index = m_currentTabIndices.at(actions.indexOf(action) + m_lastVisibleIndex + 1);
            if (m_currentIndex != index) {
                m_currentIndex = index;
                update();
                event->accept();
                emit currentIndexChanged(m_currentIndex);
                return;
            }
        }
    }

    event->ignore();
}

void TabWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QRect r = rect();

    QColor baseColor = palette().window().color();
    QColor backgroundColor = baseColor.darker(110);
    QColor lineColor = backgroundColor.darker();

    // draw top level tab bar
    r.setHeight(TAB_HEIGHT);

    // Fill top bar background
    if (!m_drawFrame)
        painter.fillRect(r, backgroundColor);

    QFontMetrics fm(font());
    int baseline = (r.height() + fm.ascent()) / 2 - 1;

    // top level title
    if (!m_title.isEmpty()) {
        painter.setPen(Utils::StyleHelper::panelTextColor());
        painter.drawText(MARGIN, baseline, m_title);
    }
    // draw content background
    QRect content(r);
    content.setTop(r.height());
    content.setHeight(height() - r.height());
    painter.fillRect(content, baseColor);

    // frames
    if (m_drawFrame) {
        painter.setPen(lineColor);
        painter.drawRect(content.adjusted(0, 0, -1, -1));
    }
    else {
        painter.setPen(lineColor);
        painter.drawLine(r.left(), r.height(), r.right(), r.height());
    }

    // top level tabs
    int x = m_title.isEmpty() ? 0 :
            2 * MARGIN + qMax(fm.width(m_title), MIN_LEFT_MARGIN);

    // calculate sizes
    QList<int> nameWidth;
    int width = x;
    int indexSmallerThanOverflow = -1;
    int indexSmallerThanWidth = -1;
    for (int i = 0; i < m_tabs.size(); ++i) {
        const Tab& tab = m_tabs.at(i);
        int w = fm.width(tab.name);
        nameWidth << w;
        width += 2 * MARGIN + w;
        if (width < r.width())
            indexSmallerThanWidth = i;
        if (width < r.width() - OVERFLOW_DROPDOWN_WIDTH)
            indexSmallerThanOverflow = i;
    }
    m_lastVisibleIndex = -1;
    m_currentTabIndices.resize(m_tabs.size());
    if (indexSmallerThanWidth == m_tabs.size() - 1) {
        // => everything fits
        for (int i = 0; i < m_tabs.size(); ++i)
            m_currentTabIndices[i] = i;
        m_lastVisibleIndex = m_tabs.size()-1;
    } else {
        // => we need the overflow thingy
        if (m_currentIndex <= indexSmallerThanOverflow) {
            // easy going, simply draw everything that fits
            for (int i = 0; i < m_tabs.size(); ++i)
                m_currentTabIndices[i] = i;
            m_lastVisibleIndex = indexSmallerThanOverflow;
        } else {
            // now we need to put the current tab into
            // visible range. for that we need to find the place
            // to put it, so it fits
            width = x;
            int index = 0;
            bool handledCurrentIndex = false;
            for (int i = 0; i < m_tabs.size(); ++i) {
                if (index != m_currentIndex) {
                    if (!handledCurrentIndex) {
                        // check if enough room for current tab after this one
                        if (width + 2 * MARGIN + nameWidth.at(index)
                                + 2 * MARGIN + nameWidth.at(m_currentIndex)
                                < r.width() - OVERFLOW_DROPDOWN_WIDTH) {
                            m_currentTabIndices[i] = index;
                            ++index;
                            width += 2 * MARGIN + nameWidth.at(index);
                        } else {
                            m_currentTabIndices[i] = m_currentIndex;
                            handledCurrentIndex = true;
                            m_lastVisibleIndex = i;
                        }
                    } else {
                        m_currentTabIndices[i] = index;
                        ++index;
                    }
                } else {
                    ++index;
                    --i;
                }
            }
        }
    }

    // actually draw top level tabs
    for (int i = 0; i <= m_lastVisibleIndex; ++i) {
        int actualIndex = m_currentTabIndices.at(i);
        const Tab& tab = m_tabs.at(actualIndex);

        painter.setPen(lineColor);

        // top
        if (m_drawFrame) {
            painter.drawLine(x, 0, x + 2 * MARGIN + fm.width(tab.name), 0);
        }

        if (actualIndex == m_currentIndex) {            
            // tab background
            painter.fillRect(QRect(x, 1,
                                   2 * MARGIN + fm.width(tab.name),
                                   r.height() + 1),
                             baseColor);

            // Left
            if (actualIndex == 0) {
                if (m_drawFrame)
                    painter.drawLine(x, 0, x, r.height()+1);
            }
            else {
                painter.drawLine(x - 1, 0, x - 1, r.height() - 1);
                painter.setPen(QColor(255, 255, 255, 170));
                painter.drawLine(x, 1, x, r.height());
            }

            x += MARGIN;
            painter.setPen(Qt::black);
            painter.drawText(x, baseline, tab.name);
            x += nameWidth.at(actualIndex);
            x += MARGIN;
            painter.setPen(lineColor);
            painter.drawLine(x, 0, x, r.height() - 1);
            painter.setPen(QColor(0, 0, 0, 20));
            painter.drawLine(x + 1, 0, x + 1, r.height() - 1);
            painter.setPen(QColor(255, 255, 255, 170));
            painter.drawLine(x - 1, 1, x - 1, r.height());
        } else {
            // tab background
            painter.fillRect(QRect(x + 1, 1,
                                   2 * MARGIN + fm.width(tab.name),
                                   r.height()-1),
                             backgroundColor);

            // left
            if (m_drawFrame && (actualIndex == 0))
                painter.drawLine(x, 0, x, r.height());

            x += MARGIN;
            painter.setPen(QColor(0, 0, 0, 190));
            painter.drawText(x + 1, baseline, tab.name);
            x += nameWidth.at(actualIndex);
            x += MARGIN;
            if (!m_drawFrame || (actualIndex != m_lastVisibleIndex))
                drawFirstLevelSeparator(&painter, QPoint(x, 1), QPoint(x, r.height()-1));
        }

        // end of tabs right vertical line
        if (m_drawFrame && (actualIndex == m_lastVisibleIndex)) {
            painter.setPen(lineColor);
            painter.drawLine(x, 0, x, r.height() - 1);
        }
    }

    // draw overflow button
    if (m_lastVisibleIndex < m_tabs.size() - 1) {
        QStyleOption opt;
        opt.rect = QRect(x, 0, OVERFLOW_DROPDOWN_WIDTH - 1, r.height() - 1);
        style()->drawPrimitive(QStyle::PE_IndicatorArrowDown,
                               &opt, &painter, this);
        drawFirstLevelSeparator(&painter, QPoint(x + OVERFLOW_DROPDOWN_WIDTH, 0),
                                QPoint(x + OVERFLOW_DROPDOWN_WIDTH, r.height()-1));
    }
}

bool TabWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpevent = static_cast<QHelpEvent*>(event);
        QPair<HitArea, int> hit = convertPosToTab(helpevent->pos());
        if (hit.first == HITTAB)
            QToolTip::showText(helpevent->globalPos(), m_tabs.at(m_currentTabIndices.at(hit.second)).name, this);
        else
            QToolTip::showText(helpevent->globalPos(), QString(), this);
    }
    return QWidget::event(event);
}
