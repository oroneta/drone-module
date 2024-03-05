import mongoose from 'mongoose';
import logger from '../controllers/logger.js';

export default class DBConnect {
    uri; connection;
    constructor(uri, database) {
        this.uri = uri;
    };

    async connect(options) {
        try {
            logger.warn(`Connecting to MongoDB ${this.uri}`);
            await mongoose.connect(this.uri, options);
            this.connection = mongoose.connection;

            // Evento 'connected': conexión exitosa
            this.connection.on('connected', () => {
                logger.debug(`Connected to MongoDB ${this.uri}`);
            });

            // Evento 'error': error en la conexión
            this.connection.on('error', (error) => {
                logger.crit(`Status connection ${mongoose.connection.readyState}`);
                logger.crit(`Error connecting to ${this.uri}: ${error}`);
            });

            // Evento 'disconnected': conexión cerrada
            this.connection.on('disconnected', () => {
                logger.debug(`Disconnected from MongoDB ${this.uri}`);
            });

            return true;
        } catch (error) {
            logger.crit(`Error connecting to ${this.uri}: ${error}`);
            return false;
        }
    }
}