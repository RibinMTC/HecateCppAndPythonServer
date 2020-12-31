## HecateCppAndPythonServer

HecateCppAndPythonServer is a tool to predict image and video metrics for the aesthetic assessment project. The project utilizes the [Hecate](https://github.com/yahoo/hecate) video processing library.


1. The predictor uses a python server running Flask (by default listening on *localhost:5004*) to listen to incoming requests, which should contain the path to the image or video to predict.
2. The python server communicates with a prebuild c++ hecate code, to perform the prediction and returns the results back to the client.
3. Hecate can predict the following features: RMS_Contrast, Sharpness, Brightness, Uniformity, Asymmetry, Entropy, Contrast Balance,
            Exposure Balance, JPEG Quality. (See [here](https://github.com/yahoo/hecate/blob/master/include/hecate/image_metrics.hpp) for more details)

### Requirements

This project requires 
- Python 3
- OpenCv 
- ffmpeg

### Installation

Clone this repository and from the project root run the following commands to build the c++ hecate code:

```bash
cd hecate
make all
make distribute
```

Run the following commands from the project root to install the python server requirements.
```bash
cd HecateImageMetricsPredictorServer
pip install -r requirements.txt
```
 
### Usage

The recommended usage is with a docker-compose file.

For testing the code without using Flask, run the following code:

```python
from src.hecate.hecate_content_metrics_predictor import HecateContentMetricsPredictor

hecate_base_file = "path/to/hecate/build/folder"  # Example: ../hecate/distribute/bin/hecate_image_metric_test"
hecateContentMetricsPredictor = HecateContentMetricsPredictor(hecate_base_file)
hecateContentMetricsPredictor.predict("path/to/image", 0, 0)
# hecateContentMetricsPredictor.predict("path/to/video", startFrame, endFrame)
```

To start the Flask Api, which listens for incoming prediction requests run:

```bash
gunicorn --config gunicorn_config.py --env API_CONFIG=api_config.json aesthetics_predictor_api_pkg.predictor_api_server:app
```