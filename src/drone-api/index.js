/*
    Internal import layer
*/
import _server_config from './config/server.js';
import _ddbb_config from './config/ddbb.js';

/*
    Custom import modules
*/
import express from 'express';
// Cors
import cors from 'cors';
import routes from './routes/routes.js';

/*
    Controller modules
*/
import logger from './controllers/logger.js';
import _Common from './controllers/common.js';
import _Migration from './controllers/migration.js';

/*
    Model Database connection and migration handler
*/
import DBConnect from './models/model.js';

// Init
logger.warn("Lauching server");

const DatabaseConn = new DBConnect(_ddbb_config.url, _ddbb_config.database);

try {
    // Connect to database
    if (!(await DatabaseConn.connect(_ddbb_config.options))) {
        _Common.onErrorExit('Error when connecting to MongoDB Server');
    } else {
        logger.info('MongoDB connected! Hi!');
    }

    // Launch Migration Routine
    const MigrationRoutine = new _Migration(DatabaseConn.connection.db);
} catch (error) {
    logger.crit(`Error on try/catch ${error}`);
    _Common.onErrorExit('Error when trying connecting to MongoDB Server');
}

/*
    Create the server Object
*/
const app = express();

/*
    Use external objects and middlewares

    * JSON
    * ROUTES
    * CHECKAUTH
*/
app.use(express.json());
app.use(cors());
app.use(routes);


/*
    Start the server
*/
app.listen(_server_config.port, '0.0.0.0', () => {
    var info = _server_config.log.init
                                    .replace("$port", _server_config.port);
    // Server activated
    logger.info(info);
});


process.on('SIGINT', () => {
    mongoose.connection.close(() => {
        process.exit(0);
    });
});