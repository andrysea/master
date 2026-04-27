import pickle
import numpy as np

from entity.language_response import LanguageResponse
from exceptions.unidentified_language import UnidentifiedLanguage
from common.logger_manager import LoggerManager

# Dichiarazione e inizializzazione del logger
audit_logger = LoggerManager(name="Model", log_file="registro_audit.log")

# Definizione classe Model, per eseguire le previsioni
class Model:

    # Metodo costruttore
    def __init__(self):
        self.modello = None

    # Metodo per il caricamento del modello tramite stringa filename
    # esempio: filename = 'language_detection_pipeline.pkl'
    def load_model(self, filename: str):
        audit_logger.log_info("Inizio esecuzione metodo load_model.")
        self.modello= pickle.load(open(filename, 'rb'))
        audit_logger.log_info("Caricamento modello avvenuto con successo.")
        audit_logger.log_info("Fine esecuzione metodo load_model.")

    # Metodo per la predizione della lingua di un testo (JSON), tramite l'utilizzo del modello
    # esempio input:
    # {
    #     "text": "Questo è un esempio di testo."
    # }
    # esempio output:
    # {
    #     "language_code": "IT",
    #     "confidence": 0.98
    # }

    def predict(self, text: str):
        audit_logger.log_info("Inizio esecuzione metodo predict.")
        audit_logger.log_info("Testo inserito da tastiera: " + text)

        try:
            # Esecuzione della predizione
            raw_prediction = self.modello.predict([text])
            probs = self.modello.predict_proba([text])

            # Estrazione valori
            lang_code = str(raw_prediction[0])
            max_conf = np.max(probs)

            # Se la confidenza è troppo bassa (< 0.70) o se il testo è troppo corto
            # allora lancio eccezione UnidentifiedLanguage
            if max_conf < 0.70 or len(text.strip()) < 3:
                audit_logger.log_error(f"Predizione debole ({max_conf}) per '{text}'. Lancio UnidentifiedLanguageError.")
                raise UnidentifiedLanguage()

            # Costruzione oggetto result
            result = LanguageResponse(
                language_code= lang_code,
                confidence=str(round(max_conf, 2)) # Arrotondato a 2 decimali
            )

            audit_logger.log_info("Predizione della lingua del test effettuata.")
            audit_logger.log_info("Fine esecuzione metodo predict.")
            return result

        except Exception as e:
            audit_logger.log_error(f"Errore durante la predizione: {str(e)}")
            raise UnidentifiedLanguage()
