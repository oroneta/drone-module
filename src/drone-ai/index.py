from flask import Flask, request, abort
from waitress import serve
import numpy as np
import cv2, os, logging
from ultralytics import YOLO

# Import custom libs
# ./lib/ddbb.py
# ./lib/predict.py

from lib.ddbb import DDBB
from lib.logRotate import logRotate
from lib.predict import predictAI

log_name = os.path.abspath(os.path.join(os.path.dirname(__file__), 'var', 'logs', 'server.log'))
# Rotate logs
logRotate(
    path=os.path.dirname(log_name),
    filename=log_name.split('/')[-1],
    max_size=1e6,
    max_days=15
)

# Set logging level
logging.basicConfig(filename=log_name, 
                    filemode='a+', 
                    format='%(process)d %(levelname)s - %(asctime)s - %(message)s',
                    level=(logging.DEBUG if os.getenv('SERVER_ENV', 'dev') == 'dev' else logging.INFO))


# Create database connection
db = DDBB(
        os.getenv('MONGO_DBUSER', "admin"),
        os.getenv('MONGO_DBPASS', "admin"),
        os.getenv('MONGO_DBSEVR', "127.0.0.1"),
        os.getenv('MONGO_DBPORT', 27017),
        os.getenv('MONGO_DBNAME', "drone-module-db")
    )


'''

    Precharge model to predict images

'''
# Load model
model = YOLO('./model/'+os.getenv('AI_MODEL_NAME', 'FireDetectModel.pt'))


# Create API server
app = Flask(__name__)

@app.route('/<dic>', methods=['POST'])
def postCamera(dic: str) -> None:
    auth_header = request.headers.get('Authorization')
    if auth_header:
        auth_header = auth_header.split(' ')[-1]
    else:
        # If client (Drone) does not send an Authorization header, return 401
        abort(401) # Unauthorized

    # Check if exists the drone in the database
    if not db.getDrone(dic, auth_header):
        # If not, return 404
        abort(404)  # Not Found

    logging.debug(f'POST /{dic} Authorized: {auth_header}')

    # Get image from request as bytes
    image = request.data

    # Convert image to numpy array
    image = cv2.imdecode(
                np.frombuffer(image, dtype=np.uint8), cv2.IMREAD_COLOR)
    
    # Resize image if necessary, for example to 640x480
    # Many models require a fixed size image to predict
    # image = cv2.resize(image, (640, 480))

    # Predict image
    status, confidence, processed_image_path = predictAI(image, model=model, name=f'{dic}.jpg', precision=os.getenv('AI_MODEL_PRECISION', 50))

    # If the image has a fire, insert it into the database
    if status:
        image_id = db.insertUpdateAlarm(dic, auth_header, processed_image_path)
        logging.debug(f'Image {image_id} has a fire')
        return 'Success with alarm', 200


    return 'Success without alarm', 201


if __name__ == '__main__':
    # Get port from environment variable: AI_PORT
    port = os.getenv('AI_PORT', 60003)

    if (os.getenv('SERVER_ENV', 'dev') == 'dev'):
        # Run the app with Flask
        app.run(host="0.0.0.0", port=port, debug=True)

    else:
        # Serve the app with waitress
        serve(app, host="0.0.0.0", port=port)
