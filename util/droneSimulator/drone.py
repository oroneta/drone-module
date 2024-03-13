# Open an image and send it to localhost:60003

import requests
import cv2
import numpy as np

# Read the image
img = cv2.imread('../fireDetectionAI/img/t000.jpg')

# Convert the image to bytes
_, img_encoded = cv2.imencode('.jpg', img)

try:

    # Send the image to localhost:6003 with header 'Authorization': 'Bearer 0
    response = requests.post('http://localhost:60003/0', headers={'Authorization': 'Bearer 0'}, data=img_encoded.tobytes())


    # Print the response status code and content
    print(response.status_code)

except requests.exceptions.RequestException as e:
    print(e)
