import ActionModel from "../models/Actions.js";
import DroneModel from "../models/Drones.js";
import Ai_entityModel from "../models/Ai_entities.js";

export default async (db) => {
    // Basic data for Action id = 0
    const newAction = new ActionModel({
        id: 0,
        name: 'Null Action'
    });
    await newAction.save();


    // Basic data for Drone dic = 0
    const newDrone = new DroneModel({
        dic: '0',
        auth_code: '0'
    });
    await newDrone.save();

    // Basic data for Drone dic = 1
    const newDrone1 = new DroneModel({
        dic: '1',
        auth_code: '1'
    });
    await newDrone1.save();


    // Add Ai_entities
    const newAi_entity = new Ai_entityModel({
        id: "DefaultIP",
        server_ip: '127.0.0.1',
        server_port: 60003,
        capacity: 0,
        max_capacity: 1000
    });
    await newAi_entity.save();
}
