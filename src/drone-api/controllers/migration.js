import { URL } from 'url';
import { createRequire } from 'module'

const require = createRequire(import.meta.url)
const __filename = new URL('', import.meta.url).pathname;
const __dirname = new URL('.', import.meta.url).pathname;

import fs from 'fs';
import path from 'path';
import logger from './logger.js';

import MigrationModel from '../models/Migration.js';

// Import all migrations
export default class _Migration {
    db;
    constructor (db) {
        this.db = db;

        const mPath = path.join(__dirname, '../migration');
        let migrationScript = fs.readdirSync(mPath);

        migrationScript.forEach(async scriptName => {
            if (['.', '..', 'index.js'].includes(scriptName))
                return;

            const migrationName = scriptName.split('-')[0];
            const m = require(`../migration/${scriptName}`);
            
            try {
                const migration = await MigrationModel.findOne({ migration: migrationName });
                
                if (!migration) {
                    logger.info(`Migration ${migrationName} not registered, executing and registering...`);
                    await m(db);

                    try {
                        // Crear una nueva instancia del modelo Migration
                        const newMigration = new MigrationModel({
                          migration: migrationName
                        });
                    
                        await newMigration.save();
                    } catch (error) {
                        logger.warn(`Migration ${migrationName} cannot be saved, please consider to add it manually: ${error}`);
                    }
                }
            } catch (error) {
                logger.warn(`Migration ${migrationName} cannot be searched and proccessed, continue to next migration: ${error}`);
            }
        });
    }
}