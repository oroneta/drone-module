import cv2
from ultralytics import YOLO
import math
import os


'''
    Function to predict image

    :param str path: Path to image
    :return: Tuple of (int, str) -> (confidence, output_path)

    :Example:
'''
def predictAI(frame: bytes, model, name: str, precision: int) -> tuple:
    # Read the classes
    classnames = ['fire', 'neutral', 'smoke']

    # Create a directory to save the processed images
    os.makedirs('./var/tmp', exist_ok=True)

    # Process the image with YOLO
    result = model(frame, stream=True)

    confidence_list = []
    status = False
    precision = int(precision)

    # Get confidence and class names
    for info in result:
        boxes = info.boxes
        for box in boxes:
            confidence = box.conf[0]
            confidence = math.ceil(confidence * 100)
            Class = int(box.cls[0])
            if confidence > precision:
                status = True
                confidence_list.append(confidence)
                x1, y1, x2, y2 = box.xyxy[0]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
                # Write the class name and confidence
                cv2.putText(frame, f'{classnames[Class]} {confidence}%', (x1 - 8, y1 - 8), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 1)

    if status:
        # Save the processed image
        processed_image_path = os.path.join('./var/tmp', name)
        cv2.imwrite(processed_image_path, frame)

    return status, confidence_list, processed_image_path