import AlarmModel from "../models/Alarms.js";
import ActionModel from "../models/Actions.js";
import DroneModel from "../models/Drones.js";

export default async (db) => {
    AlarmModel.createCollection();
    ActionModel.createCollection();
    DroneModel.createCollection();
}