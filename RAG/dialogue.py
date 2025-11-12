from pydantic import BaseModel
from typing import List

# Define Pydantic Model
class UnrealChatEntry(BaseModel):
    type: str   # "human", "ai"
    content: str  # Conversation Content

class DialogueRequest(BaseModel):
    user_question: str   # Current User Question
    chat_history: List[UnrealChatEntry] = []

class DialogueResponse(BaseModel):
    npc_answer: str