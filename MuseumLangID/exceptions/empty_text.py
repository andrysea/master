class EmptyTextException(Exception):
    def __init__(self, message="Errore: Il testo inserito non può essere vuoto"):
        self.message = message
        super().__init__(self.message)
