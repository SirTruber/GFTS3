#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QtGraphs>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_editRows_editingFinished();

    void on_editColumns_editingFinished();

    void on_checkBox_clicked(bool checked);

    void on_comboBox_currentIndexChanged(int index);

    void on_epsEdit_editingFinished();

    void on_NmaxEdit_editingFinished();

    void on_comboBox_3_currentIndexChanged(int index);

private:
    bool flagColumns = true, flagRows = true, flagNmax = true, flagEps = true;

    int rows = 11;
    int columns = 11;
    int NMax = 10000;
    double Eps = 0.0001;

    Q3DSurface* surfaceGraph = nullptr;
    QSurface3DSeries *series = nullptr;
    QSurface3DSeries *begin = nullptr;
    QSurface3DSeries *halfSteps = nullptr;
    QSurface3DSeries *error = nullptr;
    QSurface3DSeries* surfaces[5];

private:
    void uiMWR();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
