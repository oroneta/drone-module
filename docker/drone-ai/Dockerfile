FROM python:3.9

# Set the working directory in the container
WORKDIR /opt/drone-ai

# Copy the rest of the application code to the working directory
COPY ./src/drone-ai .

# Install OpenCV dependencies
RUN apt-get update
RUN apt-get install -y libgl1-mesa-glx libglib2.0-0

# Install dependencies
RUN pip install -r requirements.txt

# Download model if not exists
COPY ./util/fireDetectionAI/dist/v1/FireDetectModel.pt ./model/${AI_MODEL_NAME}
COPY ./util/fireDetectionAI/img/t008.jpg ./img/${AI_INITIALIZE_IMAGE}

# Create var if not exists
RUN mkdir -p ./var
RUN mkdir -p ./var/logs
RUN mkdir -p ./var/tmp

# Command to run the application
CMD ["python", "index.py"]