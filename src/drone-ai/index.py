from flask import Flask, request, abort
import numpy as np
import cv2


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




if __name__ == '__main__':
    app.run(debug=True)
