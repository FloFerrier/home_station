import yaml

def config_read(filename: str = "") -> dict:
    config = dict()
    with open(filename) as file:
        config = yaml.safe_load(file)
    return config

if __name__ == "__main__":
    pass
