'''
    Real time predict with cv camera and show it
'''
import cv2
import numpy as np
import os
from ultralytics import YOLO



# Load model
model = YOLO('./FireDetectModel.pt')

# Read the classes
classnames = ['fire', 'smoke', 'neutral']


# Capture image from camera
cap = cv2.VideoCapture(0)

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
            confidence = int(confidence * 100)
            Class = int(box.cls[0])
            if confidence > 10:
                x1, y1, x2, y2 = box.xyxy[0]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                print(f'{classnames[Class]}, Confidence: {confidence}%')
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
                # Write the class name and confidence
                cv2.putText(frame, f'{classnames[Class]} {confidence}%', (x1 - 8, y1 - 8), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 1)

    # Show the image
    cv2.imshow('Fire Detection AI', frame)

    # Press 'q' to exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break