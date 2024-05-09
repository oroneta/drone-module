import express from 'express';
/*
    Controller import
*/
import _Common from '../controllers/common.js';
import _Middleware from '../controllers/middleware.js';
import logger from '../controllers/logger.js';
import _ProviderComm from '../controllers/provider.js';

import _DroneController from '../controllers/Drones.js';
import _ImageController from '../controllers/Images.js';

const router = express.Router();
const provider = new _ProviderComm();

if (provider.testProvider()) {
    logger.info('Provider is online');
} else {
    logger.crit('Provider is offline');
}

// ========================================

/*
    @example: /metadata/all/<dic>
    @obligated: Add authorization header
*/
router.get('/metadata/:opt/:dic', _Middleware.checkAuth, async (req, res) => {
    if (_Middleware.checkMetadataType(req.params.opt) && req.headers.authorization.startsWith('Bearer ')) {
        let drone, result = {};
        try {
            // Remove Bearer Clause
            let authList = req.headers.authorization.split(' ')[1].split(';');
            let droneList = req.params.dic.split(';');

            // Check if the length of the two list is the same
            // Number of drones request must be equal to the number of auth codes
            if (droneList.length != authList.length) {
                return _Common.r400(res);
            }

            // Get the metadata of every drone
            for (let i = 0; i < droneList.length; i++) {
                drone = await _DroneController.findDroneExist(droneList[i], authList[i]);
                if (drone.length == 0) {
                    // If not exist, continue to the next drone
                    continue;
                }

                if (req.params.opt == 'all') {
                    result[droneList[i]] = drone[0].metadata;
                } else {
                    result[droneList[i]] = drone[0].metadata[req.params.opt];
                }
            }

            // If any drone exist, return 404
            if (Object.keys(result).length == 0) {
                return _Common.r404(res);
            }
        } catch (error) {
            logger.crit(error);
            return _Common.r500(res);
        }

        return _Common.rJson(res, result);
    }

    return _Common.r400(res);
});

/*
    @example: /status/<dic>
    @obligated: Add authorization header
*/
router.get('/status/:dic', _Middleware.checkAuth, async (req, res) => {
    if (req.headers.authorization.startsWith('Bearer ')) {
        let drone, result = {};
        try {
            // Remove Bearer Clause
            let authList = req.headers.authorization.split(' ')[1].split(';');
            let droneList = req.params.dic.split(';');
 
            // Check if the length of the two list is the same
            // Number of drones request must be equal to the number of auth codes
             if (droneList.length != authList.length) {
                return _Common.r400(res);
            }
 
            // Get the status of every drone
            for (let i = 0; i < droneList.length; i++) {
                drone = await _DroneController.findDroneExist(droneList[i], authList[i]);
                if (drone.length == 0) {
                    // If not exist, continue to the next drone
                    continue;
                }

                result[droneList[i]] = drone[0].status;
            }

            // If any drone exist, return 404
            if (Object.keys(result).length == 0) {
                return _Common.r404(res);
            }
        } catch (error) {
            logger.crit(error);
            return _Common.r500(res);
        }

        return _Common.rJson(res, result);
    }

    return _Common.r400(res);
});

// ========================================

/*
    @example: /route/<dic>
    @obligated: Add authorization header
*/
router.get('/routes/:dic', _Middleware.checkAuth, async (req, res) => {
    if (req.headers.authorization.startsWith('Bearer ')) {
        let drone, result = {};
        try {
            // Remove Bearer Clause
            let authList = req.headers.authorization.split(' ')[1].split(';');
            let droneList = req.params.dic.split(';');
 
            // Check if the length of the two list is the same
            // Number of drones request must be equal to the number of auth codes
             if (droneList.length != authList.length) {
                return _Common.r400(res);
            }
 
            // Get the status of every drone
            for (let i = 0; i < droneList.length; i++) {
                drone = await _DroneController.findDroneExist(droneList[i], authList[i]);
                if (drone.length == 0) {
                    // If not exist, continue to the next drone
                    continue;
                }

                result[droneList[i]] = drone[0].flight;
            }

            // If any drone exist, return 404
            if (Object.keys(result).length == 0) {
                return _Common.r404(res);
            }
        } catch (error) {
            logger.crit(error);
            return _Common.r500(res);
        }

        return _Common.rJson(res, result);
    }

    return _Common.r400(res);
});

