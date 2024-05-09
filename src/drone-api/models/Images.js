import mongoose from 'mongoose';


const imageSchema = new mongoose.Schema({
    id: {
        type: String,
        unique: true, // PRIMARY KEY
        required: true,
        ref: 'Action'
    },
    data: {
        type: Buffer,
        required: true
    },
    expire_date: {
        type: Date,
        required: true,
        default: Date.now // DATENOW()
    }
});

const ImageModel = mongoose.model('Image', imageSchema, 'images');

export default ImageModel;
