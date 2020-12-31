//
// Created by cribin on 11/18/20.
//
#include <iostream>   // for standard I/O
#include <fstream>
#include <vector>

#include "hecate/image_metrics.hpp"
#include "hecate/hecate_aesthetic_metrics_predictor.hpp"


void print_image_metrics(const string metric_name, const double metric_value){
    printf("%s : %f\n", metric_name.c_str(), metric_value);
}

double handle_video(string videoPath, int startFrame, int endFrame)
{
    cv::VideoCapture cap(videoPath);
    if( !cap.isOpened()){
        cout << "Cannot open the video file" << endl;
        return -1;
    }
    cap.set(cv::CAP_PROP_POS_FRAMES, startFrame);

    int numOfFramesRead = 0;
    int numOfFramesToRead = endFrame - startFrame + 1;
    double avgBrightness = 0;
    double avgAsymmetry = 0;
    double avgSharpness = 0;
    for (int i = 0; i < numOfFramesToRead; ++i) {
        cv::Mat frame;
        bool success = cap.read(frame);
        if (!success){
            printf("Cannot read frame. Skipping it");
            continue;
        }
        cv::Mat greyFrame;
        cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
        avgBrightness += hecate::calc_brightness(frame);
        avgSharpness += hecate::calc_sharpness(greyFrame);
        avgAsymmetry += hecate::calc_asymmetry(greyFrame);
        numOfFramesRead++;
    }

    if (numOfFramesRead > 0){
        return avgBrightness / double(numOfFramesRead);
    }
    return -1;
}

int main(int argc, char** argv ) {
    /*if(argc != 2){
        std::cout << argv[0] << "\n";
        std::cout << argv[1] << "\n";
        return -1;
    }
    std::string image_path = argv[1];
     // std::string video_path = "/local/home/cribin/Documents/AestheticsBackup/HecateCppAndPythonServer/hecate/output/video_sum.mp4";
    //std::string image_path = "/local/home/cribin/Documents/AestheticsBackup/Yahoo_Hecate_Image_Signatures/hecate/output/video_00.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty())
    {
        printf("Could not read the image: ");
        return 1;
    }

    cv::Mat greyImg;
    cv::cvtColor(img, greyImg, cv::COLOR_BGR2GRAY);

    double brightness = hecate::calc_brightness( img);
    double contrast_balance = hecate::calc_contrast_balance(img);
    double exposure_balance = hecate::calc_exposure_balance(img);


    double rms_contrast = hecate::calc_rms_contrast(greyImg);
    double sharpness = hecate::calc_sharpness(greyImg);
    double uniformity = hecate::calc_uniformity(greyImg);
    double asymmetry = hecate::calc_asymmetry(greyImg);
   // vector<double> third_saliency = hecate::calc_third_saliency(greyImg);
    double entropy = hecate::calc_entropy(greyImg);
    double jpeg_quality = hecate::calc_jpeg_quality(greyImg);

    print_image_metrics("Brightness", brightness);
    print_image_metrics("ContrastBalance", contrast_balance);
    print_image_metrics("ExposureBalance", exposure_balance);
    print_image_metrics("RMSContrast", rms_contrast);
    print_image_metrics("Sharpness", sharpness);
    print_image_metrics("Uniformity", uniformity);
    print_image_metrics("Asymmetry", asymmetry);
    print_image_metrics("Entropy", entropy);
    print_image_metrics("JpegQuality", jpeg_quality);*/

    if(argc != 4){
        printf("Expected 4 parameters. Received only %i", argc);
        return -1;
    }
    std::string content_path = argv[1];
    int startFrame = atoi(argv[2]);
    int endFrame = atoi(argv[3]);

    hecate::HecateAestheticMetricsPredictor aestheticMetricsPredictor = hecate::HecateAestheticMetricsPredictor();

    aestheticMetricsPredictor.predictAestheticMetrics(content_path, startFrame, endFrame);

    return 0;
}


