from langchain_openai import ChatOpenAI
from langchain.memory import ConversationBufferMemory
from langchain.prompts import ChatPromptTemplate, MessagesPlaceholder
from langchain.chains import create_history_aware_retriever, create_retrieval_chain
from langchain.chains.combine_documents import create_stuff_documents_chain

# Make Conversational Retrieval Chain
def create_conversational_chain(vectorstore, model_name: str, system_prompt: str):
  # the retriever is an abstraction over the VectorStore that will be used during RAG
  retriever = vectorstore.as_retriever(search_kwargs={"k":5})
  # Create a new Chat with OpenAI
  llm = ChatOpenAI(temperature=0.7, model_name=model_name)
  # Setup the conversation memory for the chat
  memory = ConversationBufferMemory(memory_key="chat_history", return_messages=True, output_key="answer")

  # Create prompt template
  prompt = ChatPromptTemplate.from_messages([
        MessagesPlaceholder("chat_history"),
        ("human", "{input}"),
        ("system", "Given the above conversation, generate a search query to look up in order to get information relevant to the conversation. If the user is just greeting or saying thanks, respond with 'no relevant search query'.")
    ])

  # Create History-Aware Retriever
  history_aware_retriever = create_history_aware_retriever(llm, retriever, prompt)

  qa_prompt = ChatPromptTemplate.from_messages([
        ("system", system_prompt + "\n\nRETRIEVED_DOCS:\n{context}"),
        MessagesPlaceholder("chat_history"),
        ("human", "{input}")
    ])

  # Create Question-Answer Chain
  question_answer_chain = create_stuff_documents_chain(llm=llm, prompt=qa_prompt)

  # Create Retrieval-Augmented Generation (RAG) Chain
  rag_chain = create_retrieval_chain(history_aware_retriever, question_answer_chain)

  return rag_chain