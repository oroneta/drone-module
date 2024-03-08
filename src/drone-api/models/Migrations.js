import mongoose from 'mongoose';

const migrationSchema = new mongoose.Schema({
  migration: {
    type: String,
    required: true,
    unique: true // Key
  },
  applied_at: {
    type: Date,
    required: true,
    default: Date.now // DATENOW()
  }
});

const MigrationModel = mongoose.model('Migration', migrationSchema, 'migrations');

export default MigrationModel;
