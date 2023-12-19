#ifndef DRAWBOARD_H
#define DRAWBOARD_H

#include "ssdfile.h"

#include <QWidget>

#include <optional>

class DrawingBoard : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingBoard(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *) override;

protected:
    std::optional <SSDFile> file;
    QFont font;

public slots:
    void setFile(const QString& file_name);

};

#endif // DRAWBOARD_H
