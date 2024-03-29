#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    clusterer = std::make_shared<KMeansClusterer>();
    ui->setupUi(this);
    connect(ui->uploadButton, SIGNAL(clicked()), this, SLOT(uploadImage()));
    connect(ui->kmeansButton, &QPushButton::clicked, this, [&]()
    {
        if (fileName.isEmpty())
        {
            QMessageBox::critical(this, tr("Error"), tr("Upload an image first."));
            return;
        }
        QPixmap pixmap(fileName);
        auto data = clusterer->getImageData(pixmap.toImage());

        if (data.isEmpty())
        {
            QMessageBox::critical(this, tr("Error"), tr("Image data not available."));
            return;
        }
        centroids = clusterer->performKMeans(data, clusterer->getNumberOfIteration());
        assinments = clusterer->assignToCentroids(data, centroids);
        clusterer->paintImage(pixmap.toImage(), assinments, centroids);

        createColorPalette(centroids);  // Pass the centroids as an argument
            ui->paintLabel->setPixmap(clusterer->getPaintedPixmap().scaled( ui->paintLabel->size(), Qt::KeepAspectRatio));
    });
}

Widget::~Widget()
{
    delete ui;
}

auto Widget::createColorPalette(const QVector<QRgb>& centroids) -> void
{
    const int maxCentroidsToShow = 10;
    const int numCentroids = qMin(centroids.size(), maxCentroidsToShow);
    int numberOfColumns = qCeil(qSqrt(numCentroids));
    for (int i = 0; i < numCentroids; ++i)
    {
        QLabel* colorLabel = new QLabel;

        QPixmap pixmap(30, 30);
        pixmap.fill(centroids[i]);
        qDebug() << centroids[i];
        colorLabel->setPixmap(pixmap);
        int row = i / numberOfColumns;
        int col = i % numberOfColumns;
        ui->gridLayout->addWidget(colorLabel, row, col);
    }
}

auto Widget::uploadImage() -> void
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"), "", tr("Images (*.png *.jpg *.bmp *.gif);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        QPixmap pixmap(fileName);
        ui->imageLabel->setPixmap(pixmap.scaled( ui->imageLabel->size(), Qt::KeepAspectRatio));
    }

}
auto Widget::kMeansClusterer() -> void
{
    QPixmap pixmap(fileName);
    auto data = clusterer->getImageData(pixmap.toImage());
    centroids = clusterer->performKMeans(data, 100);
}

