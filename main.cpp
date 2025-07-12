#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < 4) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    // This function uses hardcoded Bernstein polynomial

    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;

    }
}

cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{
    // TODO: Implement de Casteljau's algorithm
    if (control_points.size() == 0)
    {
        std::cout << "Error!\n" << std::endl;
        return cv::Point2f();
    }

    if (control_points.size() == 1)
    {
        return control_points[0];
    }


    std::vector<cv::Point2f> c_points;
    cv::Point2f p_result;

    for (int i = 0; i + 1 < control_points.size(); i++)
    {
        cv::Point2f point = control_points[i];
        cv::Point2f next_p = control_points[i+1];

        cv::Point2f new_p = point * (1 - t) + next_p * t;

        c_points.push_back(new_p);
    }

    p_result = recursive_bezier(c_points, t);    

    return p_result;
}

void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{
    // Iterate through all t = 0 to t = 1 with small steps, and call de Casteljau's recursive Bezier algorithm.
    for (double t = 0.0; t <= 1.0; t += 0.0001)
    {
        auto point = recursive_bezier(control_points, t);

        int left = std::floor(point.x);
        int right = std::ceil(point.x);
        int bottom = std::ceil(point.y);
        int top = std::floor(point.y);

        cv::Point2i lb_pixel(left, bottom);
        cv::Point2i lt_pixel(left, top);
        cv::Point2i rb_pixel(right, bottom);
        cv::Point2i rt_pixel(right, top);

        float right_weight = point.x - (float)left;
        float left_weight = 1.0f - right_weight;
        float bottom_weight = point.y - (float)top;
        float top_weight = 1.0f - bottom_weight;

        int lb_val = std::round(255 * left_weight * bottom_weight);
        int lt_val = std::round(255 * left_weight * top_weight);
        int rb_val = std::round(255 * right_weight * bottom_weight);
        int rt_val = std::round(255 * right_weight * top_weight);

        // Helper lambda for bounds checking
        auto in_bounds = [&](const cv::Point2i& pt) {
            return pt.x >= 0 && pt.x < window.cols && pt.y >= 0 && pt.y < window.rows;
        };

        if (in_bounds(lb_pixel)) {
            cv::Vec3b &pixel = window.at<cv::Vec3b>(lb_pixel.y, lb_pixel.x);
            pixel[1] = std::max(lb_val, (int)pixel[1]);
        }
        if (in_bounds(lt_pixel)) {
            cv::Vec3b &pixel = window.at<cv::Vec3b>(lt_pixel.y, lt_pixel.x);
            pixel[1] = std::max(lt_val, (int)pixel[1]);
        }
        if (in_bounds(rb_pixel)) {
            cv::Vec3b &pixel = window.at<cv::Vec3b>(rb_pixel.y, rb_pixel.x);
            pixel[1] = std::max(rb_val, (int)pixel[1]);
        }
        if (in_bounds(rt_pixel)) {
            cv::Vec3b &pixel = window.at<cv::Vec3b>(rt_pixel.y, rt_pixel.x);
            pixel[1] = std::max(rt_val, (int)pixel[1]);
        }
    }
}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    // for debugs
    control_points.emplace_back(108, 473);
    control_points.emplace_back(232, 188);
    control_points.emplace_back(485, 113);
    control_points.emplace_back(561, 381);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == 4) 
        {
            // naive_bezier(control_points, window);
            bezier(control_points, window);

            // std::cout << "hi" << std::endl;

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
