/*
 * @Author: Dragon-qing
 * @Date: 2026-04-26
 * @LastEditors: Dragon-qing
 * @FilePath: \ToolKit\src\components\overlay.h
 * @Description: 透明遮罩组件
 */
#ifndef OVERLAY_H
#define OVERLAY_H
#include <QWidget>
#include <QTimer>

class Overlay : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ Opacity WRITE SetOpacity)
    Q_PROPERTY(qreal glowRadius READ GlowRadius WRITE SetGlowRadius)
public:
    explicit Overlay(QWidget *parent = nullptr);
    ~Overlay();

    qreal Opacity()    const { return m_opacity;    }
    qreal GlowRadius() const { return m_glowRadius; }

    void SetOpacity(qreal v)    { m_opacity = v;    update(); }
    void SetGlowRadius(qreal v) { m_glowRadius = v; update(); }

    void FadeIn(int duration = 300);
    void FadeOut(int duration = 300);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override; 

private:
    qreal m_opacity = 0.0; // 不透明度
    qreal m_glowRadius = 200.0;
    QTimer *m_pulseTimer;
    bool m_pulseUp = true;
};

#endif