import cProfile
import pstats
import io
import logging

def profile_sample_function():
    def some_heavy_function():
        total = 0
        for i in range(1000000):
            total += i * i
        return total

    try:
        profiler = cProfile.Profile()
        profiler.enable()
        some_heavy_function()
        profiler.disable()
    except Exception as e:
        logging.error(f"Error durante el perfilado de la función: {e}")
        return {"profile": "Error durante la ejecución del perfilado."}

    try:
        s = io.StringIO()
        ps = pstats.Stats(profiler, stream=s).sort_stats('cumulative')
        ps.print_stats(10)
        return {"profile": s.getvalue()}
    except Exception as e:
        logging.error(f"Error al procesar estadísticas del perfilador: {e}")
        return {"profile": "Error al procesar las estadísticas del perfilador."}
