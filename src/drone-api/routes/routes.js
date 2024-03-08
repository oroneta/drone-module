import express from 'express';
/*
    Controller import
*/
import _Common from '../controllers/common.js';
import _Middleware from '../controllers/middleware.js';
import logger from '../controllers/logger.js';

import _DroneController from '../controllers/Drones.js';

/*
    @example: /metadata/all/<dic>
    @obligated: Add authorization header
*/
router.get('/metadata/:opt/:dic', async (req, res) => {
    if (_Middleware.checkMetadataType(req.params.opt) && req.headers.authorization.startsWith('Bearer ')) {
        let drone;
        try {
            // Remove Bearer Clause
            let auth = req.headers.authorization.split(' ')[1];
            logger.info(req.params.dic, auth)
            drone = await _DroneController.findDroneExist(req.params.dic, auth);
        } catch (error) {
            logger.error(error);
            return _Common.r500(res);
        }

        // If not exist
        if (drone.length == 0) {
            return _Common.r404(res);
        }

        // If exist and set all metadata
        if (req.params.opt == 'all') {
            return _Common.rJson(res, drone[0].metadata);
        }

        return _Common.rJson(res, drone[0].metadata[req.params.opt]);
    }

    return _Common.r400(res);
});

/*
    @example: /route/<dic>
    @obligated: Add authorization header
*/
router.get('/route/:dic', async (req, res) => {
    if (req.headers.authorization.startsWith('Bearer ')) {
        let drone;
        try {
            // Remove Bearer Clause
            let auth = req.headers.authorization.split(' ')[1];
            logger.info(req.params.dic, auth)
            drone = await _DroneController.findDroneExist(req.params.dic, auth);
        } catch (error) {
            logger.error(error);
            return _Common.r500(res);
        }

        // If not exist
        if (drone.length == 0) {
            return _Common.r404(res);
        }

        return _Common.rJson(res, drone[0].flight);
    }

    return _Common.r400(res);
});

//The 404 Route (ALWAYS Keep this as the last route)
router.get('*', function(req, res){
    return _Common.r404(res);
});

export default router;