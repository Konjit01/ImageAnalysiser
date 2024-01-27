#include "analysis.h"

Analysis::Analysis()
{}
void Analysis::print()
{

    // QVector<QRgb> data = getImageData();

    // for (int i = 0; i < data.size(); ++i) {
    //     QRgb pixel = data[i];
    //     int red = qRed(pixel);
    //     int green = qGreen(pixel);
    //     int blue = qBlue(pixel);
    //     int alpha = qAlpha(pixel);

    //     qDebug() << "Pixel " << i << ": RGBA(" << red << ", " << green << ", " << blue << ", " << alpha << ")";
    // }

}

auto Analysis::getImageData(QImage img) -> QVector<QRgb>
{
    QVector<QRgb> data;
    scan<4, QRgb>(img, [&](int x, int y, QRgb& rgb)
                  {
                      rgb = img.pixel(x, y);
                      data.push_back(rgb);
                  });
    return data;
}
