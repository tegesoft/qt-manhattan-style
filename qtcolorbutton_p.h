#include "qtcolorbutton.h"

namespace Manhattan {

class QtColorButtonPrivate: public QObject
{
    Q_OBJECT
    QtColorButton *q_ptr;
    Q_DECLARE_PUBLIC(QtColorButton)
public slots:
    void slotEditColor();

public:
    QColor shownColor() const;
    QPixmap generatePixmap() const;

    QColor m_color;
#ifndef QT_NO_DRAGANDDROP
    QColor m_dragColor;
    QPoint m_dragStart;
    bool m_dragging;
#endif
    bool m_backgroundCheckered;
    bool m_alphaAllowed;
};

} // namespace Utils
