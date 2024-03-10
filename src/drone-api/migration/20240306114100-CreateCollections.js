import AlarmModel from "../models/Alarms.js";
import ActionModel from "../models/Actions.js";
import DroneModel from "../models/Drones.js";
import ImageModel from "../models/Images.js";
import Ai_entityModel from "../models/Ai_entities.js";

export default async (db) => {
    AlarmModel.createCollection();
    ActionModel.createCollection();
    DroneModel.createCollection();
    ImageModel.createCollection();
    Ai_entityModel.createCollection();
}