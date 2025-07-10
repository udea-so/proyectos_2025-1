from fastapi import APIRouter, HTTPException
from app.services import system_monitor
import logging

router = APIRouter(prefix="/metrics/system", tags=["System"])

@router.get("/")
def get_system_metrics():
    try:
        uptime = system_monitor.get_system_info()
        threads = system_monitor.get_thread_count()
        processes = uptime["process_count"]  # Reutiliza lo ya consultado
        return {
            "uptime": uptime,
            "threads": threads,
            "processes": processes
        }
    except Exception as e:
        logging.error(f"Error en GET /metrics/system/: {e}")
        raise HTTPException(status_code=500, detail="No se pudo obtener métricas del sistema")

@router.get("/network")
def get_network_metrics():
    try:
        return system_monitor.get_network_info()
    except Exception as e:
        logging.error(f"Error en GET /metrics/system/network: {e}")
        raise HTTPException(status_code=500, detail="No se pudo obtener métricas de red")
