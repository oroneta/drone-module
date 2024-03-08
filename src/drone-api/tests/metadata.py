import sys, time, requests


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Example of use: python {} <metadata>".format(sys.argv[0]))
        exit(-1)

    url = 'http://127.0.0.1:60001/metadata/'+sys.argv[1]+'/0'
    headers = {
        'Authorization': 'Bearer 0'
    }
    try:
        response = requests.get(url, headers=headers)
        if response.status_code == 200:
            print("Request succeded with text:\n", response.text)
        else:
            print("Request failed with code:", response.status_code, '\nContent:\n', response.text)
    except requests.exceptions.RequestException as e:
        print("Error doing GET request:", e)