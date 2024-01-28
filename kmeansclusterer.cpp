 #include "kmeansclusterer.h"

KMeansClusterer::KMeansClusterer(int k) : k{k}{}


auto KMeansClusterer::getImageData(QImage img) -> QVector<QRgb>
{
    QVector<QRgb> data;
    scan<4, QRgb>(img, [&](int x, int y, QRgb& rgb)
                  {
                      rgb = img.pixel(x, y);
                      data.push_back(rgb);
                  });
    return data;
}

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

QVector<QRgb> KMeansClusterer::initCentroids(const QVector<QRgb>& data, int k) {
    QVector<QRgb> centroids;
    QRandomGenerator randomGenerator;

    int firstCentroidIndex = randomGenerator.bounded(data.size());
    centroids.push_back(data[firstCentroidIndex]);
    while (centroids.size() < k)
    {
        QVector<double> distancesSquared(data.size(), std::numeric_limits<double>::max());
        for (int dataIndex = 0; dataIndex < data.size(); ++dataIndex)
        {
            for (const QRgb& centroid : centroids)
            {
                double distanceSquared = calculateDistance(data[dataIndex], centroid);
                distancesSquared[dataIndex] = std::min(distancesSquared[dataIndex], distanceSquared);
            }
        }
        double totalDistanceSquared = std::accumulate(distancesSquared.begin(), distancesSquared.end(), 0.0);
        double randomValue = randomGenerator.bounded(totalDistanceSquared);
        double accumulatedDistanceSquared = 0.0;
        int chosenIndex = 0;

        while (accumulatedDistanceSquared < randomValue && chosenIndex < data.size())
        {
            accumulatedDistanceSquared += distancesSquared[chosenIndex++];
        }

        centroids << data[chosenIndex - 1];
    }
    return centroids;
}

auto KMeansClusterer::assignToCentroids(const QVector<QRgb> &data, const QVector<QRgb>& centroids) -> QVector<int>
{
    QVector<int> assignments;
    for(const QRgb& pixel : data)
    {
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

auto KMeansClusterer::updateCentriods(const QVector<QRgb>& data, const QVector<int> assignments) -> QVector<QRgb>
{
    QVector<QRgb> updatedCentroids(k, qRgb(0, 0, 0));
    QVector<int> counts(k, 0);

    int accumulatedRed, accumulatedGreen, accumulatedBlue = 0;

    for (int i = 0; i < data.size(); ++i)
    {
        if (int clusterIndex = assignments[i]; clusterIndex >= 0 && clusterIndex < k)
        {
            accumulatedRed += qRed(data[i]);
            accumulatedGreen += qGreen(data[i]);
            accumulatedBlue += qBlue(data[i]);
            updatedCentroids[clusterIndex] = qRgb(accumulatedRed, accumulatedGreen, accumulatedBlue);
            counts[clusterIndex]++;
        }
        else
        {
          qDebug() << "Error: Invalid clusterIndex" << clusterIndex;
        }
    }

    for (int i = 0; i < k; ++i)
    {
        if (counts[i] > 0)
        {
            updatedCentroids[i] = qRgb(accumulatedRed / counts[i], accumulatedGreen / counts[i],accumulatedBlue / counts[i]);
        }
    }
    return updatedCentroids;
}

auto KMeansClusterer::performKMeans(const QVector<QRgb>& data, int maxIterations) -> QVector<QRgb>
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

void KMeansClusterer::printImageData(QVector<QRgb> data)
{
    for (int i = 0; i < data.size(); ++i)
    {
        QRgb pixel = data[i];
        int red = qRed(pixel);
        int green = qGreen(pixel);
        int blue = qBlue(pixel);
        int alpha = qAlpha(pixel);

        qDebug() << "Pixel " << i << ": RGBA(" << red << ", " << green << ", " << blue << ", " << alpha << ")";
    }
}

QPixmap KMeansClusterer::getPaintedPixmap() const
{
    return paintedPixmap;
}

int KMeansClusterer::getNumberOfIteration() const
{
    return numberOfIteration;
}

void KMeansClusterer::paintImage(const QImage& originalImage, const QVector<int>& assignments, const QVector<QRgb>& centroids) {

    paintedPixmap = QPixmap::fromImage(originalImage);  // Create a pixmap with the same size as the original image
    QPainter painter(&paintedPixmap);

    scanConst<4, QRgb>(originalImage, [&](int x, int y, const QRgb &pixel) {
        int assignment = assignments[y * originalImage.width() + x];
        QRgb centroidColor = centroids[assignment];
        QPen pen;
        pen.setColor(QColor(centroidColor));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(QColor(centroidColor));
        painter.drawRect(x, y, 1, 1);
    });
}
