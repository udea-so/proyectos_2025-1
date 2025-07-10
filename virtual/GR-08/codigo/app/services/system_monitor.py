import psutil
import time
import logging
from prometheus_client import Gauge, Counter

# Definición de métricas
process_count = Gauge("system_process_count", "Número de procesos activos")
thread_count = Gauge("system_thread_count", "Número de hilos activos")
uptime_seconds = Gauge("system_uptime_seconds", "Tiempo desde el último arranque (segundos)")
net_bytes_sent = Counter("network_bytes_sent_total", "Bytes enviados desde el arranque")
net_bytes_recv = Counter("network_bytes_received_total", "Bytes recibidos desde el arranque")

def update_system_metrics():
    try:
        # Procesos
        pids = psutil.pids()
        process_count.set(len(pids))
    except Exception as e:
        logging.error(f"Error al contar procesos: {e}")

    try:
        # Hilos
        total_threads = 0
        for pid in psutil.pids():
            try:
                p = psutil.Process(pid)
                total_threads += p.num_threads()
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                continue
        thread_count.set(total_threads)
    except Exception as e:
        logging.error(f"Error al contar hilos: {e}")

    try:
        # Uptime
        uptime = psutil.boot_time()
        current = time.time()
        uptime_seconds.set(current - uptime)
    except Exception as e:
        logging.error(f"Error al calcular uptime: {e}")

    try:
        # Red
        net_io = psutil.net_io_counters()
        net_bytes_sent.inc(net_io.bytes_sent)
        net_bytes_recv.inc(net_io.bytes_recv)
    except Exception as e:
        logging.error(f"Error al obtener estadísticas de red: {e}")

def get_system_info():
    try:
        return {
            "process_count": len(psutil.pids()),
            "uptime_seconds": time.time() - psutil.boot_time()
        }
    except Exception as e:
        logging.error(f"Error en get_system_info: {e}")
        return {
            "process_count": 0,
            "uptime_seconds": 0.0
        }

def get_network_info():
    try:
        io = psutil.net_io_counters()
        return {
            "bytes_sent": io.bytes_sent,
            "bytes_received": io.bytes_recv
        }
    except Exception as e:
        logging.error(f"Error en get_network_info: {e}")
        return {
            "bytes_sent": 0,
            "bytes_received": 0
        }

def get_thread_count():
    try:
        count = 0
        for pid in psutil.pids():
            try:
                count += psutil.Process(pid).num_threads()
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                continue
        return count
    except Exception as e:
        logging.error(f"Error en get_thread_count: {e}")
        return 0
