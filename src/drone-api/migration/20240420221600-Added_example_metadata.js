import DroneModel from "../models/Drones.js";

export default async (db) => {
    // Basic data for Drone dic = 0
    const query = { dic: '0' };
    const update = { 
        $set: {
            metadata: {
                gps: [39.455109, -0.318429],
                altitude: 10,
                speed: 0,
                battery: 100,
                status: 1,
                mode: 0,
                mission: 0,
                last_update: new Date().getTime()
            },
            status: true,
            flight: Buffer.from([]),
            flight_priority: 0,
            flight_altitude: 0,
            alarm_status: false,
            alarm_code: 0,
            alarm_data: '',
            default_action: 0,
        }
    };

    await DroneModel.findOneAndUpdate(query, update);
}