const _ddbb_config = {
    database: process.env.MONGO_DBNAME || "drone-module.db",
    url: `mongodb://${process.env.MONGO_DBSEVR ?? '127.0.0.1'}:${process.env.MONGO_DBPORT ?? '27017'}/`,
    options: {
        auth: { 
            authSource: "admin" 
        },
        user: process.env.MONGO_DBUSER ?? "Oroneta_Admin",
        pass: process.env.MONGO_DBPASS ?? "Oroneta_Password",
    }
}

export default _ddbb_config;