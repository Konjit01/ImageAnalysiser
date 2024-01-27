#include "kmeansclusterer.h"

KMeansClusterer::KMeansClusterer(int k) {}


auto KMeansClusterer::calculateDistance(QRgb color1, QRgb color2) -> qreal
{
    int red1   = qRed(color1);
    int green1 = qGreen(color1);
    int blue1  = qBlue(color1);

    int red2   = qRed(color2);
    int green2 = qGreen(color2);
    int blue2  = qBlue(color2);

    int deletaRed   = red2 - red1;
    int deletaGreen = green2 - green1;
    int deletaBlue  = blue2 - blue1;

    return qSqrt(deletaRed* deletaRed + deletaGreen * deletaGreen + deletaBlue * deletaBlue);
}

auto KMeansClusterer::initCentroids(const QVector<QRgb> &data, int k) -> QVector<QRgb>
{
    QVector<QRgb> centroids;
    QSet<int> centroidIndices;

    QRandomGenerator randomGenerator;

    while(centroids.size() < k)
    {
        int randomIndex = randomGenerator.bounded(data.size());

        if(!centroidIndices.contains(randomIndex))
        {
            centroidIndices << (randomIndex);
            centroids << data[randomIndex];
        }
    }

    return centroids;
}
QVector<int> KMeansClusterer::assignToCentroids(const QVector<QRgb> &data, const QVector<QRgb> &centroids)
{
    QVector<int> assignments;
    for(const QRgb& pixel : data)
    {
        //qreal minValue = QNumericFormatter<qreal>::minimumFinite();
        auto minValue = std::numeric_limits<double>::max();
        int closestCentroid = -1;

        for(int i = 0; i < centroids.size(); i++)
        {
            auto distance = calculateDistance(pixel, centroids.at(i));

            if(distance < minValue)
            {
                minValue = distance;
                closestCentroid = i;
            }
        }
        assignments << closestCentroid;
    }

    return assignments;
}

auto KMeansClusterer::updateCentriods(const QVector<QRgb> &data, const QVector<int> assigments) -> QVector<QRgb>
{
    QVector<QRgb> updatedCentroid(k, qRgb(0, 0, 0));
    QVector<int> counts(k, 0);

    for(int i = 0; i < data.size(); i++)
    {
        int clusterIndex = assigments.at(i);

        updatedCentroid[clusterIndex] = qRgb(qRed(updatedCentroid.at(clusterIndex))   + qRed(data.at(i)),
                                             qGreen(updatedCentroid.at(clusterIndex)) + qGreen(data.at(i)),
                                             qBlue(updatedCentroid.at(clusterIndex))  + qBlue(data.at(i))
                                             );

        counts[clusterIndex]++;
    }

    for (int i = 0; i < k; ++i)
    {
        if (counts[i] > 0)
        {
            updatedCentroid[i] = qRgb(
                                       qRed(updatedCentroid[i]) / counts[i],
                                       qGreen(updatedCentroid[i]) / counts[i],
                                       qBlue(updatedCentroid[i]) / counts[i]
                                     );
        }
    }

    return updatedCentroid;
}

auto KMeansClusterer::performKMeans(const QVector<QRgb>& data, int k, int maxIterations) -> QVector<QRgb>
{
    auto centroids = initCentroids(data, k);

    for(int i = 0; i < maxIterations; i++)
    {
        auto assignments = assignToCentroids(data, centroids);
        auto updatedCentroids = updateCentriods(data, assignments);

        centroids = updatedCentroids;
    }
    return centroids;
}
