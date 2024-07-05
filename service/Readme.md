# README
## Prerequisites
### Host Operating System
All the following commands are running on a Linux distribution:
- Ubuntu:22.04
### Python virtual environment
1. Intall the python3-venv package:
```bash
$ sudo apt install python3.10-venv
``` 
2. Create a Python virtual environment for the project:
```bash
$ python3 -m venv virtualenv
```
3. Activate this:
```bash
$ source virtualenv/bin/activate
```
### InfluxDB v2 OSS
[Website Link](https://docs.influxdata.com/influxdb/v2/install/?t=Linux)
#### Install InfluxBD
1. Download the package:
```bash
$ wget -q https://repos.influxdata.com/influxdata-archive_compat.key
$ echo '393e8779c89ac8d958f81f942f9ad7fb82a25e133faddaf92e15b16e6ac9ce4c influxdata-archive_compat.key' | sha256sum -c && cat influxdata-archive_compat.key | gpg --dearmor | sudo tee /etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg > /dev/null
$ echo 'deb [signed-by=/etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg] https://repos.influxdata.com/debian stable main' | sudo tee /etc/apt/sources.list.d/influxdata.list
```
2. Install the package:
```bash
$ sudo apt-get update && sudo apt-get install influxdb2
```
#### Start the service
2. Start the InfluxDB service:
```bash
$ sudo service influxdb start
```
3. Verify if the service is running correctly:
```bash
$ sudo service influxdb status
```
#### Run InfluxDB UI
Open a browser on the following link:
- http://localhost:8086/

### Grafana OSS
[Website Link](https://grafana.com/docs/grafana/latest/setup-grafana/installation/debian/)
#### Install Grafana
1. Install the prerequisite packages:
```bash
$ sudo apt-get install -y apt-transport-https software-properties-common wget
```
2. Import the GPG key:
```bash
$ sudo mkdir -p /etc/apt/keyrings/
$ wget -q -O - https://apt.grafana.com/gpg.key | gpg --dearmor | sudo tee /etc/apt/keyrings/grafana.gpg > /dev/null
```
3. Add a repository for stable releases:
```bash
$ echo "deb [signed-by=/etc/apt/keyrings/grafana.gpg] https://apt.grafana.com stable main" | sudo tee -a /etc/apt/sources.list.d/grafana.list
```
4. Update the list of available packages and install grafana OSS:
```bash
$ sudo apt-get update && sudo apt-get install grafana
```
#### Start the Grafana server
1. Start the server with systemd:
```bash
$ sudo systemctl daemon-reload && sudo systemctl start grafana-server
```
2. Verify that the server is running:
```bash
$ sudo systemctl status grafana-server
```
3. Configure the Grafana server to start at boot:
```bash
$ sudo systemctl enable grafana-server.service
```
4. Restart the Grafana server:
```bash
$ sudo systemctl restart grafana-server
```
#### Run Grafana UI
Open a browser on the following link:
- http://localhost:3000/
## Python dependencies
Install dependencies with this command:
```bash
(virtualenv)$ pip install -r requirements.txt
```
