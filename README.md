### A4 纸区域内检测矩形物体长+宽

```c++
/**
 * @brief MainWindow::getContours 【获取轮廓信息】
 * @param img 源图像
 * @param cThr0 边缘检测阈值1
 * @param cThr1 边缘检测阈值2
 * @param showCanny 是否显示边缘检测图标志
 * @param minArea 检测的闭合边缘的最小面积
 * @param filter 轮廓多边形拟合的顶点数筛选值
 * @param draw 是否绘制检测轮廓
 * @return 检测轮廓信息结构体向量
 */
vector<UserDefContour> MainWindow::getContours(Mat img,int cThr0, int cThr1, bool showCanny, float minArea, int filter, bool draw)
```



```c++
/**
 * @brief MainWindow::warpImg 【将图像仿射变换】
 * @param img 源图像
 * @param points 目标图像的4顶点
 * @param w 目标图像宽度
 * @param h  目标图像高度
 * @param pad 边缘裁剪量
 * @return 目标图像
 */
Mat MainWindow::warpImg(Mat img, vector<Point> points, int w, int h, int pad)
```



```c++
/**
 * @brief MainWindow::reOrder 【顶点重排序】
 * @param points 输入点向量
 * @return 左上角起点，顺时针排序后点向量
 */
vector<Point> MainWindow::reOrder(vector<Point> points)
```



### 检测结果

<center  class="half">
     <img src="https://cdn.jsdelivr.net/gh/Code-Newborn/PicBed@main/img/Github-Qt/ExperimentTest0119/1-20220211232654.jpg" height="600"/>
    <img src="https://cdn.jsdelivr.net/gh/Code-Newborn/PicBed@main/img/Github-Qt/ExperimentTest0119/result-20220211235055.png" height="600"/>
</center >

