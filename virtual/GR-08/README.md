# 🚀 Proyecto final SO:  Performance API - Monitoreo de Recursos con FastAPI y Prometheus

|Integrante|correo|usuario github|
|---|---|---|
|Jonathan Mazo González|jonathan.mazog@udea.edu.co|Jonathanmg1415|
|José David Henao Gallego|jose.henao1@udea.edu.co|JoseHenao16|
|Juan Esteban Aristizabal Aristizabal|jesteban.aristizabal@udea.edu.co|Jesteban97|
|Sharid Samantha Madrid Ospina|sharid.madrid@udea.edu.co|SamanthaMadrid19|

## Video
https://youtu.be/czDRhuk4LYw

## Instaladores
https://drive.google.com/drive/folders/14CrkRh-kaXsW-5RJXbu6AM-cJ3KBorgO?usp=sharing
https://www.python.org/downloads/

## Descripción
Este proyecto propone el diseño e implementación de una API de monitoreo de rendimiento orientada a aplicaciones en tiempo real. Esta construida con FastAPI que expone métricas de sistema (CPU, memoria, disco, red y procesos) utilizando `psutil` y `prometheus_client`, con sistema de alertas por correo electrónico cuando se superan ciertos umbrales.

## 📌 Propósito del Proyecto

Este proyecto tiene como objetivo:
- Monitorear recursos del sistema en tiempo real (CPU, RAM, Disco, etc.)
- Exponer métricas en formato compatible con Prometheus para visualización en Grafana
- Enviar alertas por correo si los valores superan umbrales configurables

## 💻 Requisitos

### 🧰 Software
- Python 3.8 o superior
- pip
- FastAPI
- Uvicorn
- psutil
- prometheus_client
- python-dotenv
- pytest (para tests)

### 🖥️ Hardware (mínimo recomendado)
- CPU de 2 núcleos
- 2 GB de RAM
- Acceso a red (para enviar correos y exponer métricas)
---

# ⚙️ Instalación

## 1. Clona el repositorio
### Instalar dependencias
pip install -r requirements.txt
### (O manualmente)
pip install fastapi uvicorn prometheus-client psutil

### Iniciar servidor FastAPI con exportador Prometheus
uvicorn app.main:app --reload

### Ejecutar Prometheus (desde su carpeta)
prometheus.exe --config.file=prometheus.yml

### Ejecutar Grafana (desde su carpeta)
cd "C:\Program Files\GrafanaLabs\grafana"
bin\grafana-server.exe

### Ejecutar pruebas unitarias 
pytest en la raiz del proyecto

# Nota:
El sistema envía alertas por correo electrónico cuando:

CPU > 80%
Memoria > 80%
Disco > 80%

Se evita el envío repetitivo usando estado interno.


