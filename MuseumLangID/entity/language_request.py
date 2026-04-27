from pydantic import BaseModel, Field

class LanguageRequest(BaseModel):
    text: str = Field(..., description="Il testo da analizzare")
