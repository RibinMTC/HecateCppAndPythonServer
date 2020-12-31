FROM spmallick/opencv-docker:opencv

MAINTAINER ribin chalumattu <cribin@inf.ethz.ch>

# This prevents Python from writing out pyc files
ENV PYTHONDONTWRITEBYTECODE 1
# This keeps Python from buffering stdin/stdout
ENV PYTHONUNBUFFERED 1

# set work directory
WORKDIR /imageMetricsPredictor/hecateCpp

# copy hecate folder
COPY ./hecate .

RUN ["make", "all"]
RUN ["make", "distribute"]

# set work directory
WORKDIR /imageMetricsPredictor/hecatePythonServer

# install system dependencies
#RUN apt-get update \
#    && apt -y install python3 python3-pip
#
## install dependencies
#RUN python3 -m pip install --no-cache-dir --upgrade pip

# copy requirements.txt
COPY ./HecateImageMetricsPredictorServer/requirements.txt ./requirements.txt

# install project requirements
RUN pip install --no-cache-dir -r requirements.txt


# copy project
COPY ./HecateImageMetricsPredictorServer .

# set app port
EXPOSE 5004


#ENTRYPOINT ["gunicorn", "--config", "gunicorn_config.py", "src.content_metrics_predictor_server:app"]
ENTRYPOINT ["gunicorn", "--config", "gunicorn_config.py", "--env", "API_CONFIG=api_config.json", "aesthetics_predictor_api_pkg.predictor_api_server:app"]
