from flask import Flask, request, abort
from waitress import serve
import numpy as np
import cv2, os, logging

# Import custom libs
# ./lib/ddbb.py
# ./lib/predict.py

from lib.ddbb import DDBB
from lib.logRotate import logRotate

log_name = './var/logs/server.log'
# Rotate logs
logRotate(
    path=os.path.dirname(log_name),
    filename=log_name.split('/')[-1],
    max_size=1e6,
    max_days=15
)

# Set logging level
logging.basicConfig(filename=log_name, 
                    filemode='a', 
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

# Create API server
app = Flask(__name__)

@app.route('/<dic>', methods=['POST'])
def postCamera(dic: str) -> None:
    auth_header = request.headers.get('Authorization')
    if not auth_header:
        # If client (Drone) does not send an Authorization header, return 401
        abort(401)  # Unauthorized

    # Get image from request as bytes
    image = request.data

    # Convert image to numpy array
    image = np.frombuffer(image, dtype=np.uint8)

    # Save image to tmp folder
    cv2.imwrite(f'./tmp/{dic}.jpg', image)

    # Call AI model to process image
    # ...


    return '', 204  # No Content

@app.route('/test', methods=['GET'])
def aa()-> None:
    print("aaa")
    db.insertUpdateAlarm('0', '0', './images-2.png')
    return "Holaa", 200

# @app.route('/test', methods=['GET'])


if __name__ == '__main__':
    # Get port from environment variable: AI_PORT
    port = os.getenv('AI_PORT', 60003)

    if (os.getenv('SERVER_ENV', 'dev') == 'dev'):
        # Run the app with Flask
        app.run(port=port, debug=True)

    else:
        # Serve the app with waitress
        serve(app, port=port)
