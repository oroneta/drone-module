FROM node:20.11.1

# Set the working directory in the container
WORKDIR /opt/drone-api

# Copy the rest of the application code to the working directory
COPY ./src/drone-api .

# Install dependencies
RUN npm install

# Command to run the application
CMD ["node", "index.js"]

LABEL Name=oroneta.drone-module.drone-api