import mongoose, { Schema } from 'mongoose';


const alarmSchema = new mongoose.Schema({
    id: {
        type: Number,
        unique: true, // PRIMARY KEY
        required: true
    },
    description: {
        type: String,
        required: true,
        default: null
    },
    level: {
        type: Number,
        min: 0,
        max: 9,
        required: true,
        default: 0
    },
    action: {
        type: [
            {
                type: mongoose.Schema.Types.ObjectId,
                ref: 'Action',
            }
        ],
        required: true,
        default: []
    }
});

const AlarmModel = mongoose.model('Alarm', alarmSchema, 'alarms');

export default AlarmModel;
