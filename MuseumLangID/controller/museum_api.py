from fastapi import FastAPI, HTTPException
from entity.language_request import LanguageRequest
from exceptions.empty_text import EmptyTextException
from exceptions.unidentified_language import UnidentifiedLanguage
from common.logger_manager import LoggerManager
from entity.model import Model

# Istruzioni eseguite all'avvio del server
app = FastAPI()
filename = 'language_detection_pipeline.pkl'
model = Model()
model.load_model(filename)
audit_logger = LoggerManager(name="API_MuseumLangID", log_file="registro_audit.log")

# API
@app.post("/identify_language")
def identify_language(request: LanguageRequest):
    input_text = request.text

    try:
        if not input_text.strip():
            raise EmptyTextException

        audit_logger.log_info("Inizio esecuzione dell'API /identify_language")
        result = model.predict(input_text)
        audit_logger.log_info("Fine esecuzione dell'API /identify_language")
        return result

    except EmptyTextException as e:
        # Trasformo l'eccezione in un errore HTTP 400
        audit_logger.log_error(f"Validazione fallita: {str(e)}")
        raise HTTPException(status_code=400, detail=str(e))

    except UnidentifiedLanguage as e:
        # Gestisco il caso in cui il modello non riconosce la lingua
        audit_logger.log_error(f"Modello fallito: {str(e)}")
        raise HTTPException(status_code=400, detail=str(e))

    except Exception as e:
        # Errore generico
        audit_logger.log_error(f"Errore imprevisto: {str(e)}")
        raise HTTPException(status_code=500, detail="Errore interno del server")


# Comando start: uvicorn controller.museum_api:app --reload

# Test:
# --------------------------------------
# INPUT:
# {
#   "text": "Ciao mi chiamo Andrea"
# }
# OUTPUT:
# {
#     "language_code": "IT",
#     "confidence": "0.83"
# }
# --------------------------------------
# INPUT:
# {
#   "text": "Questo è un esempio di testo."
# }
# OUTPUT:
# {
#     "language_code": "IT",
#     "confidence": "1.0"
# }
# --------------------------------------
# INPUT:
# {
#   "text": "How old are you?"
# }
# OUTPUT:
# {
#     "language_code": "EN",
#     "confidence": "1.0"
# }
# --------------------------------------
# INPUT:
# {
#   "text": ""
# }
# OUTPUT:
# {
#     "detail": "Errore: Il testo inserito non può essere vuoto"
# }
# --------------------------------------
# INPUT:
# {
#   "text": "kkkkkkkkkkkkkk"
# }
# OUTPUT:
# {
#     "detail": "Errore: La lingua del testo inserito non può essere identificata"
# }
# --------------------------------------
