/* ====================================
    
    Middleware functions to call

 =================================== */

import { createLogger, format, transports } from 'winston';
import 'winston-daily-rotate-file';
import moment from 'moment';

/*
    Import configuration
*/
import _server_config from '../config/server.js';


// set default log level.
const logLevel = 'info'

var logger = createLogger({
    level: logLevel,
    levels: {
        fatal: 0,
        crit: 1,
        warn: 2,
        info: 3,
        debug: 4,
        trace: 5
    },
    format: format.combine(
        format.prettyPrint(),
        format.timestamp({
            format: 'DD-MM-YYYY hh:mm:ss A'
        }),
        format.printf(nfo => {
            return `${nfo.timestamp} - ${nfo.level}: ${nfo.message}`
        })
    ),
    transports: [
        new transports.Console(),
        new transports.File({
            filename: _server_config.log.path + "/" + _server_config.log.logfile
        }),
        new transports.DailyRotateFile ({
            filename: _server_config.log.logfile.split('.')[0] + ".%DATE%.log",
            dirname: _server_config.log.path + "/backup",
            datePattern: _server_config.log.dateformat,
            colorize: true,
            zippedArchive: true,
            maxFiles: _server_config.log.maxfile
        }),
    ]
})

// Extend logger object to properly log 'Error' types
var origLog = logger.log

logger.log = function (level, msg) {
    if (msg instanceof Error) {
        var args = Array.prototype.slice.call(arguments)
        args[1] = msg.stack
        origLog.apply(logger, args)
    } else {
        origLog.apply(logger, arguments)
    }
}

/* LOGGER EXAMPLES
  var log = require('./log.js')
  log.trace('testing')
  log.debug('testing')
  log.info('testing')
  log.warn('testing')
  log.crit('testing')
  log.fatal('testing')
 */

export default logger