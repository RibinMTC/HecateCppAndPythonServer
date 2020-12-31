"""
Module for testing hecate predictor
"""
from src.hecate.hecate_content_metrics_predictor import HecateContentMetricsPredictor

hecate_base_file = "path/to/hecate/build/folder"  # Example: ../hecate/distribute/bin/hecate_image_metric_test"
hecateContentMetricsPredictor = HecateContentMetricsPredictor(hecate_base_file)
hecateContentMetricsPredictor.predict("path/to/image", 0, 0)
# hecateContentMetricsPredictor.predict("path/to/video", startFrame, endFrame)
