#include "simpleprogressbar.h"
#include "../stylehelper.h"

#include <QPainter>
#include <QBrush>
#include <QColor>

using namespace Manhattan;

SimpleProgressBar::SimpleProgressBar(int width, int height, QWidget *parent)
    : QWidget(parent)
    , m_error(false)
    , m_progressWidth(width)
    , m_progressHeight(height + ((height % 2) + 1) % 2) // make odd
    , m_minimum(1)
    , m_maximum(100)
    , m_value(1)
    , m_finished(false)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

SimpleProgressBar::~SimpleProgressBar()
{
}

void SimpleProgressBar::reset()
{
    m_value = m_minimum;
    m_finished = false;
    m_error = false;
    update();
}

void SimpleProgressBar::setRange(int minimum, int maximum)
{
    m_minimum = minimum;
    m_maximum = maximum;
    if (m_value < m_minimum || m_value > m_maximum)
        m_value = m_minimum;
    update();
}

void SimpleProgressBar::setValue(int value)
{
    if (m_value == value
            || m_value < m_minimum
            || m_value > m_maximum) {
        return;
    }
    m_value = value;
    update();
}

void SimpleProgressBar::setFinished(bool b)
{
    if (b == m_finished)
        return;
    m_finished = b;
    update();
}

bool SimpleProgressBar::hasError() const
{
    return m_error;
}

void SimpleProgressBar::setError(bool on)
{
    m_error = on;
    update();
}

QSize SimpleProgressBar::sizeHint() const
{
    QSize s;
    s.setWidth(m_progressWidth + 6);
    s.setHeight(m_progressHeight + 6);
    return s;
}

void SimpleProgressBar::paintEvent(QPaintEvent *)
{
    if (bar.isNull())
        bar.load(QLatin1String(":/core/images/progressbar.png"));

    double range = maximum() - minimum();
    double percent = 0.;
    if (range != 0)
        percent = (value() - minimum()) / range;
    if (percent > 1)
        percent = 1;
    else if (percent < 0)
        percent = 0;

    if (finished())
        percent = 1;

    QPainter p(this);

    // draw outer rect
    QRect rect((size().width() - m_progressWidth) / 2, (size().height() - m_progressHeight) / 2, m_progressWidth, m_progressHeight-1);
    p.setPen(Utils::StyleHelper::panelTextColor());
    Utils::StyleHelper::drawCornerImage(bar, &p, rect, 2, 2, 2, 2);

    // draw inner rect
    QColor c = Utils::StyleHelper::panelTextColor();
    c.setAlpha(180);
    p.setPen(Qt::NoPen);

    QRect inner = rect.adjusted(3, 2, -2, -2);
    inner.adjust(0, 0, qRound((percent - 1) * inner.width()), 0);
    if (m_error) {
        QColor red(255, 60, 0, 210);
        c = red;
        // avoid too small red bar
        if (inner.width() < 10)
            inner.adjust(0, 0, 10 - inner.width(), 0);
    } else if (m_finished) {
        c = QColor(90, 170, 60);
    }

    // Draw line and shadow after the gradient fill
    if (value() > 0 && value() < maximum()) {
        p.fillRect(QRect(inner.right() + 1, inner.top(), 2, inner.height()), QColor(0, 0, 0, 20));
        p.fillRect(QRect(inner.right() + 1, inner.top(), 1, inner.height()), QColor(0, 0, 0, 60));
    }
    QLinearGradient grad(inner.topLeft(), inner.bottomLeft());
    grad.setColorAt(0, c.lighter(130));
    grad.setColorAt(0.5, c.lighter(106));
    grad.setColorAt(0.51, c.darker(106));
    grad.setColorAt(1, c.darker(130));
    p.setPen(Qt::NoPen);
    p.setBrush(grad);
    p.drawRect(inner);
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(QColor(0, 0, 0, 30), 1));
    p.drawLine(inner.topLeft(), inner.topRight());
    p.drawLine(inner.topLeft(), inner.bottomLeft());
    p.drawLine(inner.topRight(), inner.bottomRight());
    p.drawLine(inner.bottomLeft(), inner.bottomRight());
    p.drawPoint(inner.bottomLeft());
    p.drawPoint(inner.bottomRight());
}
