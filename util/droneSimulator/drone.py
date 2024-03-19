# Entire drone simulator arch
import socket, requests
import sys, os
import cv2
import numpy as np
from time import sleep

# Client Parser
import sys
from lib.parser import ClientParser

# Check if exists argument
if len(sys.argv) != 3:
    print(f'Usage: python {sys.argv[0]} <ip> <sleep>')
    sys.exit(1)

# Get the IP
IP = sys.argv[1]
SLEEP = int(sys.argv[2])
DRONE_ID = 'ESP00000-000-0001'
DRONE_KEY = 'b76f35d359b0db069a2ec05822df4731'
DRONE_MODULES = ["camera", "gps", "altitude", "humidity"]
DRONE_TIMEVAL = 120
DRONE_MANAGER = 2


try:
    # Connect to the server 60002 with plain socket (no http)
    while True:
        print(f'Connecting to Manager {IP} 60002')
        SOCK_60002 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        SOCK_60002.connect((IP, 60002))
        print('Connected')

        # Send WAKE with own protocol ADMP
        '''
            Example:

            =======
            ADMP 0.1\r\n
            WAKE\r\n
            ESP00000-000-0001\r\n
            b76f35d359b0db069a2ec05822df4731\r\n
            \r\n
            {"set": {"module": ["camera", "gps", "altitude", "humidty"], "timeval": 120, "manager": 2}}\r\n
            \r\n
        '''
        print('Sending WAKE')
        SOCK_60002.sendall(f'ADMP 0.1\r\nWAKE\r\n{DRONE_ID}\r\n{DRONE_KEY}\r\n\r\n{{"set": {{"module": {DRONE_MODULES}, "timeval": {DRONE_TIMEVAL}, "manager": {DRONE_MANAGER}}}}}\r\n\r\n'.encode('utf-8'))
        print('Sent')

        # Receive the response
        print('Receiving')
        data = SOCK_60002.recv(1024)
        print('Received', repr(data))
        data = ClientParser(data.decode('utf-8'))

        # Check if the command is WAKE
        if data["command"] != 'WAKE':
            print(f'Command "{data["command"]}" received')       
            sleep(SLEEP)
        
        else:
            print('Command WAKE received')

            # Check status code
            if data["header"]["status"][0] != '201' or data["header"]["status"][0] != '200':
                print(f'Status code "{data["header"]["status"][0]}" received')
                sleep(SLEEP)

            else:
                print('Status code 201 received')
                break
    
    # Connected to the server 60002 and operative
    print("=======================")
    # Get the camera server and port
    CAMERA_SERVER = data["body"]["set"]["camera_server"]
    CAMERA_PORT = data["body"]["set"]["camera_port"]

    '''
        Fork the process:

        1. Manager
        2. Camera
    '''
    # Fork the process
    pid = os.fork()

    # Child for camera
    if pid == 0:
        # Child process use HTTP to send current camera image
        print('Child process started')
        print('Connecting to Camera', CAMERA_SERVER, CAMERA_PORT)
        # Get image from camera and send it to the server via POST
        while True:
            # Get the image
            cap = cv2.VideoCapture(0)
            ret, frame = cap.read()
            cap.release()
            cv2.destroyAllWindows()
            # Encode the image
            _, img_encoded = cv2.imencode('.jpg', frame)
            # Send the image to the server
            try:
                response = requests.post(f'http://{CAMERA_SERVER}:{CAMERA_PORT}/camera', files={'image': img_encoded.tostring()})
                print('Image sent', response.status_code)
            except requests.exceptions.RequestException as e:
                print(e)
            sleep(DRONE_TIMEVAL/1000)

    # Parent for manager
    else:
        # Send data to the server 60002
        print('Parent process started')

        GPS = [0, 0]

        # Send random simulated data to the server 60002

        '''
            Example:

            =======
            ADMP 0.1\r\n
            PUSH\r\n
            ESP00000-000-0001\r\n
            b76f35d359b0db069a2ec05822df4731\r\n
            \r\n
            {"set": {"gps": [0, 0], "altitude": 0, "humidity": 0}}\r\n
            \r\n
        '''
        while True:
            push = f'ADMP 0.1\r\nPUSH\r\n{DRONE_ID}\r\n{DRONE_KEY}\r\n\r\n{{"set": {{"gps": {GPS}, "altitude": 0, "humidity": 0}}\r\n\r\n'
            print(f'Sending PUSH {push}')
            SOCK_60002.sendall(push.encode('utf-8'))

            # Receive the response
            print('Receiving')
            data = SOCK_60002.recv(1024)
            print('Received', repr(data))

            # Parse the response
            data = ClientParser(data.decode('utf-8'))

            # Check if the command is RECEIVE
            if data["command"] != 'RECEIVE':
                print(f'Command "{data["command"]}" received')       

                print('Error. Cannot continue due server error. Exiting... (Drone stopping)')
                SOCK_60002.close()
                sys.exit(1)
            
            else:
                print('Command RECEIVE received')

                # Check status code
                if data["header"]["status"][0] != '200':
                    print(f'Status code "{data["header"]["status"][0]}" received')

                    print('Error. Cannot continue due server error. Exiting... (Drone stopping)')
                    SOCK_60002.close()
                    sys.exit(1)

                else:
                    print('Status code 200 received')
                    # Show body
                    print('Body', data["body"])
                    
            sleep(DRONE_TIMEVAL/1000)
        

except KeyboardInterrupt:
    print('Drone stopping')
    SOCK_60002.close()
    sys.exit(0)
