import mongoose from 'mongoose';


const actionSchema = new mongoose.Schema({
    id: {
        type: String,
        unique: true, // PRIMARY KEY
        required: true
    },
    name: {
        type: String,
        required: true
    }
});

const ActionModel = mongoose.model('Action', actionSchema, 'actions');

export default ActionModel;
