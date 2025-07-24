import psutil
from prometheus_client import Gauge
import logging

# Crear la métrica
cpu_usage_percent = Gauge("cpu_usage_percent", "Uso de CPU en porcentaje")

def get_cpu_percent():
    try:
        value = psutil.cpu_percent(interval=1)
        cpu_usage_percent.set(value)  # Actualiza métrica para Prometheus
        return value
    except Exception as e:
        logging.error(f"Error al obtener uso de CPU: {e}")
        return 0.0  # Retorna un valor por defecto si falla
