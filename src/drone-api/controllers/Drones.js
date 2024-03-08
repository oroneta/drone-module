import DroneModel from '../models/Drones.js';
import _Common from './common.js';

export default class _DroneController {
    static async findAll() {
        const drones = await DroneModel.find().exec();
        return drones;
    }

    static async findDroneExist(dic, auth_code) {
        const drones = await DroneModel.find({ dic, auth_code }).exec();
        return drones;
    }


    static async insertOne (dic, auth_code) {
        const newDrone = new DroneModel({
            dic,
            auth_code
        });

        await newDrone.save();
        return true;
    }
}