import os
from dotenv import load_dotenv
from preprocess import load_documents, load_prompt
from vectorstore import build_vectorstore
from retrieval_chain import create_conversational_chain
from langchain_core.messages import AIMessage, HumanMessage
from fastapi import FastAPI
from dialogue import DialogueRequest, DialogueResponse

# Load Environment Variables
load_dotenv()

# Setup Environment Variables
DATABASE_DIR = os.getenv("DATABASE_DIR")
DATA_DIR = os.getenv("DATA_DIR")
MODEL_NAME = os.getenv("MODEL_NAME")
OPENAI_API_KEY = os.getenv("OPENAI_API_KEY")

SYSTEM_PROMPT = load_prompt("prompts/system_prompt.md")

# Load Documents
documents = load_documents(DATA_DIR)

# Create Vector Store
vectorstore = build_vectorstore(documents, DATABASE_DIR)

# Create Conversational Retrieval Chain
conversation_chain = create_conversational_chain(vectorstore, MODEL_NAME, SYSTEM_PROMPT)

# Initialize Chat History
chat_history = []

# Load Question Prompt
question = load_prompt("prompts/question_prompt.md")
response = conversation_chain.invoke({
    "input": question,
    "chat_history": chat_history
})

chat_history.extend(
    [
        HumanMessage(content=question),
        AIMessage(content=response["answer"]),
    ]
)

# Initialize Fast API App
app = FastAPI()


@app.post("/dialogue", response_model=DialogueResponse)
async def handle_dialogue(request: DialogueRequest):
    """
    Receive Questions and History from Unreal Engine, Run LangChain and RAG, and Return Answers
    """

    # 1. Convert LangChain Message Object List to Dictionary List of Unreal Engine
    langchain_chat_history = []
    for message in request.chat_history:
        if message.type == "human":
            langchain_chat_history.append(HumanMessage(content=message.content))
        elif message.type == "ai":
            langchain_chat_history.append(AIMessage(content=message.content))
        # 'system' 타입은 LangChain RAG 체인의 프롬프트에 이미 포함되어 있으므로 무시합니다.

    # 2. Run LangChain
    try:
        response = conversation_chain.invoke({
            "input": request.user_question,
            "chat_history": langchain_chat_history
        })

        # 3. Return Answer
        return DialogueResponse(npc_answer=response["answer"])

    except Exception as e:
        # Error Message
        print(f"Chain Error {e}")
        return DialogueResponse(npc_answer="There's a problem, please try later.")
