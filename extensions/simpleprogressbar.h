#ifndef SIMPLEPROGRESSBAR_H
#define SIMPLEPROGRESSBAR_H

#include "../qt-manhattan-style_global.hpp"
#include <QWidget>

namespace Manhattan {

class QTMANHATTANSTYLESHARED_EXPORT SimpleProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleProgressBar(int width = 50, int height = 12, QWidget *parent = 0);
    ~SimpleProgressBar();

    void setError(bool on);
    bool hasError() const;
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *);
    int minimum() const { return m_minimum; }
    int maximum() const { return m_maximum; }
    int value() const { return m_value; }
    bool finished() const { return m_finished; }
    void setRange(int minimum, int maximum);
    void setValue(int value);
    void setFinished(bool b);

public slots:
    void reset();

private:
    QImage bar;
    QString m_text;
    bool m_error;
    int m_progressHeight;
    int m_progressWidth;
    int m_minimum;
    int m_maximum;
    int m_value;
    bool m_finished;
};

} // namespace Manhattan

#endif // SIMPLEPROGRESSBAR_H
