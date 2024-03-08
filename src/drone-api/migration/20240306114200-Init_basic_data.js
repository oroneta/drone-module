import ActionModel from "../models/Actions.js";
import DroneModel from "../models/Drones.js";

export default async (db) => {
    // Basic data for Action id = 0
    const newAction = new ActionModel({
        id: 0,
        name: 'Null Action'
    });

    await newAction.save();

    const newDrone = new DroneModel({
        dic: '0',
        auth_code: '0'
    });

    await newDrone.save();
}