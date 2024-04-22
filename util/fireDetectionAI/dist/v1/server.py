import cv2
from ultralytics import YOLO
from flask import Flask, request
import numpy as np
import math
import os

# Cargar el modelo
model = YOLO('./FireDetectModel.pt')

# Leer las clases
classnames = ['fire', 'smoke', 'neutral']

# Flask
app = Flask(__name__)

@app.route('/0', methods=['POST'])
def upload():
    print('Received image')
    # Check content type is image/jpeg
    if request.headers['Content-Type'] != 'image/jpeg':
        return 'Content-Type debe ser image/jpeg'
    
    img_data = request.data  # obtén los datos de la imagen
    nparr = np.fromstring(img_data, np.uint8)
    frame = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

    # Procesar la imagen con YOLO
    result = model(frame, stream=True)
    # Get confidence and class names
    # print(result[0])

    # Obtener la información de bbox, confianza y nombres de clases para trabajar con ella
    for info in result:
        boxes = info.boxes
        for box in boxes:
            confidence = box.conf[0]
            confidence = math.ceil(confidence * 100)
            Class = int(box.cls[0])
            if confidence > 10:
                x1, y1, x2, y2 = box.xyxy[0]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                print(f'Class: {classnames[Class]}, Confidence: {confidence}%')
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 0, 255), 2)
                # Write the class name and confidence
                cv2.putText(frame, f'{classnames[Class]} {confidence}%', (x1 - 8, y1 - 8), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 1)


                # cvzone.putTextRect(frame, f'{classnames[Class]} {confidence}%', [x1 + 8, y1 + 100],
                #                    scale=1.5, thickness=2)

    # Show the image
    # cv2.imshow('Image', frame)
    cv2.imwrite('../../server-processed.jpg', frame)


    # Return 201
    return '', 201

if __name__ == '__main__':
    # Run server and listen on port 8000 to be accessible from the network
    app.run(host='0.0.0.0', port=8000)
