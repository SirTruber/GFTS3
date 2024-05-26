#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFuture>

void MainWindow::taskTest(std::vector<QSurfaceDataArray>& array)
{
    size_t n = rows - 1;
    size_t m = columns - 1;

    size_t count = 0, count1 = 0;

    double epsMax = 0.0;
    double err;
    double xMax = 0., yMax = 0.;
    double x, y;

    QFuture<void> future;
    std::unique_ptr<numeric_method::Matrix_solver> sN_test;

    future = QtConcurrent::run([&]{
        manager.createTask(n, m, Numerical_method::MWR_TEST, Eps, NMax);
        sN_test = manager.returnTask(count1, Numerical_method::MWR_TEST);
        ;});

    future.waitForFinished();

    double stepX = (bX - aX) / static_cast<double>(n);
    double stepY = (bY - aY) / static_cast<double>(m);

    for(size_t i = 0; i < n + 1; ++i)
    {
        QSurfaceDataRow newRow[4];

        for (int i = 0; i < 4; i++)
        {
            newRow[i].reserve(m + 1);
        }

        x = qMin(bX, i * stepX);

        for(size_t j = 0; j < m + 1; ++j)
        {
            ui->progressBar->setValue(i * columns + j);

            y = qMin(bY, j * stepY);


            double tmp = std::sin(numeric_method::pi * static_cast<double>(i * j) / static_cast<double>(n * m));
            tmp = std::exp(tmp * tmp);

            err = sN_test->v[i][j] - tmp;
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(tmp)));
            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(sN_test->v[i][j])));
            ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(QString::number(err)));

            newRow[0].append(QSurfaceDataItem(y, tmp, x));
            newRow[1].append(QSurfaceDataItem(y, 0., x));
            newRow[2].append(QSurfaceDataItem(y, sN_test->v[i][j], x));
            newRow[3].append(QSurfaceDataItem(y, abs(err), x));

            if (epsMax < abs(err))
            {
                epsMax = abs(err);

                xMax = x;
                yMax = y;
            }
        }

        for (int i = 0; i < 4; i++)
        {
            array[i] << newRow[i];
        }
    }

    ui->edit_n->setText(QString::number(n));
    ui->edit_m->setText(QString::number(m));

    ui->edit_epsmet->setText(QString::number(Eps, 'g', 10));
    ui->edit_Nmax->setText(QString::number(NMax));

    ui->edit_N->setText(QString::number(count1));
    ui->edit_Eps1->setText(QString::number(epsMax));

    ui->edit_MaxX->setText(QString::number(xMax));
    ui->edit_MaxY->setText(QString::number(yMax));
}

void MainWindow::taskMain(std::vector<QSurfaceDataArray>& array)
{
    size_t n = rows - 1;
    size_t m = columns - 1;

    size_t count = 0, count1 = 0;

    double epsMax = 0.0;
    double err;
    double xMax = 0., yMax = 0.;
    double x, y;

    manager.createTask(n, m, Numerical_method::MWR_MAIN, Eps, NMax);
    manager.createTask(n, m, Numerical_method::MWR_BIGGER, Eps, NMax);
    std::unique_ptr<numeric_method::Matrix_solver> s2N = manager.returnTask(count1, Numerical_method::MWR_BIGGER);
    std::unique_ptr<numeric_method::Matrix_solver> sN = manager.returnTask(count, Numerical_method::MWR_MAIN);

    double stepX = (bX - aX) / static_cast<double>(n);
    double stepY = (bY - aY) / static_cast<double>(m);

    for(size_t i = 0; i < n + 1; ++i)
    {
        QSurfaceDataRow newRow[5];

        for (int i = 0; i < 5; i++)
        {
            newRow[i].reserve(m + 1);
        }

        x = qMin(bX, static_cast<double>(i) * stepX);

        for(size_t j = 0; j < m + 1; ++j)
        {
            ui->progressBar->setValue(i * columns + j);

            y = qMin(bY, static_cast<double>(j) * stepY);

            err = sN->v[i][j] - s2N->v[i * 2][j * 2];

            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(sN->v[i][j])));

            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(s2N->v[2 * i][2 * j])));

            ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(QString::number(err)));

            newRow[0].append(QSurfaceDataItem(y, 0., x));
            newRow[1].append(QSurfaceDataItem(y, 0., x));
            newRow[2].append(QSurfaceDataItem(y, sN->v[i][j], x));
            newRow[3].append(QSurfaceDataItem(y, s2N->v[i * 2][j * 2], x));
            newRow[4].append(QSurfaceDataItem(y, err, x));

            if (epsMax < abs(err))
            {
                epsMax = abs(err);

                xMax = x;
                yMax = y;
            }
        }

        for (int i = 0; i < 5; i++)
        {
            array[i] << newRow[i];
        }
    }

    ui->edit_n->setText(QString::number(n));
    ui->edit_m->setText(QString::number(m));

    ui->edit_epsmet->setText(QString::number(Eps, 'g', 10));
    ui->edit_Nmax->setText(QString::number(NMax));

    ui->edit_N->setText(QString::number(count1));
    ui->edit_Eps1->setText(QString::number(epsMax));

    ui->edit_MaxX->setText(QString::number(xMax));
    ui->edit_MaxY->setText(QString::number(yMax));
}
