#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    scale = 3.0;
    wP = (int)(210 * scale);
    hP = (int)(297 * scale);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief findDis 两坐标点之间的距离
 * @param p1 点1(x1,y1)
 * @param p2 点2(x2,y2)
 * @return 两坐标点之间的欧式距离
 */
float findDis(Point p1, Point p2)
{
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

void MainWindow::on_pushButton_clicked()
{
    Mat srcImage = imread("../1.jpg", IMREAD_COLOR);
    Mat imgwarp;

    QImage image_show = QImage((uchar *)(srcImage.data), srcImage.cols, srcImage.rows, QImage::Format_BGR888);
    ui->label_image->setPixmap(QPixmap::fromImage(image_show));
    ui->label_image->resize(image_show.size());
    ui->label_image->show();

    vector<UserDefContour> conts = getContours(srcImage, 100, 100, false, 50000, 4, false);
    if (conts.size() > 0)
    {
        vector<Point> biggest = conts[0].approx;
        imgwarp = warpImg(srcImage, biggest, wP, hP); // A4纸区域提取透视变换
        imshow("imgwarp", imgwarp);
        vector<UserDefContour> conts2 = getContours(imgwarp, 50, 50, false, 2000, 4, false);
        if (conts2.size() > 0)
        {
            foreach (UserDefContour obj, conts2)
            {
                polylines(imgwarp, obj.approx, true, Scalar(0, 255, 0), 2);
                vector<Point> npoints = reOrder(obj.approx);
                float nW = findDis(npoints[0] / scale, npoints[1] / scale) / 10;
                float nH = findDis(npoints[0] / scale, npoints[3] / scale) / 10;
                arrowedLine(imgwarp, npoints[0], npoints[1], Scalar(255, 0, 255), 3, 8, 0, 0.05);
                arrowedLine(imgwarp, npoints[0], npoints[3], Scalar(255, 0, 255), 3, 8, 0, 0.05);
                float x = obj.bbox.x;
                float y = obj.bbox.y;
                float w = obj.bbox.width;
                float h = obj.bbox.height;
                cv::putText(imgwarp, format("%f cm", nW), Point(x + 30, y - 10), FONT_HERSHEY_COMPLEX_SMALL, 1.5, Scalar(255, 0, 255), 2);
                cv::putText(imgwarp, format("%f cm", nH), Point(x - 70, y + h / 2), FONT_HERSHEY_COMPLEX_SMALL, 1.5, Scalar(255, 0, 255), 2);
            }
        }
        cv::imshow("A4", imgwarp);
    }
    // cv::resize(srcImage,srcImage,Size(0,0),0.5,0.5);
    // cv::imshow("Original",srcImage);
}

/**
 * @brief areaCompare 用于排序的qsort函数的函数指针
 * @param elem1
 * @param elem2
 * @return
 */
int areaCompare(const void *elem1, const void *elem2)
{
    UserDefContour *contour1 = (UserDefContour *)elem1;
    UserDefContour *contour2 = (UserDefContour *)elem2;
    return (*contour2).area > (*contour1).area ? 1 : -1; // 从大到小排列
}

/**
 * @brief MainWindow::getContours
 * @param img 源图像
 * @param cThr0 边缘检测阈值1
 * @param cThr1 边缘检测阈值2
 * @param showCanny 是否显示边缘检测图标志
 * @param minArea 检测的闭合边缘的最小面积
 * @param filter 轮廓多边形拟合的顶点数筛选值
 * @param draw 是否绘制检测轮廓
 * @return 检测轮廓信息结构体向量
 */
vector<UserDefContour> MainWindow::getContours(Mat img, int cThr0, int cThr1, bool showCanny, float minArea, int filter, bool draw)
{
    imshow("Original", img);
    Mat imgGray, imgBlur, imgCanny, imgDila, imgThre;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    imshow("imgGray", imgGray);
    GaussianBlur(imgGray, imgBlur, Size(5, 5), 1);
    imshow("imgBlur", imgBlur);
    Canny(imgBlur, imgCanny, cThr0, cThr1);
    imshow("imgCanny", imgCanny);
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(imgCanny, imgDila, element, Point(-1, -1), 3);
    imshow("imgDila", imgDila);
    erode(imgDila, imgThre, element, Point(-1, -1), 3);
    imshow("imgThre", imgThre);

    if (showCanny)
        imshow("Canny", imgThre);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(imgThre, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<UserDefContour> finalCountours;
    foreach (vector<Point> contour, contours)
    {
        UserDefContour userDefContour;
        userDefContour.area = contourArea(contour);
        if (userDefContour.area > minArea)
        {
            float peri = arcLength(contour, true);
            approxPolyDP(contour, userDefContour.approx, 0.03 * peri, true);
            userDefContour.bbox = boundingRect(userDefContour.approx);
            if (filter > 0)
            {
                if ((int)userDefContour.approx.size() == filter)
                {
                    userDefContour.contour = contour;
                    finalCountours.push_back(userDefContour);
                }
            }
            else
            {
                userDefContour.contour = contour;
                finalCountours.push_back(userDefContour);
            }
        }
    }
    qsort(&finalCountours[0], finalCountours.size(), sizeof(UserDefContour), areaCompare);
    if (draw)
    {
        foreach (UserDefContour con, finalCountours)
        {
            drawContours(img, vector<vector<Point>>{ con.contour }, -1, Scalar(0, 0, 255), 2);
        }
        imshow("contours", img);
    }
    return finalCountours;
}

/**
 * @brief MainWindow::warpImg 仿射变换
 * @param img 源图像
 * @param points 目标图像的4顶点
 * @param w 目标图像宽度
 * @param h  目标图像高度
 * @param pad 边缘裁剪量
 * @return 目标图像
 */
Mat MainWindow::warpImg(Mat img, vector<Point> points, int w, int h, int pad)
{
    Mat imgwarp;

    points = reOrder(points);
    vector<Point2f> pts1;
    cv::Mat(points).convertTo(pts1, CV_32FC1); // vector<Point>转换vector<Point2f>
    vector<Point2f> pts2 = { Point2f(0, 0), Point2f(w, 0), Point2f(w, h), Point2f(0, h) };
    Mat matrix = getPerspectiveTransform(pts1, pts2);
    warpPerspective(img, imgwarp, matrix, Size(w, h));
    imgwarp = imgwarp(Range(pad, imgwarp.rows - pad), Range(pad, imgwarp.cols - pad));
    return imgwarp;
}

/**
 * @brief MainWindow::reOrder 顶点重排序
 * @param points 输入点向量
 * @return 左上角起点，顺时针排序后点向量
 */
vector<Point> MainWindow::reOrder(vector<Point> points)
{
    Point2f tmppoint;

    // 按x坐标从小到大排列
    for (int i = 0; i < 3; i++)
    {
        for (int j = i + 1; j < 4; j++)
        {
            if (points[i].x > points[j].x)
            {
                tmppoint = points[i];
                points[i] = points[j];
                points[j] = tmppoint;
            }
        }
    }

    // 左上角[0]、左下角[1]
    if (points[0].y > points[1].y)
    {
        tmppoint = points[0];
        points[0] = points[1];
        points[1] = tmppoint;
    }
    // 右上角[2]、右下角[3]
    if (points[2].y > points[3].y)
    {
        tmppoint = points[2];
        points[2] = points[3];
        points[3] = tmppoint;
    }

    // 以左上角为起点顺时针排列
    vector<Point> points_clockwise;
    points_clockwise.push_back(points[0]);
    points_clockwise.push_back(points[2]);
    points_clockwise.push_back(points[3]);
    points_clockwise.push_back(points[1]);

    return points_clockwise;
}
