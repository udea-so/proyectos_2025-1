import psutil
from prometheus_client import Gauge
import logging

memory_usage_percent = Gauge("memory_usage_percent", "Uso de memoria RAM en porcentaje")

def get_memory_usage():
    try:
        mem = psutil.virtual_memory()
        memory_usage_percent.set(mem.percent)
        return {
            "total": mem.total,
            "used": mem.used,
            "percent": mem.percent
        }
    except Exception as e:
        logging.error(f"Error al obtener uso de memoria: {e}")
        return {
            "total": 0,
            "used": 0,
            "percent": 0.0
        }
