import cv2
from ultralytics import YOLO
import math
import os

# Cargar el modelo
model = YOLO('./FireDetectBestModel.pt')

# Leer las clases
classnames = ['fire', 'smoke', 'neutral']

# Crear un directorio para guardar las imágenes procesadas
os.makedirs('../../tmp', exist_ok=True)

# Lista de imágenes a procesar
# Coger las imagenes de ./img sin . .. y otros que no sean .jpg
images = [img for img in os.listdir('../../img') if img.endswith('.jpg')]

for image_path in images:
    # Leer la imagen
    frame = cv2.imread(f'../../img/{image_path}')
    frame = cv2.resize(frame, (640, 480))

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

    # Guardar la imagen procesada
    processed_image_path = os.path.join('../../tmp', os.path.basename(image_path))
    cv2.imwrite(processed_image_path, frame)
