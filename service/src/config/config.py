import yaml

def config_read(filename: str = "") -> dict:
    config = dict()
    try:
        file = open(filename)
    except Exception as e:
        print(e)
    else:
        config = yaml.safe_load(file)
        file.close()
    return config

if __name__ == "__main__":
    pass
