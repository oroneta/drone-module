FROM python:3.9

# Set the working directory in the container
WORKDIR /opt/drone-ai

# Copy the rest of the application code to the working directory
COPY ./src/drone-ai .

# Install dependencies
RUN pip install -r requirements.txt

# Get the model
RUN mkdir ./model
RUN wget -P ./model -q -O model.h5 https://github.com/OlafenwaMoses/FireNET/releases/download/v1.0/detection_model-ex-33--loss-4.97.h5

# Command to run the application
CMD ["python", "index.py"]