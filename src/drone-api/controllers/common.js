/* ====================================
    
    Utils functions to manage server

 =================================== */
import logger from "./logger.js";

class _Common {
    /* ============================

        Static methods for returning HTTP Codes:
        https://en.wikipedia.org/wiki/List_of_HTTP_status_codes

    =============================== */
    static r400(res) {
        return res.status(400).json(
            {
                status: "error",
                message: "401 Bad Request",
                errcode: 400
            }
        );
    }
    static r401(res) {
        return res.status(401).json(
            {
                status: "error",
                message: "401 Unauthorized",
                errcode: 401
            }
        );
    }
    static r404(res) {
        return res.status(404).json(
            {
                status: "error",
                message: "404 Not Found",
                errcode: 404
            }
        );
    }
    static r500(res) {
        return res.status(500).json(
            {
                status: "error",
                message: "500 Internal Server Error",
                errcode: 500
            }
        );
    }


    static rJson(res, obj) {
        if (obj == null || !obj)
            obj = {}

        return res.status(200).json(
            {
                status: "OK",
                message: "200 OK",
                data: obj
            }
        );
    }
    static onErrorExit(string) {
        logger.fatal(string);
        process.exit(-1);
    }
}

export default _Common;