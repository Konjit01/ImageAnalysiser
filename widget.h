#ifndef WIDGET_H
#define WIDGET_H

#include <memory>
#include <QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

#include "kmeansclusterer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    auto createColorPalette(const QVector<QRgb>& centroids)->void;

private:
    Ui::Widget *ui;

    std::shared_ptr<KMeansClusterer> clusterer;
    QString fileName;
    QVector<QRgb> centroids;
    QVector<int> assinments;
public slots:
    auto uploadImage()->void;
    auto kMeansClusterer() -> void;
};
#endif // WIDGET_H
