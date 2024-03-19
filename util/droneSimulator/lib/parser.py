'''

    Protocol ADMP parser

    Example:

        =======
        ADMP 0.1\r\n
        WAKE\r\n
        201 OK\r\n
        \r\n
        {"set": {"camera_server": "192.168.0.1", "camera_port": 60003, "timeval": 200}}\r\n
        \r\n
        ========
        {
            protocol: "ADMP",
            version: "0.1",
            command: "WAKE",
            header: {
                status: ["201", "OK"]
            },
            body: {
                set: {
                    camera_server: "192.168.0.1",
                    camera_port: 60003,
                    timeval: 200
                }
            }
        }
'''

import re
import json

def ClientParser(data):
    # Split the data
    data = data.split('\r\n')

    # Get the protocol
    protocol = data[0].split(' ')[0]

    # Get the version
    version = data[0].split(' ')[1]

    # Get the command
    command = data[1]

    # Get the header
    header = data[2].split(' ')

    # Get the body
    body = data[4]

    # Parse the body
    body = json.loads(body)

    return {
        'protocol': protocol,
        'version': version,
        'command': command,
        'header': {
            'status': header
        },
        'body': body
    }