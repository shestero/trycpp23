#ifndef DRAWBOARD_H
#define DRAWBOARD_H

#include "ssdfile.h"
#include <QWidget>
#include <functional>
#include <optional>

class DrawingBoard : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingBoard(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *) override;

private:
    void paintInfo(QPainter& painter, QRect& graphic);
    void paintGrid(QPainter& painter, QRect& graphic,
                   const double tdelta, const double vdelta,
                   auto convt, auto convv);
    void paintGrph(QPainter& painter,
                   auto convt, auto convv);

    QFont font;

    std::optional <SSDFile> file;

public slots:
    void setFile(const QString& file_name);

};

#endif // DRAWBOARD_H
