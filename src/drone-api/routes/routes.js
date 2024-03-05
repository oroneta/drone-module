import express from 'express';
/*
    Controller import
*/
import _Common from '../controllers/common.js';
import _Middleware from '../controllers/middleware.js';
import logger from '../controllers/logger.js';

const router = express.Router();
/*
    @example: /metadata/all/<code>
    @obligated: Add authorization header
*/
router.get('/', (req, res) => {
    res.json({});
});
router.get('/metadata/:opt/:dic', async (req, res) => {
    logger.info("ppp");
    if (_Middleware.checkMetadataType(req.params.opt)) {
        
    }

    return _Common.r400(res);
});

//The 404 Route (ALWAYS Keep this as the last route)
router.get('*', function(req, res){
    return _Common.r404(res);
});

export default router;