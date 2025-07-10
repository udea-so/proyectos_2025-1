import smtplib
from email.message import EmailMessage
import logging


EMAIL_ADDRESS = "chapou2025@gmail.com"
EMAIL_PASSWORD = "tdihgtrtpxwlaloh"
EMAIL_RECEIVER = "jesteban.aristizabal@udea.edu.co"

def enviar_alerta(asunto, cuerpo):
    print(f"Enviando correo de alerta: {EMAIL_ADDRESS} -> {EMAIL_RECEIVER} password: {EMAIL_PASSWORD}")
    try:
        msg = EmailMessage()
        msg["Subject"] = asunto
        msg["From"] = EMAIL_ADDRESS
        msg["To"] = EMAIL_RECEIVER
        msg.set_content(cuerpo)

        with smtplib.SMTP_SSL("smtp.gmail.com", 465) as smtp:
            smtp.login(EMAIL_ADDRESS, EMAIL_PASSWORD)
            smtp.send_message(msg)
            logging.info(f"Correo de alerta enviado: {asunto}")
    except Exception as e:
        logging.error(f"Error al enviar correo: {e}")
