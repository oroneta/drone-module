/* ====================================
    
    Middleware functions to call

 =================================== */
import _Common from './common.js';


class _Middleware {
    static metadataType = ['all', 'battery'];
    /*
        @bried: Check if the request contains Authorization header
        @param: req - request object
        @param: res - response object
        @param: next - Continue function callbback

        @return: Void || status[_Common.r401]
        @see: _Common.r401(res)
    */
    static checkAuth(req, res, next) {
        if (!req.headers.authorization) {
            return _Common.r401(res);
        }
        next();
    }

    static checkMetadataType(el) {
        if (_Middleware.metadataType.includes(el))
            return true;

        return false;
    }
}

export default _Middleware;