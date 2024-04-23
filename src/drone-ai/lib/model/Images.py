import hashlib, datetime, random

'''
    Image model to create new instances of mongo objects
'''

class ImageModel:
    def __init__(self, image_binary):
        # Generate expire_date now() + 3days
        expire_date = datetime.datetime.now() + datetime.timedelta(days=3)
        hashD = hashlib.md5((datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S") + str(random.randint(100, 999))).encode('utf-8')).hexdigest()
        
        self.id = hashD
        self.data = image_binary
        self.expire_date = expire_date

    def getId(self):
        return self.id