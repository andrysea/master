class UnidentifiedLanguage(Exception):
    def __init__(self, message="Errore: La lingua del testo inserito non può essere identificata"):
        self.message = message
        super().__init__(self.message)