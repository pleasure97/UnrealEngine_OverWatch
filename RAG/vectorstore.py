import os
from langchain.text_splitter import CharacterTextSplitter
from langchain_openai import OpenAIEmbeddings
from langchain_chroma import Chroma

# Split Text and Create Vector Database
def build_vectorstore(documents, persistent_dir):
  try:
    # Put the chunks of data into a Vector Store that associates a Vector Embedding with each chunk
    embeddings = OpenAIEmbeddings(model="text-embedding-3-small")
  except:
    raise NotImplementedError("OpenAI Embeddings Not Implemented")

  # Split
  splitter = CharacterTextSplitter(chunk_size=1500, chunk_overlap=200)
  chunks = splitter.split_documents(documents)

  # Create or Load Chroma
  # Check if a Chroma Datastore already exists - if so, delete the collection to start from scratch
  if not os.path.exists(persistent_dir):
    os.makedirs(persistent_dir, exist_ok=True)
    vectorstore = Chroma.from_documents(documents=chunks, embeddings=embeddings, persist_directory=persistent_dir)
    vectorstore.persist()
  else:
    vectorstore = Chroma(persist_directory=persistent_dir, embedding_function=embeddings)

  return vectorstore