import sys, time, requests


if __name__ == "__main__":
    url = 'http://localhost:60001/alarm/0'
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
