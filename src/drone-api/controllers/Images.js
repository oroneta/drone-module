import ImageModel from '../models/Images.js';
import _Common from './common.js';

export default class _ImageController {
    static async findImageData(id) {
        const image = await ImageModel.find({ id }).exec();
        return image;
    }
}