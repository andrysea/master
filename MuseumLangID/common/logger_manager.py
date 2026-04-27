import logging

class LoggerManager:
    def __init__(self, name="AppLogger", log_file="audit.log"):
        # Creo il logger
        self.logger = logging.getLogger(name)
        self.logger.setLevel(logging.INFO)

        # Formato del log (Data - Nome - Livello - Messaggio)
        formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

        # Handler per scrivere su File
        file_handler = logging.FileHandler(log_file)
        file_handler.setFormatter(formatter)

        # Handler per vedere i log anche in console
        console_handler = logging.StreamHandler()
        console_handler.setFormatter(formatter)

        # Aggiungiamo gli handler al logger
        self.logger.addHandler(file_handler)
        self.logger.addHandler(console_handler)

    # Metodo per  invio di messaggio con livello INFO al logger configurato
    def log_info(self, message):
        self.logger.info(message)

    # Metodo per  invio di messaggio con livello ERROR al logger configurato
    def log_error(self, message):
        self.logger.error(message)
