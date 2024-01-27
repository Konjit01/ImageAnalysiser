#ifndef KMEANSCLUSTERER_H
#define KMEANSCLUSTERER_H

#include <QSet>
#include <QVector>
#include <QRandomGenerator>
#include <QColor>
#include <QtMath>
#include <QtGlobal>

class KMeansClusterer
{
public:
    KMeansClusterer(int k = 3);
    auto calculateDistance(QRgb color1, QRgb color2) -> qreal;
    auto initCentroids(const QVector<QRgb>& data, int k) -> QVector<QRgb>;
    QVector<int> assignToCentroids(const QVector<QRgb>& data, const QVector<QRgb>& centroids);
    auto updateCentriods(const QVector<QRgb>& data, const QVector<int> assigments) -> QVector<QRgb>;
    auto performKMeans(const QVector<QRgb>& data, int k, int maxIterations) -> QVector<QRgb>;
private:
    int k;
};

#endif // KMEANSCLUSTERER_H
