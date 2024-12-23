VENV = virtualenv
PYTHON = $(VENV)/bin/python
PIP = $(VENV)/bin/pip

.PHONY: help
help:
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

$(VENV)/bin/activate: requirements.txt
	python3 -m venv $(VENV)
	$(PIP) install -r requirements.txt

.PHONY: run
run: $(VENV)/bin/activate ## Run the application
	$(PYTHON) script.py

.PHONY: test
test: $(VENV)/bin/activate ## Run the test suite
	$(PYTHON) -m pytest -s -v tests

.PHONY: clean
clean: ## Clean the workspace
	rm -rf $(VENV) */__pycache__ */*/__pycache__ .pytest_cache */.pytest_cache
