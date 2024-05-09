import mongoose from 'mongoose';


const droneSchema = new mongoose.Schema({
    dic: {
        type: String,
        unique: true, // PRIMARY KEY
        required: true
    },
    auth_code: {
        type: String,
        required: true
    },
    metadata: {
        type: Object,
        required: true,
        default: {}
    },
    status: {
        type: Boolean,
        required: true,
        default: false
    },
    flight: {
        type: Object,
        required: true,
        default: []
    },
    flight_priority: {
        type: Number,
        required: true,
        default: 0
    },
    flight_altitude: {
        type: Number,
        required: true,
        default: 0
    },
    alarm_status: {
        type: Boolean,
        required: true,
        default: false
    },
    alarm_code: {
        type: Number,
        required: true,
        ref: 'Alarm',
        default: 0
    },
    alarm_data: {
        type: String,
        required: false
    },
    default_action: {
        type: Number,
        ref: 'Action',
        required: true,
        default: 0
    },
    registered_at: {
        type: Date,
        required: true,
        default: Date.now // DATENOW()
    }
});

const DroneModel = mongoose.model('Drone', droneSchema, 'drones');

export default DroneModel;
