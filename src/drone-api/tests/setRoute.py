import sys, time, requests


if __name__ == "__main__":

    url = 'http://127.0.0.1:60001/metadata/'+sys.argv[1]+'/0'
    headers = {
        'Authorization': 'Bearer 0'
    }

    route = [(1,2), (2,3)]
    try:
        # Send post request to set the route, route to body
        response = requests.post(url, headers=headers, json=route)
        if response.status_code == 200:
            print("Request succeded with text:\n", response.text)
        else:
            print("Request failed with code:", response.status_code, '\nContent:\n', response.text)
    except requests.exceptions.RequestException as e:
        print("Error doing GET request:", e)
