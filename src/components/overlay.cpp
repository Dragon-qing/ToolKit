#include <QPainter>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QPropertyAnimation>
#include <QPainterPath>

#include "overlay.h"

Overlay::Overlay(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    
    // 光晕脉冲动画
    m_pulseTimer = new QTimer(this);
    connect(m_pulseTimer, &QTimer::timeout, this, [this]() {
        if (m_pulseUp) {
            m_glowRadius += 0.8;
            if (m_glowRadius >= 80.0) m_pulseUp = false;
        } else {
            m_glowRadius -= 0.8;
            if (m_glowRadius <= 50.0) m_pulseUp = true;
        }
        update();
    });
    m_pulseTimer->start(16); // 60 fps
    hide();
}

Overlay::~Overlay()
{

}

void Overlay::paintEvent(QPaintEvent*)
{
     QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QRectF rc = rect();
    const QPointF center = rc.center();

    /* ── 第一层：全屏深色蒙版 ───────────────────── */
    QLinearGradient bg(0, 0, 0, rc.height());
    bg.setColorAt(0.0, QColor(40, 60, 100,  static_cast<int>(160 * m_opacity)));
    bg.setColorAt(1.0, QColor(20,  35,  75,  static_cast<int>(180 * m_opacity)));
    p.fillRect(rc, bg);

    /* ── 第二层：中心柔光 (径向渐变) ─────────────── */
    QRadialGradient spotlight(center, rc.width() * 0.5);
    spotlight.setColorAt(0.0, QColor(100, 120, 255, static_cast<int>(30  * m_opacity)));
    spotlight.setColorAt(0.5, QColor(60,  80,  200, static_cast<int>(10  * m_opacity)));
    spotlight.setColorAt(1.0, QColor(0,   0,   0,   0));
    p.setBrush(spotlight);
    p.setPen(Qt::NoPen);
    p.drawEllipse(center, rc.width() * 0.5, rc.height() * 0.5);

    /* ── 第三层：脉冲光晕圆环 ────────────────────── */
    qreal r = m_glowRadius;
    QRadialGradient glow(center, r);
    glow.setColorAt(0.60, QColor(100, 140, 255, static_cast<int>(0   * m_opacity)));
    glow.setColorAt(0.80, QColor(130, 160, 255, static_cast<int>(80  * m_opacity)));
    glow.setColorAt(0.92, QColor(80,  120, 230, static_cast<int>(140 * m_opacity)));
    glow.setColorAt(1.00, QColor(0,   0,   0,   0));
    p.setBrush(glow);
    p.drawEllipse(center, r, r);

    /* ── 第四层：细边描边圆圈 ────────────────────── */
    p.setBrush(Qt::NoBrush);
    for (int i = 0; i < 3; ++i) {
        qreal ringR = r * (0.85 + i * 0.12);
        int   alpha  = static_cast<int>((60 - i * 18) * m_opacity);
        p.setPen(QPen(QColor(160, 190, 255, alpha), 0.8));
        p.drawEllipse(center, ringR, ringR);
    }

    /* ── 第五层：顶部线性渐变遮罩 ────────────────── */
    QLinearGradient topFade(0, 0, 0, rc.height() * 0.3);
    topFade.setColorAt(0.0, QColor(20, 30, 80, static_cast<int>(50 * m_opacity)));
    topFade.setColorAt(1.0, QColor(0,  0,  0,  0));
    p.fillRect(QRectF(0, 0, rc.width(), rc.height() * 0.3), topFade);

    /* ── 第六层：底部渐变 ────────────────────────── */
    QLinearGradient botFade(0, rc.height() * 0.7, 0, rc.height());
    botFade.setColorAt(0.0, QColor(0,  0,  0,  0));
    botFade.setColorAt(1.0, QColor(5,  8,  40, static_cast<int>(60 * m_opacity)));
    p.fillRect(QRectF(0, rc.height() * 0.7, rc.width(), rc.height() * 0.3), botFade);
}

void Overlay::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    update();
}

void Overlay::FadeIn(int duration)
{
    auto *anim = new QPropertyAnimation(this, "opacity", this);
    anim->setDuration(duration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    show();
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void Overlay::FadeOut(int duration)
{
    auto *anim = new QPropertyAnimation(this, "opacity", this);
    anim->setDuration(duration);
    anim->setStartValue(m_opacity);
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InCubic);
    connect(anim, &QPropertyAnimation::finished, this, &QWidget::hide);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}