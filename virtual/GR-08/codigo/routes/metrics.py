from fastapi import APIRouter, HTTPException
from app.services import cpu_monitor, memory_monitor, io_monitor
import logging

router = APIRouter(prefix="/metrics", tags=["Metrics"])

@router.get("/cpu")
def get_cpu():
    try:
        return {"cpu_percent": cpu_monitor.get_cpu_percent()}
    except Exception as e:
        logging.error(f"Error al obtener uso de CPU: {e}")
        raise HTTPException(status_code=500, detail="No se pudo obtener el uso de CPU")

@router.get("/memory")
def get_memory():
    try:
        return memory_monitor.get_memory_usage()
    except Exception as e:
        logging.error(f"Error al obtener uso de memoria: {e}")
        raise HTTPException(status_code=500, detail="No se pudo obtener el uso de memoria")

@router.get("/disk")
def get_disk():
    try:
        return io_monitor.get_disk_usage()
    except Exception as e:
        logging.error(f"Error al obtener uso de disco: {e}")
        raise HTTPException(status_code=500, detail="No se pudo obtener el uso de disco")
