import cv2
from ultralytics import YOLO
import math
import os

# Cargar el modelo
model = YOLO('./FireDetectModel.pt')


# Leer las clases
classnames = ['fire', 'smoke', 'neutral']


# Get video from ../video/FireForest.mp4 and save it to ../video/FireForestProcessed.mp4

# Capture video from file
cap = cv2.VideoCapture('../../video/FireForest2.mp4')

# Get frame and predict with AI and save it to a new video
fps = int(cap.get(cv2.CAP_PROP_FPS))
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

out = cv2.VideoWriter('../../video/FireForest2Processed.mp4', cv2.VideoWriter_fourcc(*'mp4v'), fps, (width, height))

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Process the image with YOLO
    result = model(frame, stream=True)

    # Get confidence and class names
    for info in result:
        boxes = info.boxes
        for box in boxes:
            confidence = box.conf[0]
            confidence = math.ceil(confidence * 100)
            Class = int(box.cls[0])
            if confidence > 10:
                x1, y1, x2, y2 = box.xyxy[0]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                # print(f'Class: {classnames[Class]}, Confidence: {confidence}%')
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
                # Write the class name and confidence
                cv2.putText(frame, f'{classnames[Class]} {confidence}%', (x1 - 8, y1 - 8), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 1)

    cv2.putText(frame, f'Oroneta', (40, 40), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    # Save the image
    out.write(frame)

# Release the video
cap.release()
out.release()
cv2.destroyAllWindows()