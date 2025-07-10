import os

class Settings:
    PROMETHEUS_PORT = int(os.getenv("PROMETHEUS_PORT", 8001))

settings = Settings()