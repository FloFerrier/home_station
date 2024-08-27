# README
## Project
tbd
## Acceptance Test
### Prerequisites
1. Get your python version on your distro:
```bash
$ python3 --version
Python 3.XX.YY
```
2. Install the python virtual package:
```bash
$ sudo apt install python3.XX-venv
```
3. Create a Python virtual environment:
```bash
$ python3 -m venv virtualenv
```
### Use virtual environment to development
1. Activate this:
```bash
$ source virtualenv/bin/activate
```
2. Install dependencies:
```bash
(virtualenv) pip install -r requirements.txt
```
3. And when you have finished to deactivate:
```bash
(virtualenv) deactivate
```
### Run test suite
Start emulator(or plug the board with firmware)
```bash
$ renode --disable-gui config/nucleo-f446re.resc &>/dev/null &
```
Launch python script:
```bash
(virtualenv) robot -d output TestShell.robot
```
