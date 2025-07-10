import psutil
from prometheus_client import Gauge
import logging

disk_usage_percent = Gauge("disk_usage_percent", "Uso de disco en porcentaje")

def get_disk_usage():
    try:
        disk = psutil.disk_usage('/')
        disk_usage_percent.set(disk.percent)
        return {
            "total": disk.total,
            "used": disk.used,
            "percent": disk.percent
        }
    except Exception as e:
        logging.error(f"Error al obtener uso de disco: {e}")
        return {
            "total": 0,
            "used": 0,
            "percent": 0.0
        }
