//
// Created by cribin on 11/27/20.
//

#ifndef HECATE_HECATE_AESTHETIC_METRICS_PREDICTOR_HPP
#define HECATE_HECATE_AESTHETIC_METRICS_PREDICTOR_HPP

#endif //HECATE_HECATE_AESTHETIC_METRICS_PREDICTOR_HPP

#include <iostream>   // for standard I/O
#include <fstream>
#include <vector>

// OpenCV library
#include <opencv2/opencv.hpp>

#include "hecate/image_metrics.hpp"

using namespace std;
using namespace cv;

namespace hecate {

    enum image_metrics {Brightness, ContrastBalance, ExposureBalance, RmsContrast, Sharpness, Uniformity, Asymmetry, Entropy, JpegQuality, Last} imgMetric;

    const int numOfMetrics = static_cast<int>(image_metrics::Last);

    array<string, numOfMetrics> image_metrics_string = {"Brightness", "ContrastBalance", "ExposureBalance", "RmsContrast", "Sharpness", "Uniformity", "Asymmetry", "Entropy", "JpegQuality"};

    struct aesthetic_metrics {
        array<double, numOfMetrics> predictedImageMetrics;
    };

    typedef double (*pointerToImageMetricFunction)(const Mat &);

    class HecateAestheticMetricsPredictor {
    public:
        HecateAestheticMetricsPredictor();

        aesthetic_metrics predictAestheticMetrics(const string contentPath, int startFrame, int endFrame);

    private:

        void initImageMetricFunctionPointers();

        aesthetic_metrics predictImageAestheticMetrics(const string imagePath);

        aesthetic_metrics predictVideoAestheticMetrics(const string videoPath, int startFrame, int endFrame);

        inline void printImageMetrics(const string metric_name, const double metric_value){
            printf("%s : %f\n", metric_name.c_str(), metric_value);
        }

    private:

        //array<string, static_cast<int>(image_metrics::Last> image_metrics_string_bla;// = {"Brightness", "ContrastBalance", "ExposureBalance", "RmsContrast", "Sharpness", "Uniformity", "Asymmetry", "Entropy", "JpegQuality"};

        array<pointerToImageMetricFunction, numOfMetrics> imageMetricFunctionPointerArray;

    };
}