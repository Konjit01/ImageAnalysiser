#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    analysis = std::make_shared<Analysis>();
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
        auto data = analysis->getImageData(pixmap.toImage());
        if (data.isEmpty())
        {
            QMessageBox::critical(this, tr("Error"), tr("Image data not available."));
            return;
        }
        qDebug() << "In connect for cluster" ;
        centroids = clusterer->performKMeans(data, 3, 20);
        createColorPalette(centroids);  // Pass the centroids as an argument
    });
}

Widget::~Widget()
{
    delete ui;
}

auto Widget::createColorPalette(const QVector<QRgb>& centroids)->void
{
    const int maxCentroidsToShow = 10;
    const int numCentroids = qMin(centroids.size(), maxCentroidsToShow);

    for (int i = 0; i < numCentroids; ++i)
    {
        QLabel* colorLabel = new QLabel;

        QPixmap pixmap(30, 30);
        pixmap.fill(centroids[i]);
        colorLabel->setPixmap(pixmap);
        ui->colorHorizotallayout->addWidget(colorLabel);
    }
}
void Widget::uploadImage()
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
    auto data = analysis->getImageData(pixmap.toImage());
    centroids = clusterer->performKMeans(data, 3, 20);
}
