const _server_config = {
    name: "oroneta.drone-module.drone-api",
    port: process.env.API_PORT || 60001,
    log: {
        path: "./logs",
        logfile: "server.log",
        init: "Server on port: $port",
        maxfile: "14d",
        dateformat: "YYYYMMDD"
    },
    os: "unix",
}

export default _server_config;