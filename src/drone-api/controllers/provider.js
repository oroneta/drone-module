// Import all migrations
export default class _ProviderComm {
    #provider_host;
    #provider_port;
    #provider_protocol;
    constructor () {
        this.#provider_protocol = process.env.CORESYSTEM_PROTOCOL;
        this.#provider_host = process.env.CORESYSTEM_SEVR;
        this.#provider_port = process.env.CORESYSTEM_PORT;
    }

    getProviderHost() {
        return this.#provider_host;
    }
    getProviderPort() {
        return this.#provider_port;
    }
    getProviderProtocol() {
        return this.#provider_protocol;
    }

    
    // ------------------------
    /*
        @param {Array} array
        @return Json

        @description: Create POST request to provider with array data
                      header includes auth code in Authorization
                      get params includes dic

    */
    async getProvider(dic, code, array) {
        try {
            const response = await fetch(`${this.#provider_protocol}://${this.#provider_host}:${this.#provider_port}/route/${dic}`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${code}`
                },
                body: JSON.stringify(array)
            });
            return response;
        } catch (error) {
            return {
                status: 0,
                message: error
            };
        }
    }


    async testProvider() {
        try {
            const response = await fetch(`${this.#provider_protocol}://${this.#provider_host}:${this.#provider_port}`);
            return response;
        } catch (error) {
            return error;
        }
    }
}