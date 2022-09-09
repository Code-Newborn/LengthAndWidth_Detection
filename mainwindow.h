#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <QMainWindow>
#include <stdlib.h>

using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

struct UserDefContour
{
    int approx_items;
    float area;
    vector<Point> approx;
    Rect bbox;
    vector<Point> contour;
    UserDefContour() {}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    float scale;
    int wP;
    int hP;

    vector<UserDefContour>
    getContours(Mat img, int cThr0 = 100, int cThr1 = 100, bool showCanny = false, float minArea = 1000, int filter = 0, bool draw = false);
    Mat warpImg(Mat img, vector<Point> points, int w, int h, int pad = 20);
    vector<Point> reOrder(vector<Point> points);
};
#endif // MAINWINDOW_H
