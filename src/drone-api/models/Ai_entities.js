import mongoose from 'mongoose';


const ai_entitySchema = new mongoose.Schema({
    id: {
        type: String,
        unique: true, // PRIMARY KEY
        required: true,
    },
    server_ip: {
        type: String,
        required: true,
    },
    server_port: {
        type: Number,
        required: true,
    },
    capacity: {
        type: Number,
        required: true,
        default: 0
    },
    max_capacity: {
        type: Number,
        required: true,
    }
});

const Ai_entityModel = mongoose.model('Ai_entity', ai_entitySchema, 'ai_entities');

export default Ai_entityModel;
