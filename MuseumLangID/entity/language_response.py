from pydantic import BaseModel, Field

class LanguageResponse(BaseModel):
    language_code: str = Field(..., description="La lingua di riferimento (es. 'it', 'en')")
    confidence: str = Field(..., description="Confidenza legata alla scelta della lingua di riferimento")