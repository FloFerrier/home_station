# README
## Prerequisites
### Host Operating System
All the following commands are running on a Linux distribution:
- Ubuntu:22.04
### Python tools
```bash
$ sudo apt install make python3 python3-venv
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
[Website Link](https://grafana.com/grafana/download?pg=get&plcmt=selfmanaged-box1-cta1&edition=oss)
#### Install Grafana
1. Install the prerequisite packages:
```bash
$ sudo apt-get install -y adduser libfontconfig1 musl
```
2. Import the Grafana package:
```bash
$ wget https://dl.grafana.com/oss/release/grafana_11.5.1_amd64.deb
```
3. Install the Grafana package:
```bash
$ sudo dpkg -i grafana_11.5.1_amd64.deb
```
#### Start the Grafana server
1. Start the server with systemd:
```bash
$ sudo systemctl daemon-reload && sudo service grafana-server start
```
2. Verify that the server is running:
```bash
$ sudo service grafana-server status
```
3. Configure the Grafana server to start at boot:
```bash
$ sudo sservice grafana-server enable
```
4. Restart the Grafana server:
```bash
$ sudo service grafana-server restart
```
#### Run Grafana UI
Open a browser on the following link:
- http://localhost:3000/

## CLI commands
The project has a Makefile to run and clean application or test suite with a virtual environment,
refer you to the help command :
```bash
$ make help
clean                          Clean the workspace
run                            Run the application
test                           Run the test suite
```

## Production setup
Linux service with SystemD and virtual environment for python.
