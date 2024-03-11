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

# Create var if not exists
RUN mkdir -p ./var

RUN mkdir -p ./var/model
# Download model if not exists
# RUN [ -e "./${AI_MODEL_NAME}" ] || wget -P ./model -q -O model.h5 https://github.com/OlafenwaMoses/FireNET/releases/download/v1.0/${AI_MODEL_NAME}

RUN mkdir -p ./var/logs

# Command to run the application
CMD ["python", "index.py"]