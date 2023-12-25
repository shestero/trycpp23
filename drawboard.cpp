#include "drawboard.h"

#include <math.h>
#include <cmath>
#include "error.h"
#include <QApplication>
#include <QMessageBox>
#include <QPainter>

/**
 * Design paramters here
 * in such old facion way.
 **/
#define     margin      10
#define     half_margin 5
#define     left_header 80
#define     text_height 15
#define     grid_color  QColor(0xC0, 0xC0, 0xC0)
#define     plot_color  QColor(0x00, 0x40, 0x00)


DrawingBoard::DrawingBoard(QWidget *parent) : QWidget(parent), font("Monospace")
{
    font.setStyleHint(QFont::TypeWriter);
    setMinimumSize(QSize(600, 600));
}

/**
 *  Load the SSD file by name
 */
void DrawingBoard::setFile(const QString& file_name) {

    QApplication::setOverrideCursor(Qt::WaitCursor);
    try {
        file.emplace(SSDFile(file_name));
        QApplication::restoreOverrideCursor();
    } catch (Error& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(
            this,
            QString("Cannot load the file: %1").arg(file_name),
            e.message
        );
        file.reset();
    }

    repaint();
}

/**
 * @brief DrawingBoard::paintEvent
 * Draw the plot line in widget.
 */
void DrawingBoard::paintEvent(QPaintEvent *)
{
    if (!file.has_value())
        return; // file wasn't defined

    QPainter painter(this);
    painter.setFont(font);
    painter.drawText(margin, half_margin+text_height, file->file_name);

    // drawing area
    QRect graphic = rect();
    if (!graphic.isValid() || graphic.isEmpty())
        return;
    graphic.adjust(margin, margin+text_height+half_margin, -margin, -margin);
    painter.drawRect(graphic);
    graphic.adjust(1,1,-1,-1);

    paintInfo(painter, graphic);

    graphic.adjust(margin+left_header, text_height*4, -margin, -text_height*7);

    // Calculation of coefficients
    const double tdelta = file->tmax - file->tmin;
    const double vdelta = file->vmax - file->vmin;
    const double kt = (graphic.right()-graphic.left())/tdelta; // >0
    const double kv = (graphic.top()-graphic.bottom())/vdelta; // <0

    // Physical to screen conversions:
    auto convt =
        [&](const double& t) -> int {
                return graphic.left() + trunc(kt*(t - file->tmin));
        };
    auto convv =
        [&](const double& v) -> int {
                return graphic.bottom() + ceil(kv*(v - file->vmin));
        };

    paintGrid(painter, graphic, tdelta, vdelta, convt, convv);
    paintGrph(painter, convt, convv);
}

void DrawingBoard::paintInfo(QPainter& painter, QRect& graphic)
{
    // Draw header information
    painter.drawText(QRectF(graphic), Qt::AlignLeft|Qt::AlignTop,
                     file->header.mes_type.c_str());
    painter.drawText(QRectF(graphic), Qt::AlignRight|Qt::AlignTop,
                     file->header.date_and_time.c_str());
    painter.drawText(QRectF(graphic), Qt::AlignRight|Qt::AlignBottom,
                     QString("Size: %1 data lines.").arg(file->size()));
    painter.drawText(QRectF(graphic), Qt::AlignLeft|Qt::AlignBottom,
                     QString::fromLocal8Bit(file->header.other.c_str())
                     );
    QFont bold = font;
    bold.setBold(true);
    bold.setUnderline(true);
    painter.setFont(bold);
    painter.drawText(QRectF(graphic), Qt::AlignHCenter|Qt::AlignTop,
                     QString("\n")+file->header.org_app_ver.c_str());
    painter.setFont(font);
}

void DrawingBoard::paintGrid(
        QPainter& painter, QRect& graphic,
        const double tdelta, const double vdelta,
        auto convt, auto convv
    )
{
    // log10 for grids:
    int t10 = trunc(log10(tdelta));
    int v10 = trunc(log10(vdelta));
    int tmax10 = ceil(log10(file->tmax)); // assume tmax>0
    int vmax10 = ceil(log10(std::max(fabs(file->vmin),fabs(file->vmax))));

    // Drawing the grid
    painter.setPen(grid_color);

    // vertical grid
    double dt = pow(10, t10-1);
    for (double t = pow(10, tmax10); t > file->tmin; t-=dt) {
        if (t > file->tmax)
            continue;
        const int i = convt(t);
        painter.drawLine(i, graphic.top(), i, graphic.bottom());
        painter.drawText(i, graphic.top(), QString::number(t));
    }
    const int i = convt(file->tmin);
    painter.drawLine(i, graphic.top(), i, graphic.bottom());

    // horizontal grid
    double dv = pow(10, v10-1);
    // Note: some assumption here that vmax is positive
    for (double v = pow(10, vmax10); v > file->vmin; v-=dv) {
        if (v > file->vmax)
            continue;
        const int j = convv(v);
        painter.drawLine(graphic.left()-margin, j, graphic.right(), j);
        painter.drawText(graphic.left()-left_header, j, QString::number(v));
    }
    const int j = convv(file->vmin);
    painter.drawLine(graphic.left(), j, graphic.right(), j);

    painter.setPen(plot_color);
}

void DrawingBoard::paintGrph(
        QPainter& painter,
        auto convt, auto convv
    )
{
    auto view =     // points are chunked by the X screen coordinates
        file->range()
            | std::views::transform([convt, convv](const auto& op) ->
                std::optional<std::pair<int,int>>
                {
                    return
                        op.transform([convt, convv](const auto& p) {
                        const double t = p.first;
                        const double v = p.second;
                        return std::make_pair(convt(t),convv(v));
                    });
                })
            | std::views::chunk_by([](auto const& op1, auto const& op2) {
                return op1.has_value() && op2.has_value() && op1->first==op2->first;
            });

    std::optional<std::pair<int,int>> prev; // the connection in the previouse chunk
    for (auto subop : view) {
        auto sub = subop
            | std::views::filter([](auto const &op) { return op.has_value(); })
            | std::views::transform([](auto const &op) { return *op; });

        // draw point marks (may be switched off here)
        for (const auto& p : sub) {
            painter.drawEllipse(QPointF(p.first, p.second), 1.5, 1.5);
        }

        // draw a compound vertical segment that contain several points with the same X on screen
        if (subop.size()>1||!sub.empty()) {
            auto [bottom, top] = std::minmax_element(sub.begin(), sub.end());
            painter.drawLine((*bottom).first, (*bottom).second, (*top).first, (*top).second);
        }

        // draw a "normal" segment
        if (prev.has_value()) {
            const std::optional<std::pair<int,int>>& first = *(subop.cbegin());
            if (first.has_value()) {
                painter.drawLine(
                    prev->first, prev->second,
                    first->first, first->second
                );
            }
        }

        // store connection between chunks
        const std::optional<std::pair<int,int>>& last = *(--subop.cend());
        if (last.has_value())
            prev.emplace(*last); // normal plot
        else
            prev.reset(); // gape
    }
}

