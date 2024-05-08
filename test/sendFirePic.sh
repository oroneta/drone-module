curl -v -X POST \
    'http://localhost:60003/0' \
    -H 'Authorization: Bearer 0' \
    -H 'Content-Type: image/jpg' \
    --data-binary '@./fire.jpg'