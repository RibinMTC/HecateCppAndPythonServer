import subprocess

from flask import jsonify


class HecateContentMetricsPredictor:
    """
    Main class which predicts attributes for a given image and video.
        1.  Communicates with the compiled c++ hecate code via subprocess to send images and videos and receive response.
        2.  Predicted Features: RMS_Contrast, Sharpness, Brightness, Uniformity, Asymmetry, Entropy, Contrast Balance,
            Exposure Balance, JPEG Quality
    This class is utilized by the aesthetics_predictor_api_pk, which delegates flask requests to the predict method and
    sends the response back to the client.
    """

    def __init__(self, custom_hecate_executable_path=None):
        """
        :param custom_hecate_executable_path: Path to the c++ hecate executable(leave empty, when used in a docker setup)
        """
        if custom_hecate_executable_path is not None:
            self.__hecate_executable_path = custom_hecate_executable_path
        else:
            self.__hecate_executable_path = "/imageMetricsPredictor/hecateCpp/distribute/bin/hecate_image_metric_test"

    def predict_content_metrics_debug(self, content_path, start_frame, end_frame):
        result = subprocess.run([self.__hecate_executable_path, content_path, start_frame, end_frame], stdout=subprocess.PIPE)
        predicted_image_metrics_str = str(result.stdout)
        print(predicted_image_metrics_str)

    def predict(self, content_path, start_frame, end_frame):
        result = subprocess.run([self.__hecate_executable_path, content_path, start_frame, end_frame], stdout=subprocess.PIPE)
        predicted_image_metrics_str = str(result.stdout)
        print("Hecate Output: " + predicted_image_metrics_str)
        if self.__validate_predicted_content_metrics(predicted_image_metrics_str):
            return self.__jsonify_results(predicted_image_metrics_str)
        return "Error in Prediction!", 400

    def __validate_predicted_content_metrics(self, predicted_content_metrics_str):
        if predicted_content_metrics_str is not None and "Brightness" in predicted_content_metrics_str:
            return True
        return False

    def __jsonify_results(self, predicted_content_metrics_str):
        split_content_metrics = predicted_content_metrics_str.split('\\n')
        if len(split_content_metrics) == 0:
            print("Could not parse image metrics: " + predicted_content_metrics_str + ": correctly. Aborting prediction")
            return
        # remove last element, as this is simply the dash character
        split_content_metrics.pop()
        # remove invalid character from first image metric
        split_content_metrics[0] = split_content_metrics[0][2:]
        content_metric_name_to_value_dict = {}
        for content_metric in split_content_metrics:
            split_content_metric = content_metric.split(':')
            if len(split_content_metric) != 2:
                print("Could not parse image metrics: " + split_content_metrics + " correctly. Aborting prediction")
                continue
            content_metric_name = split_content_metric[0].strip()
            content_metric_value = float(split_content_metric[1].strip())
            content_metric_name_to_value_dict[content_metric_name] = content_metric_value

        return jsonify(content_metric_name_to_value_dict)