/*
    @example: /route/<dic>
    @obligated: Add authorization header

    @body: Array of coordinates
*/
router.post('/routes/:dic', async (req, res) => {
    if (req.headers.authorization.startsWith('Bearer ')) {
        let route = req.body;
        try {
            let auth = req.headers.authorization.split(' ')[1];

            // Send the route to the provider
            let response = await provider.getProvider(req.params.dic, auth, route);

            if (response.status == 200) {
                let jsonStr = response.json()
                // Get the status of the route
                if (jsonStr.status == 1) {
                    // Insrt drone with all data, and if already exist update the data
                    let drone = await _DroneController.findDroneExist(req.params.dic, auth);
                    
                    route = JSON.stringify(route);
                    if (drone.length == 0) {
                        await _DroneController.insertOneWithFlight(req.params.dic, auth, route, 1, jsonStr.dangerous_level);
                    }
                    else {
                        await _DroneController.updateOneWithFlight(req.params.dic, auth, route, 1, jsonStr.dangerous_level);
                    }
                }

                return _Common.rJson(res, jsonStr);
            }

            if (response.status == 404) {
                return _Common.r404(res);
            }

            return _Common.r500(res);
        }
        catch (error) {
            logger.crit(error);
            return _Common.r500(res);
        }
    }

    return _Common.r400(res);
});

// ========================================

/*
    @example: /alarm/<dic>
    @obligated: Add authorization header
*/
router.get('/alarm/:dic', _Middleware.checkAuth, async (req, res) => {
    if (req.headers.authorization.startsWith('Bearer ')) {
        let drone, alarm, result = {};
        try {
            // Remove Bearer Clause
            let authList = req.headers.authorization.split(' ')[1].split(';');
            let droneList = req.params.dic.split(';');
 
            // Check if the length of the two list is the same
            // Number of drones request must be equal to the number of auth codes
             if (droneList.length != authList.length) {
                return _Common.r400(res);
            }
 
            // Get the status of every drone
            for (let i = 0; i < droneList.length; i++) {
                drone = await _DroneController.findDroneExist(droneList[i], authList[i]);

                if (drone.length == 0) {
                    // If not exist, continue to the next drone
                    continue;
                }

                if (drone[0].alarm_status) {
                    alarm = await _ImageController.findImageData(drone[0].alarm_data);
                    result[droneList[i]] = {
                        status: 1,
                        expire_date: alarm[0].expire_date,
                        image_path: `/alarm/image/${drone[0].alarm_data}`
                    };
                }
                else {
                    result[droneList[i]] = {
                        status: 0
                    };
                }
            }

            // If any drone exist, return 404
            if (Object.keys(result).length == 0) {
                return _Common.r404(res);
            }
        } catch (error) {
            logger.crit(error);
            return _Common.r500(res);
        }

        return _Common.rJson(res, result);
    }

    return _Common.r400(res);
});

/*
    @example: /alarm/<dic>
    @obligated: Add authorization header

    @brief: disable alarm with DELETE method
*/
router.delete('/alarm/:dic', _Middleware.checkAuth, async (req, res) => {
    if (req.headers.authorization.startsWith('Bearer ')) {
        let drone;
        try {
            // Remove Bearer Clause
            let auth = req.headers.authorization.split(' ')[1];
            let droneList = req.params.dic.split(';');

            // Get the status of every drone
            for (let i = 0; i < droneList.length; i++) {
                drone = await _DroneController.findDroneExist(droneList[i], auth);

                if (drone.length == 0) {
                    // If not exist, continue to the next drone
                    continue;
                }

                if (drone[0].alarm_status) {
                    await _DroneController.updateAlarmStatus(droneList[i], auth, 0);
                }
            }

            // If any drone exist, return 404
            if (Object.keys(drone).length == 0) {
                return _Common.r404(res);
            }
        } catch (error) {
            logger.crit(error);
            return _Common.r500(res);
        }

        return _Common.rJson(res, {});
    }

    return _Common.r400(res);
});


/*
    @example: /alarm/image/<image>
*/
router.get('/alarm/image/:image', async (req, res) => {
    // Return image from the database
    try {
        let image = await _ImageController.findImageData(req.params.image);
        if (image.length == 0) {
            return _Common.r404(res);
        }

        res.writeHead(200, {
            'Content-Type': 'image/jpeg',
            'Content-Length': image[0].data.length
        });
        res.end(image[0].data);
    } catch (error) {
        logger.crit(error);
        return _Common.r500(res);
    }
});

// ========================================

//The 404 Route (ALWAYS Keep this as the last route)
router.get('*', function(req, res){
    return _Common.r404(res);
});

export default router;