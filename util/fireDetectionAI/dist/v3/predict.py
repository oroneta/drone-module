import cv2
from ultralytics import YOLO
import math
import os

# Cargar el modelo
model = YOLO('./FireGuardBestModel.pt')

# Leer las clases
classnames = ['fire', 'neutral', 'smoke']

# Crear un directorio para guardar las imágenes procesadas
os.makedirs('../../tmp', exist_ok=True)

# Lista de imágenes a procesar
# Coger las imagenes de ./img sin . .. y otros que no sean .jpg
images = [img for img in os.listdir('../../img') if img.endswith('.jpg')]

def draw_text(img, text,
          font=cv2.FONT_HERSHEY_SIMPLEX,
          pos=(0, 0),
          font_scale=1,
          font_thickness=1,
          text_color=(0, 255, 0),
          text_color_bg=(0, 0, 0)
          ):

    x, y = pos
    text_size, _ = cv2.getTextSize(text, font, font_scale, font_thickness)
    text_w, text_h = text_size
    cv2.rectangle(img, pos, (x + text_w, y + text_h), text_color_bg, -1)
    cv2.putText(img, text, (x, y + text_h + font_scale - 1), font, font_scale, text_color, font_thickness)

    return text_size

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
            if confidence > 8:
                x1, y1, x2, y2 = box.xyxy[0]
                x1, y1, x2, y2 = int(x1), int(y1), int(x2), int(y2)
                print(f'Class: {classnames[Class]}, Confidence: {confidence}%')
                # BGR Smoke = Yellow, Fire = Red, Neutral = Green
                if Class == 0:
                    B, G, R = 0, 0, 255
                elif Class == 1:
                    continue
                else:
                    B, G, R = 0, 255, 255
                cv2.rectangle(frame, (x1, y1), (x2, y2), (B, G, R), 2)

                # Write the class name and confidence with background
                POS = (x1 + 1, y1 + 8)
                text = f'{classnames[Class]} {confidence}%'
                text_size = draw_text(frame, text, pos=POS, text_color_bg=(B, G, R), text_color=(0, 0, 0))


    # Guardar la imagen procesada
    processed_image_path = os.path.join('../../tmp', os.path.basename(image_path))
    cv2.imwrite(processed_image_path, frame)