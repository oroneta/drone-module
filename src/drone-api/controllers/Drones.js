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

    static async insertOneWithFlight (dic, auth_code, route, priority, altitude) {
        const newDrone = new DroneModel({
            dic,
            auth_code,
            flight: route,
            flight_priority: priority,
            flight_altitude: altitude
        });

        await newDrone.save();
        return true;
    }

    static async updateOneWithFlight (dic, auth_code, route, priority, altitude) {
        await DroneModel.updateOne({
            dic,
            auth_code
        }, {
            flight: route,
            flight_priority: priority,
            flight_altitude: altitude
        });
        return true;
    }

    static async updateAlarmStatus (dic, auth_code, alarm_status) {
        await DroneModel.updateOne({
            dic,
            auth_code
        }, {
            alarm_status
        });
        return true;
    }


    
}