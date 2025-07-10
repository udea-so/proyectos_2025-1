from fastapi import APIRouter, HTTPException
from app.services import profiler
import logging

router = APIRouter(prefix="/profiling", tags=["Profiling"])

@router.get("/function")
def profile_function():
    try:
        return profiler.profile_sample_function()
    except Exception as e:
        logging.error(f"Error al perfilar función: {e}")
        raise HTTPException(status_code=500, detail="Error durante el perfilado de la función")
