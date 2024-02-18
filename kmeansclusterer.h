#ifndef KMEANSCLUSTERER_H
#define KMEANSCLUSTERER_H

#include <QSet>
#include <QVector>
#include <QRandomGenerator>
#include <QColor>
#include <QtMath>
#include <QtGlobal>
#include <QImage>
#include <QDebug>
#include <QtAlgorithms>
#include <algorithm>
#include <QPixmap>
#include <QPainter>
#include <cmath>

using DataFrame = QVector<QRgb>;

/*I did not implement this template function.*/
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

template<int byteDepth, typename T>
void scanConst(const QImage &image, std::function<void(int, int, const T&)> f) {
    int w = image.width(), h = image.height();

    for (int y = 0; y < h; y++) {
        const uchar* scan = image.constScanLine(y);
        for (int x = 0; x < w; x++)
            f(x, y, *reinterpret_cast<const T*>(scan + x * byteDepth));
    }
}

class KMeansClusterer
{
public:
    KMeansClusterer(int k = 10);
    auto getImageData(QImage img) -> QVector<QRgb>;
    auto calculateDistance(QRgb color1, QRgb color2) -> qreal;
    auto initCentroids(const QVector<QRgb>& data, int k) -> QVector<QRgb>;
    auto assignToCentroids(const QVector<QRgb>& data, const QVector<QRgb>& centroids) -> QVector<int>;
    auto updateCentriods(const QVector<QRgb>& data, const QVector<int> assignments) -> QVector<QRgb>;
    auto performKMeans(const QVector<QRgb>& data, int maxIterations) -> QVector<QRgb>;
    auto paintImage(const QImage &originalImage, const QVector<int>& assignments, const QVector<QRgb>& centroids) -> void;
    auto printImageData(QVector<QRgb> data) -> void;
    auto getPaintedPixmap() const ->QPixmap;

    int getNumberOfIteration() const;

private:
    int k;
    QPixmap paintedPixmap;
    QRandomGenerator randomGenerator;
    const int numberOfIteration = 100;
};

#endif // KMEANSCLUSTERER_H
