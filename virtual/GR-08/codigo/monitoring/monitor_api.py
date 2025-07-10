from fastapi import FastAPI
from pydantic import BaseModel
import psutil

app = FastAPI(title="Monitor de Procesos del SO")

class ProcesoInfo(BaseModel):
    pid: int
    nombre: str
    cpu: float
    memoria: float

def obtener_info_proceso(proc):
    try:
        with proc.oneshot():
            pid = proc.pid
            nombre = proc.name()
            cpu = proc.cpu_percent(interval=0.1)
            memoria = proc.memory_percent()
        return ProcesoInfo(pid=pid, nombre=nombre, cpu=cpu, memoria=memoria)
    except (psutil.NoSuchProcess, psutil.AccessDenied):
        return None

@app.get("/procesos", response_model=list[ProcesoInfo])
def listar_procesos():
    procesos = []
    for proc in psutil.process_iter():
        info = obtener_info_proceso(proc)
        if info:
            procesos.append(info)
    return procesos

@app.get("/proceso/max-cpu", response_model=ProcesoInfo)
def proceso_max_cpu():
    procesos = []
    for proc in psutil.process_iter():
        info = obtener_info_proceso(proc)
        if info:
            procesos.append(info)
    max_cpu_proc = max(procesos, key=lambda p: p.cpu, default=None)
    return max_cpu_proc

@app.get("/proceso/max-memoria", response_model=ProcesoInfo)
def proceso_max_memoria():
    procesos = []
    for proc in psutil.process_iter():
        info = obtener_info_proceso(proc)
        if info:
            procesos.append(info)
    max_mem_proc = max(procesos, key=lambda p: p.memoria, default=None)
    return max_mem_proc
