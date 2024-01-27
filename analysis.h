#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QImage>
#include <QVector>
#include <functional>
#include <QDebug>
#include <QRandomGenerator>


template<int byteDepth, typename T>
void scan(QImage &image, std::function<void(int, int, T&)> f)
{
    int w = image.width(), h = image.height();

    for (int y = 0; y < h; y++)
    {
        uchar* scan = image.scanLine(y);
        for (int x = 0; x < w; x++)
            f(x, y, *reinterpret_cast<T*>(scan + x*byteDepth));
    }
}

class Analysis
{
public:
    Analysis();
    QImage getImg() const;
    void setImg(const QImage &newImg);
    void print();
    QVector<QRgb> getImageData( QImage img);
private:

    int numClusters;

};

#endif // ANALYSIS_H
