from fastapi.testclient import TestClient
from app.main import app

client = TestClient(app)

# ---------- /metrics ----------
def test_metrics_cpu():
    response = client.get("/metrics/cpu")
    assert response.status_code == 200
    data = response.json()
    assert "cpu_percent" in data
    assert isinstance(data["cpu_percent"], (float, int))

def test_metrics_memory():
    response = client.get("/metrics/memory")
    assert response.status_code == 200
    data = response.json()
    assert "percent" in data
    assert isinstance(data["percent"], (float, int))

def test_metrics_disk():
    response = client.get("/metrics/disk")
    assert response.status_code == 200
    data = response.json()
    assert "percent" in data
    assert isinstance(data["percent"], (float, int))

# ---------- /metrics/system ----------
def test_system_metrics():
    response = client.get("/metrics/system/")
    assert response.status_code == 200
    data = response.json()
    assert "uptime" in data
    assert "threads" in data
    assert "processes" in data
    assert "process_count" in data["uptime"]
    assert "uptime_seconds" in data["uptime"]

def test_system_network():
    response = client.get("/metrics/system/network")
    assert response.status_code == 200
    data = response.json()
    assert "bytes_sent" in data
    assert "bytes_received" in data
    assert isinstance(data["bytes_sent"], int)
    assert isinstance(data["bytes_received"], int)

# ---------- /profiling ----------
def test_profiling_function():
    response = client.get("/profiling/function")
    assert response.status_code == 200
    data = response.json()
    assert "profile" in data
    assert isinstance(data["profile"], str)
    assert "function calls" in data["profile"]