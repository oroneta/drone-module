import pymongo, time

# Import model
# ../model/Images.py

from .model.Images import ImageModel

class DDBB:
    def __init__(self, user, pasw, host, port, db):
        self.client = pymongo.MongoClient(f'mongodb://{user}:{pasw}@{host}:{port}/')

        # Get list of databases, and check if the database exists
        # If not, wait until it is created
        dblist = self.client.list_database_names()
        while db not in dblist:
            print("The database does not exist.")
            time.sleep(3)
            dblist = self.client.list_database_names()

        self.db = self.client[db]


    '''
        Get all drones from database
        @return list

        Return all drones from database
    '''
    def getAllDrones(self):
        return self.db['drones'].find({})
    
    '''
        Insert image to database
        @param image_path: str
        @return str

        Insert image to database and return the id
    '''
    def insertImage(self, image_path):
        with open(image_path, "rb") as image_file:
            binary_image = image_file.read()
            imageInstance = ImageModel(binary_image)

            self.db['images'].insert_one(imageInstance.__dict__)

            return imageInstance.getId()
        

    '''
        Update drone alarm
        @param drone_dic: str
        @param drone_auth: str
        @param image_id: str

        Update drone alarm
    '''
    def updateDroneAlarm(self, drone_dic, drone_auth, image_id):
        '''
            Original:
            Update drone alarm
            {
                "dic": str,
                "auth_code": str,
                "alarm_status": int,
                "alarm_code": int,
                "alarm_data": str
            }
        '''
        # Update drone alarm
        self.db['drones'].update_one(
            {"dic": drone_dic, "auth_code": drone_auth},
            {"$set": {
                "alarm_status": 1,
                "alarm_code": 0,
                "alarm_data": image_id
            }}
        )

    


    # -------- Useful functions --------
        
    def insertUpdateAlarm(self, drone_dic, drone_auth, image_path):
        idImage = self.insertImage(image_path)
        self.updateDroneAlarm(drone_dic, drone_auth, idImage)