//
// Created by cribin on 11/27/20.
//

#include "hecate/hecate_aesthetic_metrics_predictor.hpp"

using namespace std;
using namespace cv;
using namespace hecate;

HecateAestheticMetricsPredictor::HecateAestheticMetricsPredictor() {
    initImageMetricFunctionPointers();
}

void HecateAestheticMetricsPredictor::initImageMetricFunctionPointers() {
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::Brightness)] = &hecate::calc_brightness;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::ContrastBalance)] = &hecate::calc_contrast_balance;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::ExposureBalance)] = &hecate::calc_exposure_balance;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::RmsContrast)] = &hecate::calc_rms_contrast;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::Sharpness)] = &hecate::calc_sharpness;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::Uniformity)] = &hecate::calc_uniformity_wrapper;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::Asymmetry)] = &hecate::calc_asymmetry;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::Entropy)] = &hecate::calc_entropy_wrapper;
    imageMetricFunctionPointerArray[static_cast<int>(image_metrics::JpegQuality)] = &hecate::calc_jpeg_quality;

}

aesthetic_metrics
HecateAestheticMetricsPredictor::predictAestheticMetrics(const string contentPath, int startFrame, int endFrame) {
    if (endFrame == 0) {
        //printf("Predicting for image\n");
        return predictImageAestheticMetrics(contentPath);
    } else if (endFrame > 0) {
        //printf("Predicting for video\n");
        return predictVideoAestheticMetrics(contentPath, startFrame, endFrame);
    }

    aesthetic_metrics test;
    return test;
}

aesthetic_metrics HecateAestheticMetricsPredictor::predictImageAestheticMetrics(const string imagePath) {
    aesthetic_metrics test;

    Mat img = imread(imagePath, IMREAD_COLOR);
    if (img.empty()) {
        printf("Could not read the image: ");
        return test;
    }

    cv::Mat greyImg;
    cv::cvtColor(img, greyImg, cv::COLOR_BGR2GRAY);

    //!!!local array is not 0 initialized!!!!
    array<double, numOfMetrics> predictedImageMetrics;
    predictedImageMetrics.fill(0.0);
    Mat currentImg;
    for (int i = image_metrics::Brightness; i != image_metrics::Last; i++) {
        currentImg = (i < 3) ? img : greyImg;
        predictedImageMetrics[i] = imageMetricFunctionPointerArray[i](currentImg);
        printImageMetrics(image_metrics_string[i], predictedImageMetrics[i]);
    }
    test = {predictedImageMetrics};
    return test;
}

aesthetic_metrics
HecateAestheticMetricsPredictor::predictVideoAestheticMetrics(const string videoPath, int startFrame, int endFrame) {
    aesthetic_metrics test;

    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cout << "Cannot open the video file" << endl;
        return test;
    }
    cap.set(CAP_PROP_POS_FRAMES, startFrame);

    int numOfFramesRead = 0;
    int numOfFramesToRead = endFrame - startFrame;


    array<double, numOfMetrics> predictedAvgFrameMetrics;
    predictedAvgFrameMetrics.fill(0.0);
    Mat currentFrame;
    for (int i = 0; i < numOfFramesToRead; i++) {
        Mat frame;
        bool success = cap.read(frame);
        if (!success) {
            printf("Cannot read frame. Skipping it");
            continue;
        }

        Mat greyFrame;
        cvtColor(frame, greyFrame, COLOR_BGR2GRAY);

        for (int metric = image_metrics::Brightness; metric != image_metrics::Last; metric++) {
            currentFrame = (metric < 3) ? frame : greyFrame;
            predictedAvgFrameMetrics[metric] += imageMetricFunctionPointerArray[metric](currentFrame);
        }

        numOfFramesRead++;
    }

    //printf("Rms Contrast Test %f",  predictedAvgFrameMetrics[static_cast<int>(image_metrics::RmsContrast)]);
    if (numOfFramesRead > 0) {
        for (int metric = image_metrics::Brightness; metric != image_metrics::Last; metric++) {
            predictedAvgFrameMetrics[metric] /= numOfFramesRead;
            printImageMetrics(image_metrics_string[metric], predictedAvgFrameMetrics[metric]);
        }
    }

    test = {predictedAvgFrameMetrics};
    return test;
}